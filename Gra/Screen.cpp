#include "Screen.h"


ButtonBase::ButtonBase(const vec2d& position,uint32_t textureRect_x, uint32_t textureRect_y, const char* text, const float& scale, const char* filename) : m_filename(filename)
{
	Utils::Log(this);
	const std::string fpath = std::format("assets\\{0}", filename) + ".png";

	m_texture = new sf::Texture();
	sprite = new sf::Sprite();
	m_font = new sf::Font();

	try
		{
			if (!m_texture->loadFromFile(fpath)) throw Utils::ErrorLog("Button texture loading error");
			if (!m_font->loadFromFile("assets\\Font.ttf")) throw Utils::ErrorLog("Button font loading error");
			if (position.y > sf::VideoMode::getDesktopMode().height || position.y < 0) throw Utils::ErrorLog("Button y out of bounds");
			if (position.x > sf::VideoMode::getDesktopMode().width || position.x < 0) throw Utils::ErrorLog("Button x out of bounds");
		}
		catch (const bool& e)
		{
			return;
		}

	sprite->setTexture(*m_texture);
	frame = (sf::IntRect(0, 0, textureRect_x, textureRect_y));
	sprite->setTextureRect(frame);

	sprite->setScale(scale, scale);
	sprite->setOrigin(sprite->getTextureRect().width / 2.f, sprite->getTextureRect().height / 2.f);
	sprite->setPosition(position.x, position.y);

	m_hitbox = {
		{sprite->getPosition().x - sprite->getGlobalBounds().width / 2 ,sprite->getPosition().y - sprite->getGlobalBounds().height / 2 },
		{sprite->getPosition().x + sprite->getGlobalBounds().width / 2 ,sprite->getPosition().y - sprite->getGlobalBounds().height / 2 },
		{sprite->getPosition().x + sprite->getGlobalBounds().width / 2 ,sprite->getPosition().y + sprite->getGlobalBounds().height / 2 },
		{sprite->getPosition().x - sprite->getGlobalBounds().width / 2 ,sprite->getPosition().y + sprite->getGlobalBounds().height / 2 }
	};

	m_text.setFont(*m_font);
	m_text.setString(text);
	m_text.setOrigin(m_text.getGlobalBounds().left + m_text.getGlobalBounds().width / 2.f, m_text.getGlobalBounds().top + m_text.getGlobalBounds().height / 2.f);
	m_text.setScale(3 * scale, 3 * scale);

	m_text.setPosition(sprite->getPosition().x, position.y);
}

ButtonBase::~ButtonBase()
{
	delete sprite;
	delete m_texture;
	delete m_font;
}

std::ostream& operator<<(std::ostream& s, const ButtonBase& button)
{
	s << button.getText().getString().getData() << ":\n" <<
		"size: " << button.getWidth() << " x " << button.getHeight() <<
		"\nPosition: " << button.getPosition().x << ", " << button.getPosition().y;
	return s;
}