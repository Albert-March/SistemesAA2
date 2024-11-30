#pragma once
#include "../3Nodes/Vector2.h"
#include <json/json.h>
#include <vector>

class Player
{
public:
	Vector2 position;
	int health;
	int lives;
	int damage;
	int coins;	
	int potions;
	std::vector<std::string> weapons;

	Player(Vector2 pPosition, int pHealth, int pLifes, int pDamage, int pCoins, int pPotions)
		: position(pPosition), health(pHealth), lives(pLifes), damage(pDamage), coins(pCoins), potions(pPotions), weapons(3, "") {}


	void AddWeapon(const std::string& weapon);

	void Attack(int EnemyHp, int damage);
};

