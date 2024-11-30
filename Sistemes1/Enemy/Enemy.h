#pragma once
#include <thread>
#include "../3Nodes/Vector2.h"
#include "../Player/Player.h"
class Enemy
{
private:
    Vector2 position;
    int health;
    int attackDamage;
    bool alive;
    int movementType;
    std::thread movementThread;

    void Movement(int moveDelay) {
        while (alive) {
            std::this_thread::sleep_for(std::chrono::seconds(moveDelay));
            if (alive) {
                MoveRandomly();
            }
        }
    }

    void MoveRandomly() {
        movementType = 0;
        //movementType = rand() % 3;
        switch (movementType)
        {
        case 0:
            position.X += 1; //De moment
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        }        
    }

public:
    Enemy(int pHealth, int pAttackDamage, int pDropChance)
        : health(pHealth), attackDamage(pAttackDamage), alive(true) {}

    ~Enemy() {
        if (movementThread.joinable()) {
            movementThread.join();
        }
    }

    void StartMoving(int moveDelay) {
        movementThread = std::thread(&Enemy::Movement, this, moveDelay);
    }

    void AttackPlayer(int& playerHealth) {
        if (!alive) {
            return;
        }
        playerHealth -= attackDamage;
        if (playerHealth <= 0) {
            playerHealth = 0;
        }
    }

    void TakeDamage(int damage) {
        if (!alive) {
            return;
        }

        health -= damage;

        if (health <= 0) {
            Die();
        }
    }

    void Die() {
        alive = false;

        if (rand() % 100 < 50) {
            //Dropea arma
        }
        else {
            //Dropea potion
        }
    }

    //void DropLoot(Player& player) {
    //    int dropType = rand() % 4; // 0: Espada, 1: Arco, 2 y 3: Poción
    //    if (dropType == 0) {
    //        player.AddWeapon(Arma("Espada", 10));
    //    }
    //    else if (dropType == 1) {
    //        player.AddWeapon(Arma("Arco", 5));
    //    }
    //    else {
    //        player.AddPotion();
    //    }
    //}

    bool IsAlive() const {
        return alive;
    }

};

