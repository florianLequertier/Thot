#include "UIButton.h"
#include "InputEvent.h"

namespace Thot {

UIButton::UIButton(UIManager* manager) : super(manager)
{
	//TODO
}

UIButton::~UIButton()
{
	//TODO
}

bool UIButton::OnInputEvent(InputEvent& input)
{
	//mouse outside widget
	if (!Contains(input.mouseX, input.mouseY))
	{
		if (clickableState == ClickableState::HOVERED)
		{
			SetColor(m_defaultColor);
			clickableState = ClickableState::RELEASED;
		}
		return false;
	}

	if (input.mouseButton == 0 && input.mouseButtonState == InputEvent::BUTTON_PRESSED)
	{
		OnPressed(input);

		//consume event
		input.reset();
		return true;
	}
	else if (input.mouseButton == 0 && input.mouseButtonState == InputEvent::BUTTON_RELEASED && clickableState == ClickableState::PRESSED)
	{
		OnReleased(input);

		//consume event
		input.reset();
		return true;
	}
	else if (clickableState != ClickableState::HOVERED)
	{
		clickableState = ClickableState::HOVERED;
		OnHovered(input);

		//consume event
		input.reset();
		return true;
	}

	return false;
}

void UIButton::OnPressed(InputEvent& input)
{
	//TODO
	clickableState = PRESSED;
	if (m_buttonPressedCallback)
		m_buttonPressedCallback(input);
	SetColor(m_pressedColor);
}

void UIButton::OnHovered(InputEvent& input)
{
	//TODO
	clickableState = HOVERED;
	//TODO
	SetColor(m_hoveredColor);
}

void UIButton::OnReleased(InputEvent& input)
{
	//TODO
	clickableState = RELEASED;
	if (m_buttonReleasedCallback)
		m_buttonReleasedCallback(input);
	SetColor(m_defaultColor);
}

void UIButton::SetOnButtonPressedCallback(std::function<void(InputEvent&)> callback)
{
	assert(callback);
	m_buttonPressedCallback = callback;
}

void UIButton::SetOnButtonReleasedCallback(std::function<void(InputEvent&)> callback)
{
	assert(callback);
	m_buttonReleasedCallback = callback;
}

void UIButton::SetHoveredColor(const sf::Color& color)
{
	m_hoveredColor = color;
	if (clickableState == ClickableState::HOVERED)
		SetColor(m_hoveredColor);
}

void UIButton::SetPressedColor(const sf::Color& color)
{
	m_hoveredColor = color;
	if (clickableState == ClickableState::PRESSED)
		SetColor(m_pressedColor);
}

void UIButton::SetDefaultColor(const sf::Color& color)
{
	m_defaultColor = color;
	if (clickableState == ClickableState::RELEASED)
		SetColor(m_defaultColor);
}

///////////////////////////////////////////////////////////////

UICheckButton::UICheckButton(UIManager* manager) : super(manager)
{
	//TODO
}

UICheckButton::~UICheckButton()
{
	//TODO
}

bool UICheckButton::OnInputEvent(InputEvent& input)
{
	//mouse outside widget
	if (!Contains(input.mouseX, input.mouseY))
		return false;

	if (input.mouseButton != 0 && input.mouseButtonState == InputEvent::BUTTON_PRESSED)
	{
		if (m_isChecked)
			OnReleased(input);
		else
			OnPressed(input);

		//consume event
		input.reset();
		return true;
	}

	return false;
}

}