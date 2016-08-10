#include "UIWindow.h"
#include "InputEvent.h"
#include "UIManager.h"
#include "UIClickableArea.h"
#include "UIList.h"
#include "NinePatch.h"
#include "UIButton.h"

namespace Thot {

UIWindow::UIWindow(UIManager* manager, unsigned int flags) 
	: super(manager)
	, m_flags(flags)
	, m_isDragged(false)
	, m_isResized(false)
	, m_currentResizeSide(LEFT)
	, m_resizeDirection(0, 0)
{
	SetVisual(std::make_shared<GradientNinePatch>(5));

	m_windowInside = std::make_shared<UIVerticalList>(manager);
	m_windowInside->SetPosition(0.5, 0.5);
	m_windowInside->SetPixelSize(9, 9);
	AddChild(m_windowInside, false);

	m_windowContent = std::make_shared<UIFrame>(manager);

	if (m_flags & WindowFlags::HAS_HEADER)
	{
		m_windowHeader = std::make_shared<UIFrame>(manager);
		m_windowHeader->SetResizingProcess(ShapeProcessing::FIXED, ShapeAxis::Y);
		m_windowHeader->SetPixelSize(9, 20);

		if (m_flags & WindowFlags::HAS_CLOSE_BUTTON)
		{
			m_closeButton = std::make_shared<UIButton>(manager);
			m_closeButton->SetOnButtonReleasedCallback([this](Thot::InputEvent& inputEvent) {this->CloseWindow(); });
			m_closeButton->SetResizingProcess(ShapeProcessing::FIXED, ShapeAxis::X_Y);
			m_closeButton->SetRelativePosition(m_windowHeader.get(), 1, 0.5);
			m_closeButton->SetPixelOffset(-15, 0);
			m_closeButton->SetOrigin(0.5, 0.5);
			//m_closeButton->SetPosition(m_closeButton->GetPosition().x - 1, m_closeButton->GetPosition().x);
			m_closeButton->SetPixelSize(15, 15);
			// Temporary
			m_closeButton->SetDefaultColor(sf::Color::Blue);
			m_closeButton->SetPressedColor(sf::Color::Red);
			m_closeButton->SetHoveredColor(sf::Color::Magenta);

			m_windowHeader->AddChild(m_closeButton);
		}

		m_windowInside->AddChild(m_windowHeader, true);
	}

	m_windowInside->AddChild(m_windowContent, true);

	// temporary : 
	m_windowHeader->SetColor(sf::Color(100, 100, 100, 150));
	m_windowContent->SetColor(sf::Color(120, 120, 120, 90));
	m_windowInside->SetColor(sf::Color::Transparent);

}

UIWindow::~UIWindow()
{
	//TODO
}

void UIWindow::CloseWindow()
{
	RemoveFromParent();
}

void UIWindow::MoveOnTop()
{
	if (HasParent())
	{
		m_parent->PrepareToMoveChildOnTop(this);
	}
}

void UIWindow::ProcessWindowDrag()
{
	sf::Vector2i mousePos = InputManagerSFML::Instance().GetMousePosition();
	sf::Vector2i nextWindowPos = mousePos - m_dragAnchor;
	SetPosition(nextWindowPos.x, nextWindowPos.y, true);
}

void UIWindow::ProcessWindowResize()
{
	sf::Vector2i mousePos = InputManagerSFML::Instance().GetMousePosition();
	sf::Vector2i deltaPos = mousePos - m_dragAnchor;
	deltaPos = deltaPos * m_resizeAxis;

	sf::Vector2i newSize = m_sizeBeforeResizeProcess + deltaPos;
	if (m_resizeDirection.x < 0)
		newSize.x = m_sizeBeforeResizeProcess.x - deltaPos.x;
	if(m_resizeDirection.y < 0)
		newSize.y = m_sizeBeforeResizeProcess.y - deltaPos.y;

	if (newSize.x <= 0) {
		newSize.x = 1;
		deltaPos.x = m_sizeBeforeResizeProcess.x - 1;
	}
	if (newSize.y <= 0) {
		newSize.y = 1;
		deltaPos.y = m_sizeBeforeResizeProcess.x - 1;
	}

	SetPixelSize(newSize.x, newSize.y);

	float posOffsetX = 0;
	float posOffsetY = 0;
	bool needPosOffset = false;
	if (m_resizeDirection.x < 0) {
		posOffsetX = deltaPos.x;
		needPosOffset = true;
	}
	if (m_resizeDirection.y < 0) {
		posOffsetY = deltaPos.y;
		needPosOffset = true;
	}
	if (needPosOffset)
		SetPosition(m_positionBeforeResizeProcess.x + posOffsetX, m_positionBeforeResizeProcess.y + posOffsetY);
}

void UIWindow::ProtectedUpdateShape()
{
	super::ProtectedUpdateShape();
	m_windowInside->SetPosition(this, 5, 5, false);
	m_windowInside->SetPixelSize(m_size.x - 10, m_size.y - 10, false);
}


bool UIWindow::OnInputEvent(InputEvent& input)
{
	//propagation
	if (super::OnInputEvent(input))
		return true;

	//drag and dropping ? 
	if ((m_flags & WindowFlags::DRAGGABLE) && (m_isDragged || m_isResized))
	{
		if (InputManagerSFML::Instance().IsMouseButtonPressed(sf::Mouse::Left))
		{
			if(m_isDragged)
				ProcessWindowDrag();
			if (m_isResized)
				ProcessWindowResize();
		}
		if (input.mouseButtonState == InputEvent::BUTTON_RELEASED)
		{
			m_isDragged = false;
			m_isResized = false;
			m_manager->ResetClickedWidget();
		}
	}

	//mouse is outside
	if (!Contains(input.mouseX, input.mouseY))
	{
		if (m_flags & WindowFlags::MODALE_AUTO_CLOSABLE) {
			//No propagation, plus we close the modale
			CloseWindow();
			input.reset();
			return true;
		}
		else if (m_flags & WindowFlags::MODALE) {
			//No propagation if the window is a modale
			input.reset();
			return true;
		}
		//input not consumed
		return false;
	}
	else //mouse inside
	{
		//Own event process
		if (input.mouseButtonState == InputEvent::BUTTON_PRESSED)
		{
			if (HasParent()) {
				m_parent->PrepareToMoveChildOnTop(this);
			}

			if (m_flags & (WindowFlags::DRAGGABLE | WindowFlags::HAS_HEADER) 
				&& m_windowHeader != nullptr && m_windowHeader->Contains(input.mouseX, input.mouseY))
			{
				//Begin drag and drop
				m_isDragged = true;
				m_manager->SetClickedWidget(shared_from_this());
				m_dragAnchor = InputManagerSFML::Instance().GetMousePosition() - sf::Vector2i(m_position.x, m_position.y);
				//consume input
				input.reset();
				return true;
			}

			if (m_flags && WindowFlags::RESIZABLE)
			{
				m_resizeDirection = sf::Vector2i(0, 0);
				m_resizeAxis = sf::Vector2i(0, 0);
				if (input.mouseX > m_position.x + m_size.x - 7) {
					m_resizeDirection.x += 1;
					m_resizeAxis.x += 1;
				}
				else if (input.mouseX < m_position.x + 7) {
					m_resizeDirection.x -= 1;
					m_resizeAxis.x += 1;
				}
				if (input.mouseY > m_position.y + m_size.y - 7) {
					m_resizeDirection.y += 1;
					m_resizeAxis.y += 1;
				}
				else if (input.mouseY < m_position.y + 7) {
					m_resizeDirection.y -= 1;
					m_resizeAxis.y += 1;
				}

				//resize event
				if (m_resizeDirection != sf::Vector2i(0, 0)) {
					m_isResized = true;
					m_sizeBeforeResizeProcess = sf::Vector2i(m_size.x, m_size.y);
					m_positionBeforeResizeProcess = sf::Vector2i(m_position.x, m_position.y);
					m_manager->SetClickedWidget(shared_from_this());
					m_dragAnchor = InputManagerSFML::Instance().GetMousePosition();// - sf::Vector2i(m_position.x, m_position.y);
					//consume input
					input.reset();
					return true;
				}
			}

		}

		////propagation
		//for (auto& widget : m_widgets)
		//{
		//	if (widget->OnInputEvent(input))
		//		return true; // we can leave process, input has been consumed
		//}
		//we consume the input
		input.reset();
		return true;
	}
}
void UIWindow::SetColor(const sf::Color & color)
{
	super::SetColor(color);
	m_windowInside->SetColor(color);
}

}