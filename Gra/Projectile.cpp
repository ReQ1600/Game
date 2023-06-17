#include "Projectile.h"
#include <format>

//position only needed when enemu is shooting
Projectile::Projectile(ProjectileType type, uint32_t damage, sf::RenderWindow& window, sf::Vector2f pos) : m_damage(damage), m_type(type)
{
	m_pSprite = new sf::Sprite();
	m_pTexture = new sf::Texture();

	try
	{
		const char* fname = "";
		switch (type)
		{
		case ProjectileType::Knight:
			fname = "projectileKnight";
			m_velocity = 10;
			break;
		case ProjectileType::Witch:
			fname = "projectileWitch";
			m_velocity = 12;
			break;
		case ProjectileType::Enemy:
			fname = "projectileEnemy";
			m_velocity = 5;
			break;
		default:
			throw Utils::ErrorLog("Projectile creation: type out of scope");
			break;
		}

		if (!m_pTexture->loadFromFile(std::format("assets\\{0}.png", fname))) throw Utils::ErrorLog("Projectile creation: texture loading error");
	}
	catch (const bool&)
	{
		return;
	}

	
	

	Vertex playerLocation = { sf::VideoMode::getDesktopMode().width / 2.f, sf::VideoMode::getDesktopMode().height / 2.f };
	vec2d unitVec;
	sf::Vector2f startingPosition;

	if (m_type == ProjectileType::Enemy)
	{
		startingPosition = pos;
		unitVec = Utils::CalculateUnitVector(vec2d(pos.x, pos.y), playerLocation);
	}
	else
	{
		startingPosition = { sf::VideoMode::getDesktopMode().width / 2.f, sf::VideoMode::getDesktopMode().height / 2.f };
		sf::Mouse mouse;
		Vertex mouseLocation(mouse.getPosition(window).x, mouse.getPosition(window).y);

		unitVec = Utils::CalculateUnitVector(playerLocation, mouseLocation);
	}
	m_step = { (float)(unitVec.x * m_velocity), (float)(unitVec.y * m_velocity) };
	
	float scale = 0.6 * sf::VideoMode::getDesktopMode().width / 1920;
	m_pSprite->setTexture(*m_pTexture);
	m_pSprite->setScale(scale, scale);
	m_pSprite->setOrigin(m_pSprite->getGlobalBounds().width / 2.f, m_pSprite->getGlobalBounds().height / 2.f);
	m_pSprite->setPosition(startingPosition);

	//every pprojectile hitbox is the same size
	//starting from top left
	m_hitbox = {
		{10,0},
		{23,0},
		{32,10},
		{32,23},
		{23,32},
		{10,32},
		{0,23},
		{0,10}
	};

	for (auto& [x, y] : m_hitbox)
	{
		x = (x - m_pSprite->getGlobalBounds().width / 2.0) * scale + startingPosition.x;
		y = (y - m_pSprite->getGlobalBounds().height / 2.0) * scale + startingPosition.y;
	}

	m_alive = true;
}

void Projectile::Draw(sf::RenderWindow& window)
{
	float scale = 0.6 * sf::VideoMode::getDesktopMode().width / 1920;
	sf::Vector2f pos = m_pSprite->getPosition();

	//if its out of bounds object will be killed
	if (!(pos.x - 32 * scale< sf::VideoMode::getDesktopMode().width && pos.x > -32 * scale && pos.y + 32 * scale > 0 && pos.y - 32 * scale < sf::VideoMode::getDesktopMode().height))
	{
		m_alive = false;
		return;
	}

	Utils::OffsetVertices(m_hitbox, vec2d(m_step.x,m_step.y));
	m_pSprite->move(m_step);
	window.draw(*m_pSprite);
}
