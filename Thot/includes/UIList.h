#pragma once

#include <memory>

#include "UIContainer.h"

namespace Thot{

class UIHorizontalList : public UIContainer
{
	typedef UIContainer super;

public:
	UIHorizontalList(UIManager* manager);
	virtual ~UIHorizontalList();

	//TODO ?
	//void AddChild(std::shared_ptr<UIWidget> widget, bool IsChildPositionRelative) override;
	//void RemoveChild(std::shared_ptr<UIWidget> widget) override;

	virtual bool OnInputEvent(InputEvent& input) override;

protected:
	virtual void ProtectedUpdateShape();
};

class UIVerticalList : public UIContainer
{
	typedef UIContainer super;

public:
	UIVerticalList(UIManager* manager);
	virtual ~UIVerticalList();

	//TODO ?
	//void AddChild(std::shared_ptr<UIWidget> widget, bool IsChildPositionRelative) override;
	//void RemoveChild(std::shared_ptr<UIWidget> widget) override;

	virtual bool OnInputEvent(InputEvent& input) override;

protected:
	virtual void ProtectedUpdateShape();
};

}


