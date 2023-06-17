#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <ostream>
#include <vector>
#include <string>
#include "Collision.h"
#include "Screen.h"

namespace Menu
{
	enum class ButtonType
	{
		Start = 0,
		Shop,
		Exit,
		Credits,
		Empty
	};

	class Button final : public ButtonBase
	{
	public:
		Button(uint16_t y, const char* text, const float& scale) : ButtonBase(vec2d(sf::VideoMode::getDesktopMode().width/2, y), 1050, 180, text, scale, "button") { Utils::Log("button: "); Utils::Log(this); };//ButtonBase(vec2d(0, y), 1050, 180, text, scale, "button") { Utils::Log(this); };
		~Button() {};
		void draw(sf::RenderWindow& window) override;
	};

	

	class ParentMenu : public IScreen
	{
	public:
		friend std::ostream& operator<<(std::ostream& s, const ParentMenu& menu);
		ParentMenu();
		virtual ~ParentMenu();
		void Draw(sf::RenderWindow& window) const = 0;
		virtual ButtonType Select() = 0;
	protected:
		float scale;
		unsigned __int16 m_width = 0;
		unsigned __int16 m_height = 0;
		std::vector<std::unique_ptr<Button>> m_buttons;
	};

	class Main final : public ParentMenu
	{
	public:
		Main(const Main&) = delete;
		static Main& Get();
		void Draw(sf::RenderWindow& window) const override;
		ButtonType Select() override;
		~Main();
	private:		
		Main();
		static Main s_Instance;
		sf::Text * m_title;
	};
};

std::ostream& operator<<(std::ostream& s, const Menu::Button& button);
std::ostream& operator<<(std::ostream& s, const Menu::ParentMenu& menu);
