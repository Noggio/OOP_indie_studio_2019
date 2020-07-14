/*
** EPITECH PROJECT, 2020
** MeshCollider.hpp
** File description:
** MeshCollider
*/

#ifndef MESHCOLLIDER_HPP_
#define MESHCOLLIDER_HPP_
#include "../ECS.hpp"
#include "../Components.hpp"
#include "../GameManager.hpp"

extern GameManager GM;

class TerrainCollider : public ISystems{
    public:
        TerrainCollider() {
        };
        void update() {
            for (Entity ID = 0; ID != MAX_ENTITIES; ++ID) {
                if (GM.getComponent(ID, 9).bit != 0 && GM.getComponent(ID, 6).bit != 0 && GM.getComponent(ID, 8).bit != 0) {
                    AnimatedMesh &aMesh = dynamic_cast<AnimatedMesh&>(GM.getComponent(ID, 6));
                    KeyBinding &kb = dynamic_cast<KeyBinding&>(GM.getComponent(ID, 8));
                    RigidBody& rb = dynamic_cast<RigidBody&>(GM.getComponent(ID, 9));
                    for (Entity nID = 0; nID != MAX_ENTITIES; ++nID) {
                        if (GM.getComponent(nID, 10).bit != 0 && nID != ID) {
                            TerrainMesh& tMesh = dynamic_cast<TerrainMesh&>(GM.getComponent(nID, 10));
                            if (aMesh.animModel->getTransformedBoundingBox().intersectsWithBox(tMesh.nMesh->getTransformedBoundingBox())) {
                                rb.pendingPos = aMesh.animModel->getPosition() + vector3df(0, 1, 0);
                            }
                        }
                    }
                }
            }
        };
        ~TerrainCollider() {
        };
};

#endif /* !MESHCOLLIDER_HPP_ */
