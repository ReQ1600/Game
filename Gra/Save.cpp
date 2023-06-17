#include "Save.h"
#include <string>

void Utils::Save::SaveFile::Write()
{
    std::ofstream file;
    file.open("save.sav");
    
    file << m_SaveData->PlayerMoney << ";";
    file << m_SaveData->shop.Health << ";";
    file << m_SaveData->shop.Damage << ";";
    file << m_SaveData->shop.Speed << ";";
    file << m_SaveData->shop.Money << ";";
    file << m_SaveData->shop.Experience << ";";
    file << m_SaveData->shop.Cooldown << ";";
    file.close();
}

void Utils::Save::SaveFile::Load()
{
    std::ifstream file;
    file.open("save.sav");
    if (!file) 
    {
        file.close();
        Write();
        file.open("save.sav");
    }

    std::string tmp_str;
    Data tmp_data;

    std::getline(file, tmp_str, ';');
    tmp_data.PlayerMoney =  std::stoi(tmp_str);
    std::getline(file, tmp_str, ';');
    tmp_data.shop.Health = std::stoi(tmp_str);
    std::getline(file, tmp_str, ';');
    tmp_data.shop.Damage = std::stoi(tmp_str);
    std::getline(file, tmp_str, ';');
    tmp_data.shop.Speed = std::stoi(tmp_str);
    std::getline(file, tmp_str, ';');
    tmp_data.shop.Money = std::stoi(tmp_str);
    std::getline(file, tmp_str, ';');
    tmp_data.shop.Experience = std::stoi(tmp_str);
    std::getline(file, tmp_str, ';');
    tmp_data.shop.Cooldown = std::stoi(tmp_str);

    file.close();

    delete m_SaveData;
    m_SaveData = new Data(std::move(tmp_data));
}

std::ostream& Utils::Save::operator<<(std::ostream& os, const Utils::Save::SaveFile& saveFile)
{
    os << "current Save data:\n";
    os << "Player money: " << saveFile.m_SaveData->PlayerMoney << "\n";
    os << "Player Health upgrade: " << saveFile.m_SaveData->shop.Health << "\n";
    os << "Player Damage upgrade: " << saveFile.m_SaveData->shop.Damage << "\n";
    os << "Player Speed upgrade: " << saveFile.m_SaveData->shop.Speed << "\n";
    os << "Player Money gain upgrade: " << saveFile.m_SaveData->shop.Money << "\n";
    os << "Player Experience gain upgrade: " << saveFile.m_SaveData->shop.Experience << "\n";
    os << "Player Cooldown reduction upgrade: " << saveFile.m_SaveData->shop.Cooldown << std::endl;
    return os;
}
