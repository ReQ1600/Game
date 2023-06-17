#include "CharacterSelectionScreen.h"
#include <format>
#include "Sounds.h"

CharacterSelectionScreen::Screen CharacterSelectionScreen::Screen::s_Instance;

#pragma region CharaBtn
CharacterSelectionScreen::CharaButton::CharaButton(const Character chara, const vec2d& position, const float& scale) : character(chara), ButtonBase(position, 128, 128, "", scale,
		//setting file name
		[&chara]()->const char* {
			const char* characterName = NULL;
			switch (chara)
			{
			case Character::Knight:
				characterName = "Knight";
				break;
			case Character::Witch:
				characterName = "Witch";//placeholder
				break;
			case Character::MissingNo:
				characterName = "placeholder";
				break;
			default:
				std::cout << "Character number out of scope" << std::endl;
				break;
			}
			return characterName;
		}())

		//constructor starts here
	{
		m_border.setOutlineThickness(3 * scale);
		m_border.setOutlineColor(sf::Color::White);
		m_border.setFillColor(sf::Color::Transparent);
		m_border.setSize({ sprite->getTextureRect().width * 1.2f, sprite->getTextureRect().height * 1.2f });
		m_border.setOrigin(m_border.getGlobalBounds().width / 2.f, m_border.getGlobalBounds().height / 2.f);
		m_border.setPosition(position.x, position.y);

		Vertices hitbox = {
			{m_border.getPosition().x - m_border.getGlobalBounds().width / 2.f ,m_border.getPosition().y - m_border.getGlobalBounds().height / 2.f },
			{m_border.getPosition().x + m_border.getGlobalBounds().width / 2.f ,m_border.getPosition().y - m_border.getGlobalBounds().height / 2.f },
			{m_border.getPosition().x + m_border.getGlobalBounds().width / 2.f ,m_border.getPosition().y + m_border.getGlobalBounds().height / 2.f },
			{m_border.getPosition().x - m_border.getGlobalBounds().width / 2.f ,m_border.getPosition().y + m_border.getGlobalBounds().height / 2.f }
		};
		this->setHitbox(hitbox);

		m_name.setFont(*this->getFont());
		m_name.setString(this->getFilename());
		m_name.setOrigin(m_name.getGlobalBounds().left + m_name.getGlobalBounds().width / 2.f, m_name.getGlobalBounds().top + m_name.getGlobalBounds().height / 2.f);
		m_name.setScale(scale, scale);
		m_name.setPosition(m_border.getPosition().x, m_border.getPosition().y / 2);
	}

	void CharacterSelectionScreen::CharaButton::draw(sf::RenderWindow& window)
	{
		sf::Mouse mouse;
		Vertex mouseVertex(mouse.getPosition(window).x, mouse.getPosition(window).y);
		Vertices mouseVertices = { mouseVertex };

		if (Collision::Intersection(this->getHitbox(), mouseVertices))
		{
			m_border.setOutlineColor(sf::Color::Yellow);

			if (animationCooldown >= 1500)
			{
				frame.left = (frame.left == 0) ? 128 : 0;
				animationCooldown = 0;
			}
		}
		else
		{
			m_border.setOutlineColor(sf::Color::White);
		}

		animationCooldown++;
		sprite->setTextureRect(frame);


		window.draw(*sprite);
		window.draw(m_border);
		window.draw(m_name);
	}
#pragma endregion

#pragma region Button
	void CharacterSelectionScreen::Button::draw(sf::RenderWindow& window)
	{
		sf::Mouse mouse;
		Vertex mouseVertex(mouse.getPosition(window).x, mouse.getPosition(window).y);
		Vertices mouseVertices = { mouseVertex };

		if (Collision::Intersection(this->getHitbox(), mouseVertices)) frame.left = 1050;
		else frame.left = 0;

		sprite->setTextureRect(frame);

		window.draw(*sprite);
		window.draw(this->getText());
	}
#pragma endregion


#pragma region screen

	CharacterSelectionScreen::Screen::Screen()
	{
		Utils::CharacterDataReader reader;
		m_dataOfCharacters.resize(3);

		m_dataOfCharacters[0] = std::make_unique<CharacterData>(reader.Read("data\\Knight.dat"));
		m_dataOfCharacters[1] = std::make_unique<CharacterData>(reader.Read("data\\Witch.dat"));
		m_dataOfCharacters[2] = std::make_unique<CharacterData>(reader.Read("data\\MissingNo.dat"));

		m_buttons.resize(3);
		float scale = sf::VideoMode::getDesktopMode().width / 1920.f;
		double y = 0.25 * sf::VideoMode::getDesktopMode().height; //y same for all chara buttons
		double xSegment = 1.f / 6.f * sf::VideoMode::getDesktopMode().width;

		m_buttons[0] = std::make_unique<CharaButton>(Character::Knight, vec2d(2 * xSegment, y), scale);
		m_buttons[1] = std::make_unique<CharaButton>(Character::Witch, vec2d(3 * xSegment, y), scale);
		m_buttons[2] = std::make_unique<CharaButton>(Character::MissingNo, vec2d(4 * xSegment, y), scale);

		float btnScale = 0.3 * 1920 / sf::VideoMode::getDesktopMode().width;
		vec2d pos = { 1080 * btnScale / 2,sf::VideoMode::getDesktopMode().height - 210 * btnScale / 2 };//sprite is 1050x180
		m_pExit = new Button(pos, btnScale, "Back");
		pos.x = 5 * xSegment;
		pos.y = sf::VideoMode::getDesktopMode().height/2.f;
		m_pStart = new Button(pos, 1.5 * btnScale, "Start");

		m_pFont = m_pExit->getFont();
		m_selectedCharacterStats.resize(4);

		y = 1/10.f * sf::VideoMode::getDesktopMode().height;

		auto& [txt,name] = m_selectedCharaName;
		txt = new sf::Text();
		txt->setFont(*m_pFont);
		txt->setOrigin(txt->getGlobalBounds().left + txt->getGlobalBounds().width / 2.f, txt->getGlobalBounds().top + txt->getGlobalBounds().height / 2.f);
		txt->setString("selected character:");
		txt->setCharacterSize(30 * scale);
		txt->setPosition(sf::VideoMode::getDesktopMode().width / 2.f - txt->getGlobalBounds().width/2.f, 4 * y);
		

		name  = new sf::Text();
		name->setFont(*m_pFont);
		name->setString("");
		name->setCharacterSize(50 * scale);
		
		

		int i = 4;
		for (auto& txt : m_selectedCharacterStats)
		{
			txt = std::make_unique<sf::Text>();
			txt->setPosition(xSegment, i*y);
			txt->setFont(*m_pFont);
			txt->setString("");
			txt->setCharacterSize(30*scale);
			++i;
		}
	}
	CharacterSelectionScreen::Screen::~Screen()
	{
		delete m_pExit, m_pStart;
		delete m_selectedCharaName.first, m_selectedCharaName.second;
	}

	void CharacterSelectionScreen::Screen::Draw(sf::RenderWindow& window) const
	{
		for (auto& c : m_buttons)
		{
			c->draw(window);
		}

		m_pExit->draw(window);
		m_pStart->draw(window);

		for (const auto& txt : m_selectedCharacterStats)
		{
			window.draw(*txt);
		}

		window.draw(*m_selectedCharaName.first);
		window.draw(*m_selectedCharaName.second);
	}

	CharacterSelectionScreen::Screen& CharacterSelectionScreen::Screen::Get()
	{
		return s_Instance;
	}

	CharacterData CharacterSelectionScreen::Screen::getCharacterData(Character chara)
	{
		switch (chara)
		{
		case CharacterSelectionScreen::Character::Knight:
			return *m_dataOfCharacters[0];
			break;
		case CharacterSelectionScreen::Character::Witch:
			return *m_dataOfCharacters[1];
			break;
		case CharacterSelectionScreen::Character::MissingNo:
			return *m_dataOfCharacters[2];
			break;
		default:
			break;
		}
		return CharacterData();
	}

	void CharacterSelectionScreen::Screen::ChangeCurrentlyDisplayingStats(Character chara)
	{
		auto hp = &m_selectedCharacterStats[0];
		auto dmg = &m_selectedCharacterStats[1];
		auto spd = &m_selectedCharacterStats[2];
		auto cldwn = &m_selectedCharacterStats[3];
		//done to prevent eye bleeding in switch
		//cant structure bind vectors :G

		if (chara != selectedCharacter)//checks if character is already selected
		{
			switch (chara)
			{
			case CharacterSelectionScreen::Character::Knight:
				m_selectedCharaName.second->setString("Knight");
				hp->get()->setString("Health: " + std::to_string(m_dataOfCharacters[0]->Health));
				dmg->get()->setString("Damage: " + std::to_string(m_dataOfCharacters[0]->Damage));
				spd->get()->setString("Movement speed: " + std::to_string(m_dataOfCharacters[0]->Speed));
				cldwn->get()->setString("Weapon cooldown: " + std::to_string(m_dataOfCharacters[0]->Cooldown));
				break;
			case CharacterSelectionScreen::Character::Witch:
				m_selectedCharaName.second->setString("Witch");
				hp->get()->setString("Health: " + std::to_string(m_dataOfCharacters[1]->Health));
				dmg->get()->setString("Damage: " + std::to_string(m_dataOfCharacters[1]->Damage));
				spd->get()->setString("Movement speed: " + std::to_string(m_dataOfCharacters[1]->Speed));
				cldwn->get()->setString("Weapon cooldown: " + std::to_string(m_dataOfCharacters[1]->Cooldown));
				break;
			case CharacterSelectionScreen::Character::MissingNo:
				m_selectedCharaName.second->setString("err:MissingNO");
				hp->get()->setString("Health: ?");
				dmg->get()->setString("Damage: ?");
				spd->get()->setString("Movement speed: ?");
				cldwn->get()->setString("Weapon cooldown: ?");
				break;
			default:
				break;
			}//i hate this ^

			m_selectedCharaName.second->setPosition(sf::VideoMode::getDesktopMode().width / 2.f, 4.5 / 10.f * sf::VideoMode::getDesktopMode().height);
		}
	}

	void CharacterSelectionScreen::Screen::SelectedCharacterReset()
	{
		m_selectedCharaName.second->setString("");

		for (auto& stat : m_selectedCharacterStats)
		{
			stat->setString("");
		}
	}

	CharacterSelectionScreen::ButtonPressed CharacterSelectionScreen::Screen::ButtonHandler()
	{
		for (const auto& b : m_buttons)
		{
			if (b->mouseOnButton()) 
			{
				Sound::Sounds::buttonSound.Play();

				ChangeCurrentlyDisplayingStats(b->getCharaType());
				return static_cast<ButtonPressed>(b->getCharaType());
			}
		}

		if (m_pExit->mouseOnButton())
		{
			Sound::Sounds::buttonSound.Play();

			SelectedCharacterReset();
			return ButtonPressed::Exit;
		}
		else if (m_pStart->mouseOnButton())
		{
			Sound::Sounds::buttonSound.Play();

			SelectedCharacterReset();
			return ButtonPressed::Start;
		}

		return ButtonPressed::None;
	}
#pragma endregion