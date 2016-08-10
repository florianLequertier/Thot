#pragma once

#include "UIContainer.h"

namespace Thot{

class UIClickableArea;
class UIVerticalList;
class UIButton;

class UIWindow : public UIContainer
{
	typedef UIContainer super;

	enum WindowFlags {NONE = 0, DRAGGABLE = 1<<0, RESIZABLE = 1<<1, MODALE = 1<<2, MODALE_AUTO_CLOSABLE = 1<<3, HAS_HEADER = 1<<4, HAS_CLOSE_BUTTON = 1<<5};
	enum WindowSides {LEFT, TOP, RIGHT, BOTTOM}; //TODO add corners

public:
	UIWindow(UIManager* manager, unsigned int flags = (DRAGGABLE | RESIZABLE | HAS_HEADER | HAS_CLOSE_BUTTON) );
	virtual ~UIWindow();

	virtual bool OnInputEvent(InputEvent& input) override;

	virtual void SetColor(const sf::Color& color) override;

	void CloseWindow();
	void MoveOnTop();
	void ProcessWindowDrag();
	void ProcessWindowResize();

private:
	unsigned int m_flags;
	bool m_isDragged;
	bool m_isResized;
	WindowSides m_currentResizeSide;
	sf::Vector2i m_dragAnchor;
	sf::Vector2i m_sizeBeforeResizeProcess;
	sf::Vector2i m_positionBeforeResizeProcess;
	sf::Vector2i m_resizeDirection;
	sf::Vector2i m_resizeAxis;

	std::shared_ptr<UIVerticalList> m_windowInside;
	std::shared_ptr<UIFrame> m_windowContent;
	std::shared_ptr<UIFrame> m_windowHeader;
	std::shared_ptr<UIButton> m_closeButton;

protected:
	virtual void ProtectedUpdateShape() override;
};

}