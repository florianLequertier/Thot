#include <SFML/Graphics.hpp>
#include "UIWidget.h"

#include "InputEvent.h"
#include "UIManager.h"
#include "UILayer.h"
#include "UIWidget.h"
#include "UIWindow.h"

void fillEvent_fromSFML(Thot::InputEvent& input, const sf::Event& sfInput)
{
	switch (sfInput.type)
	{
	case (sf::Event::KeyPressed):
		input.keyState = Thot::InputEvent::KEY_PRESSED;
		input.key = (unsigned int)sfInput.key.code;
		break;
	case (sf::Event::KeyReleased):
		input.keyState = Thot::InputEvent::KEY_RELEASED;
		input.key = (unsigned int)sfInput.key.code;
		break;
	case (sf::Event::MouseButtonPressed):
		input.mouseButtonState = Thot::InputEvent::BUTTON_PRESSED;
		input.mouseButton = (unsigned int)sfInput.mouseButton.button;
		input.mouseX = sfInput.mouseButton.x;
		input.mouseY = sfInput.mouseButton.y;
		break;
	case (sf::Event::MouseButtonReleased):
		input.mouseButtonState = Thot::InputEvent::BUTTON_RELEASED;
		input.mouseButton = (unsigned int)sfInput.mouseButton.button;
		input.mouseX = sfInput.mouseButton.x;
		input.mouseY = sfInput.mouseButton.y;
		break;
	case (sf::Event::TextEntered):
		input.textEntered = true;
		input.unicode = static_cast<char>(sfInput.text.unicode);
		break;
	case (sf::Event::Resized) :
		input.windowResized = true;
		input.windowWidth = sfInput.size.width;
		input.windowHeight = sfInput.size.height;
		break;
	case sf::Event::MouseMoved:
		input.mouseX = sfInput.mouseMove.x;
		input.mouseY = sfInput.mouseMove.y;
	default:
		break;
	}
}

int main()
{

	sf::RenderWindow window(sf::VideoMode(600, 600), "Thot");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	window.setKeyRepeatEnabled(false);

	static const float s_InitialWindowWidth = 600;
	static const float s_InitialWindowHeight = 600;

	//SetUp : 
	Thot::UIManager uiManager(s_InitialWindowWidth, s_InitialWindowHeight);
	auto layer = std::shared_ptr<Thot::UILayer>(new Thot::UILayer(&uiManager));

	auto widget = std::shared_ptr<Thot::UIWindow>(new Thot::UIWindow(&uiManager));
	widget->SetRepositionningProcess(Thot::UIWidget::ShapeProcessing::FIXED);
	widget->SetResizingProcess(Thot::UIWidget::ShapeProcessing::FIXED);
	widget->SetPosition(100, 100);
	widget->SetPixelSize(100, 100);
	widget->SetColor(sf::Color::Transparent);
	widget->SetBorderColor(sf::Color::Blue);
	widget->SetBorderTickness(3);

	//auto widget2 = std::shared_ptr<Thot::UIWindow>(new Thot::UIWindow(&uiManager));
	//widget2->SetShapeProcessing(Thot::UIWidget::ShapeProcessing::FIXED);
	//widget2->SetPosition(200, 200);
	//widget2->SetPixelSize(200, 200);
	//widget2->SetColor(sf::Color::White);
	//widget2->SetBorderColor(sf::Color::Blue);
	//widget2->SetBorderTickness(3);

	//auto widget3 = std::shared_ptr<Thot::UIWindow>(new Thot::UIWindow(&uiManager));
	//widget3->SetShapeProcessing(Thot::UIWidget::ShapeProcessing::RELATIVE);
	//widget3->SetPosition(100, 100);
	//widget3->SetPixelSize(50, 50);
	//widget3->SetColor(sf::Color::Red);
	//widget3->SetBorderColor(sf::Color::Green);
	//widget3->SetBorderTickness(3);

	//widget2->AddChild(widget3, true);

	layer->AddChild(widget);
	//layer->AddChild(widget2);
	uiManager.AddLayer(std::shared_ptr<Thot::UILayer>(layer));

	sf::View viewForUI(sf::FloatRect(0, 0, s_InitialWindowWidth, s_InitialWindowHeight));

	while (window.isOpen())
	{
		sf::Event event;
		Thot::InputEvent thotEvent;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::Resized)
			{
				sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
				viewForUI.reset(visibleArea);
			}

			fillEvent_fromSFML(thotEvent, event);
			if (thotEvent.keyState == thotEvent.KEY_PRESSED)
				std::cout << "event key pressed : " << thotEvent.key << std::endl;
			if (thotEvent.keyState == thotEvent.KEY_RELEASED)
				std::cout << "event key released : " << thotEvent.key << std::endl;
			
			uiManager.ProcessEvent(thotEvent);
		}

		//Draw : 
		window.clear();

		//Draw ui : 
		//Setup the proper view : 
		window.setView(viewForUI);
		window.draw(uiManager);

		//window.draw(shape);

		window.display();

		uiManager.ProcessSorting();
		//uiManager.ProcessUpdateShapeRequests(); //TODO : resolve bug
		uiManager.ProcessRemoveRequests();
	}

	return 0;
}