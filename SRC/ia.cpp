/*
** EPITECH PROJECT, 2020
** OOP_indie_studio_2019
** File description:
** ia
*/

#include "../headers/ia.hpp"

bool is_a_dangerous_zone(int x, int y)
{
    if (_map_da[y][x] == '1')
        return true;
    return false;
}

bool not_a_solid_block(int x, int y)
{
    if (_map_col[y][x] == '1')
        return false;
    return true;
}
