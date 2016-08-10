#include "UIWidget.h"
#include "UIContainer.h"
#include "UIManager.h"

namespace Thot {

UIWidget::UIWidget(UIManager* manager)
	: m_manager(manager)
	, m_position(0, 0)
	, m_size(10, 10)
	, m_isDirty(false)
	, m_repositionningProcess_x(ShapeProcessing::RELATIVE)
	, m_repositionningProcess_y(ShapeProcessing::RELATIVE)
	, m_resizingProcess_x(ShapeProcessing::RELATIVE)
	, m_resizingProcess_y(ShapeProcessing::RELATIVE)
	//, m_repositionningMode(ShapeProcessing::IN_PERCENT)
	//, m_resizingMode(ShapeProcessing::IN_PERCENT)
	, m_parent(nullptr)
{
	setDirty();
}

UIWidget::~UIWidget()
{
	m_manager = nullptr;
}

bool UIWidget::OnInputEvent(InputEvent& input)
{
	for (auto& child : m_widgets)
	{
		if (child->OnInputEvent(input))
			return true;
	}

	return false;
}

bool UIWidget::Contains(float x, float y)
{
	if (x <= (m_position.x - m_origin.x) || x >= (m_position.x - m_origin.x) + m_size.x
		|| y <= (m_position.y - m_origin.y) || y >= (m_position.y - m_origin.y) + m_size.y)
		return false;

	return true;
}

void UIWidget::UpdateShape()
{
	ProtectedUpdateShape();
	UpdateVisual();

	for (auto& child : m_widgets) {
		child->UpdateShape();
	}
}

void UIWidget::setDirty()
{
	m_isDirty = true;

	assert(m_manager != nullptr);
	m_manager->RequestUpdateShapeOnComponent(this);
}

bool UIWidget::HasParent() const
{
	return m_parent != nullptr;
}

UIWidget* UIWidget::GetParent()
{
	return m_parent;
}

void UIWidget::AttachToParent(UIWidget* parent, bool childPositionIsRelative, bool childSizeRelativeToParent)
{
	assert(m_parent == nullptr); //If Not, this widget is already atached to an other parent, call remove from parent before.
	m_parent = parent;
	parent->AddChildAtomic(shared_from_this());

	if (childPositionIsRelative) {
		m_position += parent->GetPosition();
	}
	if (childSizeRelativeToParent) {
		m_size = m_size * parent->GetSize();
	}
	// TODO : same thing with size ?

	SetUpFromParent();
}

void UIWidget::AddChild(std::shared_ptr<UIWidget> widget, bool childPositionIsRelative, bool childSizeRelativeToParent)
{
	widget->AttachToParent(this, childPositionIsRelative, childSizeRelativeToParent);
}

void UIWidget::AddChildAtomic(std::shared_ptr<UIWidget> child)
{
	m_widgets.push_back(child);
}

void UIWidget::SetUpFromParent(ShapeAxis axis, ShapeTransformation transformation)
{
	if (!HasParent())
		return;

	//set up relative size and position : 
	sf::Vector2f parentSize = m_parent->GetSize();
	sf::Vector2f parentPosition = m_parent->GetPosition();

	//assert(parentSize.x > 0.00001 && parentSize.y > 0.00001);

	if ((axis & ShapeAxis::X) != 0) {
		if((transformation & ShapeTransformation::SIZE) != 0)
			m_relativeSize.x = (m_size.x / parentSize.x);
		if ((transformation & ShapeTransformation::POSITION) != 0)
			m_relativePosition.x = (m_position.x - parentPosition.x) / parentSize.x;
	}
	if ((axis & ShapeAxis::Y) != 0) {
		if ((transformation & ShapeTransformation::SIZE) != 0)
			m_relativeSize.y = (m_size.y / parentSize.y);
		if ((transformation & ShapeTransformation::POSITION) != 0)
			m_relativePosition.y = (m_position.y - parentPosition.y) / parentSize.y;
	}
}

void UIWidget::RemoveFromParent()
{
	assert(m_manager != nullptr);
	m_manager->RequestRemoveFromParent(this);
}

void UIWidget::RemoveChild(std::shared_ptr<UIWidget> widget)
{
	auto found = std::find(m_widgets.begin(), m_widgets.end(), widget);
	if (found != m_widgets.end())
		m_widgets.erase(found);
}

void UIWidget::RemoveChild(UIWidget* widget)
{
	auto found = std::find_if(m_widgets.begin(), m_widgets.end(), [widget](const std::shared_ptr<UIWidget>& element) {return element.get() == widget; });
	if (found != m_widgets.end())
		m_widgets.erase(found);
}

void UIWidget::OnChapeChanged(bool updateChilds)
{
	if(updateChilds) //TODO : immediate process or not ?
	{ 
		//setDirty();

		for (auto& child : m_widgets)
		{
			UpdateShape();
		}
	}

	//update origin
	m_origin = m_clampedOrigin * m_size;

	UpdateVisual();
}

void UIWidget::SetResizingProcess(ShapeProcessing processingMode, ShapeAxis axis)
{
	if (axis == ShapeAxis::X)
		m_resizingProcess_x = processingMode;
	else if (axis == ShapeAxis::Y)
		m_resizingProcess_y = processingMode;
	else {
		m_resizingProcess_x = processingMode;
		m_resizingProcess_y = processingMode;
	}
}

void UIWidget::SetRepositionningProcess(ShapeProcessing processingMode, ShapeAxis axis)
{
	if (axis == ShapeAxis::X)
		m_repositionningProcess_x = processingMode;
	else if (axis == ShapeAxis::Y)
		m_repositionningProcess_y = processingMode;
	else{
		m_repositionningProcess_x = processingMode;
		m_repositionningProcess_y = processingMode;
	}
}

UIWidget::ShapeProcessing UIWidget::GetResizingProcess(ShapeAxis axis) const
{
	if (axis == ShapeAxis::X)
		return m_resizingProcess_x;
	else if (axis == ShapeAxis::Y)
		return m_resizingProcess_y;
	else {
		assert(m_resizingProcess_x == m_resizingProcess_y);
		return m_resizingProcess_x;
	}
}

UIWidget::ShapeProcessing UIWidget::GetRepositionningProcess(ShapeAxis axis) const
{
	if (axis == ShapeAxis::X)
		return m_repositionningProcess_x;
	else if (axis == ShapeAxis::Y)
		return m_repositionningProcess_y;
	else {
		assert(m_repositionningProcess_x == m_repositionningProcess_y);
		return m_repositionningProcess_x;
	}
}

//void UIWidget::SetRepositionningMode(UIWidget::ShapeProcessing mode)
//{
//	m_repositionningMode = mode;
//}
//
//void UIWidget::SetResizingMode(UIWidget::ShapeProcessing mode)
//{
//	m_resizingMode = mode;
//}
//
//UIWidget::ShapeProcessing UIWidget::GetRepositionningMode() const
//{
//	return m_repositionningMode;
//}
//
//UIWidget::ShapeProcessing UIWidget::GetResizingMode() const
//{
//	return m_resizingMode;
//}

void UIWidget::SetPixelSize(float width, float height, bool updateChilds)
{
	m_size.x = width;
	m_size.y = height;

	SetUpFromParent(ShapeAxis::X_Y, ShapeTransformation::SIZE);
	OnChapeChanged(updateChilds);
}

void UIWidget::SetPosition(float x, float y, bool updateChilds)
{
	m_position.x = x;
	m_position.y = y;

	SetUpFromParent(ShapeAxis::X_Y, ShapeTransformation::POSITION);
	OnChapeChanged(updateChilds);
}

void UIWidget::SetPosition(const UIWidget* anchor, float x, float y, bool updateChilds)
{
	sf::Vector2f anchorPosition = anchor->GetPosition();

	m_position.x = x + anchorPosition.x;
	m_position.y = y + anchorPosition.y;

	SetUpFromParent(ShapeAxis::X_Y, ShapeTransformation::POSITION);
	OnChapeChanged(updateChilds);
}

void UIWidget::SetRelativePosition(const UIWidget* anchor, float x, float y, bool updateChilds)
{
	sf::Vector2f anchorPosition = anchor->GetPosition();
	sf::Vector2f anchorSize = anchor->GetSize();

	m_relativePosition.x = x;
	m_relativePosition.y = y;

	m_position.x = x * anchorSize.x + anchorPosition.x;
	m_position.y = y * anchorSize.y + anchorPosition.y;

	OnChapeChanged(updateChilds);
}

void UIWidget::SetRelativeSize(const UIWidget* anchor, float x, float y, bool updateChilds)
{
	sf::Vector2f anchorPosition = anchor->GetPosition();
	sf::Vector2f anchorSize = anchor->GetSize();

	m_relativeSize.x = x;
	m_relativeSize.y = y;

	m_size.x = x * anchorSize.x;
	m_size.y = y * anchorSize.y;

	OnChapeChanged(updateChilds);
}

void UIWidget::SetPixelOffset(float x, float y, bool updateChilds)
{
	m_pixelOffset.x = x;
	m_pixelOffset.y = y;

	OnChapeChanged(updateChilds);
}

void UIWidget::SetOrigin(float clampedX, float clampedY, bool updateChilds)
{
	m_clampedOrigin.x = clampedX;
	m_clampedOrigin.y = clampedY;

	OnChapeChanged(updateChilds);
}

sf::Vector2f UIWidget::GetSize() const
{
	return m_size;
}

sf::Vector2f UIWidget::GetPosition() const
{
	return m_position;
}

sf::Vector2f UIWidget::GetPixelOffset() const
{
	return m_pixelOffset;
}

sf::Vector2f UIWidget::GetOrigin() const
{
	return m_clampedOrigin;
}

int UIWidget::GetDrawingIndex() const
{
	return m_drawingIndex;
}

void UIWidget::PrepareToMoveChildOnTop(UIWidget * child)
{
	//Nothing
}

void UIWidget::ProcessInternalSorting()
{
	//Nothing
}

void UIWidget::UpdateVisual()
{
	// Nothing
}

void UIWidget::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	//Drawing propagation
	for (auto& widget : m_widgets)
	{
		target.draw(*widget, states);
	}
}

void UIWidget::ProtectedUpdateShape()
{
	if (HasParent())
	{
		sf::Vector2f parentSize = m_parent->GetSize();
		sf::Vector2f parentPosition = m_parent->GetPosition();
		//sf::Vector2f previousParentSize = m_size / m_relativeSize;
		//sf::Vector2f previousParentPosition = m_position - m_relativePosition * previousParentSize;

		/*
		//X axis :
		if ((m_repositionningProcess_x & ShapeProcessing::RELATIVE) != 0) {
			if((m_repositionningMode & ShapeProcessing::IN_PERCENT) != 0)
				m_position.x = (m_relativePosition.x * parentSize.x + parentPosition.x);
			else
				m_position.x += (parentPosition.x - previousParentPosition.x);
		}
		else
			SetUpFromParent(ShapeAxis::X, ShapeTransformation::POSITION);


		//X axis :
		if ((m_resizingProcess_x & ShapeProcessing::RELATIVE) != 0) {
			if ((m_resizingMode & ShapeProcessing::IN_PERCENT) != 0)
				m_size.x = m_relativeSize.x * parentSize.x;
			else
				m_size.x *= (parentSize.x / previousParentSize.x);
		}
		else
			SetUpFromParent(ShapeAxis::X, ShapeTransformation::SIZE);



		//Y axis 
		if ((m_repositionningProcess_y & ShapeProcessing::RELATIVE) != 0) {
			if((m_repositionningMode & ShapeProcessing::IN_PERCENT) != 0)
				m_position.y = (m_relativePosition.y * parentSize.y + parentPosition.y);
			else
				m_position.y += (parentPosition.y - previousParentPosition.y);
		}
		else
			SetUpFromParent(ShapeAxis::Y, ShapeTransformation::POSITION);

		if ((m_resizingProcess_y & ShapeProcessing::RELATIVE) != 0) {
			if ((m_resizingProcess_y & ShapeProcessing::IN_PERCENT) != 0)
				m_size.y = m_relativeSize.y * parentSize.y;
			else
				m_size.y *= (parentSize.y / previousParentSize.y);
		}
		else
			SetUpFromParent(ShapeAxis::Y, ShapeTransformation::SIZE);
		*/

		//X axis :
		if ((m_repositionningProcess_x & ShapeProcessing::RELATIVE) != 0)
			m_position.x = (m_relativePosition.x * parentSize.x + parentPosition.x) + m_pixelOffset.x;
		else
			SetUpFromParent(ShapeAxis::X, ShapeTransformation::POSITION);

		if ((m_resizingProcess_x & ShapeProcessing::RELATIVE) != 0) {
				m_size.x = m_relativeSize.x * parentSize.x;
		}
		else
			SetUpFromParent(ShapeAxis::X, ShapeTransformation::SIZE);

		//Y axis 
		if ((m_repositionningProcess_y & ShapeProcessing::RELATIVE) != 0)
			m_position.y = (m_relativePosition.y * parentSize.y + parentPosition.y) + m_pixelOffset.y;
		else
			SetUpFromParent(ShapeAxis::Y, ShapeTransformation::POSITION);

		if ((m_resizingProcess_y & ShapeProcessing::RELATIVE) != 0)
				m_size.y = m_relativeSize.y * parentSize.y;
		else
			SetUpFromParent(ShapeAxis::Y, ShapeTransformation::SIZE);
	}

	m_isDirty = false;
}

}