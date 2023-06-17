#include "EndScreen.h"
#include "Sounds.h"

EndScreen::Button::Button(ButtonType type, const vec2d& position, const float scale) : m_type(type), ButtonBase(position, 1050,180,
	[&type]() -> const char* {if (type == ButtonType::Menu) return "Menu"; return "Try Again"; }(), scale,"button")
{}//thats the constructor
void EndScreen::Button::draw(sf::RenderWindow& window)
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

EndScreen::Screen::Screen(const GameState state, const uint32_t& money, const int& money_mult)
{
	m_pFont = new sf::Font;
	m_pEndingStateText = new sf::Text;
	m_pMoneyText = new sf::Text;

	try
	{
		if (!m_pFont->loadFromFile("assets\\Font.ttf")) throw Utils::ErrorLog("Button font loading");
	}
	catch (const bool&)
	{
		return;
	}

	switch (state)
	{
	case GameState::Won:
		Sound::Sounds::GameWon.Play();
		break;
	case GameState::Lost:
		Sound::Sounds::GameLost.Play();
		break;
	}

	float scale = sf::VideoMode::getDesktopMode().width / 1920;
	std::string statetxt = state == GameState::Won ? "Won!" : "Lost";
	unsigned short segment = 1.f / 9 * sf::VideoMode::getDesktopMode().height;

	m_pEndingStateText->setFont(*m_pFont);
	m_pEndingStateText->setString("You " + statetxt);
	m_pEndingStateText->setCharacterSize(60 * scale);
	m_pEndingStateText->setOrigin(m_pEndingStateText->getGlobalBounds().left + m_pEndingStateText->getGlobalBounds().width / 2, m_pEndingStateText->getGlobalBounds().top + m_pEndingStateText->getGlobalBounds().height / 2);
	m_pEndingStateText->setPosition(sf::VideoMode::getDesktopMode().width / 2, 3 * segment);

	const double additionalMoney = 0.2 * money_mult * money;
	std::string additional = (int)additionalMoney == 0 ? "" : std::format(" + {0} multiplier bonus",(int)additionalMoney);
	std::string bonus = state == GameState::Won ? " + 2500 win bonus" : "";
	m_pMoneyText->setFont(*m_pFont);
	m_pMoneyText->setString(std::format("You earned: {0} coins{1}{2}",money,additional,bonus));
	m_pMoneyText->setCharacterSize(30 * scale);
	m_pMoneyText->setOrigin(m_pMoneyText->getGlobalBounds().left + m_pMoneyText->getGlobalBounds().width / 2, m_pMoneyText->getGlobalBounds().top + m_pMoneyText->getGlobalBounds().height / 2);
	m_pMoneyText->setPosition(sf::VideoMode::getDesktopMode().width / 2, 4 * segment);

	m_pRestartButton = new Button(ButtonType::Restart, vec2d(sf::VideoMode::getDesktopMode().width / 2, 5 * segment), 0.3 * scale);
	m_pMenuButton = new Button(ButtonType::Menu, vec2d(sf::VideoMode::getDesktopMode().width / 2, 6 * segment),0.3 * scale);

}

void EndScreen::Screen::Draw(sf::RenderWindow& window) const
{
	m_pRestartButton->draw(window);
	m_pMenuButton->draw(window);
	window.draw(*m_pMoneyText);
	window.draw(*m_pEndingStateText);
}

//returns pressed button
EndScreen::ButtonType EndScreen::Screen::ButtonPressed()
{
	if (m_pMenuButton->mouseOnButton()) return ButtonType::Menu;
	else if (m_pRestartButton->mouseOnButton()) return ButtonType::Restart;
	else return ButtonType::None;
}
