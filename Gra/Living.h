#pragma once
#include "Utils.h"
#include "SFML/Graphics.hpp"
#include "Timer.h"

class Living //enemies/player
{
public:
	Living() {};
	virtual ~Living() { delete m_pAttackTimer, m_pAnimationTimer, m_pTexture, m_pSprite; };
	void Move(const vec2c& velocity);
	inline void Hit(const uint32_t damage) { m_health -= damage; if (m_health < 0) m_health = 0; if (m_health <= 0) m_alive = false; };
	//getters
	inline int32_t getHealth() const { return m_health; };
	inline sf::Vector2f getPosition() const { return m_position; };
	inline bool isAlive() const { return m_alive; }
	inline Vertices getHitbox() const { return m_hitbox; };
	inline uint32_t getDamage() const { return m_damage; };
	virtual void Attack(sf::RenderWindow& window) = 0;
protected:
	Utils::Timer* m_pAnimationTimer = nullptr;
	Utils::Timer* m_pAttackTimer = nullptr;
	sf::Vector2f m_position = {0, 0};
	sf::Texture* m_pTexture = nullptr;
	sf::Sprite* m_pSprite = nullptr;
	sf::IntRect m_frame;
	Vertices m_hitbox = {};
	int32_t m_health = 0;
	uint32_t m_damage = 0;
	bool m_facingRight = true;
	bool m_lastFacingRight = true;
	double m_speed = 0;
	double m_cooldown = 0;
	bool m_alive = true;
};