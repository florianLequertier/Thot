#pragma once

#include "UIClickableArea.h"

namespace Thot{

class UIButton : public UIClickableArea
{
	typedef UIClickableArea super;

private:
	using UIClickableArea::SetColor;

public:
	UIButton(UIManager* manager);
	virtual ~UIButton();

	virtual bool OnInputEvent(InputEvent& input) override;

	virtual void OnPressed(InputEvent& input) override;
	virtual void OnHovered(InputEvent& input) override;
	virtual void OnReleased(InputEvent& input) override;

	void SetOnButtonPressedCallback(std::function<void(InputEvent&)> callback);
	void SetOnButtonReleasedCallback(std::function<void(InputEvent&)> callback);

	void SetHoveredColor(const sf::Color& color);
	void SetPressedColor(const sf::Color& color);
	void SetDefaultColor(const sf::Color& color);

private:
	std::function<void(InputEvent&)> m_buttonPressedCallback;
	std::function<void(InputEvent&)> m_buttonReleasedCallback;

	sf::Color m_pressedColor;
	sf::Color m_hoveredColor;
	sf::Color m_defaultColor;
};

class UICheckButton : public UIButton
{
	typedef UIButton super;

public:
	UICheckButton(UIManager* manager);
	virtual ~UICheckButton();

	virtual bool OnInputEvent(InputEvent& input) override;

private:
	bool m_isChecked;
};

}