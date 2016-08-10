#pragma once

#include "Singleton.h"

#include <SFML/Graphics.hpp>

namespace Thot {


class InputManagerBase
{
public:
	virtual bool IsKeyPressed(unsigned int keyCode) = 0;
	virtual bool IsKeyReleased(unsigned int keyCode) = 0;
	virtual bool IsMouseButtonPressed(unsigned int button) = 0;
	virtual bool IsMouseButtonReleased(unsigned int button) = 0;
	virtual sf::Vector2i GetMousePosition() const = 0;
};

class InputManagerSFML : public InputManagerBase, public Singleton<InputManagerSFML>
{
	friend class Singleton<InputManagerSFML>;

protected:
	InputManagerSFML() {}
public:
	InputManagerSFML(const InputManagerSFML& other) = delete;
	void operator=(const InputManagerSFML& other) = delete;

public:
	virtual bool IsKeyPressed(unsigned int keyCode) override
	{
		return sf::Keyboard::isKeyPressed((sf::Keyboard::Key)keyCode);
	}

	virtual bool IsKeyReleased(unsigned int keyCode) override
	{
		return !sf::Keyboard::isKeyPressed((sf::Keyboard::Key)keyCode);
	}

	virtual bool IsMouseButtonPressed(unsigned int button) override
	{
		return sf::Mouse::isButtonPressed((sf::Mouse::Button)button);
	}

	virtual bool IsMouseButtonReleased(unsigned int button) override
	{
		return !sf::Mouse::isButtonPressed((sf::Mouse::Button)button);
	}

	virtual sf::Vector2i GetMousePosition() const override
	{
		return sf::Mouse::getPosition();
	}

};

struct InputEvent
{
	enum KeyState { KEY_PRESSED, KEY_RELEASED, KEY_NONE};
	enum MouseButtonState {BUTTON_PRESSED, BUTTON_RELEASED, BUTTON_NONE };

	bool windowResized;
	bool textEntered;
	char unicode;
	KeyState keyState;
	MouseButtonState mouseButtonState;
	unsigned int key;
	unsigned int mouseButton;
	float mouseX, mouseY;
	float mouseDeltaX, mouseDeltaY;
	float windowWidth;
	float windowHeight;

	InputEvent() : key(0), mouseX(0), mouseY(0), mouseDeltaX(0), mouseDeltaY(0), keyState(KEY_NONE), mouseButtonState(BUTTON_NONE), mouseButton(0), textEntered(false), unicode(0), windowHeight(0), windowWidth(0), windowResized(false)
	{}

	bool MouseMoved()
	{
		return mouseDeltaX + mouseDeltaY >= 0.00001f;
	}

	void reset()
	{
		key = 0;
		mouseX = 0;
		mouseY = 0;
		mouseDeltaX = 0;
		mouseDeltaY = 0;
		keyState = KEY_NONE;
		mouseButtonState = BUTTON_NONE;
		mouseButton = 0;
		textEntered = false;
		unicode = 0;
		windowHeight = 0;
		windowWidth = 0;
		windowResized = false;
	}
};

}
