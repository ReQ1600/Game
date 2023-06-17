#include "ShopScreen.h"
#include "format"
#include "Sounds.h"

#pragma region btn

Shop::Button::Button(ButtonType type, const vec2d& position, const float scale) : m_type(type), ButtonBase(position,
	[&type]() -> uint32_t { if (type == ButtonType::Exit) return 1050; return 64; }(), //making texture rectangle the right size
	[&type]() -> uint32_t { if (type == ButtonType::Exit) return 180; return 64; }(),
	[&type]() -> const char* {if (type == ButtonType::Exit) return "Back"; return ""; }(), scale,
	//setting file path
	[&type]() -> const char* {
		const char* fname = NULL;
		switch (type)
		{
		case Shop::ButtonType::Plus:
			fname = "Plus";
			break;
		case Shop::ButtonType::Minus:
			fname = "Minus";
			break;
		case Shop::ButtonType::Exit:
			fname = "button";
			break;
		default:
			Utils::Log("shop button type out of bounds");
			break;
		}
		return fname;
	}())
{}//to niez³y konstruktor

Shop::Button::~Button()
{
}

void Shop::Button::draw(sf::RenderWindow& window)
{
	sf::Mouse mouse;
	Vertex mouseVertex(mouse.getPosition(window).x, mouse.getPosition(window).y);
	Vertices mouseVertices = { mouseVertex };

	if (Collision::Intersection(this->getHitbox(), mouseVertices))
	{
		frame.left = (m_type == ButtonType::Exit) ? 1050 : 64;
	}
	else frame.left = 0;

	sprite->setTextureRect(frame);

	window.draw(*sprite);
	window.draw(this->getText());

}

#pragma endregion

#pragma region upgrade
Shop::Upgrade::Upgrade(UpgradeName name, vec2d& position, const float& scale)
{
	std::string filename;
	switch (name)
	{
	case Shop::UpgradeName::Health:
		filename = "hp";
		this->m_name.setString("Health Up");
		break;
	case Shop::UpgradeName::Damage:
		filename = "dmg";
		this->m_name.setString("Damage Up");
		break;
	case Shop::UpgradeName::Speed:
		filename = "speed";
		this->m_name.setString("Speed Up");
		break;
	case Shop::UpgradeName::Money:
		filename = "money";
		this->m_name.setString("Money mult");
		break;
	case Shop::UpgradeName::Experience:
		filename = "exp";
		this->m_name.setString("Experience mult");
		break;
	case Shop::UpgradeName::Cooldown:
		filename = "cooldown";
		this->m_name.setString("Cooldown redu");
		break;
	default:
		break;
	}

	try
	{
		if (!m_texture.loadFromFile(std::format("assets\\{0}.png", filename))) throw Utils::ErrorLog("Shop button position out of bounds");
		if (!m_font.loadFromFile("assets\\Font.ttf")) throw Utils::ErrorLog("Button font loading error");
		if (position.y > sf::VideoMode::getDesktopMode().height || position.y < 0) throw Utils::ErrorLog("Shop button y out of bounds");
		if (position.x > sf::VideoMode::getDesktopMode().width || position.x < 0) throw Utils::ErrorLog("Shop button x out of bounds");
	}
	catch (const bool& e)
	{
		return;
	}

	m_sprite.setTexture(m_texture);
	sf::IntRect frame = (sf::IntRect(0, 0, 32, 32));
	m_sprite.setTextureRect(frame);

	m_sprite.setScale(4 * scale, 4 * scale);
	m_sprite.setOrigin(m_sprite.getTextureRect().width / 2.f, m_sprite.getTextureRect().height / 2.f);
	m_sprite.setPosition(position.x, position.y);

	//buttons
	float btnScale = 0.5;

	vec2d pos = { m_sprite.getGlobalBounds().left + btnScale * 64 / 4 ,m_sprite.getGlobalBounds().top + m_sprite.getGlobalBounds().height * 1.25 };
	m_upMinus = std::make_unique<Button>(ButtonType::Minus, pos, btnScale);

	pos.x += m_sprite.getGlobalBounds().width - btnScale * 64 / 2;
	m_upPlus = std::make_unique<Button>(ButtonType::Plus, pos, btnScale);

	//text
	this->m_name.setFont(m_font);
	this->m_name.setScale(scale, scale);
	this->m_name.setOrigin(this->m_name.getGlobalBounds().left + this->m_name.getGlobalBounds().width / 2.f, this->m_name.getGlobalBounds().top + this->m_name.getGlobalBounds().height / 2.f);
	this->m_name.setPosition(m_sprite.getPosition().x, m_sprite.getPosition().y - m_sprite.getGlobalBounds().height * 0.7);

	m_amount.setFont(m_font);
	m_amount.setString(std::to_string(m_amountBought));
	m_amount.setOrigin(m_amount.getGlobalBounds().left + m_amount.getGlobalBounds().width / 2.f, m_amount.getGlobalBounds().top + m_amount.getGlobalBounds().height / 2.f);
	m_amount.setPosition(m_sprite.getPosition().x, pos.y);

	//money
	m_priceTxt.setFont(m_font);
	m_priceTxt.setString(std::to_string(m_price));
	m_priceTxt.setOrigin(m_priceTxt.getGlobalBounds().left + m_priceTxt.getGlobalBounds().width / 2.f, m_priceTxt.getGlobalBounds().top + m_priceTxt.getGlobalBounds().height / 2.f);
	m_priceTxt.setPosition(m_sprite.getPosition().x, m_upPlus->getSprite().getPosition().y + m_sprite.getGlobalBounds().height * 0.3);
}

Shop::Upgrade::~Upgrade()
{
}

void Shop::Upgrade::Draw(sf::RenderWindow& window)
{
	m_upPlus->draw(window);
	m_upMinus->draw(window);
	window.draw(m_priceTxt);

	window.draw(m_amount);
	window.draw(m_name);
	window.draw(m_sprite);
}

void Shop::Upgrade::ButtonPressed(uint32_t& money)
{
	if (m_upPlus->mouseOnButton())
	{
		Sound::Sounds::buttonSound.Play();

		if (money >= m_price && m_amountBought < m_max_amnt)
		{
			money -= m_price;
			m_price += m_initial_price;
			m_priceTxt.setString(std::to_string(m_price));
			m_amountBought++;
			m_amount.setString(std::to_string(m_amountBought));

			m_priceTxt.setOrigin(m_priceTxt.getGlobalBounds().width / 2.f, m_priceTxt.getGlobalBounds().height / 2.f); //origin needs to change bc text changes  in length
		}
	}
	else if (m_upMinus->mouseOnButton())
	{
		Sound::Sounds::buttonSound.Play();

		if (m_amountBought > 0)
		{
			Sound::Sounds::buttonSound.Play();

			money += m_price - m_initial_price;
			m_price -= m_initial_price;
			m_priceTxt.setString(std::to_string(m_price));
			m_amountBought--;
			m_amount.setString(std::to_string(m_amountBought));

			m_priceTxt.setOrigin(m_priceTxt.getGlobalBounds().width / 2.f, m_priceTxt.getGlobalBounds().height / 2.f);
		}
	}
	
}

void Shop::Upgrade::setAmountBought(uint8_t amnt)
{
	m_amountBought = amnt;
	m_amount.setString(std::to_string(amnt));
	m_price = (m_amountBought + 1) * m_initial_price;
	m_priceTxt.setString(std::to_string(m_price));
	m_priceTxt.setOrigin(m_priceTxt.getGlobalBounds().width / 2.f, m_priceTxt.getGlobalBounds().height / 2.f);
}

#pragma endregion

#pragma region scr
Shop::Screen Shop::Screen::s_Instance;

void Shop::Screen::UpdateMoney()
{
	m_money = pSave->getSaveData().PlayerMoney;
	m_moneyTxt.setString(std::format("money: {0}", m_money));
}

Shop::Screen::Screen()
{
	float exit_scale = 0.3 * 1920 / sf::VideoMode::getDesktopMode().width;
	vec2d pos = {1050 * exit_scale / 2,sf::VideoMode::getDesktopMode().height - 180 * exit_scale / 2};//sprite is 1050x180
	m_upExitButton = std::make_unique<Button>(ButtonType::Exit, pos, exit_scale);

	const float upgrades_scale = sf::VideoMode::getDesktopMode().width / 1920.f;
	std::pair segment = std::make_pair<double,double>(1.f / 8 * sf::VideoMode::getDesktopMode().width, 1.f / 4 * sf::VideoMode::getDesktopMode().height);
	pos = { 3 * segment.first, segment.second };

	//loading the save
	pSave = new Utils::Save::SaveFile;
	pSave->Load();
	Utils::Save::Data save;
	save = pSave->getSaveData();

	//moneymoneymoneymoneymoneymoneymoneymoneymoneymoneymoneymoneymoneymoneymoneymoneymoneymoneymoney
	m_money = save.PlayerMoney;

	m_moneyTxt.setFont(*m_upExitButton->getFont());
	m_moneyTxt.setString(std::format("money: {0}", m_money));
	m_moneyTxt.setScale(1.3 * upgrades_scale, 1.3 * upgrades_scale);
	m_moneyTxt.setPosition(60 * exit_scale,0);

	//upgrades
	m_upgrades.resize(6);

	m_upgrades[0] = std::make_unique<Upgrade>(UpgradeName::Health, pos, upgrades_scale);
	m_upgrades[0]->setAmountBought(save.shop.Health);
	pos.x = 4 * segment.first;
	m_upgrades[1] = std::make_unique<Upgrade>(UpgradeName::Damage, pos, upgrades_scale);
	m_upgrades[1]->setAmountBought(save.shop.Damage);
	pos.x = 5 * segment.first;
	m_upgrades[2] = std::make_unique<Upgrade>(UpgradeName::Speed, pos, upgrades_scale);
	m_upgrades[2]->setAmountBought(save.shop.Speed);

	pos.y = 2 * segment.second;
	m_upgrades[5] = std::make_unique<Upgrade>(UpgradeName::Cooldown, pos, upgrades_scale);
	m_upgrades[5]->setAmountBought(save.shop.Cooldown);
	pos.x = 4 * segment.first;
	m_upgrades[4] = std::make_unique<Upgrade>(UpgradeName::Experience, pos, upgrades_scale);
	m_upgrades[4]->setAmountBought((uint8_t)save.shop.Experience);
	pos.x = 3 * segment.first;
	m_upgrades[3] = std::make_unique<Upgrade>(UpgradeName::Money, pos, upgrades_scale);
	m_upgrades[3]->setAmountBought(save.shop.Money);
}

Shop::Screen::~Screen()
{
	delete pSave;
}

Shop::Screen& Shop::Screen::Get()
{

	return s_Instance;
}

void Shop::Screen::Draw(sf::RenderWindow& window) const
{
	m_upExitButton->draw(window);
	window.draw(m_moneyTxt);

	for (auto& item : m_upgrades)
	{
		item->Draw(window);
	}
}

//returns true if exit is pressed handles all button presses on screen
bool Shop::Screen::ButtonHandler()
{
	if (m_upExitButton->mouseOnButton())
	{
		Sound::Sounds::buttonSound.Play();

		Utils::Save::Data tmp;
		tmp.PlayerMoney = m_money;
		tmp.shop.Health = m_upgrades[0]->getAmountBought();
		tmp.shop.Damage = m_upgrades[1]->getAmountBought();
		tmp.shop.Speed = m_upgrades[2]->getAmountBought();
		tmp.shop.Money = m_upgrades[3]->getAmountBought();
		tmp.shop.Experience = m_upgrades[4]->getAmountBought();
		tmp.shop.Cooldown= m_upgrades[5]->getAmountBought();

		pSave->setSaveData(tmp);
		pSave->Write();
		return true;
	}

	for (auto& upgrade : m_upgrades)
	{
		upgrade->ButtonPressed(m_money);
		m_moneyTxt.setString(std::format("money: {0}", m_money));
	}

	return false;
}
#pragma endregion

