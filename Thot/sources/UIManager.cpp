#include "UIManager.h"
#include "UILayer.h"
#include "UIWidget.h"
#include "InputEvent.h"

namespace Thot {

UIManager::UIManager(float windowWidth, float windowHeight) : m_windowSize(windowWidth, windowHeight)
{
	//TODO
}

UIManager::~UIManager()
{
	//TODO
}

bool UIManager::filterInputEvent(InputEvent& input)
{
	//priority to clicked widget
	if (!m_clickedWidget.expired())
	{
		m_clickedWidget.lock()->OnInputEvent(input);

		//consume event
		input.reset();
		return false;
	}

	return true;
}

void UIManager::ProcessEvent(InputEvent& input)
{
	if (input.windowResized)
	{
		OnResizeWindow(input.windowWidth, input.windowHeight);
		return;
	}

	if (filterInputEvent(input))
	{
		for (auto& layer : m_layers)
		{
			if (layer->OnInputEvent(input)) //consumed ?
				return; //stop processing layers bellow
		}
	}
}

void UIManager::ProcessUpdateShapeRequests()
{
	for (auto& widget : m_widgetsToUpdate) {

		//update parent instead of widget to have batter placement
		UIWidget* widgetToUpdate = widget;
		if (widget->HasParent()) {
			widgetToUpdate = widget->GetParent();
		}

		widgetToUpdate->UpdateShape();
	}
	m_widgetsToUpdate.clear();
}

void UIManager::ProcessRemoveRequests()
{
	for (auto& widget : m_widgetsToRemove) {
		assert(widget->HasParent());
		widget->GetParent()->RemoveChild(widget);
	}
	m_widgetsToRemove.clear();
}

void UIManager::ProcessSorting()
{
	for (auto& layer : m_layers)
	{
		if (layer->GetNeedInternalSorting())
			layer->ProcessInternalSorting();
	}
}

void UIManager::RequestRemoveFromParent(UIWidget * widget)
{
	m_widgetsToRemove.push_back(widget);
}

void UIManager::RequestUpdateShapeOnComponent(UIWidget * widget)
{
	//better to set this at the end of the frame
	//UIWidget* widgetToUpdate = widget;
	//if (widget->HasParent()) {
	//	widgetToUpdate = widget->GetParent();
	//}

	m_widgetsToUpdate.push_back(widget);
}

void UIManager::RequestMoveOntop(UIWidget * widget)
{
	m_widgetToPlaceOnTop = widget;
}

void UIManager::OnResizeWindow(float windowWidth, float windowHeight)
{
	m_windowSize = sf::Vector2f(windowWidth, windowHeight);

	for (auto& layer : m_layers)
	{
		layer->SetPosition(0, 0);
		layer->SetPixelSize(windowWidth, windowHeight);
	}
}

void UIManager::SetClickedWidget(std::shared_ptr<UIWidget> clickedWidget)
{
	m_clickedWidget = clickedWidget;
}

void UIManager::ResetClickedWidget()
{
	m_clickedWidget.reset();
}

void UIManager::AddLayer(std::shared_ptr<UILayer> layer)
{
	assert(std::find(m_layers.begin(), m_layers.end(), layer) == m_layers.end());

	layer->SetPixelSize(m_windowSize.x, m_windowSize.y);
	m_layers.push_back(layer);
}

void UIManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto& layer : m_layers)
	{
		target.draw(*layer, states);
	}
}

}