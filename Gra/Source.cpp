
#include <iostream>
#include "Menu.h"
#include "Player.h"
#include <chrono>
#include "CharacterSelectionScreen.h"
#include "CreditsScreen.h"
#include "ShopScreen.h"
#include "Level.h"
#include "EndScreen.h"
#include "Sounds.h"

const unsigned __int16 WINDOW_X = sf::VideoMode::getDesktopMode().width;
const unsigned __int16 WINDOW_Y = sf::VideoMode::getDesktopMode().height;

enum class ScreenTypes
{
    MainMenu = 0,
    CharaSelection,
    Shop,
    Level,
    Credits,
    End
};

//handles Escape key press given current screen type and additional arg if it is required
IScreen* EscKeyHandler(ScreenTypes& scrType, void* p = nullptr)
{
    switch (scrType)
    {
    case ScreenTypes::MainMenu:
        static_cast<sf::RenderWindow*>(p)->close();
        return nullptr;
        break;
    case ScreenTypes::CharaSelection:
        scrType = ScreenTypes::MainMenu;
        return &Menu::Main::Get();
        break;
    case ScreenTypes::Shop:
        scrType = ScreenTypes::MainMenu;
        return &Menu::Main::Get();
        break;
    case ScreenTypes::Level:
        scrType = ScreenTypes::MainMenu;
        return &Menu::Main::Get();
        break;
    case ScreenTypes::Credits:
        scrType = ScreenTypes::MainMenu;
        return &Menu::Main::Get();
        break;
    default:
        Utils::Log("uh oh");
        break;
    }
}

int main()
{
    sf::Vector2f movementDirection;

    auto timer_start = std::chrono::steady_clock().now();
    auto timer_end = timer_start;
    float elapsedTime = 0;
    float refreshRate = 280000;
    const sf::Color bgColor(12, 68, 90);

    sf::RenderWindow window(sf::VideoMode(WINDOW_X, WINDOW_Y), "game", sf::Style::Fullscreen);

    IScreen* pCurrentScreen = &Menu::Main::Get();
    ScreenTypes currentScreenType = ScreenTypes::MainMenu;

    //assigning selected character to be the same in here and chara selection
    CharacterSelectionScreen::Character* pSelectedCharacter = &CharacterSelectionScreen::Screen::Get().selectedCharacter;
    CharacterSelectionScreen::Character lastSelectedCharacter;

    //i feel its better to have in here rather than global
    Utils::Save::SaveFile* pSave = Shop::Screen::Get().pSave;

    //its just easier that way
    std::shared_ptr<Level> level;
    std::shared_ptr<EndScreen::Screen> endScreen;

    void* pAdditionalEscArg = &window;
    Utils::Log("window: ");
    Utils::Log(pAdditionalEscArg);
    while (window.isOpen())
    {
        timer_end = std::chrono::steady_clock().now();
        elapsedTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timer_end - timer_start).count();
        timer_start = timer_end;



        while (elapsedTime > refreshRate)
        {

            sf::Event event;
            while (window.pollEvent(event))
            {

                if (event.type == sf::Event::Closed)
                {
                    sf::Event e;
                    e.type = sf::Event::KeyPressed;
                    e.key.code = sf::Keyboard::Escape;
                    window.pollEvent(e);
                    //shouldnt be able to press but lets be safe
                    //couldnt think of a better solution im tired
                }

                //keyboard handling
                if (event.type == sf::Event::KeyPressed)
                {
                    if (event.key.code == sf::Keyboard::Escape)
                    {
                        pCurrentScreen = EscKeyHandler(currentScreenType, pAdditionalEscArg);
                        if (pCurrentScreen == nullptr) return 0;
                    }
                    
                    //in level movement
                    if (currentScreenType == ScreenTypes::Level)
                    {
                        switch (event.key.code)
                        {
                        case sf::Keyboard::W:
                            movementDirection.y = 1;
                            break;
                        case sf::Keyboard::S:
                            movementDirection.y = -1;
                            break;
                        case sf::Keyboard::A:
                            movementDirection.x = 1;
                            break;
                        case sf::Keyboard::D:
                            movementDirection.x = -1;
                            break;
                        default:
                            break;
                        }
                    }

                }

                //in level movement p2
                if (event.type == sf::Event::KeyReleased)
                {
                    if (currentScreenType == ScreenTypes::Level)
                    {
                        if (currentScreenType == ScreenTypes::Level)
                        {
                            switch (event.key.code)
                            {
                            case sf::Keyboard::W:
                                movementDirection.y = 0;
                                break;
                            case sf::Keyboard::S:
                                movementDirection.y = 0;
                                break;
                            case sf::Keyboard::A:
                                movementDirection.x = 0;
                                break;
                            case sf::Keyboard::D:
                                movementDirection.x = 0;
                                break;
                            default:
                                break;
                            }
                        }
                    }
                }

                //mouse button press handling
                if (event.type == sf::Event::MouseButtonPressed)
                {
                    switch (currentScreenType)
                    {
                    case ScreenTypes::MainMenu:
                        pAdditionalEscArg = &window;

                        switch (static_cast<Menu::Main*>(pCurrentScreen)->Select())
                        {
                        case Menu::ButtonType::Start:
                            Sound::Sounds::buttonSound.Play();
                            pCurrentScreen = &CharacterSelectionScreen::Screen::Get();
                            currentScreenType = ScreenTypes::CharaSelection;
                            break;

                        case Menu::ButtonType::Shop:
                            Sound::Sounds::buttonSound.Play();
                            pCurrentScreen = &Shop::Screen::Get();
                            currentScreenType = ScreenTypes::Shop;
                            break;

                        case Menu::ButtonType::Exit:
                            Sound::Sounds::buttonSound.Play();
                            window.close();
                            return 0;
                            break;

                        case Menu::ButtonType::Credits:
                            Sound::Sounds::buttonSound.Play();
                            pCurrentScreen = &CreditsScreen::Get();
                            currentScreenType = ScreenTypes::Credits;
                            break;

                        default:
                            break;
                        }
                        break;
                    case ScreenTypes::CharaSelection:
                        switch (static_cast<CharacterSelectionScreen::Screen*>(pCurrentScreen)->ButtonHandler())
                        {
                        case CharacterSelectionScreen::ButtonPressed::None:
                            break;

                        case CharacterSelectionScreen::ButtonPressed::Exit:
                            *pSelectedCharacter = CharacterSelectionScreen::Character::None;
                            pCurrentScreen = &Menu::Main::Get();
                            currentScreenType = ScreenTypes::MainMenu;
                            break;

                        case CharacterSelectionScreen::ButtonPressed::Start:
                            if (*pSelectedCharacter == CharacterSelectionScreen::Character::None) break;

                            level = std::make_shared<Level>(pSave, static_cast<CharacterType>(*pSelectedCharacter), static_cast<CharacterSelectionScreen::Screen*>(pCurrentScreen)->getCharacterData(*pSelectedCharacter));
                            pCurrentScreen = level.get();
                            lastSelectedCharacter = *pSelectedCharacter;
                            *pSelectedCharacter = CharacterSelectionScreen::Character::None;
                            currentScreenType = ScreenTypes::Level;
                            break;

                        case CharacterSelectionScreen::ButtonPressed::Knight:
                            *pSelectedCharacter = CharacterSelectionScreen::Character::Knight;
                            break;

                        case CharacterSelectionScreen::ButtonPressed::Witch:
                            *pSelectedCharacter = CharacterSelectionScreen::Character::Witch;
                            break;

                        case CharacterSelectionScreen::ButtonPressed::MissingNo:
                            *pSelectedCharacter = CharacterSelectionScreen::Character::MissingNo;
                            break;

                        default:
                            break;
                        }
                        break;
                    case ScreenTypes::Shop:
                        if (static_cast<Shop::Screen*>(pCurrentScreen)->ButtonHandler())//checks if exit button inside the shop was pressed 
                        {
                            pCurrentScreen = &Menu::Main::Get();
                            currentScreenType = ScreenTypes::MainMenu;
                        }
                        break;

                    case ScreenTypes::Level:
                        level->MouseButtonPress(window);
                        break;

                    case ScreenTypes::Credits:
                        break;

                    case ScreenTypes::End:
                        //either button is pressed the game will save
                        if (static_cast<EndScreen::Screen*>(pCurrentScreen)->ButtonPressed() != EndScreen::ButtonType::None)
                        {
                            Utils::Save::Data tmp = pSave->getSaveData();
                            uint32_t winBonus = level->getGameState() == GameState::Won ? 2500 : 0;
                            tmp.PlayerMoney += level->getMoney() + 0.2 * pSave->getSaveData().shop.Money * level->getMoney() + winBonus;
                            pSave->setSaveData(tmp);
                            pSave->Write();
                            Shop::Screen::Get().UpdateMoney();
                        }

                        if (static_cast<EndScreen::Screen*>(pCurrentScreen)->ButtonPressed() == EndScreen::ButtonType::Menu)//checks if exit button inside the shop was pressed 
                        {
                            Sound::Sounds::buttonSound.Play();

                            pCurrentScreen = &Menu::Main::Get();
                            currentScreenType = ScreenTypes::MainMenu;
                        }
                        else if (static_cast<EndScreen::Screen*>(pCurrentScreen)->ButtonPressed() == EndScreen::ButtonType::Restart)
                        {
                            Sound::Sounds::buttonSound.Play();

                            level.reset();
                            level = std::make_shared<Level>(pSave, static_cast<CharacterType>(lastSelectedCharacter), CharacterSelectionScreen::Screen::Get().getCharacterData(lastSelectedCharacter));
                            pCurrentScreen = level.get();
                            currentScreenType = ScreenTypes::Level;
                        }
                        break;

                    default:
                        break;
                    }
                }
            }
                elapsedTime -= refreshRate;

                if (currentScreenType == ScreenTypes::Level)
                {
                    if (level->getGameState() != GameState::InProgress)
                    {
                        Sound::Sounds::Walking.Pause();
                        endScreen = std::make_shared<EndScreen::Screen>(level->getGameState(),level->getMoney(),pSave->getSaveData().shop.Money);
                        pCurrentScreen = endScreen.get();
                        currentScreenType = ScreenTypes::End;
                    }
                    else
                    {
                        if ((movementDirection.x != 0 || movementDirection.y != 0) && Sound::Sounds::Walking.getState() != sf::SoundSource::Playing) Sound::Sounds::Walking.Play();
                        else if (Sound::Sounds::Walking.getState() != sf::SoundSource::Paused) Sound::Sounds::Walking.Pause();
                        level->Move(movementDirection); //revenge of the in level movement
                    }
                }
                //else if(Sound::Sounds::Walking.getState() == sf::SoundSource::Playing) Sound::Sounds::Walking.Pause();

                window.clear(bgColor);
                pCurrentScreen->Draw(window);
                window.display();
            
        }
    }
    return 0;
}