#pragma once

#include "SFML/Graphics.hpp"

namespace Thot {

class IUIShape
{
public:
	virtual void SetPosition(float x, float y) = 0;
	virtual void SetSize(float x, float y) = 0;
	virtual void SetColor(const sf::Color & color) = 0;
	virtual void SetBorderColor(const sf::Color & color) = 0;
	virtual void SetBorderTickness(float tickness) = 0;
	virtual void SetOrigin(float x, float y) = 0;

	virtual void Draw(sf::RenderTarget & target, sf::RenderStates states) const = 0;
};

class UIRectangleShape : public sf::RectangleShape, public IUIShape
{
public:
	virtual void SetPosition(float x, float y) override
	{
		sf::RectangleShape::setPosition(x, y);
	}

	virtual void SetSize(float x, float y) override
	{
		sf::RectangleShape::setSize(sf::Vector2f(x, y));
	}
	virtual void SetColor(const sf::Color & color) override
	{
		sf::RectangleShape::setFillColor(color);
	}

	virtual void SetBorderColor(const sf::Color & color) override
	{
		sf::RectangleShape::setOutlineColor(color);
	}

	virtual void SetBorderTickness(float tickness) override
	{
		sf::RectangleShape::setOutlineThickness(tickness);
	}

	virtual void Draw(sf::RenderTarget & target, sf::RenderStates states) const override
	{
		target.draw(*this, states);
	}

	virtual void SetOrigin(float x, float y) override
	{
		sf::RectangleShape::setOrigin(x, y);
	}

};

}