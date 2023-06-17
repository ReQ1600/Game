#include "Level.h"
#include <SFML/Graphics.hpp>
#include "Random.h"
#include "Sounds.h"

void Level::Draw(sf::RenderWindow& window) const
{
	if (m_timer->getElapsedTime<std::chrono::minutes>().count() >= 10) m_state = GameState::Won;
	else if (m_player->getHealth() <= 0) m_state = GameState::Lost;
	float scale = sf::VideoMode::getDesktopMode().width / 1920;
	sf::Vector2f pos;

	for (int i = 0; i < MAP_SIZE / 32; ++i)
	{
		for (int j = 0; j < MAP_SIZE / 32; ++j)
		{
			pos = m_backgroundSprites[i*NUMBER_OF_SPRITES+j]->getPosition();

			//only drawing sprites in view
			if (pos.x - 32 * scale< sf::VideoMode::getDesktopMode().width && pos.x > -32 * scale && pos.y + 32 * scale > 0 && pos.y - 32 * scale < sf::VideoMode::getDesktopMode().height)
				window.draw(*m_backgroundSprites[i*NUMBER_OF_SPRITES+j]);
		}
	}
	
	m_player->Draw(window);

	this->SpawnEnemy();

	std::list<std::unique_ptr<Enemy>>::const_iterator it;
	for (it = m_enemies.begin(); it != m_enemies.end(); ++it)
	{
		if (it->get()->isAlive())
		{
			it->get()->Move();
			it->get()->Draw(window, m_player->projectiles);
			if (Collision::Intersection(it->get()->getHitbox(),m_player->getHitbox()) ||
				Collision::Intersection(it->get()->getAdditionalHitbox(), m_player->getHitbox()) ||
				it->get()->DidProjectileHit(m_player->getHitbox()))
			{
				if (m_PlayerHitTimer->getElapsedTime<std::chrono::seconds>().count() >= 1)
				{
					Sound::Sounds::PlayerHitSound.Play();

					m_player->Hit(it->get()->getDamage());
					m_pPlayerLifeText->setString(std::to_string(m_player->getHealth()));
					m_PlayerHitTimer->Reset();
				}
			}
		}
		else
		{
			Sound::Sounds::EnemyDeathSound.Play();
			m_player->AddExp(it->get()->getExperienceDropped());
			m_pPlayerExpText->setString(std::to_string(m_player->getExpPoints()));
			m_pPlayerLevelText->setString("LVL: " + std::to_string(m_player->getLevel()));
			m_enemies.erase(it);
			m_money += 2;
		}
	}

	window.draw(*m_pPlayerExpText);
	window.draw(*m_pPlayerLifeText);
	window.draw(*m_pPlayerLevelText);

	//timer
	uint8_t seconds = m_timer->getElapsedTime<std::chrono::seconds>().count() % 60;
	uint8_t minutes = m_timer->getElapsedTime<std::chrono::minutes>().count();
	m_pTimerText->setString(std::format("{0}{1}:{2}{3}", minutes / 10, minutes % 10, seconds / 10, seconds % 10));
	window.draw(*m_pTimerText);

}

Level::Level(Utils::Save::SaveFile* save, CharacterType chara, CharacterData data)
{
	m_pFont = new sf::Font;
	m_pBackgroundTexture = new sf::Texture;
	m_pMoneyText = new sf::Text;
	m_pTimerText = new sf::Text;
	m_player = std::make_unique<Player>(chara, data, save->getSaveData().shop, sf::Vector2f(MAP_SIZE / 2.f, MAP_SIZE / 2.f));
	m_pPlayerLifeText = new sf::Text;
	m_pPlayerExpText = new sf::Text;
	m_pPlayerLevelText = new sf::Text;

	m_backgroundSprites.resize(NUMBER_OF_SPRITES * NUMBER_OF_SPRITES);//row*col

	try
	{
		if (!m_pBackgroundTexture->loadFromFile("assets\\grass.png")) throw Utils::ErrorLog("grass texture loading error");
		if (!m_pFont->loadFromFile("assets\\Font.ttf")) throw Utils::ErrorLog("Button font loading error");
	}
	catch (const bool&)
	{
		return;
	}
	
	float scale = sf::VideoMode::getDesktopMode().width / 1920.f;
	const std::pair<int, int> offset(sf::VideoMode::getDesktopMode().width / 2 - MAP_SIZE / 2, sf::VideoMode::getDesktopMode().height / 2 - MAP_SIZE / 2);

	for (int i = 0; i < NUMBER_OF_SPRITES; ++i)
	{
		for (int j = 0; j < NUMBER_OF_SPRITES; ++j)
		{
			m_backgroundSprites[i*NUMBER_OF_SPRITES+j] = std::make_unique<sf::Sprite>(sf::Sprite());
			m_backgroundSprites[i*NUMBER_OF_SPRITES+j]->setTexture(*m_pBackgroundTexture);
			m_backgroundSprites[i*NUMBER_OF_SPRITES+j]->setPosition(i * 32 * scale + offset.first, j * 32 * scale + offset.second);
		}
	}

	//timer
	m_pTimerText->setFont(*m_pFont);
	m_pTimerText->setString("00:00");
	m_pTimerText->setCharacterSize(40 * scale);
	m_pTimerText->setPosition(20 * scale, 0);

	//health display
	m_pPlayerLifeText->setFont(*m_pFont);
	m_pPlayerLifeText->setString(std::to_string(m_player->getHealth()));
	m_pPlayerLifeText->setCharacterSize(40 * scale);
	m_pPlayerLifeText->setOrigin(m_pPlayerLifeText->getGlobalBounds().left + m_pPlayerLifeText->getGlobalBounds().width / 2, m_pPlayerLifeText->getOrigin().y);
	m_pPlayerLifeText->setPosition(sf::VideoMode::getDesktopMode().width/2, 0);

	//exp Display
	m_pPlayerExpText->setFont(*m_pFont);
	m_pPlayerExpText->setString(std::to_string(m_player->getExpPoints()));
	m_pPlayerExpText->setCharacterSize(40 * scale);
	m_pPlayerExpText->setPosition(sf::VideoMode::getDesktopMode().width - 6 * m_pPlayerExpText->getGlobalBounds().width, 0);

	//lvl display
	m_pPlayerLevelText->setFont(*m_pFont);
	m_pPlayerLevelText->setString("LVL: " + std::to_string(m_player->getLevel()));
	m_pPlayerLevelText->setCharacterSize(40 * scale);
	m_pPlayerLevelText->setPosition(m_pPlayerExpText->getGlobalBounds().left - 8 * m_pPlayerExpText->getGlobalBounds().width, 0);

	m_PlayerHitTimer = new Utils::Timer();
	m_EnemySpawnTimer = new Utils::Timer();
	m_timer = new Utils::Timer();
}

void Level::Move(sf::Vector2f& direction)
{
	vec2d velocity;
	if (direction.x != 0 && direction.y != 0) velocity = { direction.x * m_player->getSpeed(), direction.y * m_player->getSpeed() };
	else  velocity = { m_player->getSpeed() * direction.x * 1.4, m_player->getSpeed() * direction.y * 1.4 }; // 1.4 is a very rough approximation of sqrt of 2

	sf::Vector2f tmp = { (float)velocity.x,(float)velocity.y };

	if (m_player->Move(tmp, { MAP_SIZE,MAP_SIZE }))
	{
		for (auto& sprite : m_backgroundSprites)
		{
			sprite->move(tmp);
		}

		std::list<std::unique_ptr<Enemy>>::const_iterator it;
		for (it = m_enemies.begin(); it != m_enemies.end(); ++it)
		{
			it->get()->Move(tmp);
		}
	}
}

void Level::SpawnEnemy() const
{
	if (m_EnemySpawnTimer->getElapsedTime<std::chrono::milliseconds>().count() >= 2000.0 - 2.3 * m_timer->getElapsedTime<std::chrono::seconds>().count())
	{
		Utils::Random& r = Utils::Random::Get();
		sf::Vector2f windowBounds(sf::VideoMode().getDesktopMode().width,sf::VideoMode::getDesktopMode().height);
		auto& [x, y] = windowBounds;
		sf::Vector2f playerPos(x / 2, y / 2);
		float scale = 0.8 * sf::VideoMode::getDesktopMode().width / 1920.f;
		sf::Vector2f pos;
		
		//checking if distance between newly spawned enemy and player would be larger than distance between player and top left corner so that enemies spawn offscreen
		do
		{
			pos.x = (float)r.RandomNumber<int>((int)(-256 * scale), (int)(x + 256 * scale));
			pos.y = (float)r.RandomNumber<int>((int)(-256 * scale), y + (int)(256 * scale));
		} while (Utils::Distance2Points(pos, playerPos) < Utils::Distance2Points(playerPos, vec2d(windowBounds.x, windowBounds.y))); // i know its not a perfect solution but my sanity reserves 		
		
		EnemyType type = r.RandomNumber<int>(0, 8) == 1 ? EnemyType::eye : EnemyType::zombie;//1:9 chance that enemy is going to be an eye
		m_enemies.push_back(std::make_unique<Enemy>(type,pos,m_timer->getElapsedTime<std::chrono::seconds>().count()));
		m_EnemySpawnTimer->Reset();
	}
}
