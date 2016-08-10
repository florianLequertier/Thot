#pragma once

#include "UIWidget.h"

namespace Thot {

class IUIShape;

class UIFrame : public UIWidget
{
	typedef UIWidget super;

private:
	std::shared_ptr<IUIShape> m_visual;

public:
	UIFrame(UIManager* manager);
	virtual ~UIFrame();

	virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override;
	virtual void UpdateVisual() override;

	//TODO
	//functions to deal with shape
	virtual void SetColor(const sf::Color& color);
	void SetBorderColor(const sf::Color& color);
	void SetBorderTickness(float tickness);

	void SetVisual(std::shared_ptr<IUIShape> visual);
	IUIShape& GetVisual() const;

};

}
