#pragma once
#include "Vector2.h"
#include <iostream>

enum class CellState {
    Empty,
    Player,
    Wall,
    Enemy,
    Portal,
    Chest,
    Coin,
    Potion,
    Weapon
};

class INodeContent
{
private:
    CellState _state;

public:
    INodeContent(CellState state) : _state(state) {}

    void SetState(CellState state) {
        _state = state;
    }

    CellState GetState() const {
        return _state;
    }

    void Draw(Vector2 offset){
        char character;
        switch (_state) {
        case CellState::Empty:character = ' '; break;
        case CellState::Player:character = 'J'; break;
        case CellState::Wall:character = '#'; break;
        case CellState::Enemy:character = 'E'; break;
        case CellState::Portal:character = 'P'; break;
        case CellState::Chest:character = 'C'; break;
        case CellState::Coin:character = 'O'; break;
        case CellState::Potion:character = 'U'; break;
        case CellState::Weapon:character = 'W'; break;
        default:character = '?'; break;
        }
        std::cout << character;
    }
};

