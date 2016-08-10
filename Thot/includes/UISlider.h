#pragma once

#include "UIFrame.h"
#include "UIClickableArea.h"

namespace  Thot {

class UISlider : public UIFrame
{
	typedef UIFrame super;
public:
	UISlider(UIManager* manager);
	virtual ~UISlider();

	virtual bool OnInputEvent(InputEvent& input) override;
	virtual void OnValueChanged(InputEvent& input);

	void SetOnButtonPressedCallback(std::function<void(InputEvent&)> callback);
private:
	std::function<void(InputEvent&)> m_valueChangedCallback;
	std::shared_ptr<UIClickableArea> m_button;
	bool m_isDragging;
};

}