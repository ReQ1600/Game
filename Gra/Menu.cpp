#include "Menu.h"
#include <iostream>

#pragma region Menu
Menu::Main Menu::Main::s_Instance;

Menu::ParentMenu::ParentMenu()
{
	m_height = sf::VideoMode::getDesktopMode().height * scale;
	m_width = sf::VideoMode::getDesktopMode().width * scale;
}

Menu::ParentMenu::~ParentMenu() {}

Menu::Main::Main() : ParentMenu()
{
	m_height *= 2 / 3;
	m_width *= 3 / 5;
	m_buttons.resize(4);

	float scale = 0.4 * sf::VideoMode::getDesktopMode().width/1920;
	unsigned short segment = 1.f / 9 * sf::VideoMode::getDesktopMode().height;

	m_buttons[0] = std::make_unique<Menu::Button>(4 * segment, "Start", scale);
	m_buttons[1] = std::make_unique<Menu::Button>(5 * segment, "Shop", scale);
	m_buttons[2] = std::make_unique<Menu::Button>(6 * segment, "Exit", scale);
	m_buttons[3] = std::make_unique<Menu::Button>(7 * segment, "Credits", scale);

	m_title = new sf::Text();
	m_title->setFont(*m_buttons[0].get()->getFont());
	m_title->setString("GAME");
	m_title->setCharacterSize(500 * scale);
	m_title->setOrigin(m_title->getGlobalBounds().left + m_title->getGlobalBounds().width / 2.f, m_title->getGlobalBounds().top + m_title->getGlobalBounds().height / 2.f);
	m_title->setPosition(sf::Vector2f(sf::VideoMode::getDesktopMode().getDesktopMode().width/2, 2*segment));
}

Menu::Main::~Main() {}

Menu::Main& Menu::Main::Get()
{
	return Menu::Main::s_Instance;
}

void Menu::Main::Draw(sf::RenderWindow& window) const
{
	window.draw(*m_title);

	for (auto& b : m_buttons)
	{
		b->draw(window);
	}
}

Menu::ButtonType Menu::Main::Select()
{
	if (m_buttons[0].get()->getFrame().left == 1050) return ButtonType::Start;
	else if (m_buttons[1].get()->getFrame().left == 1050) return ButtonType::Shop;
	else if (m_buttons[2].get()->getFrame().left == 1050) return ButtonType::Exit;
	else if (m_buttons[3].get()->getFrame().left == 1050) return ButtonType::Credits;
	else return ButtonType::Empty;
}
#pragma endregion

#pragma region Button

void Menu::Button::draw(sf::RenderWindow& window)
{
	sf::Mouse mouse;
	Vertex mouseVertex(mouse.getPosition(window).x, mouse.getPosition(window).y);
	Vertices mouseVertices = { mouseVertex };

	if (Collision::Intersection(this->getHitbox(), mouseVertices)) frame.left = 1050;
	else frame.left = 0;

	sprite->setTextureRect(frame);


	window.draw(*sprite);
	window.draw(this->getText());
}
#pragma endregion



std::ostream& operator<<(std::ostream& s, const Menu::ParentMenu& menu)
{
	s << typeid(menu).name() << "\n";
	return s;
}