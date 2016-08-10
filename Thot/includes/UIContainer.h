#pragma once

#include <memory>
#include <vector>

#include "UIFrame.h"

namespace Thot {

class UIContainer : public UIFrame
{
	typedef UIFrame super;

public:
	UIContainer(UIManager* manager);
	virtual ~UIContainer();

	//Return true is the event is consumed
	virtual bool OnInputEvent(InputEvent& input) override;

};

}