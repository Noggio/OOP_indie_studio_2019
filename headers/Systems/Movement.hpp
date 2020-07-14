/*
** EPITECH PROJECT, 2020
** Movement.hpp
** File description:
** Movement
*/

#include "../ECS.hpp"
#include "../GameManager.hpp"
#include "../Components.hpp"
#include "../ia.hpp"

extern GameManager GM;
extern std::vector<std::string> _map;

class Movement : public ISystems{
    public:
        Movement() {
            if (!this->footsteps.openFromFile("8bit/footstep.wav")) {
                std::cout << "Couldn't load footsteps.wav" << std::endl;
                exit(84);
            }
            this->footsteps.setVolume(100);
        };
        void update() {
            for (Entity ID = 0; ID != MAX_ENTITIES; ++ID) {
                if (GM.getComponent(ID, 6).bit != 0 && GM.getComponent(ID, 8).bit != 0 && GM.getComponent(ID, 9).bit != 0) {
                    AnimatedMesh &PlayerMesh = dynamic_cast<AnimatedMesh&>(GM.getComponent(ID, 6));
                    KeyBinding &Binds = dynamic_cast<KeyBinding&>(GM.getComponent(ID,8));
                    RigidBody& rb = dynamic_cast<RigidBody&>(GM.getComponent(ID, 9));
                    Device &dvc = dynamic_cast<Device&>(GM.getComponent(0, 3));
                    MyEventReceiver *receiver = dynamic_cast<MyEventReceiver*>(dvc.device->getEventReceiver());
                    unsigned short speedup = 1;
                    if (Binds.powerupState == 2)
                        speedup = 2;
                    if (PlayerMesh._isAi == false) {
                        if (receiver->IsKeyDown(Binds.keybindVector[1])) {  // down
                            rb.pendingPos = PlayerMesh.animModel->getPosition() + vector3df(1 * speedup, 0, 0);
                            PlayerMesh.animModel->setAnimationSpeed(60);
                            PlayerMesh.animModel->setRotation(vector3df(0, -90, 0));
                            Binds.currentMvt = 1;
                        }
                        else if (receiver->IsKeyDown(Binds.keybindVector[0])) {  // up
                            rb.pendingPos = PlayerMesh.animModel->getPosition() + vector3df(-1 * speedup, 0, 0);
                            PlayerMesh.animModel->setAnimationSpeed(60);
                            PlayerMesh.animModel->setRotation(vector3df(0, 90, 0));
                            Binds.currentMvt = 2;
                        }
                        else if (receiver->IsKeyDown(Binds.keybindVector[3])) { // right
                            rb.pendingPos = PlayerMesh.animModel->getPosition() + vector3df(0, 0, 1 * speedup);
                            PlayerMesh.animModel->setAnimationSpeed(60);
                            PlayerMesh.animModel->setRotation(vector3df(0, -180, 0));
                            Binds.currentMvt = 3;
                        }
                        else if (receiver->IsKeyDown(Binds.keybindVector[2])) { // left
                            rb.pendingPos = PlayerMesh.animModel->getPosition() + vector3df(0, 0, -1 * speedup);
                            PlayerMesh.animModel->setAnimationSpeed(60);
                            PlayerMesh.animModel->setRotation(vector3df(0, 0, 0));
                            Binds.currentMvt = 4;
                        }
                        else if (receiver->IsKeyDown(Binds.keybindVector[4])) { //throw_bomb
                            if (PlayerMesh.bombTimer > 125) {
                                PlayerMesh.bombTimer = 0;
                                Entity bombID = GM.addEntity();
                                Bomb* bombComponent = new Bomb(dvc.device->getSceneManager(), dvc.device->getVideoDriver(), "Ressources/bomb.b3d", "Ressources/Bomb.png", PlayerMesh.animModel->getPosition());
                                if (Binds.powerupState == 1)
                                    bombComponent->radius = 3;
                                GM.addComponent(bombID, bombComponent);
                                GM.addComponent(bombID, &rb);
                                //_map.at((bombComponent->bombSceneNode->getPosition().Z / 16) + 9).at((bombComponent->bombSceneNode->getPosition().X / 16) + 12) = 'B';
                            }
                        }
                        else {
                            PlayerMesh.animModel->setAnimationSpeed(0);
                            Binds.currentMvt = 0;
                        }
                    }
                    else {
                        float checkCol = PlayerMesh.animModel->getPosition().X;
                        int y_bot = (int)PlayerMesh.animModel->getPosition().Y;
                        int x_bot = ceil(checkCol / 16 + 12);
                        y_bot = (y_bot / 16) + 9;
                        if (is_a_dangerous_zone(x_bot, y_bot) == true) {
                            if (not_a_solid_block(x_bot - 1, y_bot) == true && is_a_dangerous_zone(x_bot - 1, y_bot) == false) {
                                /*move_player x - 1*/
                                rb.pendingPos = PlayerMesh.animModel->getPosition() + vector3df(0, 0, -1 * speedup);
                                PlayerMesh.animModel->setAnimationSpeed(60);
                                PlayerMesh.animModel->setRotation(vector3df(0, 0, 0));
                                Binds.currentMvt = 4;
                            }
                            else if (not_a_solid_block(x_bot + 1, y_bot) == true && is_a_dangerous_zone(x_bot + 1, y_bot) == false) {
                                // move_player x + 1
                                rb.pendingPos = PlayerMesh.animModel->getPosition() + vector3df(0, 0, 1 * speedup);
                                PlayerMesh.animModel->setAnimationSpeed(60);
                                PlayerMesh.animModel->setRotation(vector3df(0, -180, 0));
                                Binds.currentMvt = 3;
                            }
                            else if (not_a_solid_block(x_bot, y_bot - 1) == true && is_a_dangerous_zone(x_bot, y_bot - 1) == false) {
                                //move_player y -1
                                rb.pendingPos = PlayerMesh.animModel->getPosition() + vector3df(-1 * speedup, 0, 0);
                                PlayerMesh.animModel->setAnimationSpeed(60);
                                PlayerMesh.animModel->setRotation(vector3df(0, 90, 0));
                                Binds.currentMvt = 2;
                            }
                            else if (not_a_solid_block(x_bot, y_bot + 1) == true && is_a_dangerous_zone(x_bot, y_bot + 1) == false) {
                                //move_player y + 1
                                rb.pendingPos = PlayerMesh.animModel->getPosition() + vector3df(1 * speedup, 0, 0);
                                PlayerMesh.animModel->setAnimationSpeed(60);
                                PlayerMesh.animModel->setRotation(vector3df(0, -90, 0));
                                Binds.currentMvt = 1;
                            }
                            else {
                                if (PlayerMesh.bombTimer > 125) {
                                    PlayerMesh.bombTimer = 0;
                                    Entity bombID = GM.addEntity();
                                    Bomb* bombComponent = new Bomb(dvc.device->getSceneManager(), dvc.device->getVideoDriver(), "Ressources/bomb.b3d", "Ressources/Bomb.png", PlayerMesh.animModel->getPosition());
                                    if (Binds.powerupState == 1)
                                        bombComponent->radius = 3;
                                    GM.addComponent(bombID, bombComponent);
                                    GM.addComponent(bombID, &rb);
                                    //trhow bomp
                                }
                            }
                        }
                    }
                    rb.pendingPos += vector3df(0, -rb.gravity, 0);
                    PlayerMesh.bombTimer += 1;
                }
            }
        }
        ~Movement() {
        };
        sf::Music footsteps;
};


