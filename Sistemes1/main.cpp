#include <json/json.h>
#include <fstream>
#include <iostream>
#include <../Player/Player.h>
#include "Game/GameManager.h"
#include "Game/World.h"

int main() {

	srand(time(NULL));
	Vector2 size(11, 11);
	Vector2 offset(0, 0);
	Vector2 pPosition(5, 5);

	World world(size, offset);
	Map map(&world);

	Player* player;
	player = new Player(pPosition, 100, 3, 10, 0, 0);

	NodeMap* map00 = world.GetNodeMap(5);
	map.InitializeMap(map00);

	map.InitializePlayer(player);
	player->AddWeapon("Espada");
	map.DrawMap();

	map.StartInputListener();
	map.StartMovementControl(500);

	bool isGameRunning = true;
	while (isGameRunning) {

		std::this_thread::sleep_for(std::chrono::milliseconds(50));  
	}
	map.StopThreads();
	delete player;

	std::cout << "El juego ha terminado." << std::endl;
	return 0;
}
