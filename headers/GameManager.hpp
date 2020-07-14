/*
** EPITECH PROJECT, 2020
** GameManager.hpp
** File description:
** GameManager
*/

#ifndef GAMEMANAGER_HPP_
#define GAMEMANAGER_HPP_
#include "ECS.hpp"
#include "EntityManager.hpp"
#include "Systems/ISystems.hpp"
#include "Components.hpp"

extern double fixedDTime;
extern ITimer* pTimer;
extern double lastFrame;

class GameManager {
    public:
        GameManager() {

        };
        void Reset() {
            //this->EM.~EntityManager();
            for (Entity ID = 0; ID != MAX_ENTITIES; ++ID)
                this->DeleteEntity(ID);
            this->EM = EntityManager();
        }
        Entity addEntity() {
            return this->EM.NewEntity();
        }
        void DeleteEntity(Entity ID) {
            EM.DeleteEntity(ID);
        }
        Entity getAliveEntities() {
            return EM.getAliveEntities();
        }
        void addSystem(ISystems *system) {
            this->systems.push_back(system);
        }
        void addComponent(Entity ID, IComponent *component) {
            this->EM.AddComponent(ID, component);
        }
        void update() {
            if ((pTimer->getTime() - lastFrame) > fixedDTime) {
                for (auto it = this->systems.cbegin(); it != this->systems.cend(); ++it) {
                    (*it)->update();
                }
                lastFrame = pTimer->getTime();
            }
        };
        IComponent &getComponent(Entity ID, int bit) {
            return this->EM.getComponent(ID, bit);
        }
        ~GameManager() {

        };
        void menu();
    protected:
    private:
        EntityManager EM;
        std::vector<ISystems *> systems;
};

#endif /* !GAMEMANAGER_HPP_ */
