/*
** EPITECH PROJECT, 2020
** game.cpp
** File description:
** game
*/

#include "../headers/ECS.hpp"
#include "../headers/EntityManager.hpp"
#include "../headers/Components.hpp"
#include "../headers/GameManager.hpp"
#include "../headers/Systems/Movement.hpp"
#include "../headers/Systems/Render.hpp"
#include "../headers/Systems/ButtonSong.hpp"
#include "../headers/menu.hpp"
#include <thread>
#include <iostream>
#include <fstream>
#include <irrlicht.h>
#include <stdlib.h>
#include <time.h>

using namespace irr;
using namespace std;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

extern GameManager GM;
std::vector<std::string> _map;
std::vector<std::string> _map_col;
std::vector<std::string> _map_da;
bool AISTATE1 = false;
bool AISTATE2 = false;
bool AISTATE3 = false;
bool AISTATE4 = false;

static vector<EKEY_CODE> defaultKeyPlayer1 = { KEY_KEY_Z, KEY_KEY_S, KEY_KEY_Q, KEY_KEY_D, KEY_KEY_X };
static vector<EKEY_CODE> defaultKeyPlayer2 = { KEY_KEY_T, KEY_KEY_G, KEY_KEY_F, KEY_KEY_H, KEY_SPACE };
static vector<EKEY_CODE> defaultKeyPlayer3 = { KEY_KEY_O, KEY_KEY_L, KEY_KEY_K, KEY_KEY_M, KEY_KEY_P };
static vector<EKEY_CODE> defaultKeyPlayer4 = { KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_RCONTROL };


static KeyBinding player1_keybinding(defaultKeyPlayer1[0], defaultKeyPlayer1[1], defaultKeyPlayer1[2], defaultKeyPlayer1[3], defaultKeyPlayer1[4]);
static KeyBinding player2_keybinding(defaultKeyPlayer2[0], defaultKeyPlayer2[1], defaultKeyPlayer2[2], defaultKeyPlayer2[3], defaultKeyPlayer2[4]);
static KeyBinding player3_keybinding(defaultKeyPlayer3[0], defaultKeyPlayer3[1], defaultKeyPlayer3[2], defaultKeyPlayer3[3], defaultKeyPlayer3[4]);
static KeyBinding player4_keybinding(defaultKeyPlayer4[0], defaultKeyPlayer4[1], defaultKeyPlayer4[2], defaultKeyPlayer4[3], defaultKeyPlayer4[4]);

void keybindMenu();
void soundMenu();
void selectionMenu();
void solo_mode(bool save);
void winMenu(int player);
void choiceMenuSolo();
void winMenuSolo(int minutes, int seconds);
void LoseMenuSolo();

wstring getWchar(const char* str)
{
	const size_t strSize = strlen(str) + 1;
	std::wstring wcharString(strSize, L'#');
	mbstowcs(&wcharString[0], str, strSize);

	return wcharString;
}

void menu_back(bool play)
{
    static int name = 0;
    Device& dvc = dynamic_cast<Device&>(GM.getComponent(0, 3));
    GM.Reset();
    Entity IrrD = GM.addEntity();
    Entity Player = GM.addEntity();
    Entity Camera = GM.addEntity();
    Entity Terrain = GM.addEntity();
    IVideoDriver* driver = dvc.device->getVideoDriver();
    driver->enableMaterial2D();
    IGUIEnvironment* env = dvc.device->getGUIEnvironment();
    env->clear();
    Entity ButtonPlay = GM.addEntity();
	Entity ButtonSolo = GM.addEntity();
    Entity ButtonQuit = GM.addEntity();
    Entity ButtonOption = GM.addEntity();
    IGUISkin* skin = env->getSkin();
    IGUIFont* font = env->getFont("Fonts/bomberman20px.xml");
    sf::Sound btn_hover;
    sf::Sound btn_click;
	sf::SoundBuffer enterBuffer;
	sf::SoundBuffer enterBuffer2;
    if (!enterBuffer.loadFromFile("8bit/blip-1.wav") || \
        !enterBuffer2.loadFromFile("8bit/ok-2.wav")) {
        std::cout << "Error with Loading of the btn_play_music" << std::endl;
        exit(84);
    }
	btn_hover.setBuffer(enterBuffer);
	btn_click.setBuffer(enterBuffer2);
    skin->setFont(env->getBuiltInFont(), EGDF_TOOLTIP);
	Button Solo_button(env, GUI_ID_SOLO_MODE_MENU, rect<s32>(58, 60, 58 + 316, 60 + 140), "S o l o", &btn_hover, &btn_click, driver, font);
	Button Play_button(env, GUI_ID_MENU_CHARACTER, rect<s32>(58, 220, 58 + 316, 220 + 140), "M u l t i", &btn_hover, &btn_click, driver, font);
	Button Quit_button(env, GUI_ID_QUIT_BUTTON, rect<s32>(58, 540, 58 + 316, 580 + 140), "Q u i t", \
		& btn_hover, &btn_click, driver, font);
	Button Option_button(env, GUI_ID_OPTION_BUTTON, rect<s32>(58, 380, 58 + 316, 380 + 140), "S e t t i n g s", &btn_hover, \
		& btn_click, driver, font);
	IGUIStaticText *credits = env->addStaticText(L"Game Made By: Vincent, Iskandre, Corentin", rect<s32>(1280 - 200, 720 - 20, 1280, 720), false);
    Play_button.btn->setDrawBorder(false);
	Solo_button.btn->setDrawBorder(false);
    Quit_button.btn->setDrawBorder(false);
    Option_button.btn->setDrawBorder(false);
    IGUISpriteBank* sb = env->addEmptySpriteBank(("Buttons_back_" + std::to_string(name)).c_str());
    name += 1;
    ITexture* texture_normal = driver->getTexture("Image/button_normal2.png");
    ITexture* texture_clicked = driver->getTexture("Image/button_clicked2.png");
    ITexture* texture_hovered = driver->getTexture("Image/button_hovered2.png");
    sb->addTexture(texture_normal);
    sb->addTexture(texture_clicked);
    sb->addTexture(texture_hovered);
    sb->getPositions().push_back(core::rect<s32>(core::position2di(0, 0), texture_normal->getOriginalSize()));
    sb->getPositions().push_back(core::rect<s32>(core::position2di(0, 0), texture_clicked->getOriginalSize()));
    sb->getPositions().push_back(core::rect<s32>(core::position2di(0, 0), texture_hovered->getOriginalSize()));
    gui::SGUISprite sprite;
    gui::SGUISpriteFrame sframe;
    sprite.Frames.push_back(sframe);
    sprite.Frames[0].rectNumber = 0;
    sprite.Frames[0].textureNumber = 0;
    sb->getSprites().push_back(sprite);
    sprite.Frames[0].rectNumber = 1;
    sprite.Frames[0].textureNumber = 1;
    sb->getSprites().push_back(sprite);
    sprite.Frames[0].rectNumber = 2;
    sprite.Frames[0].textureNumber = 2;
    sb->getSprites().push_back(sprite);
	Solo_button.btn->setSpriteBank(sb);
	Solo_button.btn->setSprite(EGBS_BUTTON_UP, 0, video::SColor(255, 255, 255, 255), false);
	Solo_button.btn->setSprite(EGBS_BUTTON_MOUSE_OFF, 0, video::SColor(255, 255, 255, 255), false);
	Solo_button.btn->setSprite(EGBS_BUTTON_FOCUSED, 1, video::SColor(255, 255, 255, 255), false);
	Solo_button.btn->setSprite(EGBS_BUTTON_MOUSE_OVER, 2, video::SColor(255, 255, 255, 255), false);
    //play button
    Play_button.btn->setSpriteBank(sb);
    Play_button.btn->setSprite(EGBS_BUTTON_UP, 0, video::SColor(255, 255, 255, 255), false);
    Play_button.btn->setSprite(EGBS_BUTTON_MOUSE_OFF, 0, video::SColor(255, 255, 255, 255), false);
    Play_button.btn->setSprite(EGBS_BUTTON_FOCUSED, 1, video::SColor(255, 255, 255, 255), false);
    Play_button.btn->setSprite(EGBS_BUTTON_MOUSE_OVER, 2, video::SColor(255, 255, 255, 255), false);
    //quit button
    Quit_button.btn->setSpriteBank(sb);
    Quit_button.btn->setSprite(EGBS_BUTTON_UP, 0, video::SColor(255, 255, 255, 255), false);
    Quit_button.btn->setSprite(EGBS_BUTTON_MOUSE_OFF, 0, video::SColor(255, 255, 255, 255), false);
    Quit_button.btn->setSprite(EGBS_BUTTON_DOWN, 1, video::SColor(255, 255, 255, 255), false);
    Quit_button.btn->setSprite(EGBS_BUTTON_MOUSE_OVER, 2, video::SColor(255, 255, 255, 255), false);
    //option button
    Option_button.btn->setSpriteBank(sb);
    Option_button.btn->setSprite(EGBS_BUTTON_UP, 0, video::SColor(255, 255, 255, 255), false);
    Option_button.btn->setSprite(EGBS_BUTTON_MOUSE_OFF, 0, video::SColor(255, 255, 255, 255), false);
    Option_button.btn->setSprite(EGBS_BUTTON_FOCUSED, 2, video::SColor(255, 255, 255, 255), false);
    Option_button.btn->setSprite(EGBS_BUTTON_MOUSE_OVER, 2, video::SColor(255, 255, 255, 255), false);
    GM.addComponent(IrrD, &dvc);
    GM.addComponent(ButtonPlay, &Play_button);
	GM.addComponent(ButtonSolo, &Solo_button);
    GM.addComponent(ButtonQuit, &Quit_button);
    GM.addComponent(ButtonOption, &Option_button);
    SAppContext context;
    context.device = dvc.device;
    MyEventReceiver receiver(context);
    dvc.device->setEventReceiver(&receiver);
    video::ITexture* image_background = driver->getTexture("Image/background.jpg");
	if (play == true) {
		if (dvc.background_sound2.openFromFile("8bit/Themesong.wav")) {
			dvc.background_sound2.play();
			receiver.curr_pos = (s32)receiver.sound_value;
			dvc.background_sound2.setVolume(receiver.curr_pos);
			dvc.background_sound2.setLoop(true);
			receiver.check_song = true;
		}
	}
    while (dvc.device->run() && receiver.select_options == false && receiver.options == false && receiver.solo_menu == false) {
        driver->beginScene(true, true, false);
        driver->draw2DImage(image_background, core::position2d<s32>(0, 0), core::rect<s32>(0, 0, 1280, 720), 0, video::SColor(255, 255, 255, 255), true);
        driver->setTextureCreationFlag(ETCF_ALWAYS_32_BIT, true);
        GM.update();
    }
    std::cout << "Leaving main menu." << std::endl;
    if (receiver.options == true) {
        std::cout << "Entering Settings." << std::endl;
        optionMenu();
    }
    else if (receiver.select_options == true) {
        std::cout << "Entering game." << std::endl;
        selectionMenu();
	}
	else if (receiver.solo_menu == true) {
		std::cout << "Going solo." << std::endl;
		choiceMenuSolo();
	}
}

void menu()
{
    Device dvc("Bomberman");
    Entity IrrD = GM.addEntity();
    Entity ButtonPlay = GM.addEntity();
	Entity ButtonSolo = GM.addEntity();
    Entity ButtonQuit = GM.addEntity();
    Entity ButtonOption = GM.addEntity();
    IVideoDriver* driver = dvc.device->getVideoDriver();
    driver->enableMaterial2D();
    IGUIEnvironment* env = dvc.device->getGUIEnvironment();
    IGUISkin* skin = env->getSkin();
    IGUIFont* font = env->getFont("Fonts/bomberman20px.xml");
	sf::Sound btn_hover;
	sf::Sound btn_click;
	sf::SoundBuffer enterBuffer;
	sf::SoundBuffer enterBuffer2;
	if (!enterBuffer.loadFromFile("8bit/blip-1.wav") || \
		!enterBuffer2.loadFromFile("8bit/ok-2.wav") ||
		!dvc.background_sound2.openFromFile("8bit/Themesong.wav") ||
		!dvc.background_sound.openFromFile("8bit/Themesong.wav")) {
		std::cout << "Error with Loading of the btn_play_music" << std::endl;
		exit(84);
	}
	btn_hover.setBuffer(enterBuffer);
	btn_click.setBuffer(enterBuffer2);
    skin->setFont(env->getBuiltInFont(), EGDF_TOOLTIP);
	Button Solo_button(env, GUI_ID_SOLO_MODE_MENU, rect<s32>(58, 60, 58 + 316, 60 + 140), "S o l o", &btn_hover, &btn_click, driver, font);
    Button Play_button(env, GUI_ID_MENU_CHARACTER, rect<s32>(58, 220, 58 + 316, 220 + 140), "M u l t i", &btn_hover, &btn_click, driver, font);
    Button Quit_button(env, GUI_ID_QUIT_BUTTON, rect<s32>(58, 540, 58 + 316, 580 + 140), "Q u i t", \
        & btn_hover, &btn_click, driver, font);
    Button Option_button(env, GUI_ID_OPTION_BUTTON, rect<s32>(58, 380, 58 + 316, 380 + 140), "S e t t i n g s", &btn_hover, \
        & btn_click, driver, font);
	IGUIStaticText *credits = env->addStaticText(L"Game Made By: Vincent, Iskandre, Corentin", rect<s32>(1280 - 200, 720 - 20, 1280, 720), false);
    Play_button.btn->setDrawBorder(false);
	Solo_button.btn->setDrawBorder(false);
    Quit_button.btn->setDrawBorder(false);
    Option_button.btn->setDrawBorder(false);
    IGUISpriteBank* sb = env->addEmptySpriteBank("Button");
    ITexture* texture_normal = driver->getTexture("Image/button_normal2.png");
    ITexture* texture_clicked = driver->getTexture("Image/button_clicked2.png");
    ITexture* texture_hovered = driver->getTexture("Image/button_hovered2.png");
    sb->addTexture(texture_normal);
    sb->addTexture(texture_clicked);
    sb->addTexture(texture_hovered);
    sb->getPositions().push_back(core::rect<s32>(core::position2di(0, 0), texture_normal->getOriginalSize()));
    sb->getPositions().push_back(core::rect<s32>(core::position2di(0, 0), texture_clicked->getOriginalSize()));
    sb->getPositions().push_back(core::rect<s32>(core::position2di(0, 0), texture_hovered->getOriginalSize()));
    gui::SGUISprite sprite;
    gui::SGUISpriteFrame sframe;
    sprite.Frames.push_back(sframe);
    sprite.Frames[0].rectNumber = 0;
    sprite.Frames[0].textureNumber = 0;
    sb->getSprites().push_back(sprite);
    sprite.Frames[0].rectNumber = 1;
    sprite.Frames[0].textureNumber = 1;
    sb->getSprites().push_back(sprite);
    sprite.Frames[0].rectNumber = 2;
    sprite.Frames[0].textureNumber = 2;
    sb->getSprites().push_back(sprite);
	Solo_button.btn->setSpriteBank(sb);
	Solo_button.btn->setSprite(EGBS_BUTTON_UP, 0, video::SColor(255, 255, 255, 255), false);
	Solo_button.btn->setSprite(EGBS_BUTTON_MOUSE_OFF, 0, video::SColor(255, 255, 255, 255), false);
	Solo_button.btn->setSprite(EGBS_BUTTON_FOCUSED, 1, video::SColor(255, 255, 255, 255), false);
	Solo_button.btn->setSprite(EGBS_BUTTON_MOUSE_OVER, 2, video::SColor(255, 255, 255, 255), false);
    //play button
    Play_button.btn->setSpriteBank(sb);
    Play_button.btn->setSprite(EGBS_BUTTON_UP, 0, video::SColor(255, 255, 255, 255), false);
    Play_button.btn->setSprite(EGBS_BUTTON_MOUSE_OFF, 0, video::SColor(255, 255, 255, 255), false);
    Play_button.btn->setSprite(EGBS_BUTTON_FOCUSED, 1, video::SColor(255, 255, 255, 255), false);
    Play_button.btn->setSprite(EGBS_BUTTON_MOUSE_OVER, 2, video::SColor(255, 255, 255, 255), false);
    //quit button
    Quit_button.btn->setSpriteBank(sb);
    Quit_button.btn->setSprite(EGBS_BUTTON_UP, 0, video::SColor(255, 255, 255, 255), false);
    Quit_button.btn->setSprite(EGBS_BUTTON_MOUSE_OFF, 0, video::SColor(255, 255, 255, 255), false);
    Quit_button.btn->setSprite(EGBS_BUTTON_DOWN, 1, video::SColor(255, 255, 255, 255), false);
    Quit_button.btn->setSprite(EGBS_BUTTON_MOUSE_OVER, 2, video::SColor(255, 255, 255, 255), false);
    //option button
    Option_button.btn->setSpriteBank(sb);
    Option_button.btn->setSprite(EGBS_BUTTON_UP, 0, video::SColor(255, 255, 255, 255), false);
    Option_button.btn->setSprite(EGBS_BUTTON_MOUSE_OFF, 0, video::SColor(255, 255, 255, 255), false);
    Option_button.btn->setSprite(EGBS_BUTTON_FOCUSED, 2, video::SColor(255, 255, 255, 255), false);
    Option_button.btn->setSprite(EGBS_BUTTON_MOUSE_OVER, 2, video::SColor(255, 255, 255, 255), false);
    GM.addComponent(IrrD, &dvc);
    GM.addComponent(ButtonPlay, &Play_button);
	GM.addComponent(ButtonSolo, &Solo_button);
    GM.addComponent(ButtonQuit, &Quit_button);
    GM.addComponent(ButtonOption, &Option_button);
    SAppContext context;
    context.device = dvc.device;
    MyEventReceiver receiver(context);
    dvc.device->setEventReceiver(&receiver);
    video::ITexture* image_background = driver->getTexture("Image/background.jpg");
    dvc.background_sound.play();
    dvc.background_sound.setLoop(true);
    while (dvc.device->run() && receiver.select_options == false && receiver.options == false && receiver.solo_menu == false) {
        driver->beginScene(true, true, false);
        driver->draw2DImage(image_background, core::position2d<s32>(0, 0), core::rect<s32>(0, 0, 1280, 720), 0, video::SColor(255, 255, 255, 255), true);
        driver->setTextureCreationFlag(ETCF_ALWAYS_32_BIT, true);
        GM.update();
    }
    std::cout << "Leaving main menu." << std::endl;
    if (receiver.options == true) {
        std::cout << "Entering Settings." << std::endl;
        optionMenu();
    }
    else if (receiver.select_options == true) {
        std::cout << "Entering game." << std::endl;
        selectionMenu();
    }
	else if (receiver.solo_menu == true) {
		std::cout << "Going solo." << std::endl;
		choiceMenuSolo();
	}
}

unsigned short alivePlayers(Entity P1, Entity P2, Entity P3, Entity P4)
{
	if (GM.getComponent(P1, 6).bit != 0 && GM.getComponent(P2, 6).bit == 0 && GM.getComponent(P3, 6).bit == 0 && GM.getComponent(P4, 6).bit == 0)
		return 1;
	if (GM.getComponent(P2, 6).bit != 0 && GM.getComponent(P1, 6).bit == 0 && GM.getComponent(P3, 6).bit == 0 && GM.getComponent(P4, 6).bit == 0)
		return 2;
	if (GM.getComponent(P3, 6).bit != 0 && GM.getComponent(P2, 6).bit == 0 && GM.getComponent(P1, 6).bit == 0 && GM.getComponent(P4, 6).bit == 0)
		return 3;
	if (GM.getComponent(P4, 6).bit != 0 && GM.getComponent(P2, 6).bit == 0 && GM.getComponent(P3, 6).bit == 0 && GM.getComponent(P1, 6).bit == 0)
		return 4;
	return 0;
}

void gameLoop()
{
    Device &dvc = dynamic_cast<Device&>(GM.getComponent(0, 3));
    GM.Reset();
    Entity IrrD = GM.addEntity();
    Entity Camera = GM.addEntity();
    Entity Terrain = GM.addEntity();
    IVideoDriver *driver = dvc.device->getVideoDriver();
    driver->enableMaterial2D();
    IGUIEnvironment *env = dvc.device->getGUIEnvironment();
    env->clear();
    srand(time(NULL));
	int time = 900;
	SAppContext context;
	context.device = dvc.device;
	MyEventReceiver receiver(context);
	dvc.device->setEventReceiver(&receiver);
	double timerTick = 0.0f;
	double powerupTick = 0.0f;
    ISceneManager *smgr = dvc.device->getSceneManager();
    ISceneNode* skybox = smgr->addSkyBoxSceneNode(
        driver->getTexture("Image/bluecloud_up.jpg"),
        driver->getTexture("Image/bluecloud_dn.jpg"),
        driver->getTexture("Image/bluecloud_lf.jpg"),
        driver->getTexture("Image/bluecloud_rt.jpg"),
        driver->getTexture("Image/bluecloud_ft.jpg"),
        driver->getTexture("Image/bluecloud_bk.jpg"));
    Entity Player1 = GM.addEntity();
	AnimatedMesh PlayerMesh1(smgr, driver, "Ressources/bomberman.b3d", "Ressources/body.png", vector3df(4.5f), vector3df(0, -90, 0), vector3df(-184, 10, -134), AISTATE1);
	cout << PlayerMesh1._isAi << endl;
	Entity Player2 = GM.addEntity();
	AnimatedMesh PlayerMesh2(smgr, driver, "Ressources/bomberman.b3d", "Ressources/body2.png", vector3df(4.5f), vector3df(0, -90, 0), vector3df(184, 10, -134), AISTATE2);
	cout << PlayerMesh2._isAi << endl;
	Entity Player3 = GM.addEntity();
	AnimatedMesh PlayerMesh3(smgr, driver, "Ressources/bomberman.b3d", "Ressources/body3.png", vector3df(4.5f), vector3df(0, -90, 0), vector3df(-184, 10, 112), AISTATE3);
	cout << PlayerMesh3._isAi << endl;
	Entity Player4 = GM.addEntity();
	AnimatedMesh PlayerMesh4(smgr, driver, "Ressources/bomberman.b3d", "Ressources/body4.png", vector3df(4.5f), vector3df(0, -90, 0), vector3df(184, 10, 112), AISTATE4);
	cout << PlayerMesh4._isAi << endl;
    TerrainMesh TerrainM(smgr, driver, "Ressources/FlatTerrain.b3d", "Ressources/FlatTerrain_TX.jpg",vector3df(0.8f), vector3df(-90, 0, 0));
    skybox->setVisible(true);
    CameraNode camera(smgr, TerrainM.nMesh->getBoundingBox().getCenter(), TerrainM.nMesh->getBoundingBox().getCenter() - vector3df(-180,-330, 0));
    RigidBody rbPlayer;
	RigidBody rbPlayer2;
	RigidBody rbPlayer3;
	RigidBody rbPlayer4;
    int x, y = 0;
	int playerWin = 0;
    //create map
    int _random = rand() % 6 + 1;
    ifstream map_file("map/map_" + std::to_string(_random) + ".txt");
    std::string input;
    if (map_file.is_open()) {
        while (getline(map_file, input)) {
            x = 0;
            for (int i = 0, name = 1; i <= input.length(); i++, name++) {
                if (input[i] == '0') {
                    Entity Wall = GM.addEntity();
                    AnimatedMesh *_wall = new AnimatedMesh(smgr, driver, "Ressources/wall2.obj", "Ressources/wall2.png", vector3df(10.0f), vector3df(0, -90, 0), vector3df(-200 + x, 10, -150 + y));
                    _wall->animModel->setAnimationSpeed(0);
                    GM.addComponent(Wall, _wall);
                    GM.addComponent(Wall, &rbPlayer);
                }
                else if (input[i] == 'x') {
                    Entity Wall = GM.addEntity();
                    AnimatedMesh* mesh = new AnimatedMesh(smgr, driver, "Ressources/breakable_wall.b3d", "Ressources/bWall_TX.jpeg", vector3df(15.0f), vector3df(0, -90, 0), vector3df(-200 + x, 10, -150 + y));
                    BreakableWall* bWall = new BreakableWall;
                    GM.addComponent(Wall, mesh);
                    GM.addComponent(Wall, bWall);
                    GM.addComponent(Wall, &rbPlayer);
                }
                x += 16;
            }
            y += 16;
			_map.push_back(input);
			_map_col.push_back(input);
			_map_da.push_back(input);
        }
    } else
        std::cout << "Couldn't create map" << std::endl;

	unsigned beta = 0;
	for (auto ite = _map_col.begin(); ite != _map_col.end(); ++ite, ++beta) {
		for (unsigned i = 0; i < _map_col[beta].size(); ++i) {
			if (_map_col[beta].at(i) != ' ')
				_map_col[beta].at(i) = '1';
			else
				_map_col[beta].at(i) = '0';
		}
	}

	for (unsigned int l = 0; l < _map_da.size(); l++) {
		for (unsigned int j = 0; _map_da[l][j] != 0; j++) {
			_map_da[l][j] = '0';
		}
	}

    GM.addComponent(IrrD, &dvc);
    GM.addComponent(Player1, &PlayerMesh1);
    GM.addComponent(Player1, &player1_keybinding);
    GM.addComponent(Player1, &rbPlayer);
	GM.addComponent(Player2, &PlayerMesh2);
    GM.addComponent(Player2, &player2_keybinding);
    GM.addComponent(Player2, &rbPlayer2);
	GM.addComponent(Player3, &PlayerMesh3);
    GM.addComponent(Player3, &player3_keybinding);
    GM.addComponent(Player3, &rbPlayer3);
	GM.addComponent(Player4, &PlayerMesh4);
    GM.addComponent(Player4, &player4_keybinding);
    GM.addComponent(Player4, &rbPlayer4);
    GM.addComponent(Camera, &camera);
    GM.addComponent(Terrain, &TerrainM);

    sf::Music gameplay_music;
    sf::Music gameplay_intro;
    if (!gameplay_music.openFromFile("8bit/Battle.flac") || !gameplay_intro.openFromFile("Music/go.wav")) {
        std::cout << "Couldn't load the gameplay music" << std::endl;
        exit(84);
    }
    gameplay_intro.play();
    std::this_thread::sleep_for(std::chrono::seconds(4));
    gameplay_music.play();
    gameplay_music.setVolume(50);
    gameplay_music.setLoop(true);
	IGUIFont* font = env->getFont("Fonts/bomberman20px.xml");
	ITextSceneNode* cdText = smgr->addTextSceneNode(font, stringw(time).c_str(), SColor(255, 0, 0, 0), 0, vector3df(-330, 0, 0));
    while (dvc.device->run() && time > 0 && receiver.win == false) {
        driver->beginScene(true, true, false);
		if ((pTimer->getTime() - timerTick) > 1000) {
			time -= 1;
			cdText->setText(stringw(time).c_str());
			timerTick = pTimer->getTime();
		}
		if ((pTimer->getTime() - powerupTick) > 15000) {
			unsigned short type = (rand() % 2) + 1;
			Entity upE = GM.addEntity();
			Powerup* pu = new Powerup(type);
			AnimatedMesh* puMesh;
			int posX = (rand() % 368) - 184;
			int posZ = (rand() % 268) - 134;
			if (type == 1)
				puMesh = new AnimatedMesh(smgr, driver, "Ressources/bomb.b3d", "Ressources/BombUp.png", vector3df(16.0f), vector3df(0), vector3df(posX, 0, posZ));
			else if (type == 2)
				puMesh = new AnimatedMesh(smgr, driver, "Ressources/testSpeed.b3d", "Ressources/testSpeed.png", vector3df(3.0f), vector3df(0), vector3df(posX, 5, posZ));
			GM.addComponent(upE, puMesh);
			GM.addComponent(upE, pu);
			powerupTick = pTimer->getTime();
		}
        GM.update();
		playerWin = alivePlayers(Player1, Player2, Player3, Player4);
		if (playerWin == 1 || playerWin == 2 || playerWin == 3 || playerWin == 4) {
			receiver.win = true;
			std::cout << "PLAYER: " << playerWin << std::endl;
			gameplay_music.stop();
			std::cout << "WE HAVE A WINNER" << std::endl;
		}
    }
	if (receiver.win == true) {
		winMenu(playerWin);
	}
    dvc.device->drop();
}

void solo_mode(bool save)
{
	Device& dvc = dynamic_cast<Device&>(GM.getComponent(0, 3));
	dvc.background_sound.stop();
	GM.Reset();
	Entity IrrD = GM.addEntity();
	Entity Camera = GM.addEntity();
	Entity Terrain = GM.addEntity();
	IVideoDriver* driver = dvc.device->getVideoDriver();
	driver->enableMaterial2D();
	IGUIEnvironment* env = dvc.device->getGUIEnvironment();
	env->clear();
	static int level = 1;
	ISceneManager* smgr = dvc.device->getSceneManager();
	smgr->clear();
	static int seconds = 0;
	static int minutes = 0;
	ISceneNode* skybox = smgr->addSkyBoxSceneNode(
		driver->getTexture("Image/bluecloud_up.jpg"),
		driver->getTexture("Image/bluecloud_dn.jpg"),
		driver->getTexture("Image/bluecloud_lf.jpg"),
		driver->getTexture("Image/bluecloud_rt.jpg"),
		driver->getTexture("Image/bluecloud_ft.jpg"),
		driver->getTexture("Image/bluecloud_bk.jpg"));
	Entity Player1 = GM.addEntity();
	AnimatedMesh PlayerMesh1(smgr, driver, "Ressources/bomberman.b3d", "Ressources/body.png", vector3df(4.5f), vector3df(0, -90, 0), vector3df(-184, 10, -84));
	TerrainMesh TerrainM(smgr, driver, "Ressources/FlatTerrain.b3d", "Ressources/FlatTerrain_TX.jpg", vector3df(0.8f), vector3df(-90, 0, 0));
	skybox->setVisible(true);
	CameraNode camera(smgr, TerrainM.nMesh->getBoundingBox().getCenter(), TerrainM.nMesh->getBoundingBox().getCenter() - vector3df(-80, -330, 0));
	RigidBody rbPlayer;
	int x, y = 0;
	std::string whatLevel;
	std::string input;
	fstream save_file("save.txt");
	if (save_file.is_open())
		getline(save_file, whatLevel);
	if (!save) {
		input = "map/stage_1.txt";
		level = 1;
	}
	else if (save) {
		level = atoi(whatLevel.substr(whatLevel.find('_') + 1, whatLevel.find('.')).c_str());
		if (level == 0)
			level = 1;
		if (level == 8)
			level = 1;
		input = "map/stage_" + std::to_string(level) + ".txt";
	}
	save_file.clear();
	save_file.close();
	ifstream map_file(input);
	if (map_file.is_open()) {
		while (getline(map_file, input)) {
			x = 0;
			for (int i = 0, name = 1; i <= input.length(); i++, name++) {
				if (input[i] == '0') {
					Entity Wall = GM.addEntity();
					AnimatedMesh* _wall = new AnimatedMesh(smgr, driver, "Ressources/wall2.obj", "Ressources/wall2.png", vector3df(10.0f), vector3df(0, -90, 0), vector3df(-200 + x, 10, -100 + y));
					_wall->animModel->setAnimationSpeed(0);
					GM.addComponent(Wall, _wall);
					GM.addComponent(Wall, &rbPlayer);
				}
				else if (input[i] == 'x') {
					Entity Wall = GM.addEntity();
					AnimatedMesh* mesh = new AnimatedMesh(smgr, driver, "Ressources/breakable_wall.b3d", "Ressources/bWall_TX.jpeg", vector3df(15.0f), vector3df(0, -90, 0), vector3df(-200 + x, 10, -100 + y));
					BreakableWall* bWall = new BreakableWall;
					GM.addComponent(Wall, mesh);
					GM.addComponent(Wall, bWall);
					GM.addComponent(Wall, &rbPlayer);
				}
				else if (input[i] == 'F') {
					Entity end = GM.addEntity();
					AnimatedMesh* exitMesh = new AnimatedMesh(smgr, driver, "Ressources/exit.b3d", "Ressources/exit.png", vector3df(10.0f), vector3df(0, 0, 0), vector3df(-200 + x, 10, -100 + y));
					Finish *fnsh = new Finish;
					GM.addComponent(end, exitMesh);
					GM.addComponent(end, fnsh);
				}
				x += 16;
			}
			y += 16;
			_map.push_back(input);
		}
	}
	else
		std::cout << "Couldn't create map" << std::endl;
	map_file.close();
	GM.addComponent(IrrD, &dvc);
	GM.addComponent(Player1, &PlayerMesh1);
	GM.addComponent(Player1, &player1_keybinding);
	GM.addComponent(Player1, &rbPlayer);
	GM.addComponent(Camera, &camera);
	GM.addComponent(Terrain, &TerrainM);
	SAppContext context;
	context.device = dvc.device;
	MyEventReceiver receiver(context);
	dvc.device->setEventReceiver(&receiver);
	sf::Music gameplay_music;
	if (!gameplay_music.openFromFile("8bit/Battle.flac")) {
		std::cout << "Couldn't load the gameplay music" << std::endl;
		exit(84);
	}
	double chronoTick = 0.0f;
	std::string strTime;
	IGUIFont* font = env->getFont("Fonts/bomberman20px.xml");
	ITextSceneNode* cdText = smgr->addTextSceneNode(font, stringw("0:0").c_str(), SColor(255, 0, 0, 0), 0, vector3df(0, 230, 75));
	gameplay_music.play();
	gameplay_music.setVolume(50);
	gameplay_music.setLoop(true);
	while (dvc.device->run() && !PlayerMesh1.hasWin && GM.getComponent(Player1, 6).bit != 0) {
		driver->beginScene(true, true, false);
		if ((pTimer->getTime() - chronoTick) > 1000) {
			seconds += 1;
			if (seconds % 60 == 0) {
				seconds = 0;
				minutes += 1;
			}
			if (seconds < 10)
				strTime = "T i m e :   " + std::to_string(minutes) + " : 0" + std::to_string(seconds);
			else
				strTime = "T i m e :   " + std::to_string(minutes) + " : " + std::to_string(seconds);
			cdText->setText(stringw(strTime.c_str()).c_str());
			chronoTick = pTimer->getTime();
		}
		GM.update();
	}
	if (PlayerMesh1.hasWin) {
		save_file.open("save.txt");
		level += 1;
		save_file << "map/stage_" + std::to_string(level) + ".txt";
		save_file.close();
		gameplay_music.stop();
		if (level != 8)
			solo_mode(true);
		else
			winMenuSolo(minutes, seconds);
	}
	else {
		gameplay_music.stop();
		LoseMenuSolo();
	}
	std::cout << "out of loop" << std::endl;
}

void optionMenu()
{
	static int name = 0;
	Device& dvc = dynamic_cast<Device&>(GM.getComponent(0, 3));
	GM.Reset();
	Entity IrrD = GM.addEntity();
	Entity Keybinding_btn = GM.addEntity();
	Entity Sound_btn = GM.addEntity();
	Entity Back_btn = GM.addEntity();
	IVideoDriver* driver = dvc.device->getVideoDriver();
	IGUIEnvironment* env = dvc.device->getGUIEnvironment();
	env->clear();
	ISceneManager* smgr = dvc.device->getSceneManager();
	IGUISkin* skin = env->getSkin();
	IGUIFont* font = env->getFont("Fonts/bomberman20px.xml");
	sf::Sound btn_hover;
	sf::Sound btn_click;
	sf::SoundBuffer enterBuffer;
	sf::SoundBuffer enterBuffer2;
	if (!enterBuffer.loadFromFile("8bit/blip-1.wav") || \
		!enterBuffer2.loadFromFile("8bit/ok-2.wav")) {
		std::cout << "Error with Loading of the btn_play_music" << std::endl;
		exit(84);
	}
	btn_hover.setBuffer(enterBuffer);
	btn_click.setBuffer(enterBuffer2);
	skin->setFont(env->getBuiltInFont(), EGDF_TOOLTIP);
	Button Keybinds(env, GUI_ID_KEYBIND, rect<s32>(1280 / 2 - 316 - 20, 720 / 2 - 140, 1280 / 2 - 20, 720 / 2), "K e y b i n d s", &btn_hover, &btn_click, driver, font);
	Button Sound(env, GUI_ID_SOUND, rect<s32>(1280 / 2 + 20, 720 / 2 - 140, 1280 / 2 + 316 + 20, 720 / 2), "A u d i o", &btn_hover, &btn_click, driver, font);
	Button Back(env, GUI_ID_MENU, rect<s32>(0, 720 - 88, 0 + 86, 720), "", &btn_hover, &btn_click, driver, font);
	Keybinds.btn->setDrawBorder(false);
	Sound.btn->setDrawBorder(false);
	Back.btn->setDrawBorder(false);
	IGUISpriteBank* sb = env->addEmptySpriteBank(("Buttons_" + std::to_string(name)).c_str());
	name += 1;
	ITexture* texture_normal = driver->getTexture("Image/button_normal2.png");
	ITexture* texture_clicked = driver->getTexture("Image/button_clicked2.png");
	ITexture* texture_hovered = driver->getTexture("Image/button_hovered2.png");
	ITexture* back_normal = driver->getTexture("Image/back_normal.png");
	ITexture* back_hovered = driver->getTexture("Image/back_hovered.png");
	sb->addTexture(texture_normal);
	sb->addTexture(texture_clicked);
	sb->addTexture(texture_hovered);
	sb->addTexture(back_normal);
	sb->addTexture(back_hovered);
	sb->getPositions().push_back(core::rect<s32>(core::position2di(0, 0), texture_normal->getOriginalSize()));
	sb->getPositions().push_back(core::rect<s32>(core::position2di(0, 0), texture_clicked->getOriginalSize()));
	sb->getPositions().push_back(core::rect<s32>(core::position2di(0, 0), texture_hovered->getOriginalSize()));
	sb->getPositions().push_back(core::rect<s32>(core::position2di(0, 0), back_normal->getOriginalSize()));
	sb->getPositions().push_back(core::rect<s32>(core::position2di(0, 0), back_hovered->getOriginalSize()));
	gui::SGUISprite sprite;
	gui::SGUISpriteFrame sframe;
	sprite.Frames.push_back(sframe);
	sprite.Frames[0].rectNumber = 0;
	sprite.Frames[0].textureNumber = 0;
	sb->getSprites().push_back(sprite);
	sprite.Frames[0].rectNumber = 1;
	sprite.Frames[0].textureNumber = 1;
	sb->getSprites().push_back(sprite);
	sprite.Frames[0].rectNumber = 2;
	sprite.Frames[0].textureNumber = 2;
	sb->getSprites().push_back(sprite);
	sprite.Frames[0].rectNumber = 3;
	sprite.Frames[0].textureNumber = 3;
	sb->getSprites().push_back(sprite);
	sprite.Frames[0].rectNumber = 4;
	sprite.Frames[0].textureNumber = 4;
	sb->getSprites().push_back(sprite);
	Keybinds.btn->setSpriteBank(sb);
	Keybinds.btn->setSprite(EGBS_BUTTON_UP, 0, video::SColor(255, 255, 255, 255), false);
	Keybinds.btn->setSprite(EGBS_BUTTON_MOUSE_OFF, 0, video::SColor(255, 255, 255, 255), false);
	Keybinds.btn->setSprite(EGBS_BUTTON_FOCUSED, 1, video::SColor(255, 255, 255, 255), false);
	Keybinds.btn->setSprite(EGBS_BUTTON_MOUSE_OVER, 2, video::SColor(255, 255, 255, 255), false);
	Sound.btn->setSpriteBank(sb);
	Sound.btn->setSprite(EGBS_BUTTON_UP, 0, video::SColor(255, 255, 255, 255), false);
	Keybinds.btn->setSprite(EGBS_BUTTON_MOUSE_OFF, 0, video::SColor(255, 255, 255, 255), false);
	Keybinds.btn->setSprite(EGBS_BUTTON_DOWN, 1, video::SColor(255, 255, 255, 255), false);
	Sound.btn->setSprite(EGBS_BUTTON_MOUSE_OVER, 2, video::SColor(255, 255, 255, 255), false);
	Keybinds.btn->setSprite(EGBS_BUTTON_MOUSE_OFF, 0, video::SColor(255, 255, 255, 255), false);
	Keybinds.btn->setSprite(EGBS_BUTTON_FOCUSED, 2, video::SColor(255, 255, 255, 255), false);
	Back.btn->setSpriteBank(sb);
	Back.btn->setSprite(EGBS_BUTTON_UP, 3, video::SColor(255, 255, 255, 255), false);
	Back.btn->setSprite(EGBS_BUTTON_MOUSE_OFF, 3, video::SColor(255, 255, 255, 255), false);
	Back.btn->setSprite(EGBS_BUTTON_FOCUSED, 4, video::SColor(255, 255, 255, 255), false);
	Back.btn->setSprite(EGBS_BUTTON_MOUSE_OVER, 4, video::SColor(255, 255, 255, 255), false);
	GM.addComponent(IrrD, &dvc);
	GM.addComponent(Keybinding_btn, &Keybinds);
	GM.addComponent(Sound_btn, &Sound);
	GM.addComponent(Back_btn, &Back);
	SAppContext context;
	context.device = dvc.device;
	MyEventReceiver receiver(context);
	dvc.device->setEventReceiver(&receiver);
	video::ITexture* img_background_one = driver->getTexture("Image/background.jpg");
	while (dvc.device->run() && receiver.sound_menu == false && receiver.keybinding_menu == false && receiver.back == false) {
		driver->beginScene(true, true, false);
		driver->draw2DImage(img_background_one, core::position2d<s32>(0, 0), core::rect<s32>(0, 0, 1280, 720), 0, video::SColor(255, 255, 255, 255), true);
		driver->setTextureCreationFlag(ETCF_ALWAYS_32_BIT, true);
		GM.update();
	}
	if (receiver.back == true) {
		menu_back(false);
	}
	else if (receiver.keybinding_menu == true) {
		keybindMenu();
	}
	else if (receiver.sound_menu == true) {
		soundMenu();
	}
}

void keybindMenu()
{
	Device& dvc = dynamic_cast<Device&>(GM.getComponent(0, 3));
	GM.Reset();
	Entity IrrD = GM.addEntity();
	Entity backBtn = GM.addEntity();
	Entity Player1 = GM.addEntity();
	Entity Player2 = GM.addEntity();
	Entity Player3 = GM.addEntity();
	Entity Player4 = GM.addEntity();
	Entity player1Forward = GM.addEntity();
	Entity player1Backward = GM.addEntity();
	Entity player1Left = GM.addEntity();
	Entity player1Right = GM.addEntity();
	Entity player1Bomb = GM.addEntity();
	Entity player2Forward = GM.addEntity();
	Entity player2Backward = GM.addEntity();
	Entity player2Left = GM.addEntity();
	Entity player2Right = GM.addEntity();
	Entity player2Bomb = GM.addEntity();
	Entity player3Forward = GM.addEntity();
	Entity player3Backward = GM.addEntity();
	Entity player3Left = GM.addEntity();
	Entity player3Right = GM.addEntity();
	Entity player3Bomb = GM.addEntity();
	Entity player4Forward = GM.addEntity();
	Entity player4Backward = GM.addEntity();
	Entity player4Left = GM.addEntity();
	Entity player4Right = GM.addEntity();
	Entity player4Bomb = GM.addEntity();
	IVideoDriver* driver = dvc.device->getVideoDriver();
	IGUIEnvironment* env = dvc.device->getGUIEnvironment();
	vector<Button> buttonArrayPlayer1;
	vector<Button> buttonArrayPlayer2;
	vector<Button> buttonArrayPlayer3;
	vector<Button> buttonArrayPlayer4;

	static int name = 0;
	int nb_button = 0;
	env->clear();
	SAppContext context;
	SEvent event;
	std::stringstream ss;
	std::string s;
	context.device = dvc.device;
	MyEventReceiver receiver(context);
	dvc.device->setEventReceiver(&receiver);
	ISceneManager* smgr = dvc.device->getSceneManager();
	IGUISkin* skin = env->getSkin();
	IGUIFont* font = env->getFont("Fonts/chickenPie.xml");
	IGUIFont* textFont = env->getFont("Fonts/bomberman20px.xml");
	IGUIFont* textFont2 = env->getFont("Fonts/bomberman12px.xml");
	sf::Sound btn_hover;
	sf::Sound btn_click;
	sf::SoundBuffer enterBuffer;
	sf::SoundBuffer enterBuffer2;
	if (!enterBuffer.loadFromFile("8bit/blip-1.wav") || \
		!enterBuffer2.loadFromFile("8bit/ok-2.wav")) {
		std::cout << "Error with Loading of the btn_play_music" << std::endl;
		exit(84);
	}
	btn_hover.setBuffer(enterBuffer);
	btn_click.setBuffer(enterBuffer2);
	Button Back(env, GUI_BACK_SETTINGS_KEYBIND, rect<s32>(0, 720 - 88, 0 + 86, 720), "", &btn_hover, &btn_click, driver, font);
	Back.btn->setDrawBorder(false);

	IGUIStaticText* player1Text = env->addStaticText(L"P L A Y E R  1", rect<s32>(140, 10, 140 + 215, 10 + 25), true, false, 0, -1, true);
	player1Text->setOverrideFont(textFont);
	player1Text->setOverrideColor(video::SColor(255, 0, 255, 0));
	player1Text->setBackgroundColor(video::SColor(255, 96, 96, 96));
	IGUIStaticText* player1FText = env->addStaticText(L"u p -", rect<s32>(55, 60, 55 + 95, 60 + 30), false);  // x = 50 diff   y = 15 diff
	player1FText->setOverrideFont(textFont2);
	player1FText->setOverrideColor(video::SColor(255, 0, 0, 0));
	IGUIStaticText* player1BText = env->addStaticText(L"d o w n -", rect<s32>(215, 60, 215 + 95, 60 + 30), false);   // x = 90 diff   y = 15 diff
	player1BText->setOverrideFont(textFont2);
	player1BText->setOverrideColor(video::SColor(255, 0, 0, 0));
	IGUIStaticText* player1LText = env->addStaticText(L"l e f t -", rect<s32>(30, 120, 30 + 95, 120 + 30), false);   // x = 80 diff   y = 15 diff
	player1LText->setOverrideFont(textFont2);
	player1LText->setOverrideColor(video::SColor(255, 0, 0, 0));
	IGUIStaticText* player1RText = env->addStaticText(L"r i g h t -", rect<s32>(210, 120, 210 + 95, 120 + 30), false);   // x = 95 diff   y = 15 diff
	player1RText->setOverrideFont(textFont2);
	player1RText->setOverrideColor(video::SColor(255, 0, 0, 0));
	IGUIStaticText* player1BoText = env->addStaticText(L"b o m b -", rect<s32>(15, 180, 35 + 95, 180 + 30), false);   // x = 90 diff   y = 15 diff
	player1BoText->setOverrideFont(textFont2);
	player1BoText->setOverrideColor(video::SColor(255, 0, 0, 0));

	Button player1Forward_btn(env, PLAYER1FORWARD_BTN, rect<s32>(105, 45, 105 + 95, 45 + 45), KeyNames[player1_keybinding.keybindVector[0]], &btn_hover, &btn_click, driver, font);


	Button player1Backward_btn(env, PLAYER1BACKWARD_BTN, rect<s32>(305, 45, 305 + 95, 45 + 45), KeyNames[player1_keybinding.keybindVector[1]], &btn_hover, &btn_click, driver, font);


	Button player1Left_btn(env, PLAYER1LEFT_BTN, rect<s32>(105, 105, 105 + 95, 105 + 45), KeyNames[player1_keybinding.keybindVector[2]], &btn_hover, &btn_click, driver, font);


	Button player1Right_btn(env, PLAYER1RIGHT_BTN, rect<s32>(305, 105, 305 + 95, 105 + 45), KeyNames[player1_keybinding.keybindVector[3]], &btn_hover, &btn_click, driver, font);


	Button player1Bomb_btn(env, PLAYER1BOMB_BTN, rect<s32>(105, 165, 105 + 95, 165 + 45), KeyNames[player1_keybinding.keybindVector[4]], &btn_hover, &btn_click, driver, font);





	IGUIStaticText* player2Text = env->addStaticText(L"P L A Y E R  2", rect<s32>(840, 10, 840 + 220, 10 + 25), true, false, 0, -1, true);
	player2Text->setOverrideFont(textFont);
	player2Text->setOverrideColor(video::SColor(255, 0, 255, 0));
	player2Text->setBackgroundColor(video::SColor(255, 96, 96, 96));
	IGUIStaticText* player2FText = env->addStaticText(L"u p -", rect<s32>(755, 60, 755 + 95, 60 + 30), false);  // x = 50 diff   y = 15 diff
	player2FText->setOverrideFont(textFont2);
	player2FText->setOverrideColor(video::SColor(255, 51, 255, 255));
	IGUIStaticText* player2BText = env->addStaticText(L"d o w n -", rect<s32>(915, 60, 915 + 95, 60 + 30), false);   // x = 90 diff   y = 15 diff
	player2BText->setOverrideFont(textFont2);
	player2BText->setOverrideColor(video::SColor(255, 51, 255, 255));
	IGUIStaticText* player2LText = env->addStaticText(L"l e f t -", rect<s32>(725, 120, 725 + 95, 120 + 30), false);   // x = 80 diff   y = 15 diff
	player2LText->setOverrideFont(textFont2);
	player2LText->setOverrideColor(video::SColor(255, 51, 255, 255));
	IGUIStaticText* player2RText = env->addStaticText(L"r i g h t -", rect<s32>(910, 120, 910 + 95, 120 + 30), false);   // x = 95 diff   y = 15 diff
	player2RText->setOverrideFont(textFont2);
	player2RText->setOverrideColor(video::SColor(255, 51, 255, 255));
	IGUIStaticText* player2BoText = env->addStaticText(L"b o m b -", rect<s32>(715, 180, 715 + 95, 180 + 30), false);   // x = 90 diff   y = 15 diff
	player2BoText->setOverrideFont(textFont2);
	player2BoText->setOverrideColor(video::SColor(255, 51, 255, 255));


	Button player2Forward_btn(env, PLAYER2FORWARD_BTN, rect<s32>(805, 45, 805 + 95, 45 + 45), KeyNames[player2_keybinding.keybindVector[0]], &btn_hover, &btn_click, driver, font);


	Button player2Backward_btn(env, PLAYER2BACKWARD_BTN, rect<s32>(1005, 45, 1005 + 95, 45 + 45), KeyNames[player2_keybinding.keybindVector[1]], &btn_hover, &btn_click, driver, font);


	Button player2Left_btn(env, PLAYER2LEFT_BTN, rect<s32>(805, 105, 805 + 95, 105 + 45), KeyNames[player2_keybinding.keybindVector[2]], &btn_hover, &btn_click, driver, font);


	Button player2Right_btn(env, PLAYER2RIGHT_BTN, rect<s32>(1005, 105, 1005 + 95, 105 + 45), KeyNames[player2_keybinding.keybindVector[3]], &btn_hover, &btn_click, driver, font);


	Button player2Bomb_btn(env, PLAYER2BOMB_BTN, rect<s32>(805, 165, 805 + 95, 165 + 45), KeyNames[player2_keybinding.keybindVector[4]], &btn_hover, &btn_click, driver, font);







	IGUIStaticText* player3Text = env->addStaticText(L"P L A Y E R  3", rect<s32>(140, 390, 140 + 220, 390 + 25), true, false, 0, -1, true);
	player3Text->setOverrideFont(textFont);
	player3Text->setOverrideColor(video::SColor(255, 0, 255, 0));
	player3Text->setBackgroundColor(video::SColor(255, 96, 96, 96));
	IGUIStaticText* player3FText = env->addStaticText(L"u p -", rect<s32>(55, 440, 55 + 95, 440 + 30), false);  // x = 50 diff   y = 15 diff
	player3FText->setOverrideFont(textFont2);
	player3FText->setOverrideColor(video::SColor(255, 255, 255, 255));
	IGUIStaticText* player3BText = env->addStaticText(L"d o w n -", rect<s32>(215, 440, 215 + 95, 440 + 30), false);   // x = 90 diff   y = 15 diff
	player3BText->setOverrideFont(textFont2);
	player3BText->setOverrideColor(video::SColor(255, 255, 255, 255));
	IGUIStaticText* player3LText = env->addStaticText(L"l e f t -", rect<s32>(30, 500, 30 + 95, 500 + 30), false);   // x = 80 diff   y = 15 diff
	player3LText->setOverrideFont(textFont2);
	player3LText->setOverrideColor(video::SColor(255, 255, 255, 255));
	IGUIStaticText* player3RText = env->addStaticText(L"r i g h t -", rect<s32>(210, 500, 210 + 95, 500 + 30), false);   // x = 95 diff   y = 15 diff
	player3RText->setOverrideFont(textFont2);
	player3RText->setOverrideColor(video::SColor(255, 255, 255, 255));
	IGUIStaticText* player3BoText = env->addStaticText(L"b o m b -", rect<s32>(15, 560, 35 + 95, 560 + 30), false);   // x = 90 diff   y = 15 diff
	player3BoText->setOverrideFont(textFont2);
	player3BoText->setOverrideColor(video::SColor(255, 255, 255, 255));


	Button player3Forward_btn(env, PLAYER3FORWARD_BTN, rect<s32>(105, 425, 105 + 95, 425 + 45), KeyNames[player3_keybinding.keybindVector[0]], &btn_hover, &btn_click, driver, font);


	Button player3Backward_btn(env, PLAYER3BACKWARD_BTN, rect<s32>(305, 425, 305 + 95, 425 + 45), KeyNames[player3_keybinding.keybindVector[1]], &btn_hover, &btn_click, driver, font);


	Button player3Left_btn(env, PLAYER3LEFT_BTN, rect<s32>(105, 485, 105 + 95, 485 + 45), KeyNames[player3_keybinding.keybindVector[2]], &btn_hover, &btn_click, driver, font);


	Button player3Right_btn(env, PLAYER3RIGHT_BTN, rect<s32>(305, 485, 305 + 95, 485 + 45), KeyNames[player3_keybinding.keybindVector[3]], &btn_hover, &btn_click, driver, font);


	Button player3Bomb_btn(env, PLAYER3BOMB_BTN, rect<s32>(105, 545, 105 + 95, 545 + 45), KeyNames[player3_keybinding.keybindVector[4]], &btn_hover, &btn_click, driver, font);







	IGUIStaticText* player4Text = env->addStaticText(L"P L A Y E R  4", rect<s32>(840, 390, 840 + 220, 390 + 25), true, false, 0, -1, true);
	player4Text->setOverrideFont(textFont);
	player4Text->setOverrideColor(video::SColor(255, 0, 255, 0));
	player4Text->setBackgroundColor(video::SColor(255, 96, 96, 96));

	IGUIStaticText* player4FText = env->addStaticText(L"u p -", rect<s32>(755, 440, 755 + 95, 440 + 30), false);  // x = 50 diff   y = 15 diff
	player4FText->setOverrideFont(textFont2);
	player4FText->setOverrideColor(video::SColor(255, 51, 255, 255));
	IGUIStaticText* player4BText = env->addStaticText(L"d o w n -", rect<s32>(915, 440, 915 + 95, 440 + 30), false);   // x = 90 diff   y = 15 diff
	player4BText->setOverrideFont(textFont2);
	player4BText->setOverrideColor(video::SColor(255, 51, 255, 255));
	IGUIStaticText* player4LText = env->addStaticText(L"l e f t -", rect<s32>(725, 500, 725 + 95, 500 + 30), false);   // x = 80 diff   y = 15 diff
	player4LText->setOverrideFont(textFont2);
	player4LText->setOverrideColor(video::SColor(255, 51, 255, 255));
	IGUIStaticText* player4RText = env->addStaticText(L"r i g h t -", rect<s32>(910, 500, 910 + 95, 500 + 30), false);   // x = 95 diff   y = 15 diff
	player4RText->setOverrideFont(textFont2);
	player4RText->setOverrideColor(video::SColor(255, 51, 255, 255));
	IGUIStaticText* player4BoText = env->addStaticText(L"b o m b -", rect<s32>(715, 560, 715 + 95, 560 + 30), false);   // x = 90 diff   y = 15 diff
	player4BoText->setOverrideFont(textFont2);
	player4BoText->setOverrideColor(video::SColor(255, 51, 255, 255));




	Button player4Forward_btn(env, PLAYER4FORWARD_BTN, rect<s32>(805, 425, 805 + 95, 425 + 45), KeyNames[player4_keybinding.keybindVector[0]], &btn_hover, &btn_click, driver, font);


	Button player4Backward_btn(env, PLAYER4BACKWARD_BTN, rect<s32>(1005, 425, 1005 + 95, 425 + 45), KeyNames[player4_keybinding.keybindVector[1]], &btn_hover, &btn_click, driver, font);


	Button player4Left_btn(env, PLAYER4LEFT_BTN, rect<s32>(805, 485, 805 + 95, 485 + 45), KeyNames[player4_keybinding.keybindVector[2]], &btn_hover, &btn_click, driver, font);


	Button player4Right_btn(env, PLAYER4RIGHT_BTN, rect<s32>(1005, 485, 1005 + 95, 485 + 45), KeyNames[player4_keybinding.keybindVector[3]], &btn_hover, &btn_click, driver, font);


	Button player4Bomb_btn(env, PLAYER4BOMB_BTN, rect<s32>(805, 545, 805 + 95, 545 + 45), KeyNames[player4_keybinding.keybindVector[4]], &btn_hover, &btn_click, driver, font);




	player1Forward_btn.btn->setDrawBorder(false);
	player1Backward_btn.btn->setDrawBorder(false);
	player1Left_btn.btn->setDrawBorder(false);
	player1Right_btn.btn->setDrawBorder(false);
	player1Bomb_btn.btn->setDrawBorder(false);
	player2Forward_btn.btn->setDrawBorder(false);
	player2Backward_btn.btn->setDrawBorder(false);
	player2Left_btn.btn->setDrawBorder(false);
	player2Right_btn.btn->setDrawBorder(false);
	player2Bomb_btn.btn->setDrawBorder(false);
	player3Forward_btn.btn->setDrawBorder(false);
	player3Backward_btn.btn->setDrawBorder(false);
	player3Left_btn.btn->setDrawBorder(false);
	player3Right_btn.btn->setDrawBorder(false);
	player3Bomb_btn.btn->setDrawBorder(false);
	player4Forward_btn.btn->setDrawBorder(false);
	player4Backward_btn.btn->setDrawBorder(false);
	player4Left_btn.btn->setDrawBorder(false);
	player4Right_btn.btn->setDrawBorder(false);
	player4Bomb_btn.btn->setDrawBorder(false);

	buttonArrayPlayer1.push_back(player1Forward_btn);
	buttonArrayPlayer1.push_back(player1Backward_btn);
	buttonArrayPlayer1.push_back(player1Left_btn);
	buttonArrayPlayer1.push_back(player1Right_btn);
	buttonArrayPlayer1.push_back(player1Bomb_btn);
	buttonArrayPlayer2.push_back(player2Forward_btn);
	buttonArrayPlayer2.push_back(player2Backward_btn);
	buttonArrayPlayer2.push_back(player2Left_btn);
	buttonArrayPlayer2.push_back(player2Right_btn);
	buttonArrayPlayer2.push_back(player2Bomb_btn);
	buttonArrayPlayer3.push_back(player3Forward_btn);
	buttonArrayPlayer3.push_back(player3Backward_btn);
	buttonArrayPlayer3.push_back(player3Left_btn);
	buttonArrayPlayer3.push_back(player3Right_btn);
	buttonArrayPlayer3.push_back(player3Bomb_btn);
	buttonArrayPlayer4.push_back(player4Forward_btn);
	buttonArrayPlayer4.push_back(player4Backward_btn);
	buttonArrayPlayer4.push_back(player4Left_btn);
	buttonArrayPlayer4.push_back(player4Right_btn);
	buttonArrayPlayer4.push_back(player4Bomb_btn);

	IGUISpriteBank* sb = env->addEmptySpriteBank(("backSettings" + std::to_string(name)).c_str());
	name += 1;
	ITexture* back_normal = driver->getTexture("Image/back_normal.png");
	ITexture* back_hovered = driver->getTexture("Image/back_hovered.png");
	ITexture* kb_btn_normal = driver->getTexture("Image/button_normal_keybind.png");
	ITexture* kb_btn_pressed = driver->getTexture("Image/button_focused_keybind.png");
	sb->addTexture(back_normal);
	sb->addTexture(back_hovered);
	sb->addTexture(kb_btn_normal);
	sb->addTexture(kb_btn_pressed);
	sb->getPositions().push_back(core::rect<s32>(core::position2di(0, 0), back_normal->getOriginalSize()));
	sb->getPositions().push_back(core::rect<s32>(core::position2di(0, 0), back_hovered->getOriginalSize()));
	sb->getPositions().push_back(core::rect<s32>(core::position2di(0, 0), kb_btn_normal->getOriginalSize()));
	sb->getPositions().push_back(core::rect<s32>(core::position2di(0, 0), kb_btn_pressed->getOriginalSize()));
	gui::SGUISprite sprite;
	gui::SGUISpriteFrame sframe;
	sprite.Frames.push_back(sframe);
	sprite.Frames[0].rectNumber = 0;
	sprite.Frames[0].textureNumber = 0;
	sb->getSprites().push_back(sprite);
	sprite.Frames[0].rectNumber = 1;
	sprite.Frames[0].textureNumber = 1;
	sb->getSprites().push_back(sprite);
	sprite.Frames[0].rectNumber = 2;
	sprite.Frames[0].textureNumber = 2;
	sb->getSprites().push_back(sprite);
	sprite.Frames[0].rectNumber = 3;
	sprite.Frames[0].textureNumber = 3;
	sb->getSprites().push_back(sprite);




	Back.btn->setSpriteBank(sb);
	player1Forward_btn.btn->setSpriteBank(sb);
	player1Backward_btn.btn->setSpriteBank(sb);
	player1Left_btn.btn->setSpriteBank(sb);
	player1Right_btn.btn->setSpriteBank(sb);
	player1Bomb_btn.btn->setSpriteBank(sb);
	player2Forward_btn.btn->setSpriteBank(sb);
	player2Backward_btn.btn->setSpriteBank(sb);
	player2Left_btn.btn->setSpriteBank(sb);
	player2Right_btn.btn->setSpriteBank(sb);
	player2Bomb_btn.btn->setSpriteBank(sb);
	player3Forward_btn.btn->setSpriteBank(sb);
	player3Backward_btn.btn->setSpriteBank(sb);
	player3Left_btn.btn->setSpriteBank(sb);
	player3Right_btn.btn->setSpriteBank(sb);
	player3Bomb_btn.btn->setSpriteBank(sb);
	player4Forward_btn.btn->setSpriteBank(sb);
	player4Backward_btn.btn->setSpriteBank(sb);
	player4Left_btn.btn->setSpriteBank(sb);
	player4Right_btn.btn->setSpriteBank(sb);
	player4Bomb_btn.btn->setSpriteBank(sb);


	Back.btn->setSprite(EGBS_BUTTON_UP, 0, video::SColor(255, 255, 255, 255), false);
	Back.btn->setSprite(EGBS_BUTTON_MOUSE_OFF, 0, video::SColor(255, 255, 255, 255), false);
	Back.btn->setSprite(EGBS_BUTTON_FOCUSED, 1, video::SColor(255, 255, 255, 255), false);
	Back.btn->setSprite(EGBS_BUTTON_DOWN, 1, video::SColor(255, 255, 255, 255), false);

	player1Forward_btn.btn->setSprite(EGBS_BUTTON_UP, 2, video::SColor(255, 255, 255, 255), false);
	player1Forward_btn.btn->setSprite(EGBS_BUTTON_MOUSE_OFF, 2, video::SColor(255, 255, 255, 255), false);
	//player1Forward_btn.btn->setSprite(EGBS_BUTTON_FOCUSED, 3, video::SColor(255, 255, 255, 255), false);
	player1Forward_btn.btn->setSprite(EGBS_BUTTON_DOWN, 3, video::SColor(255, 255, 255, 255), false);

	player1Backward_btn.btn->setSprite(EGBS_BUTTON_UP, 2, video::SColor(255, 255, 255, 255), false);
	player1Backward_btn.btn->setSprite(EGBS_BUTTON_MOUSE_OFF, 2, video::SColor(255, 255, 255, 255), false);
	//player1Backward_btn.btn->setSprite(EGBS_BUTTON_FOCUSED, 3, video::SColor(255, 255, 255, 255), false);
	player1Backward_btn.btn->setSprite(EGBS_BUTTON_DOWN, 3, video::SColor(255, 255, 255, 255), false);

	player1Left_btn.btn->setSprite(EGBS_BUTTON_UP, 2, video::SColor(255, 255, 255, 255), false);
	player1Left_btn.btn->setSprite(EGBS_BUTTON_MOUSE_OFF, 2, video::SColor(255, 255, 255, 255), false);
	//player1Left_btn.btn->setSprite(EGBS_BUTTON_FOCUSED, 3, video::SColor(255, 255, 255, 255), false);
	player1Left_btn.btn->setSprite(EGBS_BUTTON_DOWN, 3, video::SColor(255, 255, 255, 255), false);

	player1Right_btn.btn->setSprite(EGBS_BUTTON_UP, 2, video::SColor(255, 255, 255, 255), false);
	player1Right_btn.btn->setSprite(EGBS_BUTTON_MOUSE_OFF, 2, video::SColor(255, 255, 255, 255), false);
	//player1Right_btn.btn->setSprite(EGBS_BUTTON_FOCUSED, 3, video::SColor(255, 255, 255, 255), false);
	player1Right_btn.btn->setSprite(EGBS_BUTTON_DOWN, 3, video::SColor(255, 255, 255, 255), false);

	player1Bomb_btn.btn->setSprite(EGBS_BUTTON_UP, 2, video::SColor(255, 255, 255, 255), false);
	player1Bomb_btn.btn->setSprite(EGBS_BUTTON_MOUSE_OFF, 2, video::SColor(255, 255, 255, 255), false);
	//player1Bomb_btn.btn->setSprite(EGBS_BUTTON_FOCUSED, 3, video::SColor(255, 255, 255, 255), false);
	player1Bomb_btn.btn->setSprite(EGBS_BUTTON_DOWN, 3, video::SColor(255, 255, 255, 255), false);


	/////////

	player2Forward_btn.btn->setSprite(EGBS_BUTTON_UP, 2, video::SColor(255, 255, 255, 255), false);
	player2Forward_btn.btn->setSprite(EGBS_BUTTON_MOUSE_OFF, 2, video::SColor(255, 255, 255, 255), false);
	//player2Forward_btn.btn->setSprite(EGBS_BUTTON_FOCUSED, 3, video::SColor(255, 255, 255, 255), false);
	player2Forward_btn.btn->setSprite(EGBS_BUTTON_DOWN, 3, video::SColor(255, 255, 255, 255), false);

	player2Backward_btn.btn->setSprite(EGBS_BUTTON_UP, 2, video::SColor(255, 255, 255, 255), false);
	player2Backward_btn.btn->setSprite(EGBS_BUTTON_MOUSE_OFF, 2, video::SColor(255, 255, 255, 255), false);
	//player2Backward_btn.btn->setSprite(EGBS_BUTTON_FOCUSED, 3, video::SColor(255, 255, 255, 255), false);
	player2Backward_btn.btn->setSprite(EGBS_BUTTON_DOWN, 3, video::SColor(255, 255, 255, 255), false);

	player2Left_btn.btn->setSprite(EGBS_BUTTON_UP, 2, video::SColor(255, 255, 255, 255), false);
	player2Left_btn.btn->setSprite(EGBS_BUTTON_MOUSE_OFF, 2, video::SColor(255, 255, 255, 255), false);
	//player2Left_btn.btn->setSprite(EGBS_BUTTON_FOCUSED, 3, video::SColor(255, 255, 255, 255), false);
	player2Left_btn.btn->setSprite(EGBS_BUTTON_DOWN, 3, video::SColor(255, 255, 255, 255), false);

	player2Right_btn.btn->setSprite(EGBS_BUTTON_UP, 2, video::SColor(255, 255, 255, 255), false);
	player2Right_btn.btn->setSprite(EGBS_BUTTON_MOUSE_OFF, 2, video::SColor(255, 255, 255, 255), false);
	//player2Right_btn.btn->setSprite(EGBS_BUTTON_FOCUSED, 3, video::SColor(255, 255, 255, 255), false);
	player2Right_btn.btn->setSprite(EGBS_BUTTON_DOWN, 3, video::SColor(255, 255, 255, 255), false);

	player2Bomb_btn.btn->setSprite(EGBS_BUTTON_UP, 2, video::SColor(255, 255, 255, 255), false);
	player2Bomb_btn.btn->setSprite(EGBS_BUTTON_MOUSE_OFF, 2, video::SColor(255, 255, 255, 255), false);
	//player2Bomb_btn.btn->setSprite(EGBS_BUTTON_FOCUSED, 3, video::SColor(255, 255, 255, 255), false);
	player2Bomb_btn.btn->setSprite(EGBS_BUTTON_DOWN, 3, video::SColor(255, 255, 255, 255), false);

	/////////

	player3Forward_btn.btn->setSprite(EGBS_BUTTON_UP, 2, video::SColor(255, 255, 255, 255), false);
	player3Forward_btn.btn->setSprite(EGBS_BUTTON_MOUSE_OFF, 2, video::SColor(255, 255, 255, 255), false);
	//player3Forward_btn.btn->setSprite(EGBS_BUTTON_FOCUSED, 3, video::SColor(255, 255, 255, 255), false);
	player3Forward_btn.btn->setSprite(EGBS_BUTTON_DOWN, 3, video::SColor(255, 255, 255, 255), false);

	player3Backward_btn.btn->setSprite(EGBS_BUTTON_UP, 2, video::SColor(255, 255, 255, 255), false);
	player3Backward_btn.btn->setSprite(EGBS_BUTTON_MOUSE_OFF, 2, video::SColor(255, 255, 255, 255), false);
	//player3Backward_btn.btn->setSprite(EGBS_BUTTON_FOCUSED, 3, video::SColor(255, 255, 255, 255), false);
	player3Backward_btn.btn->setSprite(EGBS_BUTTON_DOWN, 3, video::SColor(255, 255, 255, 255), false);

	player3Left_btn.btn->setSprite(EGBS_BUTTON_UP, 2, video::SColor(255, 255, 255, 255), false);
	player3Left_btn.btn->setSprite(EGBS_BUTTON_MOUSE_OFF, 2, video::SColor(255, 255, 255, 255), false);
	//player3Left_btn.btn->setSprite(EGBS_BUTTON_FOCUSED, 3, video::SColor(255, 255, 255, 255), false);
	player3Left_btn.btn->setSprite(EGBS_BUTTON_DOWN, 3, video::SColor(255, 255, 255, 255), false);

	player3Right_btn.btn->setSprite(EGBS_BUTTON_UP, 2, video::SColor(255, 255, 255, 255), false);
	player3Right_btn.btn->setSprite(EGBS_BUTTON_MOUSE_OFF, 2, video::SColor(255, 255, 255, 255), false);
	//player3Right_btn.btn->setSprite(EGBS_BUTTON_FOCUSED, 3, video::SColor(255, 255, 255, 255), false);
	player3Right_btn.btn->setSprite(EGBS_BUTTON_DOWN, 3, video::SColor(255, 255, 255, 255), false);

	player3Bomb_btn.btn->setSprite(EGBS_BUTTON_UP, 2, video::SColor(255, 255, 255, 255), false);
	player3Bomb_btn.btn->setSprite(EGBS_BUTTON_MOUSE_OFF, 2, video::SColor(255, 255, 255, 255), false);
	//player3Bomb_btn.btn->setSprite(EGBS_BUTTON_FOCUSED, 3, video::SColor(255, 255, 255, 255), false);
	player3Bomb_btn.btn->setSprite(EGBS_BUTTON_DOWN, 3, video::SColor(255, 255, 255, 255), false);

	/////////

	player4Forward_btn.btn->setSprite(EGBS_BUTTON_UP, 2, video::SColor(255, 255, 255, 255), false);
	player4Forward_btn.btn->setSprite(EGBS_BUTTON_MOUSE_OFF, 2, video::SColor(255, 255, 255, 255), false);
	//player4Forward_btn.btn->setSprite(EGBS_BUTTON_FOCUSED, 3, video::SColor(255, 255, 255, 255), false);
	player4Forward_btn.btn->setSprite(EGBS_BUTTON_DOWN, 3, video::SColor(255, 255, 255, 255), false);

	player4Backward_btn.btn->setSprite(EGBS_BUTTON_UP, 2, video::SColor(255, 255, 255, 255), false);
	player4Backward_btn.btn->setSprite(EGBS_BUTTON_MOUSE_OFF, 2, video::SColor(255, 255, 255, 255), false);
	//player4Backward_btn.btn->setSprite(EGBS_BUTTON_FOCUSED, 3, video::SColor(255, 255, 255, 255), false);
	player4Backward_btn.btn->setSprite(EGBS_BUTTON_DOWN, 3, video::SColor(255, 255, 255, 255), false);

	player4Left_btn.btn->setSprite(EGBS_BUTTON_UP, 2, video::SColor(255, 255, 255, 255), false);
	player4Left_btn.btn->setSprite(EGBS_BUTTON_MOUSE_OFF, 2, video::SColor(255, 255, 255, 255), false);
	//player4Left_btn.btn->setSprite(EGBS_BUTTON_FOCUSED, 3, video::SColor(255, 255, 255, 255), false);
	player4Left_btn.btn->setSprite(EGBS_BUTTON_DOWN, 3, video::SColor(255, 255, 255, 255), false);

	player4Right_btn.btn->setSprite(EGBS_BUTTON_UP, 2, video::SColor(255, 255, 255, 255), false);
	player4Right_btn.btn->setSprite(EGBS_BUTTON_MOUSE_OFF, 2, video::SColor(255, 255, 255, 255), false);
	//player4Right_btn.btn->setSprite(EGBS_BUTTON_FOCUSED, 3, video::SColor(255, 255, 255, 255), false);
	player4Right_btn.btn->setSprite(EGBS_BUTTON_DOWN, 3, video::SColor(255, 255, 255, 255), false);

	player4Bomb_btn.btn->setSprite(EGBS_BUTTON_UP, 2, video::SColor(255, 255, 255, 255), false);
	player4Bomb_btn.btn->setSprite(EGBS_BUTTON_MOUSE_OFF, 2, video::SColor(255, 255, 255, 255), false);
	//player4Bomb_btn.btn->setSprite(EGBS_BUTTON_FOCUSED, 3, video::SColor(255, 255, 255, 255), false);
	player4Bomb_btn.btn->setSprite(EGBS_BUTTON_DOWN, 3, video::SColor(255, 255, 255, 255), false);



	GM.addComponent(IrrD, &dvc);
	GM.addComponent(backBtn, &Back);
	GM.addComponent(Player1, &player1_keybinding);
	GM.addComponent(Player2, &player2_keybinding);
	GM.addComponent(Player3, &player3_keybinding);
	GM.addComponent(Player4, &player4_keybinding);
	GM.addComponent(player1Forward, &player1Forward_btn);
	GM.addComponent(player1Backward, &player1Backward_btn);
	GM.addComponent(player1Left, &player1Left_btn);
	GM.addComponent(player1Right, &player1Right_btn);
	GM.addComponent(player1Bomb, &player1Bomb_btn);
	GM.addComponent(player2Forward, &player2Forward_btn);
	GM.addComponent(player2Backward, &player2Backward_btn);
	GM.addComponent(player2Left, &player2Left_btn);
	GM.addComponent(player2Right, &player2Right_btn);
	GM.addComponent(player2Bomb, &player2Bomb_btn);
	GM.addComponent(player3Forward, &player3Forward_btn);
	GM.addComponent(player3Backward, &player3Backward_btn);
	GM.addComponent(player3Left, &player3Left_btn);
	GM.addComponent(player3Right, &player3Right_btn);
	GM.addComponent(player3Bomb, &player3Bomb_btn);
	GM.addComponent(player4Forward, &player4Forward_btn);
	GM.addComponent(player4Backward, &player4Backward_btn);
	GM.addComponent(player4Left, &player4Left_btn);
	GM.addComponent(player4Right, &player4Right_btn);
	GM.addComponent(player4Bomb, &player4Bomb_btn);
	video::ITexture* img_background_two = driver->getTexture("Image/background.jpg");
	bool keyIsPressed = false;
	while (dvc.device->run() && receiver.options == false && receiver.sound_menu == false) {
		driver->beginScene(true, true, false);
		driver->draw2DImage(img_background_two, core::position2d<s32>(0, 0), core::rect<s32>(0, 0, 1280, 720), 0, video::SColor(255, 255, 255, 255), true);
		driver->setTextureCreationFlag(ETCF_ALWAYS_32_BIT, true);

		for (int i = 0; i < buttonArrayPlayer1.size(); i++) {
			for (int j = KEY_LBUTTON; j != KEY_OEM_CLEAR; j++) {
				if (buttonArrayPlayer1[i].btn->isPressed()) {
						buttonArrayPlayer1[i].btn->setPressed(true);
						if ((keyIsPressed = receiver.IsKeyDown((EKEY_CODE)j))) {
							defaultKeyPlayer1[i] = (EKEY_CODE)j;
							player1_keybinding.keybindVector[i] = (EKEY_CODE)j;
							buttonArrayPlayer1[i].btn->setText(stringw(KeyNames[defaultKeyPlayer1[i]]).c_str());
							buttonArrayPlayer1[i].btn->setPressed(false);
						}
				}
			}
		}
		for (int i = 0; i < buttonArrayPlayer2.size(); i++) {
			for (int j = KEY_LBUTTON; j != KEY_OEM_CLEAR; j++) {
				if (buttonArrayPlayer2[i].btn->isPressed()) {
					buttonArrayPlayer2[i].btn->setPressed(true);
					if ((keyIsPressed = receiver.IsKeyDown((EKEY_CODE)j))) {
						defaultKeyPlayer2[i] = (EKEY_CODE)j;
						player2_keybinding.keybindVector[i] = (EKEY_CODE)j;
						buttonArrayPlayer2[i].btn->setText(stringw(KeyNames[defaultKeyPlayer2[i]]).c_str());
						buttonArrayPlayer2[i].btn->setPressed(false);
					}
				}
			}
		}
		for (int i = 0; i < buttonArrayPlayer3.size(); i++) {
			for (int j = KEY_LBUTTON; j != KEY_OEM_CLEAR; j++) {
				if (buttonArrayPlayer3[i].btn->isPressed()) {
					buttonArrayPlayer3[i].btn->setPressed(true);
					if ((keyIsPressed = receiver.IsKeyDown((EKEY_CODE)j))) {
						defaultKeyPlayer3[i] = (EKEY_CODE)j;
						player3_keybinding.keybindVector[i] = (EKEY_CODE)j;
						buttonArrayPlayer3[i].btn->setText(stringw(KeyNames[defaultKeyPlayer3[i]]).c_str());
						buttonArrayPlayer3[i].btn->setPressed(false);
					}
				}
			}
		}
		for (int i = 0; i < buttonArrayPlayer4.size(); i++) {
			for (int j = KEY_LBUTTON; j != KEY_OEM_CLEAR; j++) {
				if (buttonArrayPlayer4[i].btn->isPressed()) {
					buttonArrayPlayer4[i].btn->setPressed(true);
					if ((keyIsPressed = receiver.IsKeyDown((EKEY_CODE)j))) {
						defaultKeyPlayer4[i] = (EKEY_CODE)j;
						player4_keybinding.keybindVector[i] = (EKEY_CODE)j;
						buttonArrayPlayer4[i].btn->setText(stringw(KeyNames[defaultKeyPlayer4[i]]).c_str());
						buttonArrayPlayer4[i].btn->setPressed(false);
					}
				}
			}
		}

		GM.update();
	}
	if (receiver.options == true) {
		optionMenu();
	}

}

void soundMenu()
{
	static int name = 0;
	Device& dvc = dynamic_cast<Device&>(GM.getComponent(0, 3));
	GM.Reset();
	Entity IrrD = GM.addEntity();
	Entity back_btn = GM.addEntity();
	Entity scroll = GM.addEntity();
	IVideoDriver* driver = dvc.device->getVideoDriver();
	IGUIEnvironment* env = dvc.device->getGUIEnvironment();
	env->clear();
	ISceneManager* smgr = dvc.device->getSceneManager();
	IGUISkin* skin = env->getSkin();
	IGUIFont* font = env->getFont("Fonts/bomberman36px.xml");
	std::stringstream ss;
	std::string s;
	IGUIStaticText* sound_text = env->addStaticText(L"V o l u m e", rect<s32>(1280 / 2 - 150, 720 / 2 + 55, 1280 / 2 + 200, 720 / 2 + 105), false);
	sound_text->setOverrideFont(font);
	skin->setFont(env->getBuiltInFont(), EGDF_TOOLTIP);
	sf::Sound btn_hover;
	sf::Sound btn_click;
	sf::SoundBuffer enterBuffer;
	sf::SoundBuffer enterBuffer2;
	if (!enterBuffer.loadFromFile("8bit/blip-1.wav") || \
		!enterBuffer2.loadFromFile("8bit/ok-2.wav")) {
		std::cout << "Error with Loading of the btn_play_music" << std::endl;
		exit(84);
	}
	btn_hover.setBuffer(enterBuffer);
	btn_click.setBuffer(enterBuffer2);
	Button Back(env, GUI_BACK_SETTINGS_SOUNDMENU, rect<s32>(0, 720 - 88, 0 + 86, 720), "", \
		& btn_hover, &btn_click, driver, font);
	Back.btn->setDrawBorder(false);
	IGUISpriteBank* sb = env->addEmptySpriteBank(("backSound" + std::to_string(name)).c_str());
	name += 1;
	ITexture* back_normal = driver->getTexture("Image/back_normal.png");
	ITexture* back_hovered = driver->getTexture("Image/back_hovered.png");
	sb->addTexture(back_normal);
	sb->addTexture(back_hovered);
	sb->getPositions().push_back(core::rect<s32>(core::position2di(0, 0), back_normal->getOriginalSize()));
	sb->getPositions().push_back(core::rect<s32>(core::position2di(0, 0), back_hovered->getOriginalSize()));
	gui::SGUISprite sprite;
	gui::SGUISpriteFrame sframe;
	sprite.Frames.push_back(sframe);
	sprite.Frames[0].rectNumber = 0;
	sprite.Frames[0].textureNumber = 0;
	sb->getSprites().push_back(sprite);
	sprite.Frames[0].rectNumber = 1;
	sprite.Frames[0].textureNumber = 1;
	sb->getSprites().push_back(sprite);
	Back.btn->setSpriteBank(sb);
	Back.btn->setSprite(EGBS_BUTTON_UP, 0, video::SColor(255, 255, 255, 255), false);
	Back.btn->setSprite(EGBS_BUTTON_MOUSE_OFF, 0, video::SColor(255, 255, 255, 255), false);
	Back.btn->setSprite(EGBS_BUTTON_FOCUSED, 1, video::SColor(255, 255, 255, 255), false);
	Back.btn->setSprite(EGBS_BUTTON_MOUSE_OVER, 1, video::SColor(255, 255, 255, 255), false);
	GM.addComponent(IrrD, &dvc);
	GM.addComponent(back_btn, &Back);
	SAppContext context;
	context.device = dvc.device;
	MyEventReceiver receiver(context);
	dvc.device->setEventReceiver(&receiver);
	if (receiver.check_song == true);
	else
		receiver.curr_pos = 100;
	ScrollBar scrollbar(env, rect<s32>(1280 / 2 - 500, 720 / 2 - 50, 1280 / 2 + 500, 720 / 2 + 50), GUI_ID_TRANSPARENCY_SCROLL_BAR, 100, receiver.curr_pos);
	GM.addComponent(scroll, &scrollbar);
	receiver.value = scrollbar.scroll->getPos();
	receiver.sound_value = env->addStaticText(stringw(receiver.value).c_str(), rect<s32>(1200 / 2, 640 / 2 + 10, 1280 / 2 + 50, 640 / 2 + 60), false);
	receiver.sound_value->setOverrideFont(font);
	video::ITexture* img_background_three = driver->getTexture("Image/background.jpg");
	while (dvc.device->run() && receiver.options == false && receiver.keybinding_menu == false) {
		driver->beginScene(true, true, false);
		driver->draw2DImage(img_background_three, core::position2d<s32>(0, 0), core::rect<s32>(0, 0, 1280, 720), 0, video::SColor(255, 255, 255, 255), true);
		driver->setTextureCreationFlag(ETCF_ALWAYS_32_BIT, true);
		receiver.value = scrollbar.scroll->getPos();
		receiver.curr_pos = receiver.value;
		receiver.sound_value->setText(stringw(receiver.curr_pos).c_str());
		dvc.background_sound.setVolume(receiver.curr_pos);
		dvc.background_sound2.setVolume(receiver.curr_pos);
		if (scrollbar.env->getHovered() == scrollbar.scroll)
			dvc.device->getCursorControl()->setActiveIcon(ECI_HAND);
		GM.update();
	}
	if (receiver.options == true) {
		optionMenu();
	}
}

void selectionMenu()
{
	static int name = 0;
	Device& dvc = dynamic_cast<Device&>(GM.getComponent(0, 3));
	std::cout << "PASSED" << std::endl;
	GM.Reset();
	Entity IrrD = GM.addEntity();
	Entity Play_btn = GM.addEntity();
	Entity Left_button_1_btn = GM.addEntity();
	Entity Right_button_1_btn = GM.addEntity();
	Entity Left_button_2_btn = GM.addEntity();
	Entity Right_button_2_btn = GM.addEntity();
	Entity Left_button_3_btn = GM.addEntity();
	Entity Right_button_3_btn = GM.addEntity();
	Entity Left_button_4_btn = GM.addEntity();
	Entity Right_button_4_btn = GM.addEntity();
	Entity Back_select_btn = GM.addEntity();
	IVideoDriver* driver = dvc.device->getVideoDriver();
	IGUIEnvironment* env = dvc.device->getGUIEnvironment();
	env->clear();
	ISceneManager* smgr = dvc.device->getSceneManager();
	IGUISkin* skin = env->getSkin();
	IGUIFont* font = env->getFont("Fonts/bomberman20px.xml");
	sf::Sound btn_hover;
	sf::Sound btn_click;
	sf::SoundBuffer enterBuffer;
	sf::SoundBuffer enterBuffer2;
	if (!enterBuffer.loadFromFile("8bit/blip-1.wav") || \
		!enterBuffer2.loadFromFile("8bit/ok-2.wav")) {
		std::cout << "Error with Loading of the btn_play_music" << std::endl;
		exit(84);
	}
	btn_hover.setBuffer(enterBuffer);
	btn_click.setBuffer(enterBuffer2);
	skin->setFont(env->getBuiltInFont(), EGDF_TOOLTIP);
	Button Play_button(env, GUI_ID_PLAY_BUTTON, rect<s32>(1280 / 2 - 158, 720 - 160, 1280 / 2 - 158 + 316, 720 - 20), "P l a y", &btn_hover, &btn_click, driver, font);
	Button Left_button_1(env, GUI_ID_LEFT_1, rect<s32>(0, 720 / 2 + 20, 0 + 45, 720 / 2 + 20 + 45), "", &btn_hover, &btn_click, driver, font);
	Button Right_button_1(env, GUI_ID_RIGHT_1, rect<s32>(250, 720 / 2 + 20, 250 + 45, 720 / 2 + 20 + 45), "", &btn_hover, &btn_click, driver, font);
	Button Left_button_2(env, GUI_ID_LEFT_2, rect<s32>(0 + 325, 720 / 2 + 20, 0 + 325 + 45, 720 / 2 + 20 + 45), "", &btn_hover, &btn_click, driver, font);
	Button Right_button_2(env, GUI_ID_RIGHT_2, rect<s32>(250 + 325, 720 / 2 + 20, 250 + 325 + 45, 720 / 2 + 20 + 45), "", &btn_hover, &btn_click, driver, font);
	Button Left_button_3(env, GUI_ID_LEFT_3, rect<s32>(0 + 325 * 2, 720 / 2 + 20, 0 + 325 * 2 + 45, 720 / 2 + 20 + 45), "", &btn_hover, &btn_click, driver, font);
	Button Right_button_3(env, GUI_ID_RIGHT_3, rect<s32>(250 + 325 * 2, 720 / 2 + 20, 250 + 325 * 2 + 45, 720 / 2 + 20 + 45), "", &btn_hover, &btn_click, driver, font);
	Button Left_button_4(env, GUI_ID_LEFT_4, rect<s32>(0 + 325 * 3, 720 / 2 + 20, 0 + 325 * 3 + 45, 720 / 2 + 20 + 45), "", &btn_hover, &btn_click, driver, font);
	Button Right_button_4(env, GUI_ID_RIGHT_4, rect<s32>(250 + 325 * 3, 720 / 2 + 20, 250 + 325 * 3 + 45, 720 / 2 + 20 + 45), "", &btn_hover, &btn_click, driver, font);
	Button Back(env, GUI_ID_MENU_CHARACTER_RETURN, rect<s32>(0, 720 - 88, 0 + 86, 720), "", &btn_hover, &btn_click, driver, font);
	IGUIStaticText *player1State = env->addStaticText(L"H U M A N", rect<s32>(55, 720 / 2 + 20 + 10, 55 + 220, 720 / 2 + 20 + 10 + 25), false);
	IGUIStaticText *player2State = env->addStaticText(L"H U M A N", rect<s32>(55 + 325, 720 / 2 + 20 + 10, 55 + 325 + 220, 720 / 2 + 20 + 10 + 25), false);
	IGUIStaticText *player3State = env->addStaticText(L"H U M A N", rect<s32>(55 + 325 * 2, 720 / 2 + 20 + 10, 55 + 325 * 2 + 220, 720 / 2 + 20 + 10 + 25), false);
	IGUIStaticText *player4State = env->addStaticText(L"H U M A N", rect<s32>(55 + 325 * 3, 720 / 2 + 20 + 10, 55 + 325 * 3 + 220, 720 / 2 + 20 + 10 + 25), false);
	IGUIStaticText *player1Text = env->addStaticText(L"P L A Y E R  1", rect<s32>(40, 720 / 2 - 60, 40 + 220, 720 / 2 - 60 + 25), true, false, 0, -1, true);
	IGUIStaticText *player2Text = env->addStaticText(L"P L A Y E R  2", rect<s32>(40 + 325, 720 / 2 - 60, 40 + 325 + 220, 720 / 2 - 60 + 25), true, false, 0, -1, true);
	IGUIStaticText *player3Text = env->addStaticText(L"P L A Y E R  3", rect<s32>(40 + 325 * 2, 720 / 2 - 60, 40 + 325 * 2+ 220, 720 / 2 - 60 + 25), true, false, 0, -1, true);
	IGUIStaticText *player4Text = env->addStaticText(L"P L A Y E R  4", rect<s32>(40 + 325 * 3, 720 / 2 - 60, 40 + 325 * 3+ 220, 720 / 2 - 60 + 25), true, false, 0, -1, true);
	player1State->setOverrideFont(font);
	player1State->setOverrideColor(video::SColor(255, 0, 255, 0));
	player1Text->setOverrideFont(font);
	player1Text->setOverrideColor(video::SColor(255, 0, 255, 0));
	player1Text->setBackgroundColor(video::SColor(255, 96, 96, 96));
	player2State->setOverrideFont(font);
	player2State->setOverrideColor(video::SColor(255, 0, 255, 0));
	player2Text->setOverrideFont(font);
	player2Text->setOverrideColor(video::SColor(255, 0, 255, 0));
	player2Text->setBackgroundColor(video::SColor(255, 96, 96, 96));
	player3State->setOverrideFont(font);
	player3State->setOverrideColor(video::SColor(255, 0, 255, 0));
	player3Text->setOverrideFont(font);
	player3Text->setOverrideColor(video::SColor(255, 0, 255, 0));
	player3Text->setBackgroundColor(video::SColor(255, 96, 96, 96));
	player4State->setOverrideFont(font);
	player4State->setOverrideColor(video::SColor(255, 0, 255, 0));
	player4Text->setOverrideFont(font);
	player4Text->setOverrideColor(video::SColor(255, 0, 255, 0));
	player4Text->setBackgroundColor(video::SColor(255, 96, 96, 96));
	Play_button.btn->setDrawBorder(false);
	Left_button_1.btn->setDrawBorder(false);
	Right_button_1.btn->setDrawBorder(false);
	Left_button_2.btn->setDrawBorder(false);
	Right_button_2.btn->setDrawBorder(false);
	Left_button_3.btn->setDrawBorder(false);
	Right_button_3.btn->setDrawBorder(false);
	Left_button_4.btn->setDrawBorder(false);
	Right_button_4.btn->setDrawBorder(false);
	Back.btn->setDrawBorder(false);
	IGUISpriteBank* sb = env->addEmptySpriteBank(("SelectButtons_" + std::to_string(name)).c_str());
	name += 1;
	ITexture* texture_normal_left = driver->getTexture("Image/left_normal.png");
	ITexture* texture_normal_right = driver->getTexture("Image/right_normal.png");
	ITexture* texture_hovered_left = driver->getTexture("Image/left_hovered.png");
	ITexture* texture_hovered_right = driver->getTexture("Image/right_hovered.png");
	ITexture* back_normal = driver->getTexture("Image/back_normal.png");
	ITexture* back_hovered = driver->getTexture("Image/back_hovered.png");
	ITexture* texture_normal = driver->getTexture("Image/button_normal2.png");
    ITexture* texture_hovered = driver->getTexture("Image/button_hovered2.png");
	sb->addTexture(texture_normal_left);
	sb->addTexture(texture_normal_right);
	sb->addTexture(texture_hovered_left);
	sb->addTexture(texture_hovered_right);
	sb->addTexture(back_normal);
	sb->addTexture(back_hovered);
	sb->addTexture(texture_normal);
	sb->addTexture(texture_hovered);
	sb->getPositions().push_back(core::rect<s32>(core::position2di(0, 0), texture_normal_left->getOriginalSize()));
	sb->getPositions().push_back(core::rect<s32>(core::position2di(0, 0), texture_normal_right->getOriginalSize()));
	sb->getPositions().push_back(core::rect<s32>(core::position2di(0, 0), texture_hovered_left->getOriginalSize()));
	sb->getPositions().push_back(core::rect<s32>(core::position2di(0, 0), texture_hovered_right->getOriginalSize()));
	sb->getPositions().push_back(core::rect<s32>(core::position2di(0, 0), back_normal->getOriginalSize()));
	sb->getPositions().push_back(core::rect<s32>(core::position2di(0, 0), back_hovered->getOriginalSize()));
	sb->getPositions().push_back(core::rect<s32>(core::position2di(0, 0), texture_normal->getOriginalSize()));
    sb->getPositions().push_back(core::rect<s32>(core::position2di(0, 0), texture_hovered->getOriginalSize()));
	gui::SGUISprite sprite;
	gui::SGUISpriteFrame sframe;
	sprite.Frames.push_back(sframe);
	sprite.Frames[0].rectNumber = 0;
	sprite.Frames[0].textureNumber = 0;
	sb->getSprites().push_back(sprite);
	sprite.Frames[0].rectNumber = 1;
	sprite.Frames[0].textureNumber = 1;
	sb->getSprites().push_back(sprite);
	sprite.Frames[0].rectNumber = 2;
	sprite.Frames[0].textureNumber = 2;
	sb->getSprites().push_back(sprite);
	sprite.Frames[0].rectNumber = 3;
	sprite.Frames[0].textureNumber = 3;
	sb->getSprites().push_back(sprite);
	sprite.Frames[0].rectNumber = 4;
	sprite.Frames[0].textureNumber = 4;
	sb->getSprites().push_back(sprite);
	sprite.Frames[0].rectNumber = 5;
	sprite.Frames[0].textureNumber = 5;
	sb->getSprites().push_back(sprite);
	sprite.Frames[0].rectNumber = 6;
	sprite.Frames[0].textureNumber = 6;
	sb->getSprites().push_back(sprite);
	sprite.Frames[0].rectNumber = 7;
	sprite.Frames[0].textureNumber = 7;
	sb->getSprites().push_back(sprite);
	Left_button_1.btn->setSpriteBank(sb);
	Left_button_1.btn->setSprite(EGBS_BUTTON_UP, 0, video::SColor(255, 255, 255, 255), false);
	Left_button_1.btn->setSprite(EGBS_BUTTON_MOUSE_OFF, 0, video::SColor(255, 255, 255, 255), false);
	Left_button_1.btn->setSprite(EGBS_BUTTON_MOUSE_OVER, 2, video::SColor(255, 255, 255, 255), false);
	Right_button_1.btn->setSpriteBank(sb);
	Right_button_1.btn->setSprite(EGBS_BUTTON_UP, 1, video::SColor(255, 255, 255, 255), false);
	Right_button_1.btn->setSprite(EGBS_BUTTON_MOUSE_OFF, 1, video::SColor(255, 255, 255, 255), false);
	Right_button_1.btn->setSprite(EGBS_BUTTON_MOUSE_OVER, 3, video::SColor(255, 255, 255, 255), false);
	Left_button_2.btn->setSpriteBank(sb);
	Left_button_2.btn->setSprite(EGBS_BUTTON_UP, 0, video::SColor(255, 255, 255, 255), false);
	Left_button_2.btn->setSprite(EGBS_BUTTON_MOUSE_OFF, 0, video::SColor(255, 255, 255, 255), false);
	Left_button_2.btn->setSprite(EGBS_BUTTON_MOUSE_OVER, 2, video::SColor(255, 255, 255, 255), false);
	Right_button_2.btn->setSpriteBank(sb);
	Right_button_2.btn->setSprite(EGBS_BUTTON_UP, 1, video::SColor(255, 255, 255, 255), false);
	Right_button_2.btn->setSprite(EGBS_BUTTON_MOUSE_OFF, 1, video::SColor(255, 255, 255, 255), false);
	Right_button_2.btn->setSprite(EGBS_BUTTON_MOUSE_OVER, 3, video::SColor(255, 255, 255, 255), false);
	Right_button_3.btn->setSpriteBank(sb);
	Left_button_3.btn->setSpriteBank(sb);
	Left_button_3.btn->setSprite(EGBS_BUTTON_UP, 0, video::SColor(255, 255, 255, 255), false);
	Left_button_3.btn->setSprite(EGBS_BUTTON_MOUSE_OFF, 0, video::SColor(255, 255, 255, 255), false);
	Left_button_3.btn->setSprite(EGBS_BUTTON_MOUSE_OVER, 2, video::SColor(255, 255, 255, 255), false);
	Right_button_3.btn->setSprite(EGBS_BUTTON_UP, 1, video::SColor(255, 255, 255, 255), false);
	Right_button_3.btn->setSprite(EGBS_BUTTON_MOUSE_OFF, 1, video::SColor(255, 255, 255, 255), false);
	Right_button_3.btn->setSprite(EGBS_BUTTON_MOUSE_OVER, 3, video::SColor(255, 255, 255, 255), false);
	Right_button_4.btn->setSpriteBank(sb);
	Left_button_4.btn->setSpriteBank(sb);
	Left_button_4.btn->setSprite(EGBS_BUTTON_UP, 0, video::SColor(255, 255, 255, 255), false);
	Left_button_4.btn->setSprite(EGBS_BUTTON_MOUSE_OFF, 0, video::SColor(255, 255, 255, 255), false);
	Left_button_4.btn->setSprite(EGBS_BUTTON_MOUSE_OVER, 2, video::SColor(255, 255, 255, 255), false);
	Right_button_4.btn->setSprite(EGBS_BUTTON_UP, 1, video::SColor(255, 255, 255, 255), false);
	Right_button_4.btn->setSprite(EGBS_BUTTON_MOUSE_OFF, 1, video::SColor(255, 255, 255, 255), false);
	Right_button_4.btn->setSprite(EGBS_BUTTON_MOUSE_OVER, 3, video::SColor(255, 255, 255, 255), false);
	Back.btn->setSpriteBank(sb);
	Back.btn->setSprite(EGBS_BUTTON_UP, 4, video::SColor(255, 255, 255, 255), false);
	Back.btn->setSprite(EGBS_BUTTON_MOUSE_OFF, 4, video::SColor(255, 255, 255, 255), false);
	Back.btn->setSprite(EGBS_BUTTON_MOUSE_OVER, 5, video::SColor(255, 255, 255, 255), false);
	Play_button.btn->setSpriteBank(sb);
	Play_button.btn->setSprite(EGBS_BUTTON_UP, 6, video::SColor(255, 255, 255, 255), false);
	Play_button.btn->setSprite(EGBS_BUTTON_MOUSE_OFF, 6, video::SColor(255, 255, 255, 255), false);
	Play_button.btn->setSprite(EGBS_BUTTON_MOUSE_OVER, 7, video::SColor(255, 255, 255, 255), false);
	GM.addComponent(IrrD, &dvc);
	GM.addComponent(Left_button_1_btn, &Left_button_1);
	GM.addComponent(Right_button_1_btn, &Right_button_1);
	GM.addComponent(Left_button_2_btn, &Left_button_2);
	GM.addComponent(Right_button_2_btn, &Right_button_2);
	GM.addComponent(Left_button_3_btn, &Left_button_3);
	GM.addComponent(Right_button_3_btn, &Right_button_3);
	GM.addComponent(Left_button_4_btn, &Left_button_4);
	GM.addComponent(Right_button_4_btn, &Right_button_4);
	GM.addComponent(Back_select_btn, &Back);
	GM.addComponent(Play_btn, &Play_button);
	SAppContext context;
	context.device = dvc.device;
	MyEventReceiver receiver(context);
	dvc.device->setEventReceiver(&receiver);
	video::ITexture* img_background_four = driver->getTexture("Image/background2.jpg");
	while (dvc.device->run() && receiver.back == false && receiver.game == false) {
		driver->beginScene(true, true, false);
		driver->draw2DImage(img_background_four, core::position2d<s32>(0, 0), core::rect<s32>(0, 0, 1280, 720), 0, video::SColor(255, 255, 255, 255), true);
		driver->setTextureCreationFlag(ETCF_ALWAYS_32_BIT, true);
		if (receiver.AI_state_1 == true) {
			AISTATE1 = true;
			player1State->setText(L"M A C H I N E");
		}
		else {
			AISTATE1 = false;
			player1State->setText(L"H U M A N");
		}
		if (receiver.AI_state_2 == true) {
			AISTATE2 = true;
			player2State->setText(L"M A C H I N E");
		}
		else {
			AISTATE2 = false;
			player2State->setText(L"H U M A N");
		}
		if (receiver.AI_state_3 == true) {
			AISTATE3 = true;
			player3State->setText(L"M A C H I N E");
		}
		else {
			AISTATE3 = false;
			player3State->setText(L"H U M A N");
		}
		if (receiver.AI_state_4 == true) {
			AISTATE4 = true;
			player4State->setText(L"M A C H I N E");
		}
		else {
			AISTATE4 = false;
			player4State->setText(L"H U M A N");
		}
		GM.update();
	}
	if (receiver.back == true) {
		menu_back(false);
	} else if (receiver.game == true) {
		dvc.background_sound.stop();
		dvc.background_sound2.stop();
		gameLoop();
	}
}

void winMenu(int player)
{
	static int name = 0;
	Device& dvc = dynamic_cast<Device&>(GM.getComponent(0, 3));
	std::cout << "PASSED" << std::endl;
	GM.Reset();
	Entity IrrD = GM.addEntity();
	Entity Retry_button_btn = GM.addEntity();
	Entity Menu_button_btn = GM.addEntity();
	IVideoDriver* driver = dvc.device->getVideoDriver();
	IGUIEnvironment* env = dvc.device->getGUIEnvironment();
	env->clear();
	ISceneManager* smgr = dvc.device->getSceneManager();
	smgr->clear();
	dvc.device->getCursorControl()->setVisible(true);
	IGUISkin* skin = env->getSkin();
	IGUIFont* fontButton = env->getFont("Fonts/bomberman20px.xml");
	IGUIFont* font = env->getFont("Fonts/bomberman68px.xml");
	sf::Sound btn_hover;
	sf::Sound btn_click;
	sf::SoundBuffer enterBuffer;
	sf::SoundBuffer enterBuffer2;
	if (!enterBuffer.loadFromFile("8bit/blip-1.wav") || \
		!enterBuffer2.loadFromFile("8bit/ok-2.wav")) {
		std::cout << "Error with Loading of the btn_play_music" << std::endl;
		exit(84);
	}
	btn_hover.setBuffer(enterBuffer);
	btn_click.setBuffer(enterBuffer2);
	skin->setFont(env->getBuiltInFont(), EGDF_TOOLTIP);
	Button Retry_button(env, GUI_ID_PLAY_BUTTON, rect<s32>(1280 / 2 - 358, 720 - 160, 1280 / 2 - 358 + 316, 720 - 20), "Retry", &btn_hover, &btn_click, driver, fontButton);
	Button Menu_button(env, GUI_ID_MENU, rect<s32>(1280 / 2, 720 - 160, 1280 / 2 + 316, 720 - 20), "M e n u", &btn_hover, &btn_click, driver, fontButton);
	IGUIStaticText* PlayerText = env->addStaticText(L"Player ", rect<s32>(180, 720 / 3, 180 + 570, 720 / 3 + 100), false);
	IGUIStaticText* numPlayer = env->addStaticText(stringw(player).c_str(), rect<s32>(620, 720 / 3, 100 + 620, 720 / 2 + 100), false);
	IGUIStaticText* WinText = env->addStaticText(L" Win !", rect<s32>(750, 720 / 3, 750 + 400, 720 / 3 + 100), false);
	PlayerText->setOverrideFont(font);
	PlayerText->setOverrideColor(video::SColor(255, 0, 255, 0));
	numPlayer->setOverrideFont(font);
	numPlayer->setOverrideColor(video::SColor(255, 0, 255, 0));
	WinText->setOverrideFont(font);
	WinText->setOverrideColor(video::SColor(255, 0, 255, 0));
	Retry_button.btn->setDrawBorder(false);
	Menu_button.btn->setDrawBorder(false);
	IGUISpriteBank* sb = env->addEmptySpriteBank(("WinButtons_" + std::to_string(name)).c_str());
	name += 1;
	ITexture* texture_normal = driver->getTexture("Image/button_normal2.png");
	ITexture* texture_hovered = driver->getTexture("Image/button_hovered2.png");
	sb->addTexture(texture_normal);
	sb->addTexture(texture_hovered);
	sb->getPositions().push_back(core::rect<s32>(core::position2di(0, 0), texture_normal->getOriginalSize()));
	sb->getPositions().push_back(core::rect<s32>(core::position2di(0, 0), texture_hovered->getOriginalSize()));
	gui::SGUISprite sprite;
	gui::SGUISpriteFrame sframe;
	sprite.Frames.push_back(sframe);
	sprite.Frames[0].rectNumber = 0;
	sprite.Frames[0].textureNumber = 0;
	sb->getSprites().push_back(sprite);
	sprite.Frames[0].rectNumber = 1;
	sprite.Frames[0].textureNumber = 1;
	sb->getSprites().push_back(sprite);
	Retry_button.btn->setSpriteBank(sb);
	Retry_button.btn->setSprite(EGBS_BUTTON_UP, 0, video::SColor(255, 255, 255, 255), false);
	Retry_button.btn->setSprite(EGBS_BUTTON_MOUSE_OFF, 0, video::SColor(255, 255, 255, 255), false);
	Retry_button.btn->setSprite(EGBS_BUTTON_MOUSE_OVER, 1, video::SColor(255, 255, 255, 255), false);
	Menu_button.btn->setSpriteBank(sb);
	Menu_button.btn->setSprite(EGBS_BUTTON_UP, 0, video::SColor(255, 255, 255, 255), false);
	Menu_button.btn->setSprite(EGBS_BUTTON_MOUSE_OFF, 0, video::SColor(255, 255, 255, 255), false);
	Menu_button.btn->setSprite(EGBS_BUTTON_MOUSE_OVER, 1, video::SColor(255, 255, 255, 255), false);
	GM.addComponent(IrrD, &dvc);
	GM.addComponent(Retry_button_btn, &Retry_button);
	GM.addComponent(Menu_button_btn, &Menu_button);
	SAppContext context;
	context.device = dvc.device;
	MyEventReceiver receiver(context);
	dvc.device->setEventReceiver(&receiver);
	video::ITexture* img_background_four = driver->getTexture("Image/background2.jpg");
	sf::Music win_music;
	if (!win_music.openFromFile("Music/win.wav")) {
		std::cout << "Error with Loading of the btn_play_music (win)" << std::endl;
		exit(84);
	}
	win_music.setVolume(50);
	win_music.play();
	while (dvc.device->run() && receiver.back == false && receiver.retry == false) {
		driver->beginScene(true, true, false);
		driver->draw2DImage(img_background_four, core::position2d<s32>(0, 0), core::rect<s32>(0, 0, 1280, 720), 0, video::SColor(255, 255, 255, 255), true);
		driver->setTextureCreationFlag(ETCF_ALWAYS_32_BIT, true);
		GM.update();
	}
	if (receiver.back == true) {
		menu_back(true);
	}
	else if (receiver.retry == true) {
		gameLoop();
	}
	dvc.device->closeDevice();
}

void choiceMenuSolo()
{
	static int name = 0;
	Device& dvc = dynamic_cast<Device&>(GM.getComponent(0, 3));
	GM.Reset();
	Entity IrrD = GM.addEntity();
	Entity Resume_btn = GM.addEntity();
	Entity NewGame_btn = GM.addEntity();
	Entity Back_btn = GM.addEntity();
	IVideoDriver* driver = dvc.device->getVideoDriver();
	IGUIEnvironment* env = dvc.device->getGUIEnvironment();
	env->clear();
	ISceneManager* smgr = dvc.device->getSceneManager();
	IGUISkin* skin = env->getSkin();
	IGUIFont* font = env->getFont("Fonts/bomberman20px.xml");
	sf::Sound btn_hover;
	sf::Sound btn_click;
	sf::SoundBuffer enterBuffer;
	sf::SoundBuffer enterBuffer2;
	if (!enterBuffer.loadFromFile("8bit/blip-1.wav") || \
		!enterBuffer2.loadFromFile("8bit/ok-2.wav")) {
		std::cout << "Error with Loading of the btn_play_music" << std::endl;
		exit(84);
	}
	btn_hover.setBuffer(enterBuffer);
	btn_click.setBuffer(enterBuffer2);
	skin->setFont(env->getBuiltInFont(), EGDF_TOOLTIP);
	Button Resume(env, GUI_ID_SOLO_MODE_SAVE, rect<s32>(1280 / 2 - 316 - 20, 720 / 2 - 140, 1280 / 2 - 20, 720 / 2), "R e s u m e", &btn_hover, &btn_click, driver, font);
	Button NewGame(env, GUI_ID_SOLO_MODE, rect<s32>(1280 / 2 + 20, 720 / 2 - 140, 1280 / 2 + 316 + 20, 720 / 2), "N e w  g a m e", &btn_hover, &btn_click, driver, font);
	Button Back(env, GUI_ID_MENU, rect<s32>(0, 720 - 88, 0 + 86, 720), "", &btn_hover, &btn_click, driver, font);
	Resume.btn->setDrawBorder(false);
	NewGame.btn->setDrawBorder(false);
	Back.btn->setDrawBorder(false);
	IGUISpriteBank* sb = env->addEmptySpriteBank(("Buttons_Choice_Solo_" + std::to_string(name)).c_str());
	name += 1;
	ITexture* texture_normal = driver->getTexture("Image/button_normal2.png");
	ITexture* texture_clicked = driver->getTexture("Image/button_clicked2.png");
	ITexture* texture_hovered = driver->getTexture("Image/button_hovered2.png");
	ITexture* back_normal = driver->getTexture("Image/back_normal.png");
	ITexture* back_hovered = driver->getTexture("Image/back_hovered.png");
	sb->addTexture(texture_normal);
	sb->addTexture(texture_clicked);
	sb->addTexture(texture_hovered);
	sb->addTexture(back_normal);
	sb->addTexture(back_hovered);
	sb->getPositions().push_back(core::rect<s32>(core::position2di(0, 0), texture_normal->getOriginalSize()));
	sb->getPositions().push_back(core::rect<s32>(core::position2di(0, 0), texture_clicked->getOriginalSize()));
	sb->getPositions().push_back(core::rect<s32>(core::position2di(0, 0), texture_hovered->getOriginalSize()));
	sb->getPositions().push_back(core::rect<s32>(core::position2di(0, 0), back_normal->getOriginalSize()));
	sb->getPositions().push_back(core::rect<s32>(core::position2di(0, 0), back_hovered->getOriginalSize()));
	gui::SGUISprite sprite;
	gui::SGUISpriteFrame sframe;
	sprite.Frames.push_back(sframe);
	sprite.Frames[0].rectNumber = 0;
	sprite.Frames[0].textureNumber = 0;
	sb->getSprites().push_back(sprite);
	sprite.Frames[0].rectNumber = 1;
	sprite.Frames[0].textureNumber = 1;
	sb->getSprites().push_back(sprite);
	sprite.Frames[0].rectNumber = 2;
	sprite.Frames[0].textureNumber = 2;
	sb->getSprites().push_back(sprite);
	sprite.Frames[0].rectNumber = 3;
	sprite.Frames[0].textureNumber = 3;
	sb->getSprites().push_back(sprite);
	sprite.Frames[0].rectNumber = 4;
	sprite.Frames[0].textureNumber = 4;
	sb->getSprites().push_back(sprite);
	Resume.btn->setSpriteBank(sb);
	Resume.btn->setSprite(EGBS_BUTTON_UP, 0, video::SColor(255, 255, 255, 255), false);
	Resume.btn->setSprite(EGBS_BUTTON_MOUSE_OFF, 0, video::SColor(255, 255, 255, 255), false);
	Resume.btn->setSprite(EGBS_BUTTON_FOCUSED, 1, video::SColor(255, 255, 255, 255), false);
	Resume.btn->setSprite(EGBS_BUTTON_MOUSE_OVER, 2, video::SColor(255, 255, 255, 255), false);
	NewGame.btn->setSpriteBank(sb);
	NewGame.btn->setSprite(EGBS_BUTTON_UP, 0, video::SColor(255, 255, 255, 255), false);
	NewGame.btn->setSprite(EGBS_BUTTON_MOUSE_OFF, 0, video::SColor(255, 255, 255, 255), false);
	NewGame.btn->setSprite(EGBS_BUTTON_FOCUSED, 1, video::SColor(255, 255, 255, 255), false);
	NewGame.btn->setSprite(EGBS_BUTTON_MOUSE_OVER, 2, video::SColor(255, 255, 255, 255), false);
	Back.btn->setSpriteBank(sb);
	Back.btn->setSprite(EGBS_BUTTON_UP, 3, video::SColor(255, 255, 255, 255), false);
	Back.btn->setSprite(EGBS_BUTTON_MOUSE_OFF, 3, video::SColor(255, 255, 255, 255), false);
	Back.btn->setSprite(EGBS_BUTTON_FOCUSED, 4, video::SColor(255, 255, 255, 255), false);
	Back.btn->setSprite(EGBS_BUTTON_MOUSE_OVER, 4, video::SColor(255, 255, 255, 255), false);
	GM.addComponent(IrrD, &dvc);
	GM.addComponent(Resume_btn, &Resume);
	GM.addComponent(NewGame_btn, &NewGame);
	GM.addComponent(Back_btn, &Back);
	SAppContext context;
	context.device = dvc.device;
	MyEventReceiver receiver(context);
	dvc.device->setEventReceiver(&receiver);
	video::ITexture* img_background_one = driver->getTexture("Image/background.jpg");
	while (dvc.device->run() && receiver.back == false && receiver.solo == false && receiver.solo_resume == false) {
		driver->beginScene(true, true, false);
		driver->draw2DImage(img_background_one, core::position2d<s32>(0, 0), core::rect<s32>(0, 0, 1280, 720), 0, video::SColor(255, 255, 255, 255), true);
		driver->setTextureCreationFlag(ETCF_ALWAYS_32_BIT, true);
		GM.update();
	}
	if (receiver.back == true) {
		menu_back(false);
	} else if (receiver.solo == true) {
		dvc.background_sound.stop();
		dvc.background_sound2.stop();
		solo_mode(false);
	}
	else if (receiver.solo_resume == true) {
		dvc.background_sound.stop();
		dvc.background_sound2.stop();
		solo_mode(true);
	}
}

void winMenuSolo(int minutes, int seconds)
{
	static int name = 0;
	Device& dvc = dynamic_cast<Device&>(GM.getComponent(0, 3));
	GM.Reset();
	Entity IrrD = GM.addEntity();
	Entity Retry_button_btn = GM.addEntity();
	Entity Menu_button_btn = GM.addEntity();
	IVideoDriver* driver = dvc.device->getVideoDriver();
	IGUIEnvironment* env = dvc.device->getGUIEnvironment();
	env->clear();
	ISceneManager* smgr = dvc.device->getSceneManager();
	smgr->clear();
	dvc.device->getCursorControl()->setVisible(true);
	IGUISkin* skin = env->getSkin();
	IGUIFont* fontButton = env->getFont("Fonts/bomberman20px.xml");
	IGUIFont* font = env->getFont("Fonts/bomberman68px.xml");
	sf::Sound btn_hover;
	sf::Sound btn_click;
	sf::SoundBuffer enterBuffer;
	sf::SoundBuffer enterBuffer2;
	if (!enterBuffer.loadFromFile("8bit/blip-1.wav") || \
		!enterBuffer2.loadFromFile("8bit/ok-2.wav") ||
		!dvc.background_sound2.openFromFile("8bit/Themesong.wav")) {
		std::cout << "Error with Loading of the btn_play_music" << std::endl;
		exit(84);
	}
	btn_hover.setBuffer(enterBuffer);
	btn_click.setBuffer(enterBuffer2);
	skin->setFont(env->getBuiltInFont(), EGDF_TOOLTIP);
	Button Retry_button(env, GUI_ID_PLAY_BUTTON, rect<s32>(1280 / 2 - 358, 720 - 160, 1280 / 2 - 358 + 316, 720 - 20), "N e w  M a t c h", &btn_hover, &btn_click, driver, fontButton);
	Button Menu_button(env, GUI_ID_MENU, rect<s32>(1280 / 2, 720 - 160, 1280 / 2 + 316, 720 - 20), "M e n u", &btn_hover, &btn_click, driver, fontButton);
	IGUIStaticText* PlayerText = env->addStaticText(L"Congragulations !", rect<s32>(150, 720 / 3, 150 + 1000, 720 / 3 + 100), false);
	IGUIStaticText* minutesPlayer = env->addStaticText(stringw(minutes).c_str(), rect<s32>(250, 720 / 2, 250 + 100, 720 / 2 + 100), false);
	IGUIStaticText* minutesText = env->addStaticText(L" minutes and ", rect<s32>(370, 720 / 2 + 100, 370 + 300, 720 / 2 + 200), false);
	IGUIStaticText* secondsPlayer = env->addStaticText(stringw(seconds).c_str(), rect<s32>(700, 720 / 2, 200 + 700, 720 / 2 + 100), false);
	IGUIStaticText* secondsText = env->addStaticText(L" seconds", rect<s32>(900, 720 / 2 + 100, 900 + 150, 720 / 2 + 200), false);
	PlayerText->setOverrideFont(font);
	PlayerText->setOverrideColor(video::SColor(255, 0, 255, 0));
	minutesPlayer->setOverrideFont(font);
	minutesPlayer->setOverrideColor(video::SColor(255, 255, 0, 0));
	minutesText->setOverrideFont(fontButton);
	minutesText->setOverrideColor(video::SColor(255, 0, 255, 0));
	secondsText->setOverrideFont(fontButton);
	secondsText->setOverrideColor(video::SColor(255, 0, 255, 0));
	secondsPlayer->setOverrideFont(font);
	secondsPlayer->setOverrideColor(video::SColor(255, 255, 0, 0));
	Retry_button.btn->setDrawBorder(false);
	Menu_button.btn->setDrawBorder(false);
	IGUISpriteBank* sb = env->addEmptySpriteBank(("WinButtonsSolo_" + std::to_string(name)).c_str());
	name += 1;
	ITexture* texture_normal = driver->getTexture("Image/button_normal2.png");
	ITexture* texture_hovered = driver->getTexture("Image/button_hovered2.png");
	sb->addTexture(texture_normal);
	sb->addTexture(texture_hovered);
	sb->getPositions().push_back(core::rect<s32>(core::position2di(0, 0), texture_normal->getOriginalSize()));
	sb->getPositions().push_back(core::rect<s32>(core::position2di(0, 0), texture_hovered->getOriginalSize()));
	gui::SGUISprite sprite;
	gui::SGUISpriteFrame sframe;
	sprite.Frames.push_back(sframe);
	sprite.Frames[0].rectNumber = 0;
	sprite.Frames[0].textureNumber = 0;
	sb->getSprites().push_back(sprite);
	sprite.Frames[0].rectNumber = 1;
	sprite.Frames[0].textureNumber = 1;
	sb->getSprites().push_back(sprite);
	Retry_button.btn->setSpriteBank(sb);
	Retry_button.btn->setSprite(EGBS_BUTTON_UP, 0, video::SColor(255, 255, 255, 255), false);
	Retry_button.btn->setSprite(EGBS_BUTTON_MOUSE_OFF, 0, video::SColor(255, 255, 255, 255), false);
	Retry_button.btn->setSprite(EGBS_BUTTON_MOUSE_OVER, 1, video::SColor(255, 255, 255, 255), false);
	Menu_button.btn->setSpriteBank(sb);
	Menu_button.btn->setSprite(EGBS_BUTTON_UP, 0, video::SColor(255, 255, 255, 255), false);
	Menu_button.btn->setSprite(EGBS_BUTTON_MOUSE_OFF, 0, video::SColor(255, 255, 255, 255), false);
	Menu_button.btn->setSprite(EGBS_BUTTON_MOUSE_OVER, 1, video::SColor(255, 255, 255, 255), false);
	GM.addComponent(IrrD, &dvc);
	GM.addComponent(Retry_button_btn, &Retry_button);
	GM.addComponent(Menu_button_btn, &Menu_button);
	SAppContext context;
	context.device = dvc.device;
	MyEventReceiver receiver(context);
	dvc.device->setEventReceiver(&receiver);
	video::ITexture* img_background_four = driver->getTexture("Image/background2.jpg");
	sf::Music win_music;
	if (!win_music.openFromFile("Music/win.wav")) {
		std::cout << "Error with Loading of the btn_play_music (win)" << std::endl;
		exit(84);
	}
	win_music.setVolume(50);
	win_music.play();
	while (dvc.device->run() && receiver.back == false && receiver.retry == false) {
		driver->beginScene(true, true, false);
		driver->draw2DImage(img_background_four, core::position2d<s32>(0, 0), core::rect<s32>(0, 0, 1280, 720), 0, video::SColor(255, 255, 255, 255), true);
		driver->setTextureCreationFlag(ETCF_ALWAYS_32_BIT, true);
		GM.update();
	}
	if (receiver.back == true) {
		menu_back(true);
	}
	else if (receiver.retry == true) {
		solo_mode(false);
	}
	dvc.device->closeDevice();
}

void LoseMenuSolo()
{
	static int name = 0;
	Device& dvc = dynamic_cast<Device&>(GM.getComponent(0, 3));
	GM.Reset();
	Entity IrrD = GM.addEntity();
	Entity Retry_button_btn = GM.addEntity();
	Entity Menu_button_btn = GM.addEntity();
	IVideoDriver* driver = dvc.device->getVideoDriver();
	IGUIEnvironment* env = dvc.device->getGUIEnvironment();
	env->clear();
	ISceneManager* smgr = dvc.device->getSceneManager();
	smgr->clear();
	dvc.device->getCursorControl()->setVisible(true);
	IGUISkin* skin = env->getSkin();
	IGUIFont* fontButton = env->getFont("Fonts/bomberman20px.xml");
	IGUIFont* font = env->getFont("Fonts/bomberman68px.xml");
	sf::Sound btn_hover;
	sf::Sound btn_click;
	sf::SoundBuffer enterBuffer;
	sf::SoundBuffer enterBuffer2;
	if (!enterBuffer.loadFromFile("8bit/blip-1.wav") || \
		!enterBuffer2.loadFromFile("8bit/ok-2.wav")) {
		std::cout << "Error with Loading of the btn_play_music" << std::endl;
		exit(84);
	}
	btn_hover.setBuffer(enterBuffer);
	btn_click.setBuffer(enterBuffer2);
	skin->setFont(env->getBuiltInFont(), EGDF_TOOLTIP);
	Button Retry_button(env, GUI_ID_PLAY_BUTTON, rect<s32>(1280 / 2 - 358, 720 - 160, 1280 / 2 - 358 + 316, 720 - 20), "R e t r y", &btn_hover, &btn_click, driver, fontButton);
	Button Menu_button(env, GUI_ID_MENU, rect<s32>(1280 / 2, 720 - 160, 1280 / 2 + 316, 720 - 20), "M e n u", &btn_hover, &btn_click, driver, fontButton);
	IGUIStaticText* LoseText = env->addStaticText(L"Game Over !", rect<s32>(250, 720 / 3, 250 + 1000, 720 / 3 + 100), false);
	LoseText->setOverrideFont(font);
	LoseText->setOverrideColor(video::SColor(255, 0, 255, 0));
	Retry_button.btn->setDrawBorder(false);
	Menu_button.btn->setDrawBorder(false);
	IGUISpriteBank* sb = env->addEmptySpriteBank(("LooseButtonsSolo_" + std::to_string(name)).c_str());
	name += 1;
	ITexture* texture_normal = driver->getTexture("Image/button_normal2.png");
	ITexture* texture_hovered = driver->getTexture("Image/button_hovered2.png");
	sb->addTexture(texture_normal);
	sb->addTexture(texture_hovered);
	sb->getPositions().push_back(core::rect<s32>(core::position2di(0, 0), texture_normal->getOriginalSize()));
	sb->getPositions().push_back(core::rect<s32>(core::position2di(0, 0), texture_hovered->getOriginalSize()));
	gui::SGUISprite sprite;
	gui::SGUISpriteFrame sframe;
	sprite.Frames.push_back(sframe);
	sprite.Frames[0].rectNumber = 0;
	sprite.Frames[0].textureNumber = 0;
	sb->getSprites().push_back(sprite);
	sprite.Frames[0].rectNumber = 1;
	sprite.Frames[0].textureNumber = 1;
	sb->getSprites().push_back(sprite);
	Retry_button.btn->setSpriteBank(sb);
	Retry_button.btn->setSprite(EGBS_BUTTON_UP, 0, video::SColor(255, 255, 255, 255), false);
	Retry_button.btn->setSprite(EGBS_BUTTON_MOUSE_OFF, 0, video::SColor(255, 255, 255, 255), false);
	Retry_button.btn->setSprite(EGBS_BUTTON_MOUSE_OVER, 1, video::SColor(255, 255, 255, 255), false);
	Menu_button.btn->setSpriteBank(sb);
	Menu_button.btn->setSprite(EGBS_BUTTON_UP, 0, video::SColor(255, 255, 255, 255), false);
	Menu_button.btn->setSprite(EGBS_BUTTON_MOUSE_OFF, 0, video::SColor(255, 255, 255, 255), false);
	Menu_button.btn->setSprite(EGBS_BUTTON_MOUSE_OVER, 1, video::SColor(255, 255, 255, 255), false);
	GM.addComponent(IrrD, &dvc);
	GM.addComponent(Retry_button_btn, &Retry_button);
	GM.addComponent(Menu_button_btn, &Menu_button);
	SAppContext context;
	context.device = dvc.device;
	MyEventReceiver receiver(context);
	dvc.device->setEventReceiver(&receiver);
	video::ITexture* img_background_four = driver->getTexture("Image/background2.jpg");
	sf::Music win_music;
	if (!win_music.openFromFile("Music/lose.wav")) {
		std::cout << "Error with Loading of the btn_play_music (win)" << std::endl;
		exit(84);
	}
	win_music.setVolume(50);
	win_music.play();
	while (dvc.device->run() && receiver.back == false && receiver.retry == false) {
		driver->beginScene(true, true, false);
		driver->draw2DImage(img_background_four, core::position2d<s32>(0, 0), core::rect<s32>(0, 0, 1280, 720), 0, video::SColor(255, 255, 255, 255), true);
		driver->setTextureCreationFlag(ETCF_ALWAYS_32_BIT, true);
		GM.update();
	}
	if (receiver.back == true) {
		menu_back(true);
	}
	else if (receiver.retry == true) {
		solo_mode(false);
	}
	dvc.device->closeDevice();
}