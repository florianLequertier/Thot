#include "UIContainer.h"
#include "InputEvent.h"

namespace Thot {

UIContainer::UIContainer(UIManager* manager) : super(manager)
{

}

UIContainer::~UIContainer()
{

}

//Return true is the event is consumed
bool UIContainer::OnInputEvent(InputEvent& input) 
{
	//mouse is outside, no propagation
	if (!Contains(input.mouseX, input.mouseY))
		return false;

	//propagation
	for (auto& widget : m_widgets)
	{
		if (widget->OnInputEvent(input))
			return true; // we can leave process, input has been consumed
	}

	return false;
}

}