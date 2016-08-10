#pragma once

#include "UIFrame.h"

namespace Thot{

struct InputEvent;

class UIClickableArea : public UIFrame
{
	typedef UIFrame super;

public:
	enum ClickableState { PRESSED, RELEASED, HOVERED };

	UIClickableArea(UIManager* manager);
	virtual ~UIClickableArea();

	virtual void OnPressed(InputEvent& input) = 0;
	virtual void OnReleased(InputEvent& input) = 0;
	virtual void OnHovered(InputEvent& input) = 0;

protected:
	ClickableState clickableState;
};

}