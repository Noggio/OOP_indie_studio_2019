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

class BombManager : public ISystems {
public:
	BombManager() {
		if (!this->enterBuffer.loadFromFile("Music/death.wav") || !this->enterBuffer2.loadFromFile("8bit/explode-1.wav"))
			exit(84);
		deathSound.setBuffer(this->enterBuffer);
		deathSound.setVolume(100);
	};
	void update() {
		for (Entity ID = 0; ID != MAX_ENTITIES; ++ID) {
			if (GM.getComponent(ID, 11).bit != 0) {
				Bomb &bomb = dynamic_cast<Bomb&>(GM.getComponent(ID, 11));
				if (bomb.timer > 200) {
					for (Entity nID = 0; nID != MAX_ENTITIES; ++nID) {
						if (nID != ID && GM.getComponent(nID, 6).bit != 0 && (GM.getComponent(nID, 12).bit != 0 || GM.getComponent(nID, 8).bit != 0)) {
							AnimatedMesh& mesh = dynamic_cast<AnimatedMesh&>(GM.getComponent(nID, 6));

							line3df FrontLine(bomb.bombSceneNode->getPosition(), bomb.bombSceneNode->getPosition() + vector3df(16 * bomb.radius, 0, 0));
							line3df BackLine(bomb.bombSceneNode->getPosition(), bomb.bombSceneNode->getPosition() + vector3df(-(16 * bomb.radius), 0, 0));
							line3df RightLine(bomb.bombSceneNode->getPosition(), bomb.bombSceneNode->getPosition() + vector3df(0, 0, 16 * bomb.radius));
							line3df LeftLine(bomb.bombSceneNode->getPosition(), bomb.bombSceneNode->getPosition() + vector3df(0, 0, -(16 * bomb.radius))); // en bas a droite
							unsigned int length = 0;
							unsigned int width = 0;


							for (unsigned int l = 0; l < _map_da.size(); l++) {
								for (unsigned int j = 0; _map_da[l][j] != 0; j++) {
									length = j;
									width = l;
								}
							}

							if (length == 26 && width == 18) {
								//collision zone
								float checkCol = bomb.bombSceneNode->getPosition().X;
								int xCol = ceil(checkCol / 16 + 12);
								int yCol = (int)bomb.bombSceneNode->getPosition().Z;
								yCol = (yCol / 16) + 9;
								if (yCol + 1 < 17) {
									_map_col.at(yCol).at(xCol) = '0';
									yCol += 1;
									_map_col.at(yCol).at(xCol) = '0';
								}
								if (yCol + 1 < 17) {
									yCol += 1;
									_map_col.at(yCol).at(xCol) = '0';
								}

								//danger zone
								float checkDan = bomb.bombSceneNode->getPosition().X;
								int xDan = ceil(checkDan / 16 + 12);
								int yDan = (int)bomb.bombSceneNode->getPosition().Z;
								yDan = (yDan / 16) + 9;
								if (yDan + 1 < 17) {
									_map_da.at(yDan).at(xDan) = '1';
									yDan += 1;
									_map_da.at(yDan).at(xDan) = '1';
								}
								if (yDan + 1 < 17) {
									yDan += 1;
									_map_da.at(yDan).at(xDan) = '1';
								}




								//collision zone
								checkCol = bomb.bombSceneNode->getPosition().X;
								xCol = ceil(checkCol / 16 + 12);
								yCol = (int)bomb.bombSceneNode->getPosition().Z;
								yCol = (yCol / 16) + 9;
								if (xCol - 1 > 1) {
									_map_col.at(yCol).at(xCol) = '0';
									xCol -= 1;
									_map_col.at(yCol).at(xCol) = '0';
								}
								if (xCol - 1 > 1) {
									xCol -= 1;
									_map_col.at(yCol).at(xCol) = '0';
								}


								//danger zone
								checkDan = bomb.bombSceneNode->getPosition().X;
								xDan = ceil(checkDan / 16 + 12);
								yDan = (int)bomb.bombSceneNode->getPosition().Z;
								yDan = (yDan / 16) + 9;
								if (xDan - 1 > 1) {
									_map_da.at(yDan).at(xDan) = '1';
									xDan -= 1;
									_map_da.at(yDan).at(xDan) = '1';
								}
								if (xDan - 1 > 1) {
									xDan -= 1;
									_map_da.at(yDan).at(xDan) = '1';
								}




								//collision zone
								checkCol = bomb.bombSceneNode->getPosition().X;
								xCol = ceil(checkCol / 16 + 12);
								yCol = (int)bomb.bombSceneNode->getPosition().Z;
								yCol = (yCol / 16) + 9;
								if (yCol - 1 > 1) {
									_map_col.at(yCol).at(xCol) = '0';
									yCol -= 1;
									_map_col.at(yCol).at(xCol) = '0';
								}
								if (yCol - 1 > 1) {
									yCol -= 1;
									_map_col.at(yCol).at(xCol) = '0';
								}


								//danger zone
								checkDan = bomb.bombSceneNode->getPosition().X;
								xDan = ceil(checkDan / 16 + 12);
								yDan = (int)bomb.bombSceneNode->getPosition().Z;
								yDan = (yDan / 16) + 9;
								if (yDan - 1 > 1) {
									_map_da.at(yDan).at(xDan) = '1';
									yDan -= 1;
									_map_da.at(yDan).at(xDan) = '1';
								}
								if (yDan - 1 > 1) {
									yDan -= 1;
									_map_da.at(yDan).at(xDan) = '1';
								}



								//collision zone
								checkCol = bomb.bombSceneNode->getPosition().X;
								xCol = ceil(checkCol / 16 + 12);
								yCol = (int)bomb.bombSceneNode->getPosition().Z;
								yCol = (yCol / 16) + 9;
								if (xCol + 1 > 25) {
									_map_col.at(yCol).at(xCol) = '0';
									xCol += 1;
									_map_col.at(yCol).at(xCol) = '0';
								}
								if (xCol + 1 > 25) {
									xCol += 1;
									_map_col.at(yCol).at(xCol) = '0';
								}

								//danger zone
								checkDan = bomb.bombSceneNode->getPosition().X;
								xDan = ceil(checkDan / 16 + 12);
								yDan = (int)bomb.bombSceneNode->getPosition().Z;
								yDan = (yDan / 16) + 9;
								if (xDan + 1 > 25) {
									_map_da.at(yDan).at(xDan) = '1';
									xDan += 1;
									_map_da.at(yDan).at(xDan) = '1';
								}
								if (xDan + 1 > 25) {
									xDan += 1;
									_map_da.at(yDan).at(xDan) = '1';
								}
							}



							if (mesh.animModel->getTransformedBoundingBox().intersectsWithLine(FrontLine) || mesh.animModel->getTransformedBoundingBox().intersectsWithLine(BackLine)
								|| mesh.animModel->getTransformedBoundingBox().intersectsWithLine(RightLine) || mesh.animModel->getTransformedBoundingBox().intersectsWithLine(LeftLine)) {
								if (GM.getComponent(nID, 8).bit != 0)
									deathSound.play();
								mesh.~AnimatedMesh();
								GM.DeleteEntity(nID);
							}
						}
					}
					createFire(bomb);
					GM.DeleteEntity(ID);
					bomb.~Bomb();
					this->explosion.setBuffer(this->enterBuffer2);
					this->explosion.play();
					for (unsigned int l = 0; l < _map_da.size(); l++) {
						for (unsigned int j = 0; _map_da[l][j] != 0; j++) {
							_map_da[l][j] = '0';
						}
					}
				} else
					bomb.timer += 1;
			}
			else if (GM.getComponent(ID, 6).bit != 0 && GM.getComponent(ID, 13).bit != 0) {
				AnimatedMesh& mesh = dynamic_cast<AnimatedMesh&>(GM.getComponent(ID, 6));
				Fire& fireElement = dynamic_cast<Fire&>(GM.getComponent(ID, 13));
				if (fireElement.timer > 60) {
					mesh.~AnimatedMesh();
					GM.DeleteEntity(ID);
				}
				else {
					fireElement.timer += 1;
				}
			}
		}
	};
	void createFire(Bomb &bmb) {
		Device& dvc = dynamic_cast<Device&>(GM.getComponent(0, 3));
		std::vector<vector3df> positions;
		for (int i = 0; i <= bmb.radius; ++i) {
			if (i == 0)
				positions.push_back(bmb.bombSceneNode->getPosition());
			else {
				positions.push_back(bmb.bombSceneNode->getPosition() + vector3df(0, 0, 16 * i));
				positions.push_back(bmb.bombSceneNode->getPosition() - vector3df(0, 0, 16 * i));
				positions.push_back(bmb.bombSceneNode->getPosition() + vector3df(16 * i, 0, 0));
				positions.push_back(bmb.bombSceneNode->getPosition() - vector3df(16 * i, 0, 0));
			}
		}
		for (int i = 0; i < positions.size(); ++i) {
			if (positions.at(i).X > -184 && positions.at(i).Z > -150 && positions.at(i).X < 184 && positions.at(i).Z < 112) {
				Entity et = GM.addEntity();
				AnimatedMesh *mesh = new AnimatedMesh(dvc.device->getSceneManager(), dvc.device->getVideoDriver(), "Ressources/fire.b3d", "Ressources/Fire.png", vector3df(0.2f), vector3df(0.0f), positions.at(i));
				Fire *fire = new Fire;
				mesh->animModel->setAnimationSpeed(80);
				GM.addComponent(et, mesh);
				GM.addComponent(et, fire);
			}
		}
	};
	~BombManager() {};
	sf::Sound deathSound;
	sf::Sound explosion;
	sf::SoundBuffer enterBuffer;
	sf::SoundBuffer enterBuffer2;
};