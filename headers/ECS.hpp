/*
** EPITECH PROJECT, 2020
** ECS.hpp
** File description:
** ECS
*/

#ifndef ECS_HPP_
#define ECS_HPP_
#include <iostream>
#include <sstream>
#include <bitset>
#include <stdint.h>
#include <cstdlib>
#include <SFML/Audio.hpp>
#include <irrlicht.h>
using namespace irr;
using namespace std;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

using Entity = std::uint32_t;
const Entity MAX_ENTITIES = 700;

using ComponentType = std::uint8_t;
const ComponentType MAX_COMPONENT = 32;

enum
{
    GUI_ID_BACKGROUND,
    GUI_ID_QUIT_BUTTON = 101,
    GUI_ID_PLAY_BUTTON,
    GUI_ID_OPTION_BUTTON,
    GUI_ID_TRANSPARENCY_SCROLL_BAR,
    GUI_ID_MENU,
    GUI_ID_KEYBIND,
    GUI_ID_SOUND,
    GUI_BACK_SETTINGS_KEYBIND,
    GUI_BACK_SETTINGS_SOUNDMENU,
    GUI_BACK_MENU,
    GUI_ID_OPTION_BACK_BUTTON,
    GUI_ID_BUTTON_KEYBIND,
    GUI_ID_MENU_CHARACTER,
    GUI_ID_SOLO_MODE,
    GUI_ID_SOLO_MODE_MENU,
    GUI_ID_SOLO_MODE_SAVE,
    GUI_ID_MENU_CHARACTER_RETURN,
    GUI_ID_LEFT_1,
    GUI_ID_RIGHT_1,
    GUI_ID_LEFT_2,
    GUI_ID_RIGHT_2,
    GUI_ID_LEFT_3,
    GUI_ID_RIGHT_3,
    GUI_ID_LEFT_4,
    GUI_ID_RIGHT_4,
    PLAYER1FORWARD_BTN,
    PLAYER1BACKWARD_BTN,
    PLAYER1LEFT_BTN,
    PLAYER1RIGHT_BTN,
    PLAYER1BOMB_BTN,
    PLAYER2FORWARD_BTN,
    PLAYER2BACKWARD_BTN,
    PLAYER2LEFT_BTN,
    PLAYER2RIGHT_BTN,
    PLAYER2BOMB_BTN,
    PLAYER3FORWARD_BTN,
    PLAYER3BACKWARD_BTN,
    PLAYER3LEFT_BTN,
    PLAYER3RIGHT_BTN,
    PLAYER3BOMB_BTN,
    PLAYER4FORWARD_BTN,
    PLAYER4BACKWARD_BTN,
    PLAYER4LEFT_BTN,
    PLAYER4RIGHT_BTN,
    PLAYER4BOMB_BTN,
};

void gameLoop();
void optionMenu();

struct SAppContext
{
    IrrlichtDevice* device;
};

class MyEventReceiver : public IEventReceiver
{
public:
    MyEventReceiver(SAppContext& context) : Context(context)
    {
        for (u32 i = 0; i < KEY_KEY_CODES_COUNT; ++i)
            KeyIsDown[i] = false;
    }
    virtual bool IsKeyDown(EKEY_CODE keyCode) const
    {
        return KeyIsDown[keyCode];
    }
    //virtual bool WhichKeyDown(EKEY_CODE keyCode)
    virtual bool OnEvent(const SEvent& event)
    {
        if (event.EventType == EET_GUI_EVENT) {
            s32 id = event.GUIEvent.Caller->getID();
            IGUIEnvironment* env = Context.device->getGUIEnvironment();
            switch (event.GUIEvent.EventType) {
            case EGET_SCROLL_BAR_CHANGED:
                if (id == GUI_ID_TRANSPARENCY_SCROLL_BAR)
                    s32 pos = ((IGUIScrollBar*)event.GUIEvent.Caller)->getPos();
                break;
            case EGET_BUTTON_CLICKED:
                switch (id) {
                case GUI_ID_QUIT_BUTTON:
                    Context.device->closeDevice();
                    std::cout << "QUIT" << std::endl;
                    return (true);
                case GUI_ID_PLAY_BUTTON:
                    std::cout << "Game launched." << std::endl;
                    Context.device->getCursorControl()->setVisible(false);
                    game = true;
                    retry = true;
                    return (true);
                case GUI_ID_OPTION_BUTTON:
                    std::cout << "Option window." << std::endl;
                    options = true;
                    keybinding_menu = false;
                    sound_menu = false;
                    return (true);
                case GUI_ID_KEYBIND:
                    std::cout << "Accessing keybind menu." << std::endl;
                    keybinding_menu = true;
                    options = false;
                    sound_menu = false;
                    return (true);
                case GUI_ID_SOUND:
                    std::cout << "Accessing sound menu." << std::endl;
                    sound_menu = true;
                    options = false;
                    keybinding_menu = false;
                    return(true);
                case GUI_BACK_SETTINGS_KEYBIND:
                    std::cout << "Back to settings from keybindings menu." << std::endl;
                    options = true;
                    game = false;
                    keybinding_menu = false;
                    sound_menu = false;
                    return (true);
                case GUI_BACK_SETTINGS_SOUNDMENU:
                    std::cout << "Back to settings from sound menu." << std::endl;
                    options = true;
                    game = false;
                    keybinding_menu = false;
                    sound_menu = false;
                    return (true);
                case GUI_ID_MENU:
                    std::cout << "Back to menu." << std::endl;
                    back = true;
                    options = false;
                    game = false;
                    win = false;
                    return (true);
                case GUI_ID_MENU_CHARACTER:
                    std::cout << "Menu characters." << std::endl;
                    select_options = true;
                    return (true);
                case GUI_ID_MENU_CHARACTER_RETURN:
                    std::cout << "Back to menu." << std::endl;
                    back = true;
                    select_options = false;
                    return (true);
                case GUI_ID_SOLO_MODE:
                    std::cout << "Solo mode." << std::endl;
                    this->solo = true;
                case GUI_ID_SOLO_MODE_SAVE:
                    std::cout << "Solo mode." << std::endl;
                    this->solo_resume = true;
                case GUI_ID_SOLO_MODE_MENU:
                    std::cout << "Solo mode." << std::endl;
                    this->solo_menu = true;
                case PLAYER1FORWARD_BTN:
                    cout << "Player 1 forward button." << endl;
                    return (true);
                case PLAYER1BACKWARD_BTN:
                    cout << "Player 1 backward button." << endl;
                    return (true);
                case PLAYER1LEFT_BTN:
                    cout << "Player 1 left button." << endl;
                    return (true);
                case PLAYER1RIGHT_BTN:
                    cout << "Player 1 right button." << endl;
                    return (true);
                case PLAYER1BOMB_BTN:
                    cout << "Player 1 drop bomb button." << endl;
                    return (true);
                case PLAYER2FORWARD_BTN:
                    cout << "Player 2 forward button." << endl;
                    return (true);
                case PLAYER2BACKWARD_BTN:
                    cout << "Player 2 backward button." << endl;
                    return (true);
                case PLAYER2LEFT_BTN:
                    cout << "Player 2 left button." << endl;
                    return (true);
                case PLAYER2RIGHT_BTN:
                    cout << "Player 2 right button." << endl;
                    return (true);
                case PLAYER2BOMB_BTN:
                    cout << "Player 2 drop bomb button." << endl;
                    return (true);
                case PLAYER3FORWARD_BTN:
                    cout << "Player 3 forward button." << endl;
                    return (true);
                case PLAYER3BACKWARD_BTN:
                    cout << "Player 3 backward button." << endl;
                    return (true);
                case PLAYER3LEFT_BTN:
                    cout << "Player 3 left button." << endl;
                    return (true);
                case PLAYER3RIGHT_BTN:
                    cout << "Player 3 right button." << endl;
                    return (true);
                case PLAYER3BOMB_BTN:
                    cout << "Player 3 drop bomb button." << endl;
                    return (true);
                case PLAYER4FORWARD_BTN:
                    cout << "Player 4 forward button." << endl;
                    return (true);
                case PLAYER4BACKWARD_BTN:
                    cout << "Player 4 backward button." << endl;
                    return (true);
                case PLAYER4LEFT_BTN:
                    cout << "Player 4 left button." << endl;
                    return (true);
                case PLAYER4RIGHT_BTN:
                    cout << "Player 4 right button." << endl;
                    return (true);
                case PLAYER4BOMB_BTN:
                    cout << "Player 4 drop bomb button." << endl;
                    return (true);
                case GUI_ID_LEFT_1:
                    if (AI_state_1 == false) {
                        AI_state_1 = true;
                    } else {
                        AI_state_1 = false;
                    }
                    cout << AI_state_1 << endl;
                    return (true);
                case GUI_ID_RIGHT_1:
                    if (AI_state_1 == false) {
                        AI_state_1 = true;
                    }
                    else {
                        AI_state_1 = false;
                    }
                    cout << AI_state_1 << endl;
                    return (true);
                case GUI_ID_LEFT_2:
                    if (AI_state_2 == false) {
                        AI_state_2 = true;
                    }
                    else {
                        AI_state_2 = false;
                    }
                    cout << AI_state_2 << endl;
                    return (true);
                case GUI_ID_RIGHT_2:
                    if (AI_state_2 == false) {
                        AI_state_2 = true;
                    }
                    else {
                        AI_state_2 = false;
                    }
                    cout << AI_state_2 << endl;
                    return (true);
                case GUI_ID_LEFT_3:
                    if (AI_state_3 == false) {
                        AI_state_3 = true;
                    }
                    else {
                        AI_state_3 = false;
                    }
                    cout << AI_state_3 << endl;
                    return (true);
                case GUI_ID_RIGHT_3:
                    if (AI_state_3 == false) {
                        AI_state_3 = true;
                    }
                    else {
                        AI_state_3 = false;
                    }
                    cout << AI_state_3 << endl;
                    return (true);
                case GUI_ID_LEFT_4:
                    if (AI_state_4 == false) {
                        AI_state_4 = true;
                    }
                    else {
                        AI_state_4 = false;
                    }
                    cout << AI_state_4 << endl;
                    return (true);
                case GUI_ID_RIGHT_4:
                    if (AI_state_4 == false) {
                        AI_state_4 = true;
                    }
                    else {
                        AI_state_4 = false;
                    }
                    cout << AI_state_4 << endl;
                    return (true);
                default:
                    Context.device->getCursorControl()->setActiveIcon(ECI_NORMAL);
                    return (false);
                }
                break;
                /*case EET_KEY_INPUT_EVENT :
                    switch (id) {
                    case GUI_ID_BUTTON_KEYBIND :
                    }*/


            default:
                Context.device->getCursorControl()->setActiveIcon(ECI_NORMAL);
                break;
            }
        }
        if (event.EventType == irr::EET_KEY_INPUT_EVENT) {
            if (event.KeyInput.Key == KEY_ESCAPE) {
                cout << "ESC KEY PRESSED" << endl;
                inWhile = false;
            }
            KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
        }
        return (false);
    }
    bool back = false;
    bool game = false;
    bool options = false;
    bool keybinding_menu = false;
    bool sound_menu = false;
    bool inWhile = true;
    bool select_options = false;
    bool solo = false;
    bool solo_menu = false;
    bool solo_resume = false;
    bool win = false;
    bool retry = false;
    //
    bool AI_state_1 = false;
    bool AI_state_2 = false;
    bool AI_state_3 = false;
    bool AI_state_4 = false;
    bool check_song = false;
    IGUIStaticText* sound_value;
    s32 value;
    s32 curr_pos;
 
private:
    SAppContext& Context;
    bool KeyIsDown[KEY_KEY_CODES_COUNT];
};

static char* KeyNames[] =
{
      "-",
      "LMOUSE",
      "RMOUSE",
      "Cancel",
      "MIDDLEMOUSE",
      "X Button 1",
      "X Button 2",
      "-",
      "Back",
      "Tab",
      "-",
      "-",
      "Clear",
      "Return",
      "-",
      "-",
      "Shift",
      "Control",
      "Menu",
      "Pause",
      "Capital",
      "Kana",
      "-",
      "Junja",
      "Final",
      "Kanji",
      "-",
      "Escape",
      "Convert",
      "Nonconvert",
      "Accept",
      "Mode Change",
      "Space",
      "Priot",
      "Next",
      "End",
      "Home",
      "Left",
      "Up",
      "Right",
      "Down",
      "Select",
      "Print",
      "Execute",
      "Snapshot",
      "Insert",
      "Delete",
      "Help",
      "0",
      "1",
      "2",
      "3",
      "4",
      "5",
      "6",
      "7",
      "8",
      "9",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "A",
      "B",
      "C",
      "D",
      "E",
      "F",
      "G",
      "H",
      "I",
      "J",
      "K",
      "L",
      "M",
      "N",
      "O",
      "P",
      "Q",
      "R",
      "S",
      "T",
      "U",
      "V",
      "W",
      "X",
      "Y",
      "Z",
      "LWDOWS",
      "RWDOWS",
      "Apps",
      "-",
      "Sleep",
      "NUM 0",
      "NUM 1",
      "NUM 2",
      "NUM 3",
      "NUM 4",
      "NUM 5",
      "NUM 6",
      "NUM 7",
      "NUM 8",
      "NUM 9",
      "NUM *",
      "NUM +",
      "NUM /",
      "NUM -",
      "NUM .",
      "NUM /",
      "F1",
      "F2",
      "F3",
      "F4",
      "F5",
      "F6",
      "F7",
      "F8",
      "F9",
      "F10",
      "F11",
      "F12",
      "F13",
      "F14",
      "F15",
      "F16",
      "F17",
      "F18",
      "F19",
      "F20",
      "F21",
      "F22",
      "F23",
      "F24",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "NUMLOCK",
      "Scroll Lock",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "LSHIFT",
      "RSHIFT",
      "LCTRL",
      "RCTRL",
      "LMENU",
      "RMENU",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "Plus",
      "Comma",
      "Minus",
      "Period",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "-",
      "Attn",
      "CrSel",
      "ExSel",
      "Erase OEF",
      "Play",
      "Zoom",
      "PA1",
      "OEM Clear",
      "-"
};

void menu();

#endif /* !ECS_HPP_ */
