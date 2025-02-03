#pragma once
#include "Map.h"
#include <vector>

class GameManager {
private:
    Map* currentMap;

public:
    GameManager(Map* map) : currentMap(map) {}

    Map* GetCurrentMap();
};
