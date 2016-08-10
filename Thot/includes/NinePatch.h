#pragma once

#include "SFML/Graphics.hpp" 
#include "Shapes.h"

namespace Thot {

class NinePatch : public sf::Transformable, public IUIShape
{
public:
	NinePatch(float offset);
	virtual ~NinePatch();

	virtual void SetPosition(float x, float y) override;
	virtual void SetSize(float x, float y) override;
	virtual void SetColor(const sf::Color & color) override;
	virtual void SetBorderColor(const sf::Color & color) override;
	virtual void SetBorderTickness(float tickness) override;
	virtual void SetOrigin(float x, float y) override;

protected:
	sf::VertexArray m_vertices;
	float m_offset;
	sf::Vector2f m_contentDim;
	sf::Color m_color;
	sf::Color m_borderColor;

protected:
	virtual void buildRectangle(int index, sf::Vector2f a, sf::Vector2f b, sf::Vector2f c, sf::Vector2f d);
	void rebuildShape();
	virtual void Draw(sf::RenderTarget & target, sf::RenderStates states) const override;
};

class GradientNinePatch : public NinePatch
{
	typedef NinePatch super;
public:
	GradientNinePatch(float offset);
	virtual ~GradientNinePatch();

protected:
	virtual void buildRectangle(int index, sf::Vector2f a, sf::Vector2f b, sf::Vector2f c, sf::Vector2f d) override;
};

}