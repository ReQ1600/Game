#pragma once
#include "Screen.h"
#include <vector>

class Text
{
public:
	Text(const char* text, const float& scale, const unsigned __int16& y);
	~Text();
	void Draw(sf::RenderWindow& window);
private:
	sf::Font font;
	sf::Text text;
};

class CreditsScreen final : public IScreen
{
public:
	CreditsScreen(const CreditsScreen&) = delete;
	~CreditsScreen();
	void Draw(sf::RenderWindow& window) const override;
	static CreditsScreen& Get();
private:
	static CreditsScreen s_Instance;
	CreditsScreen();

	std::vector<std::unique_ptr<Text>> lorem_ipsum;
};

