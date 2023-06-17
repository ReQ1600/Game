#include "CreditsScreen.h"
#include <iostream>

//all text is always centered
Text::Text(const char* text, const float& scale, const unsigned __int16& y)
{
	if (!font.loadFromFile("assets\\Font.ttf")) std::cout << "text font loading error" << std::endl;

	this->text.setFont(font);
	this->text.setString(text);
	this->text.setOrigin(this->text.getGlobalBounds().left + this->text.getGlobalBounds().width / 2.f, this->text.getGlobalBounds().top + this->text.getGlobalBounds().height / 2.f);
	this->text.setScale(scale,scale);
	this->text.setPosition(sf::VideoMode::getDesktopMode().width / 2, y);
}

Text::~Text()
{

}

void Text::Draw(sf::RenderWindow& window)
{
	window.draw(text);
}

CreditsScreen CreditsScreen::s_Instance;

CreditsScreen::CreditsScreen()
{
	float scale = 1 * 1920 / sf::VideoMode::getDesktopMode().width;
	unsigned short segment = 1.f / 3 * sf::VideoMode::getDesktopMode().height;//dividing screen into equal segments, multiply by segment number to place text into it (starts from 0)

	const char* soundcredits = "Sounds:\nbutton sound - Minecraft button click\nwalking - Minecraft old grass walking sound\nenemy death - Old School Runescape damage sound\nplayer hurt - Minecraft Old hurt sound\nplayer shooting projectiles - Plants vs Zombies peashooter shoot\nprojectile hit enemy - Plants vs Zombies peashooter projectile splat\nwin - starting riff from \"Bad To The Bone\" - George Thorogood and the Destroyers, in bad quality ofc\nlose - bruh";

	lorem_ipsum.resize(3);
	lorem_ipsum[0] = std::make_unique<Text>("Game created in C++ using SFML",scale,0.3 * segment);
	lorem_ipsum[1] = std::make_unique<Text>(soundcredits,scale,segment);
	lorem_ipsum[2] = std::make_unique<Text>("Everything else has been created by Mateusz Drogowski",scale,2*segment);
}
//great credits innit?

CreditsScreen::~CreditsScreen()
{
}

void CreditsScreen::Draw(sf::RenderWindow& window) const
{
	for  (const std::unique_ptr<Text>& txt : lorem_ipsum)
	{
		txt->Draw(window);
	}
}

CreditsScreen& CreditsScreen::Get()
{
	return s_Instance;
}



