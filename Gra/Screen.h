#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Utils.h"
#include <string>
#include <format>


//interface for all screens
class IScreen
{
public:
	IScreen() {};
	virtual ~IScreen() {};
	virtual void Draw(sf::RenderWindow& window) const = 0;
};

//base class for all buttons
class ButtonBase
{
public:
	friend std::ostream& operator<<(std::ostream& s, const ButtonBase& button);
	ButtonBase(const vec2d& position, uint32_t textureRect_x, uint32_t textureRect_y, const char* text, const float& scale, const char* filename);
	virtual ~ButtonBase();
	virtual void draw(sf::RenderWindow& window) = 0;
	inline bool mouseOnButton() const { if (frame.left == 0) return false; return true; };
	//getters
	float getWidth() const { return sprite->getGlobalBounds().width; };
	float getHeight() const { return sprite->getGlobalBounds().height; };
	sf::Vector2f getPosition() const { return sprite->getOrigin(); };
	sf::Sprite getSprite() const { return *sprite; };
	sf::Text getText() const { return m_text; };
	sf::Font* getFont() const { return m_font; };
	sf::IntRect getFrame() const { return frame; };
	Vertices getHitbox() const { return m_hitbox; };
	const char* getFilename() const { return m_filename; };
	//setters
	void setHitbox(const Vertices& hitbox) { m_hitbox = hitbox; };
protected:
	sf::IntRect frame;
	sf::Sprite* sprite;
private:
	sf::Texture* m_texture;
	sf::Font* m_font;
	sf::Text m_text;
	Vertices m_hitbox;
	const char* m_filename;
};

