#include "UILayer.h"
#include "UIWidget.h"
#include "InputEvent.h"

namespace Thot {

UILayer::UILayer(UIManager* manager) : super(manager), m_childToPutOnTop(nullptr), m_needInternalSorting(false)
{
	//TODO
}

UILayer::~UILayer()
{
	m_childToPutOnTop = nullptr;
	m_needInternalSorting = false;
	//TODO
}

bool UILayer::OnInputEvent(InputEvent& input)
{
	for (int i = m_widgets.size() - 1; i >= 0; i--) //TODO : avoid reverse parsing if possible
	{
		if (m_widgets[i]->OnInputEvent(input))
			return true; // we can leave process, input has been consumed
	}

	return false;
}

bool UILayer::GetNeedInternalSorting() const
{
	return m_needInternalSorting;
}

void UILayer::PrepareToMoveChildOnTop(UIWidget* child)
{
	m_needInternalSorting = true;
	m_childToPutOnTop = child;
}

void UILayer::ProcessInternalSorting()
{
	assert(m_childToPutOnTop != nullptr);

	UIWidget* childToPutOnTop = m_childToPutOnTop;
	auto found = std::find_if(m_widgets.begin(), m_widgets.end(), [childToPutOnTop](const std::shared_ptr<UIWidget>& element) { return element.get() == childToPutOnTop; });

	if (found != m_widgets.end())
	{
		int foundIdx = found - m_widgets.begin();
		auto childToPutOnTop = *found;
		for (unsigned int i = foundIdx; i < m_widgets.size() - 1; i++)
		{
			m_widgets[i] = m_widgets[i + 1];
		}
		m_widgets[m_widgets.size() - 1] = childToPutOnTop;
	}
}

void UILayer::UpdateVisual()
{
	//NOTHING
}

}