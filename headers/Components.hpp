/*
** EPITECH PROJECT, 2020
** Components
** File description:
** Components
*/

#ifndef COMPONENTS_HPP_
#define COMPONENTS_HPP_
#include "ECS.hpp"

extern ITimer* pTimer;

class IComponent {
    public:
        virtual ~IComponent() = default;
        int bit;
        std::string name;
};

class Crashed : public IComponent {
    public:
        Crashed() {
            this->bit = 0;
            this->name = "Crashed";
        };
        ~Crashed(){};
};

class Position : public IComponent {
    public:
        Position(int x, int y, int z) {
            this->x = x;
            this->y = y;
            this->z = z;
            this->bit = 1;
            this->name = "Position";
        };
        ~Position() {
        };
        int x, y, z = 0;
};

class Velocity : public IComponent {
    public:
        Velocity(float x, float y, float z) {
            this->x = x * 0.2;
            this->y = y * 0.2;
            this->z = z * 0.2;
            this->bit = 2;
            this->name = "Velocity";
        }
        ~Velocity(){};
        float x, y, z;
};
class Device : public IComponent {
    public:
        sf::Music background_sound;
        sf::Music background_sound2;
        Device(std::string str) {
            this->device = createDevice(video::EDT_OPENGL, dimension2d<u32>(1280, 720), 32, false, false, false, 0);
            this->device->setWindowCaption(stringw(str.c_str()).c_str());
            this->device->setResizable(true);
            pTimer = this->device->getTimer();
            pTimer->setTime(0);
            pTimer->start();
            this->bit = 3;
            this->name = "Device";
        };
        ~Device() {};
        IrrlichtDevice *device;
};

class Button : public IComponent {
    public:
        Button(IGUIEnvironment *env, s32 utility, rect<s32> position, std::string text) {
            this->btn = env->addButton(position, 0, utility, stringw(text.c_str()).c_str());
            this->bit = 4;
            this->env = env;
            this->name = "Button";
        };
        Button(IGUIEnvironment *env, s32 utility, rect<s32> position, std::string text, sf::Sound *musicHover, sf::Sound *musicClick, IVideoDriver *driver, IGUIFont *font) {
            this->btn = env->addButton(position, 0, utility, stringw(text.c_str()).c_str());
            this->bit = 4;
            this->env = env;
            this->musicHover = musicHover;
            this->musicClick = musicClick;
            this->name = "Button";
            this->btn->setOverrideFont(font);
        };
        IGUIEnvironment *env;
        IGUIButton *btn;
        sf::Sound*musicHover = NULL;
        sf::Sound*musicClick = NULL;
        bool isPlayingHover, isPlayingClick = false;
};

class ScrollBar : public IComponent {
    public:
        ScrollBar(IGUIEnvironment *env, rect<s32> position, s32 type, s32 max, s32 curr_pos) {
            scroll = env->addScrollBar(true, position, 0, type);
            scroll->setMax(max);
            scroll->setPos(curr_pos);
            this->bit = 5;
            this->env = env;
        };
        ~ScrollBar() {};
        IGUIScrollBar *scroll;
        IGUIEnvironment *env;
        //s32 curr_pos;
};

class AnimatedMesh : public IComponent {
    public:
        AnimatedMesh(ISceneManager *smgr, IVideoDriver *driver, std::string MeshPath, std::string TexturePath, vector3df scale = vector3df(1.0f), vector3df rotation = vector3df(0,0,0), vector3df pos = vector3df(0, 0, 0), bool isAi = false) {
            this->mesh = smgr->getMesh(MeshPath.c_str());
            if (!mesh)
                std::cout << "Couldn't load mesh file" << std::endl;
            this->animModel = smgr->addAnimatedMeshSceneNode(mesh);
            this->animModel->setMaterialFlag(video::EMF_LIGHTING, false);
            if (TexturePath != "")
                this->animModel->setMaterialTexture(0, driver->getTexture(TexturePath.c_str()));
            this->animModel->setScale(scale);
            this->animModel->setPosition(pos);
            animModel->setRotation(rotation);
            this->hasWin = false;
            this->_isAi = isAi;
            this->bombTimer = 0;
            this->bit = 6;
        };
        ~AnimatedMesh() {
            this->animModel->remove();
        };
        IAnimatedMesh *mesh;
        IAnimatedMeshSceneNode* animModel;
        unsigned short bombTimer;
        bool hasWin = false;
        bool _isAi = false;
};

class CameraNode : public IComponent {
    public:
        CameraNode(ISceneManager *smgr, vector3df target, vector3df pos = vector3df(0,0,0)) {
            this->camera = smgr->addCameraSceneNode();
            this->camera->setTarget(target);
            this->camera->setPosition(pos);
            this->bit = 7;
        };
        ~CameraNode()  {

        };
        ICameraSceneNode *camera;
};

class KeyBinding : public IComponent {
    public:
        KeyBinding(irr::EKEY_CODE up, irr::EKEY_CODE down, irr::EKEY_CODE left,
                    irr::EKEY_CODE right, irr::EKEY_CODE throw_bomb) {
            this->keybindVector.push_back(up);
            this->keybindVector.push_back(down);
            this->keybindVector.push_back(left);
            this->keybindVector.push_back(right);
            this->keybindVector.push_back(throw_bomb);
            this->powerupState = 0;
            this->bit = 8;
            this->name = "Key Bindings";
        };
        ~KeyBinding() {

        };
        vector<EKEY_CODE> keybindVector;
        unsigned short currentMvt;
        unsigned short powerupState;
};

class RigidBody : public IComponent {
    public:
        RigidBody() {
            this->bit = 9;
            this->name = "RigidBody";
        };
        ~RigidBody() {

        };
        vector3df pendingPos;
        double gravity = 1;
};

class TerrainMesh : public IComponent {
    public:
        TerrainMesh(ISceneManager *smgr, IVideoDriver *driver, std::string MeshPath, std::string TXPath, vector3df scale = vector3df(1.0f), vector3df rotation = vector3df(0, 0, 0), vector3df pos = vector3df(0, 0, 0)) {
            this->Mesh = smgr->getMesh(MeshPath.c_str());
            if (!Mesh)
                std::cout << "Couldn't load mesh file" << std::endl;
            this->nMesh = smgr->addMeshSceneNode(Mesh);
            this->nMesh->setMaterialFlag(video::EMF_LIGHTING, false);
            if (TXPath != "")
                this->nMesh->setMaterialTexture(0, driver->getTexture(TXPath.c_str()));
            this->nMesh->setScale(scale);
            this->nMesh->setPosition(pos);
            nMesh->setRotation(rotation);
            this->bit = 10;
        };
        ~TerrainMesh() {
        };
        IMesh *Mesh;
        IMeshSceneNode* nMesh;
};

class Bomb : public IComponent {
    public:
        Bomb(ISceneManager *smgr, IVideoDriver *driver, std::string MeshPath, std::string TexturePath, vector3df position) {
            this->bomb = smgr->getMesh(MeshPath.c_str());
            if (!bomb)
                std::cout << "Couldn't load mesh" << std::endl;
            this->bombSceneNode = smgr->addAnimatedMeshSceneNode(bomb);
            this->bombSceneNode->setMaterialFlag(video::EMF_LIGHTING, false);
            if (TexturePath != "")
                this->bombSceneNode->setMaterialTexture(0, driver->getTexture(TexturePath.c_str()));
            this->bombSceneNode->setPosition(position);
            this->bombSceneNode->setScale(vector3df(12.0f));
            this->radius = 2;
            this->timer = 0;
            this->bit = 11;
        };
        ~Bomb() {
            this->bombSceneNode->remove();
        };
        IAnimatedMesh* bomb;
        IAnimatedMeshSceneNode* bombSceneNode;
        unsigned short timer;
        unsigned short radius;
};

class BreakableWall : public IComponent {
    public:
        BreakableWall() {
            this->bit = 12;
            this->check = true;
            this->name = "BREAKABLE";
        };
        ~BreakableWall() {
        };
        bool check;
};

class Fire : public IComponent {
    public:
        Fire() {
            this->bit = 13;
            this->timer = 0;
            this->name = "Fire";
        };
        ~Fire() {

        };
        unsigned short timer;
};

class Powerup : public IComponent {
    public:
        Powerup(unsigned short type) {
            this->type = type;
            this->bit = 14;
            this->name = "Powerup";
        };
        ~Powerup() {

        };
        unsigned short type;
};

class Finish : public IComponent {
    public:
        Finish() {
            this->test = true;
            this->bit = 15;
            this->name = "Finish";
        };
        ~Finish() {

        };
        bool test;
};

class AI : public IComponent {
    AI() {
        this->bit = 16;
    };
    ~AI() {
    };
};

extern std::vector<std::string> _map_col;

extern std::vector<std::string> _map_da;

#endif /* !COMPONENTS_HPP_ */
