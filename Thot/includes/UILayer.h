#pragma once

#include "UIContainer.h"

namespace Thot{

class UILayer final  : public UIWidget
{
	typedef UIWidget super;

public:
	UILayer(UIManager* manager);
	~UILayer();

	virtual bool OnInputEvent(InputEvent& input) override;

	bool GetNeedInternalSorting() const;
	virtual void PrepareToMoveChildOnTop(UIWidget* child) override;
	virtual void ProcessInternalSorting() override;

	virtual void UpdateVisual() override;

private:
	bool m_needInternalSorting;
	UIWidget* m_childToPutOnTop;
};

}