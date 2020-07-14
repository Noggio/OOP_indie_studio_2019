/*
** EPITECH PROJECT, 2020
** Movement.hpp
** File description:
** Movement
*/

#include "../ECS.hpp"
#include "../GameManager.hpp"
#include "../Components.hpp"

extern GameManager GM;
extern std::vector<std::string> _map;

class MeshCollider : public ISystems {
public:
	MeshCollider() {
		if (!this->collect.openFromFile("8bit/coin-3.wav"))
			exit(84);
		this->collect.setVolume(100);
	};
	void update() {
		for (Entity ID = 0; ID != MAX_ENTITIES; ++ID) {
			if (GM.getComponent(ID, 6).bit != 0 && GM.getComponent(ID, 9).bit != 0 && GM.getComponent(ID, 8).bit != 0) {
				AnimatedMesh &Mesh1 = dynamic_cast<AnimatedMesh&>(GM.getComponent(ID, 6));
				RigidBody& rb = dynamic_cast<RigidBody&>(GM.getComponent(ID, 9));
				KeyBinding& binds = dynamic_cast<KeyBinding&>(GM.getComponent(ID, 8));
				vector3df currentPos = Mesh1.animModel->getPosition();
				//_map.at((Mesh1.animModel->getPosition().Z / 16) + 9).at((Mesh1.animModel->getPosition().X / 16) + 12) = ' ';
				Mesh1.animModel->setPosition(rb.pendingPos);
				for (Entity nID = 0; nID != MAX_ENTITIES; ++nID) {
					if (GM.getComponent(nID, 6).bit != 0 && GM.getComponent(nID, 9).bit != 0 && nID != ID) {
						AnimatedMesh& Mesh2 = dynamic_cast<AnimatedMesh&>(GM.getComponent(nID, 6));
						vector3df* edges = new vector3df[8];
						Mesh1.animModel->getTransformedBoundingBox().getEdges(edges);
						float width = (edges[5].X - edges[1].X);
						if (Mesh1.animModel->getTransformedBoundingBox().intersectsWithBox(Mesh2.animModel->getTransformedBoundingBox())) {
							if (Mesh2.animModel->getTransformedBoundingBox().isPointInside(Mesh1.animModel->getTransformedBoundingBox().getCenter() + vector3df(width, 0, 0)))
								Mesh1.animModel->setPosition(currentPos - vector3df(1, 0, 0));
							else if (Mesh2.animModel->getTransformedBoundingBox().isPointInside(Mesh1.animModel->getTransformedBoundingBox().getCenter() - vector3df(width, 0, 0)))
								Mesh1.animModel->setPosition(currentPos + vector3df(1, 0, 0));
							else if (Mesh2.animModel->getTransformedBoundingBox().isPointInside(Mesh1.animModel->getTransformedBoundingBox().getCenter() + vector3df(0, 0, width)))
								Mesh1.animModel->setPosition(currentPos - vector3df(0, 0, 1));
							else if (Mesh2.animModel->getTransformedBoundingBox().isPointInside(Mesh1.animModel->getTransformedBoundingBox().getCenter() - vector3df(0, 0, width)))
								Mesh1.animModel->setPosition(currentPos + vector3df(0, 0, 1));
						}
					}
					else if (GM.getComponent(nID, 6).bit != 0 && GM.getComponent(nID, 14).bit != 0 && nID != ID) {
						AnimatedMesh& Mesh2 = dynamic_cast<AnimatedMesh&>(GM.getComponent(nID, 6));
						Powerup& pu = dynamic_cast<Powerup&>(GM.getComponent(nID, 14));
						if (Mesh1.animModel->getTransformedBoundingBox().intersectsWithBox(Mesh2.animModel->getTransformedBoundingBox())) {
							if (pu.type == 1) {
								binds.powerupState = 1;
								Mesh2.~AnimatedMesh();
								GM.DeleteEntity(nID);
							}
							else if (pu.type == 2) {
								binds.powerupState = 2;
								Mesh2.~AnimatedMesh();
								GM.DeleteEntity(nID);
							}
							this->collect.play();
						}
					}
					else if (GM.getComponent(nID, 6).bit != 0 && GM.getComponent(nID, 15).bit != 0 && nID != ID) {
						AnimatedMesh& Mesh2 = dynamic_cast<AnimatedMesh&>(GM.getComponent(nID, 6));
						Finish& fnsh = dynamic_cast<Finish&>(GM.getComponent(nID, 15));
						if (Mesh1.animModel->getTransformedBoundingBox().intersectsWithBox(Mesh2.animModel->getTransformedBoundingBox())) {
							Mesh1.hasWin = !fnsh.test;
							fnsh.test = !fnsh.test;
							std::cout << "He won" << std::endl;
						}
					}
				}
				//_map.at((Mesh1.animModel->getPosition().Z / 16) + 9).at((Mesh1.animModel->getPosition().X / 16) + 12) = '1';
			}
		}
	};
	~MeshCollider() {

	};
	sf::Music collect;
};