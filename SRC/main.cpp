/*
** EPITECH PROJECT, 2020
** main.cpp
** File description:
** main
*/

#include "../headers/ECS.hpp"
#include "../headers/EntityManager.hpp"
#include "../headers/Components.hpp"
#include "../headers/GameManager.hpp"
#include "../headers/Systems/Movement.hpp"
#include "../headers/Systems/Render.hpp"
#include "../headers/Systems/ButtonSong.hpp"
#include "../headers/Systems/TerrainCollider.hpp"
#include "../headers/Systems/MeshCollider.hpp"
#include "../headers/Systems/Bomb.hpp"
#include "../headers/menu.hpp"
#include <irrlicht.h>
#include <thread>
#include <chrono>

using namespace irr;
using namespace std;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

GameManager GM;
double fixedDTime = 1.0f / 60.0f;
ITimer* pTimer;
double lastFrame = 0.0;

int main(void)
{
    Render render;
    ButtonSong btnsng;
    Movement mvt;
    TerrainCollider TCldr;
    MeshCollider MClrd;
    BombManager BmbMngr;
    GM.addSystem(&btnsng);
    GM.addSystem(&mvt);
    GM.addSystem(&TCldr);
    GM.addSystem(&MClrd);
    GM.addSystem(&BmbMngr);
    GM.addSystem(&render);
    menu();
    return 0;
}