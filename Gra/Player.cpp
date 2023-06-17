#include "Player.h"
#include <format>


Player::Player(CharacterType chara, CharacterData& data, Utils::Save::Shop upgrades, sf::Vector2f position) : m_type(chara)
{
	m_pTexture = new sf::Texture;
	const char* name = "";
	if (chara == CharacterType::Knight)
	{
		name = "Knight";
	}
	else if (chara == CharacterType::Witch)
	{
		name = "Witch";
	}
	else
	{
		name = "placeholder";
	}
	try
	{
		if (!m_pTexture->loadFromFile(std::format("assets\\{0}.png",name))) throw Utils::ErrorLog("character creation: texture loading error");

	}
	catch (const bool&)
	{
		return;
	}



	double scale = 0.8 * sf::VideoMode::getDesktopMode().width / 1920.f;
	
m_pSprite = new sf::Sprite(*m_pTexture);
	m_frame = sf::IntRect(0,0,128,128);
	m_pSprite->setTextureRect(m_frame);
	m_pSprite->setScale(scale,scale);
	m_pSprite->setOrigin(m_pSprite->getGlobalBounds().width / 2.f, m_pSprite->getGlobalBounds().height / 2.f);
	m_pSprite->setPosition(sf::VideoMode::getDesktopMode().width / 2.f, sf::VideoMode::getDesktopMode().height / 2.f);

	//setting hitbox starting from top left head vertex and going clockwise. The hitbox is very simplyfied for better performance and accuracy bc if i wanted better accuracy id need to create more hitboxes and combine them bc sat algorith cant process concave things and i dont have time for that
	//knight
	switch (chara)
	{
	case CharacterType::Knight:
		m_hitbox = {
		{52,1},
		{78,1},
		{81,33},
		{81,128},
		{44,128},
		{37,93},
		{37,34},
		};
		break;
	case CharacterType::Witch:
		m_hitbox = {
			{60,6},
			{71,6},
			{80,29},
			{80,128},
			{53,128},
			{46,118},
			{46,27}
		};
		break;
	case CharacterType::MissingNo:
		m_hitbox = {
			{32,32},
			{96,32},
			{96,96},
			{32,96}
		};
		break;
	default:
		break;
	}
	
	//scaling, offsetting and centering the hitbox
	for (auto& [x,y] : m_hitbox)

	{
		x = (x - m_pSprite->getGlobalBounds().width/2.0) * scale + sf::VideoMode::getDesktopMode().width / 2.f;
		y = (y - m_pSprite->getGlobalBounds().height/2.0) * scale + sf::VideoMode::getDesktopMode().height / 2.f;
	}

	//setting stats
	m_health = data.Health + 5 * upgrades.Health;
	m_damage = data.Damage + 2 * upgrades.Damage;
	m_cooldown = (double)data.Cooldown - 0.2 * upgrades.Cooldown;
	m_speed = (double)data.Speed + 0.5 * upgrades.Speed;
	m_expMult += 0.3 * upgrades.Experience;

	m_position = position;

	m_pAttackTimer = new Utils::Timer();
	m_pAnimationTimer = new Utils::Timer();
}

Player::~Player()
{
}

void Player::Draw(sf::RenderWindow& window)
{
	std::list<std::shared_ptr<Projectile>>::iterator it;
	for (it = projectiles.begin(); it != projectiles.end(); ++it)
	{
		it->get()->Draw(window);
		if (!it->get()->isAlive()) projectiles.erase(it);
	}

	if (m_pAnimationTimer->getElapsedTime<std::chrono::milliseconds>().count() >= 1500)
	{
		m_frame.left = m_frame.left == 0 ? 128 : 0;
		m_pSprite->setTextureRect(m_frame);
		m_pAnimationTimer->Reset();
	}
	window.draw(*m_pSprite);
}

void Player::AddExp(double points)
{
	m_experience += points * m_expMult;
	if (m_experience >= m_expToLvlUp)
	{
		LevelUp();
		m_experience -= m_expToLvlUp;
		m_expToLvlUp *= 2;
	}
}

bool Player::Move(const sf::Vector2f& velocity, sf::Vector2i bounds)
{
	//changing where playyer is looking
	if (velocity.x > 0) m_facingRight = false;
	else if (velocity.x < 0) m_facingRight = true;
	if (m_facingRight != m_lastFacingRight)
	{
		m_pSprite->scale(-1.f, 1.f);
		Utils::FlipVerticesX(m_hitbox, sf::VideoMode::getDesktopMode().width / 2.f);
	}
	m_lastFacingRight = m_facingRight;

	this->m_position -= velocity;//minus bc tiles are moving in opposite dir to player

	//checking if out of bounds
	if (this->m_position.x <= 0 || this->m_position.x >= bounds.x || this->m_position.y <= 0 || this->m_position.y >= bounds.y - this->m_pSprite->getGlobalBounds().height / 2.f)
	{
		this->m_position += velocity;
		return false;
	}
	else
	{
		std::list<std::shared_ptr<Projectile>>::iterator it;
		for (it = projectiles.begin(); it != projectiles.end(); ++it)
		{
			it->get()->Move(velocity);
		}
		return true;
	}
}

void Player::Attack(sf::RenderWindow& window)
{
	if (m_pAttackTimer->getElapsedTime<std::chrono::milliseconds>().count() >= m_cooldown * 80)
	{
		Sound::Sounds::PlayerProjectileThrow.Play();
		ProjectileType tmp = m_type == CharacterType::Witch ? ProjectileType::Witch : ProjectileType::Knight;
		projectiles.push_back(std::make_unique<Projectile>(tmp, m_damage,window));
		m_pAttackTimer->Reset();
	}
}
