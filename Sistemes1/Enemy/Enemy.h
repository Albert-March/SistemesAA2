#pragma once
#include "../3Nodes/Vector2.h"
#include <thread>

class Map; // Declaración adelantada para evitar dependencias circulares

class Enemy {
private:
    Vector2 position;
    int health;
    int damage;
    bool isMoving;
    std::thread moveThread;
    Map* mapReference;

    void MoveLoop();

public:
    Enemy(Vector2 pos, Map* map, int hp = 100, int dmg = 10);
    ~Enemy();

    bool IsDead() const;
    Vector2 GetPosition() const;
    void SetPosition(Vector2 newPos);

    void StartMoving();
    void StopMoving();
};