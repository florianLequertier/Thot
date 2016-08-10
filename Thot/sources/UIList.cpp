#include "UIList.h"
#include "InputEvent.h"
#include "UIWidget.h"

namespace Thot {

UIHorizontalList::UIHorizontalList(UIManager* manager) : super(manager)
{
	//TODO
}
UIHorizontalList::~UIHorizontalList()
{
	//TODO
}

bool UIHorizontalList::OnInputEvent(InputEvent& input)
{
	//mouse is outside, no propagation
	if (!Contains(input.mouseX, input.mouseY))
		return false;

	//propagation
	for (auto& widget : m_widgets)
	{
		if (widget->OnInputEvent(input))
			return true; // we can leave process, input has been consumed
	}
	return false;
}

void UIHorizontalList::ProtectedUpdateShape()
{
	float cursor = 0;
	float fixedWidth = 0;
	float childsResizeableWidth = 0;
	float containerResizeableWidth = 0;
	for (auto& child : m_widgets)
	{
		if ((child->GetResizingProcess(ShapeAxis::X) & ShapeProcessing::FIXED) != 0)
			fixedWidth += child->GetSize().x;
		else
			childsResizeableWidth += child->GetSize().x;
	}
	containerResizeableWidth = m_size.x - fixedWidth;
	assert(fixedWidth < m_size.x);
	assert(childsResizeableWidth > 0);

	for (auto& child : m_widgets)
	{
		float childWidth = child->GetSize().x;
		float childHeight = child->GetSize().y;

		if ((child->GetResizingProcess(ShapeAxis::X) & ShapeProcessing::FIXED) == 0)
		{
			childWidth /= childsResizeableWidth;
			childWidth *= containerResizeableWidth;
		}
		if ((child->GetResizingProcess(ShapeAxis::Y) & ShapeProcessing::FIXED) == 0)
		{
			childHeight = m_size.y;
		}

		child->SetPosition(this, cursor, 0, false);
		child->SetPixelSize(childWidth, childHeight, false);

		cursor += childWidth;
	}
}


//////////////////////////////////////////////


UIVerticalList::UIVerticalList(UIManager* manager) : super(manager)
{
	//TODO
}
UIVerticalList::~UIVerticalList()
{
	//TODO
}

bool UIVerticalList::OnInputEvent(InputEvent& input)
{
	//mouse is outside, no propagation
	if (!Contains(input.mouseX, input.mouseY))
		return false;

	//propagation
	for (auto& widget : m_widgets)
	{
		if (widget->OnInputEvent(input))
			return true; // we can leave process, input has been consumed
	}
	return false;
}

void UIVerticalList::ProtectedUpdateShape()
{
	//super::ProtectedUpdateShape();

	float cursor = 0;
	float fixedHeight = 0;
	float fixedWidth = 0;
	float childsResizeableHeight= 0;
	float containerResizeableHeight = 0;
	bool firstIteration = true;
	for (auto& child : m_widgets)
	{
		if ((child->GetResizingProcess(ShapeAxis::Y) & ShapeProcessing::FIXED) != 0)
		{
			fixedHeight += child->GetSize().y;
		}
		else
			childsResizeableHeight += child->GetSize().y;

		if ((child->GetResizingProcess(ShapeAxis::X) & ShapeProcessing::FIXED) != 0)
		{
			fixedWidth = firstIteration ? child->GetSize().x : (child->GetSize().x > fixedWidth ? child->GetSize().x : fixedWidth);
			firstIteration = false;
		}
	}
	containerResizeableHeight = m_size.y - fixedHeight;

	if (fixedHeight > m_size.y)
	{
		SetPixelSize(m_size.x, fixedHeight, false);
	}
	if (fixedWidth > m_size.x && !firstIteration)
	{
		SetPixelSize(fixedWidth, m_size.y, false);
	}

	if (childsResizeableHeight <= 0)
		childsResizeableHeight = 0;

	for (auto& child : m_widgets)
	{
		float childWidth = child->GetSize().x;
		float childHeight = child->GetSize().y;

		if ((child->GetResizingProcess(ShapeAxis::Y) & ShapeProcessing::FIXED) == 0)
		{
			childHeight /= childsResizeableHeight;
			childHeight *= containerResizeableHeight;
		}
		if ((child->GetResizingProcess(ShapeAxis::X) & ShapeProcessing::FIXED) == 0)
		{
			childWidth = m_size.x;
		}

		child->SetPosition(this, 0, cursor, false);
		child->SetPixelSize(childWidth, childHeight, false);

		cursor += childHeight;
	}
}



}