#pragma once

#include "UIWidget.h"

namespace  Thot {

	class UIText : public UIWidget, public sf::Drawable
	{
		typedef UIWidget super;

	private:
		std::shared_ptr<sf::Text> m_visual;

	public :
		UIText(UIManager* manager);
		virtual ~UIText();

		virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

		//TODO
		//functions to deal with text
	};

}