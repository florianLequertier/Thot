#pragma once

#include "SFML/System.hpp"

template<typename T>
sf::Vector2<T> operator*(const sf::Vector2<T>& right, const sf::Vector2<T>& left)
{
	return sf::Vector2<T>(right.x * left.x, right.y * left.y);
}

template<typename T>
sf::Vector2<T> operator/(const sf::Vector2<T>& right, const sf::Vector2<T>& left)
{
	assert(left.x != 0.f && left.y != 0.f);
	return sf::Vector2<T>(right.x / left.x, right.y / left.y);
}

namespace Thot {

struct Vector2 
{
	float x, y;

	Vector2(float _x, float _y) : x(_x), y(_y)
	{}
};

}