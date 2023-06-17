#pragma once
#include "Screen.h"
#include <string>
#include "Collision.h"

enum class GameState
{
	Won = 0,
	Lost,
	InProgress
};

namespace EndScreen
{
	enum class ButtonType
	{
		Menu = 0,
		Restart,
		None
	};

	class Button final : public ButtonBase
	{
	public:
		Button(ButtonType type, const vec2d& position, const float scale);
		~Button() {};
		inline bool mouseOnButton() const { if (frame.left == 0) return false; return true; };
		void draw(sf::RenderWindow& window) override;
	private:
		ButtonType m_type = ButtonType::Menu;
	};

	class Screen final : public IScreen
	{
	public:
		Screen(const GameState state, const uint32_t& money, const int& money_mult);
		~Screen() { delete m_pEndingStateText, m_pMoneyText, m_pFont, m_pMenuButton, m_pRestartButton; };
		void Draw(sf::RenderWindow& window) const override;
		ButtonType ButtonPressed();
	private:
		sf::Font* m_pFont = nullptr;
		sf::Text* m_pMoneyText = nullptr;
		sf::Text* m_pEndingStateText = nullptr; //did player win or not
		Button* m_pMenuButton = nullptr;
		Button* m_pRestartButton = nullptr;

	};
}
