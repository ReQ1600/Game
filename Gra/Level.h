#pragma once
#include "Screen.h"
#include <list>
#include "Enemy.h"
#include "Player.h"
#include "Timer.h"
#include "Save.h"
#include <format>
#include "CharacterDataReader.h"
#include "EndScreen.h"

constexpr uint16_t MAP_SIZE = 10016;//in px, the map is alway a square
constexpr int16_t NUMBER_OF_SPRITES = MAP_SIZE / 32;//in one row

class Level : public IScreen
{
public:
	void Draw(sf::RenderWindow& window) const override;
	Level(Utils::Save::SaveFile* save, CharacterType chara, CharacterData data);
	~Level() { delete m_pMoneyText, m_pTimerText, m_pBackgroundTexture, m_pFont; };
	void Move(sf::Vector2f& direction);
	inline void MouseButtonPress(sf::RenderWindow& window) { m_player->Attack(window); };
	inline uint32_t getMoney() const { return m_money; };
	inline GameState getGameState() const { return m_state; };
private:
	//tries to spawn an enemy
	void SpawnEnemy() const;
private:
	mutable GameState m_state = GameState::InProgress;
	mutable std::list<std::unique_ptr<Enemy>> m_enemies;
	std::unique_ptr<Player> m_player;
	sf::Font* m_pFont = nullptr;
	Utils::Timer* m_EnemySpawnTimer = nullptr;
	Utils::Timer* m_timer = nullptr;
	//measures time from last damage dealt to player
	Utils::Timer* m_PlayerHitTimer = nullptr;
	sf::Text* m_pTimerText = nullptr;
	sf::Text* m_pPlayerLifeText = nullptr;
	sf::Text* m_pPlayerExpText = nullptr;
	sf::Text* m_pPlayerLevelText = nullptr;
	mutable uint32_t m_money = 0;
	sf::Text* m_pMoneyText = nullptr;
	sf::Texture* m_pBackgroundTexture = nullptr;//only one texture will be used
	std::vector<std::unique_ptr<sf::Sprite>> m_backgroundSprites;//to get desired element [row*NUMBER_OF_SPRITES+column]

};

