/*
** EPITECH PROJECT, 2020
** EntityManager.hpp
** File description:
** EntityManager
*/

#ifndef ENTITYMANAGER_HPP_
#define ENTITYMANAGER_HPP_
#include <queue>
#include <array>
#include <map>
#include <list>
#include <algorithm>
#include "ECS.hpp"
#include "Components.hpp"

class EntityManager {
    public:
        EntityManager() {
            for (Entity entity = 0; entity < MAX_ENTITIES; ++entity) {
                AvailableEntities.push(entity);
                this->componentMap[entity];
                this->componentMap[entity] = std::vector<IComponent *>(MAX_COMPONENT, new Crashed());
            }
            std::cout << "Entity Manager created" << std::endl;
        };
        Entity getAliveEntities() {
            return this->AliveEntities;
        }
        Entity NewEntity() {
            if (AvailableEntities.size() != 0) {
                Entity id = AvailableEntities.front();
                AvailableEntities.pop();
                AliveEntities++;
                return id;
            } else {
                std::cout << "Too many entities" << std::endl;
                return -1;
            }
        };
        void DeleteEntity(Entity entity) {
            if (entity > MAX_ENTITIES)
                std::cout << "This entity does not exist" << std::endl;
            else {
                AvailableEntities.push(entity);
                AliveEntities -= 1;
                this->componentMap[entity].clear();
                for (uint32_t i = 0; i < MAX_COMPONENT; ++i) {
                    this->componentMap[entity].push_back(new Crashed());
                }
            }
        };
        void AddComponent(Entity ID, IComponent *component) {
            this->componentMap.at(ID).at(component->bit - 1) = component;
        };
        IComponent &getComponent(Entity ID, int bit) {
            if (this->componentMap.at(ID).size() > 0)
                return *this->componentMap.at(ID).at(bit - 1);
            else {
                std::cout << "crashed" << std::endl;
                Crashed *crash = new Crashed();
                return *crash;
            }
        }
        ~EntityManager() {
        };
    private:
        std::queue<Entity> AvailableEntities {};
        uint32_t AliveEntities;
        std::map<int, std::vector<IComponent *>> componentMap;
};
#endif /* !ENTITYMANAGER_HPP_ */