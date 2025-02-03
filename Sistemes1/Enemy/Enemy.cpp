#include "Enemy.h"
#include "../Game/Map.h"
#include <chrono>
#include <thread>
#include <iostream>

Enemy::Enemy(Vector2 pos, Map* map, int hp, int dmg)
    : position(pos), health(hp), damage(dmg), isMoving(true), mapReference(map) {
    StartMoving();
}

Enemy::~Enemy() {
    StopMoving();
}

bool Enemy::IsDead() const {
    return health <= 0;
}

Vector2 Enemy::GetPosition() const {
    return position;
}

void Enemy::SetPosition(Vector2 newPos) {
    position = newPos;
}

void Enemy::StartMoving() {
    moveThread = std::thread(&Enemy::MoveLoop, this);
}

void Enemy::StopMoving() {
    isMoving = false;
    if (moveThread.joinable()) {
        moveThread.join();
    }
}

void Enemy::MoveLoop() {
    while (isMoving) {
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));

        if (mapReference) {
            mapReference->MoveEnemy(this);
        }
    }
}

