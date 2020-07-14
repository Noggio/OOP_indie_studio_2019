/*
** EPITECH PROJECT, 2020
** Visual Studio Live Share (Workspace)
** File description:
** menu
*/

#ifndef MENU_HPP_
#define MENU_HPP_

#include "ECS.hpp"
#include "Components.hpp"
#include "GameManager.hpp"

extern GameManager GM;

class Menu : public ISystems
{
    private:
    public:
        Menu();
        void update() {};
        ~Menu();
};

#endif /* !MENU_HPP_ */
