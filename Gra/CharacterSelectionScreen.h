#pragma once
#include "Collision.h"
#include <vector>
#include "Player.h"
#include <SFML/Graphics.hpp>
#include "Screen.h"
#include "CharacterDataReader.h"

namespace CharacterSelectionScreen
{
	enum class Character
	{
		Knight = 0,
		Witch,
		MissingNo,
		None
	};

	enum class ButtonPressed
	{
		Knight = 0,
		Witch,
		MissingNo,
		Start,
		Exit,
		None
	};

	class CharaButton final : public ButtonBase
	{
	public:
		CharaButton(const Character chara, const vec2d& position, const float& scale);
		~CharaButton() {};
		inline bool mouseOnButton() const { if (m_border.getOutlineColor() == sf::Color::Yellow) return true; return false; };
		void draw(sf::RenderWindow& window) override;

		inline Character getCharaType() { return character; };
	private:
		uint32_t animationCooldown = 0;
		sf::RectangleShape m_border;
		sf::Text m_name;
		const Character character;

	};

	class Button final : public ButtonBase
	{
	public:
		Button(const vec2d& position, const float scale, const char* text) : ButtonBase(position, 1050, 180, text, scale, "button") {};
		~Button() {};
		void draw(sf::RenderWindow& window) override;
	};

	class Screen final : public IScreen
	{
	public:
		Screen(const Screen&) = delete;
		~Screen();
		void Draw(sf::RenderWindow& window) const override;
		ButtonPressed ButtonHandler();
		static Screen& Get();
		Character selectedCharacter = Character::None;
		CharacterData getCharacterData(Character chara);

	private: 
		void ChangeCurrentlyDisplayingStats(Character chara);
		void SelectedCharacterReset();

		std::vector<std::unique_ptr<CharaButton>> m_buttons;
		std::vector <std::unique_ptr<sf::Text>> m_selectedCharacterStats;
		std::vector<std::unique_ptr<CharacterData>> m_dataOfCharacters;
		std::pair<sf::Text*,sf::Text*> m_selectedCharaName;
		sf::Font* m_pFont = nullptr;
		Button* m_pExit = nullptr;
		Button* m_pStart = nullptr;
		static Screen s_Instance;
		Screen();
	};
}
