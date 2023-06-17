#include "Enemy.h"

Enemy::Enemy(const EnemyType type, const sf::Vector2f position, const uint16_t time) : m_type(type)
{
	m_pTexture = new sf::Texture;
	m_pSprite = new sf::Sprite;

	try
	{
		const char* filename = "";
		switch (type)
		{
		case EnemyType::zombie:
			filename = "zombie";
			break;
		case EnemyType::eye:
			filename = "eye";
			break;
		default:
			throw Utils::ErrorLog("Enemy creation: Enemy type out of scope");
			break;
		}

		if (!m_pTexture->loadFromFile(std::format("assets\\{0}.png", filename))) throw Utils::ErrorLog("Enemy creation: texture loading error");
	}
	catch (const bool&)
	{
		return;
	}


	float scale = 0.8 * sf::VideoMode::getDesktopMode().width / 1920.f;

	m_pSprite->setTexture(*m_pTexture);
	m_frame = sf::IntRect(0, 0, 128, 128);
	m_pSprite->setTextureRect(m_frame);
	m_pSprite->setScale(scale, scale);
	m_pSprite->setOrigin(m_pSprite->getGlobalBounds().width / 2.f, m_pSprite->getGlobalBounds().height / 2.f);
	m_pSprite->setPosition(position.x, position.y);

	//setting stats
	if (m_type == EnemyType::zombie)
	{
		m_health = 70 + 2 * time / 30;
		m_damage = 20 + time / 30;
		m_cooldown = 1;
		m_speed = 2;
		m_experienceDropped = 10 + 2 * time / 30;
	}
	else
	{
		m_health = 130 + 5 * time / 30;
		m_damage = 20 + time / 30;
		m_cooldown = 1;
		m_speed = 0.5;
		m_experienceDropped = 20 + 2.2 * time / 30;
	}
	

	//setting hitbox starting from left top vertex
	switch (m_type)
	{
	case EnemyType::zombie:
		//body
		m_hitbox = {
			{46,23},
			{81,23},
			{81,128},
			{46,128}
		};

		//hands
		m_additionalHitbox = {
			{81,30},
			{116,30},
			{116,43},
			{81,43}
		};
		break;
	case EnemyType::eye:
		m_hitbox = {
			{40,6},
			{89,6},
			{123,40},
			{123,89},
			{89,123},
			{40,123},
			{6,89},
			{6,40},
		};
		break;
	default:
		Utils::ErrorLog("Enemy creation, hitbox: type out of bounds should never happen if u see this i have no idea why probably ur fault idk");
		break;
	}

	//scaling, offsetting and centering the hitbox
	for (auto& [x, y] : m_hitbox)
	{
		x = (x - m_pSprite->getGlobalBounds().width / 2.0) * scale + position.x;
		y = (y - m_pSprite->getGlobalBounds().height / 2.0) * scale + position.y;
	}

	//additional hitbox
	for (auto& [x, y] : m_additionalHitbox)
	{
		x = (x - m_pSprite->getGlobalBounds().width / 2.0) * scale + position.x;
		y = (y - m_pSprite->getGlobalBounds().height / 2.0) * scale + position.y;
	}

	if(m_type == EnemyType::eye) m_pAttackTimer = new Utils::Timer(); // zombie doesnt need a timer
	m_pAnimationTimer = new Utils::Timer();
}

Enemy::~Enemy()
{
}

void Enemy::Draw(sf::RenderWindow& window, std::list<std::shared_ptr<Projectile>>& player_projectiles)
{
	sf::Vector2f pos(m_pSprite->getPosition());
	const float scale = 0.8 * sf::VideoMode::getDesktopMode().width / 1920;

	if (!(pos.x - 128 * scale< sf::VideoMode::getDesktopMode().width && pos.x > -128 * scale && pos.y + 128 * scale > 0 && pos.y - 128 * scale < sf::VideoMode::getDesktopMode().height))
	{
		return;
	}

	//checking if enemy got hit
	std::list<std::shared_ptr<Projectile>>::iterator it;
	for (it = player_projectiles.begin(); it != player_projectiles.end(); ++it)
	{
		if (Collision::Intersection(m_hitbox, it->get()->getHitbox()) || Collision::Intersection(m_additionalHitbox, it->get()->getHitbox()))
		{
			Sound::Sounds::ProjectileSplat.Play();
			this->Hit(it->get()->getDamage());
			player_projectiles.erase(it);
		}
	}

	this->Attack(window);

	if (m_type == EnemyType::eye)//only eye shoots
	{
		std::list<std::unique_ptr<Projectile>>::iterator it;
		for (it = m_projectiles.begin(); it != m_projectiles.end(); ++it)
		{
			it->get()->Draw(window);
			if (!it->get()->isAlive()) m_projectiles.erase(it);
		}
	}

	if (m_pAnimationTimer->getElapsedTime<std::chrono::milliseconds>().count() >= 1000)
	{
		m_frame.left = m_frame.left == 0 ? 128 : 0;
		m_pSprite->setTextureRect(m_frame);
		m_pAnimationTimer->Reset();
	}

	window.draw(*m_pSprite);
}

void Enemy::Move()
{
	Vertex playerLocation = { sf::VideoMode::getDesktopMode().width / 2.f, sf::VideoMode::getDesktopMode().height / 2.f };
	Vertex position = { m_pSprite->getPosition().x, m_pSprite->getPosition().y };
	vec2d unitVec = Utils::CalculateUnitVector(position, playerLocation);

	//turning the sprite uin proper direction
	if (unitVec.x < 0) m_facingRight = false;
	else if (unitVec.x > 0) m_facingRight = true;
	if (m_facingRight != m_lastFacingRight)
	{
		m_pSprite->scale(-1.f, 1.f);
		Utils::FlipVerticesX(m_hitbox, position.x);
		if (m_type == EnemyType::zombie) Utils::FlipVerticesX(m_additionalHitbox, position.x);
	}
	m_lastFacingRight = m_facingRight;

	unitVec *= m_speed;//only for calculations i know its not a unit vector anymore 
	//moving the hitbox
	Utils::OffsetVertices(m_hitbox, unitVec);
	//additional hitbox
	Utils::OffsetVertices(m_additionalHitbox, unitVec);
	//moving the enemy
	m_pSprite->move(unitVec.x, unitVec.y);
}

void Enemy::Move(const sf::Vector2f& velocity)
{
	//moving hitbox
	Utils::OffsetVertices(m_additionalHitbox, vec2d(velocity.x, velocity.y));
	//additional hitbox
	Utils::OffsetVertices(m_hitbox, vec2d(velocity.x, velocity.y));
	//moving the enemy
	m_pSprite->move(velocity);
	//moving enemy projectiles
	std::list<std::unique_ptr<Projectile>>::iterator it;
	for (it = m_projectiles.begin(); it != m_projectiles.end(); ++it)
	{
		it->get()->Move(velocity);
	}
}

void Enemy::Attack(sf::RenderWindow& window)
{
	if (m_type == EnemyType::zombie) return; //zombie doesnt shoot

	if (m_pAttackTimer->getElapsedTime<std::chrono::seconds>().count() >= 5)
	{
		m_projectiles.push_back(std::make_unique<Projectile>(ProjectileType::Enemy, m_damage * 1.5, window, m_pSprite->getPosition()));
		m_pAttackTimer->Reset();
	}
}

bool Enemy::DidProjectileHit(const Vertices& player_hitbox)
{
	if(m_type == EnemyType::zombie) return false;

	std::list<std::unique_ptr<Projectile>>::iterator it;
	for (it = m_projectiles.begin(); it != m_projectiles.end(); ++it)
	{
		if (Collision::Intersection(it->get()->getHitbox(), player_hitbox))
		{
			m_projectiles.erase(it);
			return true;
		}
	}
	return false;
}
