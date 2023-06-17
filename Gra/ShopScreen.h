#pragma once
#include "Screen.h"
#include "Collision.h"
#include "Save.h"

namespace Shop
{
	enum class UpgradeName
	{
		Health = 0,
		Damage,
		Speed,
		Money,
		Experience,
		Cooldown
	};

	enum class ButtonType
	{
		Plus = 0,
		Minus,
		Exit
	};

	class Button final : public ButtonBase
	{
	public:
		Button(ButtonType type, const vec2d& position, const float scale);
		~Button();
		void draw(sf::RenderWindow& window) override;
	private:
		ButtonType m_type;
	};

	class Upgrade
	{
	public:
		Upgrade(UpgradeName name,vec2d& position, const float& scale);
		~Upgrade();
		
		void Draw(sf::RenderWindow& window);
		void ButtonPressed(uint32_t& money);
	
		void setAmountBought(uint8_t amnt);
		inline uint8_t getAmountBought() { return m_amountBought; };
	private:
		uint16_t m_price = 0;
		uint16_t m_initial_price = 500;
		uint8_t m_amountBought = 0;
		const uint8_t m_max_amnt = 5;

		sf::Font m_font;
		sf::Text m_name;
		sf::Text m_amount;
		sf::Text m_priceTxt;
		sf::Texture m_texture;
		sf::Sprite m_sprite;

		std::unique_ptr<Button> m_upPlus;
		std::unique_ptr<Button> m_upMinus;
	};

	class Screen final : public IScreen
	{
	public:
		Screen(const Screen&) = delete;
		~Screen();
		static Screen& Get();
		void Draw(sf::RenderWindow& window) const override;
		bool ButtonHandler();

		//dont set to different save will be alive for the whole duration of the program dw
		Utils::Save::SaveFile* pSave = nullptr;
		//used to update displayed money for when its value changes
		void UpdateMoney();
	private:
		Screen();
		static Screen s_Instance;
		
		uint32_t m_money;
		sf::Text m_moneyTxt;
		std::unique_ptr<Button> m_upExitButton;//on click saves
		std::vector<std::unique_ptr<Upgrade>> m_upgrades;
	};
}