#pragma once
#include <cstdint>
#include "SFML/Graphics.hpp"
#include "Collision.h"

enum class ProjectileType
{
	Knight = 0,
	Witch,
	Enemy
};

class Projectile
{
public:
	Projectile(ProjectileType type, uint32_t damage, sf::RenderWindow& window, sf::Vector2f = {});
	~Projectile() { delete m_pTexture, m_pSprite; };
	void Draw(sf::RenderWindow& window);
	inline void Move(const sf::Vector2f& velocity) { Utils::OffsetVertices(m_hitbox,vec2d(velocity.x,velocity.y)); m_pSprite->move(velocity); };
	inline uint32_t getDamage() const { return m_damage; };
	inline uint16_t getVelocity() const { return m_velocity; };
	inline sf::Vector2f getPosition() const { return m_pSprite->getPosition(); };
	inline Vertices getHitbox() const { return m_hitbox; };
	inline bool isAlive() const { return m_alive; };
	inline void Kill() { m_alive = false; };
private:
	bool m_alive = true;
	ProjectileType m_type;
	int8_t m_velocity = 0;
	sf::Vector2f m_step = {0, 0}; //every time its drawn it moves one step
	uint32_t m_damage;
	Vertices m_hitbox = {};
	sf::Texture* m_pTexture = nullptr;
	sf::Sprite* m_pSprite = nullptr;
};

