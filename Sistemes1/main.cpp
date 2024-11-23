////#include "tutorialThreads.h"
//#include <iostream>
//#include "2inputSystem/InputSystem.h"
//#include "Utils/ConsoleControl.h"
//
//int main() {
//	//ThreadTutorialTest();
//	InputSystem* is = new InputSystem();
//
//	InputSystem::KeyBinding* kb1 = is->AddListener(K_1, [](){
//		CC::Lock();
//		std::cout << "Pressed 1" << std::endl;
//		CC::Unlock();
//	});
//
//	is->AddListener(K_0, [is, kb1]() {
//		is->RemoveAndDeleteListener(kb1);
//	});
//
//	is->StartListen();
//	while (true) {
//
//	}
//}
#include <json/json.h>
#include <fstream>
#include <iostream>
class Player {
public: 

	int life = 0;
	std::string name = "Test player";
	unsigned int coins = 0;

	Player() {

	}
	~Player() {

	}

	//De un json kei la variable que me interesan y las asigno a esta clase
	void Decode(Json::Value json){
		life = json["life"].asInt(); 
		name = json["name"].asString();
		coins = json["coins"].asUInt();
	}
	//A partir de esta clase creo un json rellenando con las variables de la clase
	Json::Value Enconde() {
		Json::Value json;
		
		json["life"] = life;
		json["name"] = name;
		json["coins"] = coins;

		return json;
	}

	static Player* FromJSON(Json::Value json) {
		Player* player = new Player();
		player->Decode(json);
		return player;
	}
};
int main() {
	Player* player = new Player();
	player->coins = 3;
	player->life = 30;
	player->name = "Capitana text";

	Json::Value newJson;
	newJson["player"] = player->Enconde();

	std::ofstream* jsonWriteFile = new std::ofstream("PlayerParseTest.json", std::ifstream::binary);

	if (!jsonWriteFile->fail()) {
		(*jsonWriteFile) << newJson;
		delete jsonWriteFile;
	}
	std::cout << "Finsihed Write" << std::endl;

	std::ifstream* jsonReadFile = new std::ifstream("PlayerParseTest.json", std::ifstream::binary);

	if (!jsonReadFile->fail())
	{
		Json::Value readedJson;
		(*jsonReadFile) >> readedJson;
		jsonReadFile->close();
		delete jsonReadFile;

		Player* readedPlayer = Player::FromJSON(readedJson);
	}
	std::cout << "Finsihed Read" << std::endl;
	while (true) {

	}
}