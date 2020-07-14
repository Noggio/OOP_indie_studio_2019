/*
** EPITECH PROJECT, 2020
** Systems.hpp
** File description:
** Systems
*/

#ifndef SYSTEMS_HPP_
#define SYSTEMS_HPP_
#include "../ECS.hpp"
#include "../Components.hpp"
#include "../GameManager.hpp"

class ISystems {
    public:
        virtual void update() = 0;
        virtual ~ISystems() = default;
};

#endif /* !SYSTEMS_HPP_ */
