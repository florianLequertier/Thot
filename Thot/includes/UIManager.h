#pragma once

#include <vector>
#include <memory>

#include "SFML/Graphics.hpp"

namespace Thot {

struct InputEvent;
class UIWidget;
class UILayer;

class UIManager final : public sf::Drawable
{
public:
	UIManager(float windowWidth, float windowHeight);
	~UIManager();

	bool filterInputEvent(InputEvent& input);
	void ProcessEvent(InputEvent& input);

	void ProcessUpdateShapeRequests();
	void ProcessRemoveRequests();
	void ProcessSorting();
	void RequestRemoveFromParent(UIWidget* widget);
	void RequestUpdateShapeOnComponent(UIWidget* widget);
	void RequestMoveOntop(UIWidget* widget);
	void OnResizeWindow(float windowWidth, float windowHeight);
	void SetClickedWidget(std::shared_ptr<UIWidget> clickedWidget);
	void ResetClickedWidget();

	void AddLayer(std::shared_ptr<UILayer> layer);

private:
	std::vector<std::shared_ptr<UILayer>> m_layers;
	std::weak_ptr<UIWidget> m_clickedWidget;
	std::weak_ptr<UIWidget> m_keyboardFocusedWidget;
	std::weak_ptr<UIWidget> m_mouseFocusedWidget;
	std::vector<UIWidget*> m_widgetsToUpdate;
	std::vector<UIWidget*> m_widgetsToRemove;
	UIWidget* m_widgetToPlaceOnTop;
		
protected:
	virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

	sf::Vector2f m_windowSize;

};

}