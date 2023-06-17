#pragma once
#include <iostream>
#include <fstream>

namespace Utils
{
	namespace Save
	{
		struct Shop
		{
			int Health = 0;
			int Damage = 0;
			int Speed = 0;
			int Money = 0;
			int Experience = 0;
			int Cooldown = 0;
		};

		struct Data
		{
			int PlayerMoney = 0;
			Shop shop;
		};

		class SaveFile
		{
		public:
			friend std::ostream& operator<<(std::ostream& os, const SaveFile& saveFile);
			SaveFile() { m_SaveData = new Data; };
			~SaveFile() { delete m_SaveData; };

			//creates and/or writes into save.sav file 
			void Write();
			//reads and loads save.sav into the game creates new file if it cannot be found
			void Load();

			inline Data getSaveData() const { return *m_SaveData; };//returns save data in memory if there is no data loaded returns nullptr
			inline void setSaveData(const Data data) {*m_SaveData = data; };//changes data in memory to data specified
		private:
			Data* m_SaveData = nullptr;
		};

		std::ostream& operator<<(std::ostream& os, const Utils::Save::SaveFile& saveFile);//for logging purposes
	}
}
