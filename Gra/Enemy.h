#pragma once
#include "Living.h"
#include <list>
#include "Projectile.h"
#include <format>
#include "Sounds.h"

constexpr int eyeChance = 20; // 1:x

enum class EnemyType
{
	zombie = 0,
	eye,
};

class Enemy final : public Living
{
public:
	Enemy(const EnemyType type, const sf::Vector2f position, const uint16_t time);//time is in sec
	~Enemy();
	void Draw(sf::RenderWindow& window,std::list<std::shared_ptr<Projectile>>& player_projectiles);
	//move in players direction
	void Move();
	void Move(const sf::Vector2f& velocity);//for when player moves
	void Attack(sf::RenderWindow& window) override;
	bool DidProjectileHit(const Vertices& player_vertices);
	inline double getExperienceDropped() const { return m_experienceDropped; };
	inline Vertices getAdditionalHitbox() { return m_additionalHitbox; };
private:
	std::list<std::unique_ptr<Projectile>> m_projectiles = {}; //if there are any
	double m_experienceDropped = 0;
	Vertices m_additionalHitbox = {};//only for zombie
	EnemyType m_type;

};

