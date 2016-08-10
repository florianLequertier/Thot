#include "..\includes\NinePatch.h"

namespace Thot {

NinePatch::NinePatch(float offset) 
: m_vertices(sf::PrimitiveType::Triangles, 54)
, m_offset(offset), m_contentDim(0, 0)
, m_color(sf::Color::White)
, m_borderColor(sf::Color::White)
{
	//buildRectangle((0, 0), (m_offset, 0), (m_offset, m_offset), (0, m_offset));
	//buildRectangle((m_offset, 0), (m_offset, 0), (m_offset, m_offset), (m_offset, m_offset));
	//buildRectangle((m_offset, 0), (2*m_offset, 0), (2*m_offset, m_offset), (m_offset, m_offset));

	//buildRectangle((0, 0), (m_offset, 0), (m_offset, m_offset), (0, m_offset));
	//buildRectangle((m_offset, 0), (m_offset, 0), (m_offset, m_offset), (m_offset, m_offset));
	//buildRectangle((m_offset, 0), (2 * m_offset, 0), (2 * m_offset, m_offset), (m_offset, m_offset));

	//buildRectangle((0, 0), (m_offset, 0), (m_offset, m_offset), (0, m_offset));
	//buildRectangle((0, 0), (m_offset, 0), (m_offset, m_offset), (0, m_offset));
	//buildRectangle((0, 0), (m_offset, 0), (m_offset, m_offset), (0, m_offset));
}

NinePatch::~NinePatch()
{

}

void NinePatch::buildRectangle(int index, sf::Vector2f a, sf::Vector2f b, sf::Vector2f c, sf::Vector2f d)
{
	index *= 6;

	m_vertices[index].position = a;
	m_vertices[index++].color = m_color;
	m_vertices[index].position = b;
	m_vertices[index++].color = m_color;
	m_vertices[index].position = c;
	m_vertices[index++].color = m_color;


	m_vertices[index].position = c;
	m_vertices[index++].color = m_color;
	m_vertices[index].position = d;
	m_vertices[index++].color = m_color;
	m_vertices[index].position = a;
	m_vertices[index++].color = m_color;
}

void NinePatch::rebuildShape()
{
	buildRectangle(0, sf::Vector2f(0, 0), sf::Vector2f(m_offset, 0), sf::Vector2f(m_offset, m_offset), sf::Vector2f(0, m_offset));
	buildRectangle(1, sf::Vector2f(m_offset, 0), sf::Vector2f(m_offset + m_contentDim.x , 0), sf::Vector2f(m_offset + m_contentDim.x , m_offset), sf::Vector2f(m_offset, m_offset));
	buildRectangle(2, sf::Vector2f(m_offset + m_contentDim.x, 0), sf::Vector2f(2*m_offset + m_contentDim.x, 0), sf::Vector2f(2*m_offset + m_contentDim.x, m_offset), sf::Vector2f(m_offset + m_contentDim.x, m_offset));

	buildRectangle(3, sf::Vector2f(0, m_offset), sf::Vector2f(m_offset, m_offset), sf::Vector2f(m_offset, m_offset + m_contentDim.y), sf::Vector2f(0, m_offset + m_contentDim.y));
	buildRectangle(4, sf::Vector2f(m_offset, m_offset), sf::Vector2f(m_offset + m_contentDim.x, m_offset), sf::Vector2f(m_offset + m_contentDim.x, m_offset + m_contentDim.y), sf::Vector2f(m_offset, m_offset + m_contentDim.y));
	buildRectangle(5, sf::Vector2f(m_offset + m_contentDim.x, m_offset), sf::Vector2f(2 * m_offset + m_contentDim.x, m_offset), sf::Vector2f(2 * m_offset + m_contentDim.x, m_offset + m_contentDim.y), sf::Vector2f(m_offset + m_contentDim.x, m_offset + m_contentDim.y));

	buildRectangle(6, sf::Vector2f(0, m_offset + m_contentDim.y), sf::Vector2f(m_offset, m_offset + m_contentDim.y), sf::Vector2f(m_offset, 2*m_offset + m_contentDim.y), sf::Vector2f(0, 2*m_offset + m_contentDim.y));
	buildRectangle(7, sf::Vector2f(m_offset, m_offset + m_contentDim.y), sf::Vector2f(m_offset + m_contentDim.x, m_offset + m_contentDim.y), sf::Vector2f(m_offset + m_contentDim.x, 2*m_offset + m_contentDim.y), sf::Vector2f(m_offset, 2*m_offset + m_contentDim.y));
	buildRectangle(8, sf::Vector2f(m_offset + m_contentDim.x, m_offset + m_contentDim.y), sf::Vector2f(2 * m_offset + m_contentDim.x, m_offset + m_contentDim.y), sf::Vector2f(2 * m_offset + m_contentDim.x, 2*m_offset + m_contentDim.y), sf::Vector2f(m_offset + m_contentDim.x, 2*m_offset + m_contentDim.y));
}

void NinePatch::Draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	//states.texture = &m_texture;
	target.draw(m_vertices, states);
}

void NinePatch::SetPosition(float x, float y)
{
	sf::Transformable::setPosition(x, y);
}

void NinePatch::SetSize(float x, float y)
{
	m_contentDim.x = x - 2 * m_offset;
	if (m_contentDim.x < 0)
		m_contentDim.x = 0;

	m_contentDim.y = y - 2 * m_offset;
	if (m_contentDim.y < 0)
		m_contentDim.y = 0;

	rebuildShape();
}

void NinePatch::SetColor(const sf::Color & color)
{
	m_color = color;
	rebuildShape();
}

void NinePatch::SetBorderColor(const sf::Color & color)
{
	m_borderColor = color;
	rebuildShape();
}

void NinePatch::SetBorderTickness(float tickness)
{
	//TODO
}

void NinePatch::SetOrigin(float x, float y)
{
	setOrigin(x, y);
}

///////////////////////////////////////

GradientNinePatch::GradientNinePatch(float offset) : super(offset)
{

}

GradientNinePatch::~GradientNinePatch()
{

}

void GradientNinePatch::buildRectangle(int index, sf::Vector2f a, sf::Vector2f b, sf::Vector2f c, sf::Vector2f d)
{
	int idx = index;
	index *= 6;

	if (idx == 4)
	{
		m_vertices[index].position = a;
		m_vertices[index++].color = m_color;
		m_vertices[index].position = b;
		m_vertices[index++].color = m_color;
		m_vertices[index].position = c;
		m_vertices[index++].color = m_color;


		m_vertices[index].position = c;
		m_vertices[index++].color = m_color;
		m_vertices[index].position = d;
		m_vertices[index++].color = m_color;
		m_vertices[index].position = a;
		m_vertices[index++].color = m_color;
	}
	else
	{
		m_vertices[index].position = a;
		m_vertices[index++].color = (idx == 4) || (idx == 5) || (idx == 7) || (idx == 8) ? m_borderColor : sf::Color::Transparent;
		m_vertices[index].position = b;
		m_vertices[index++].color = (idx == 3) || (idx == 4) || (idx == 6) || (idx == 7) ? m_borderColor : sf::Color::Transparent;
		m_vertices[index].position = c;
		m_vertices[index++].color = (idx == 0) || (idx == 1) || (idx == 3) || (idx == 4) ? m_borderColor : sf::Color::Transparent;


		m_vertices[index].position = c;
		m_vertices[index++].color = (idx == 0) || (idx == 1) || (idx == 3) || (idx == 4) ? m_borderColor : sf::Color::Transparent;
		m_vertices[index].position = d;
		m_vertices[index++].color = (idx == 1) || (idx == 2) || (idx == 4) || (idx == 5) ? m_borderColor : sf::Color::Transparent;
		m_vertices[index].position = a;
		m_vertices[index++].color = (idx == 4) || (idx == 5) || (idx == 7) || (idx == 8) ? m_borderColor : sf::Color::Transparent;
	}
}

}
