#pragma once
#include <cstdint>
#include <fstream>
#include <string>
#include "Utils.h"
#include <format>

struct CharacterData
{
	uint16_t  Health;
	uint16_t  Damage;
	uint16_t  Speed;
	uint16_t  Cooldown;
};

namespace Utils
{
	class CharacterDataReader
	{
	public:

		inline CharacterDataReader() {};
		inline ~CharacterDataReader() {};
		CharacterData Read(const char* fileName) //returns read data
		{
			std::ifstream file;
			file.open(fileName);
			if (!file)
			{
				Utils::Log(std::format("Data reading error:\nNo file named \"{0}\" found",fileName));
				return {};
			}

			std::string tmp_str;
			CharacterData tmp_data;

			std::getline(file, tmp_str, ';');
			tmp_data.Health = std::stoi(tmp_str);
			std::getline(file, tmp_str, ';');
			tmp_data.Damage = std::stoi(tmp_str);
			std::getline(file, tmp_str, ';');
			tmp_data.Speed = std::stoi(tmp_str);
			std::getline(file, tmp_str, ';');
			tmp_data.Cooldown = std::stoi(tmp_str);

			file.close();

			return tmp_data;
		}
	};
}

