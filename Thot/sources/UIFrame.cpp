#include "UIFrame.h"
#include "Shapes.h"
#include "NinePatch.h"

namespace Thot {

UIFrame::UIFrame(UIManager* manager): super(manager)
{
	m_visual = std::make_shared<UIRectangleShape>();
}

UIFrame::~UIFrame()
{
	m_visual.reset();
}

void UIFrame::UpdateVisual()
{
	m_visual->SetPosition(m_position.x, m_position.y);
	m_visual->SetSize(m_size.x, m_size.y);
	m_visual->SetOrigin(m_origin.x, m_origin.y);
}

void UIFrame::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	m_visual->Draw(target, states);
	//target.draw(*m_visual, states);

	//Propagation
	super::draw(target, states);
}

void UIFrame::SetColor(const sf::Color & color)
{
	m_visual->SetColor(color);
}

void UIFrame::SetBorderColor(const sf::Color & color)
{
	m_visual->SetBorderColor(color);
}

void UIFrame::SetBorderTickness(float tickness)
{
	m_visual->SetBorderTickness(tickness);
}

void UIFrame::SetVisual(std::shared_ptr<IUIShape> visual)
{
	m_visual = visual;
}

IUIShape& UIFrame::GetVisual() const
{
	return *m_visual;
}

}
