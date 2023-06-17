#pragma once
#include "Living.h"
#include "Save.h"
#include "CharacterDataReader.h"
#include "Projectile.h"
#include <list>
#include "Sounds.h"

enum class CharacterType
{
	Knight = 0,
	Witch,
	MissingNo,
};

class Player final : public Living
{
public:
	Player(CharacterType chara, CharacterData& data , Utils::Save::Shop upgrades, sf::Vector2f position);
	~Player();
	void Draw(sf::RenderWindow& window);
	void AddExp(double points);
	bool Move(const sf::Vector2f& velocity, sf::Vector2i bounds); //returns true if player can move
	inline void LevelUp() { this->m_damage += 2; ++m_level; };
	void Attack(sf::RenderWindow& window) override;
	//getters
	inline uint32_t getExpPoints() const { return m_experience; };
	inline uint16_t getLevel() const { return m_level; };
	inline double getSpeed() const { return m_speed; };
	inline double getCooldown() const { return m_cooldown; };
	inline uint32_t getProjectileNumber() { return projectiles.size(); };
	std::list<std::shared_ptr<Projectile>> projectiles;
private:
	CharacterType m_type = CharacterType::MissingNo;
	double m_experience = 0;
	double m_expMult = 1;
	uint16_t m_level = 0;
	uint16_t m_expToLvlUp = 100;

};