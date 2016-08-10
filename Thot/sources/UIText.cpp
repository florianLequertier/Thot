#include "UIText.h"

namespace Thot{

UIText::UIText(UIManager* manager): super(manager)
{
	m_visual = std::make_shared<sf::Text>();
}

UIText::~UIText()
{
	m_visual.reset();
}

void UIText::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(*m_visual, states);

	//Propagation
	super::draw(target, states);
}

}