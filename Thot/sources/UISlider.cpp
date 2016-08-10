#include "UISlider.h"
#include "InputEvent.h"
#include "UIManager.h"

Thot::UISlider::UISlider(UIManager* manager) : super(manager), m_isDragging(false)
{
}

Thot::UISlider::~UISlider()
{
}

bool Thot::UISlider::OnInputEvent(InputEvent& input)
{
	//propagation
	if (super::OnInputEvent(input))
		return true;

	if (m_isDragged)
	{
		if (input.mouseButton == 0 && input.mouseButtonState == input.BUTTON_RELEASED)
		{
			m_isDragging = false;
		}
		else
		{
			OnSliderDragged(input.mouseX, input.mouseY);
		}
	}

	if (m_button->Contains(input.mouseX, input.mouseY))
	{
		if (input.mouseButton == 0 && input.mouseButtonState == input.BUTTON_PRESSED)
		{
			m_isDragging = true;
			m_manager->SetClickedWidget(shared_from_this());
		}
	}
}

void UISlider::OnSliderDragged(float x, float y )
{
	//TODO
}

void Thot::UISlider::OnValueChanged(InputEvent & input)
{
	if (m_valueChangedCallback != nullptr)
		m_valueChangedCallback(input);
}

void Thot::UISlider::SetOnButtonPressedCallback(std::function<void(InputEvent&)> callback)
{
}
