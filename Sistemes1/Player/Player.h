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


	////De un json kei la variable que me interesan y las asigno a esta clase
	//void Decode(Json::Value json) {
	//	position.X = json["position"]["x"].asInt();
	//	position.Y = json["position"]["y"].asInt();
	//	name = json["name"].asString();
	//	health = json["health"].asInt();
	//	lifes = json["life"].asInt();
	//	damage = json["damage"].asInt();
	//	coins = json["coins"].asUInt();
	//	potions = json["potions"].asUInt();
	//}

	////A partir de esta clase creo un json rellenando con las variables de la clase
	//Json::Value Enconde() {
	//	Json::Value json;

	//	json["position"]["x"] = position.X;
	//	json["position"]["y"] = position.Y;
	//	json["name"] = name;
	//	json["health"] = health;
	//	json["life"] = lifes;
	//	json["damage"] = damage;
	//	json["coins"] = coins;
	//	json["potions"] = potions;

	//	return json;
	//}

	//static Player* FromJSON(Json::Value json) {
	//	Player* player = new Player();
	//	player->Decode(json);
	//	return player;
	//}

	void Attack(int EnemyHp, int damage);
	/*void CollectObjects(std::vector<Arma> arma, int potions);
	void Heal(int potions);*/
};

