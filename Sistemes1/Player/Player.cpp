#include "Player.h"
#include <iostream>

void Player::AddWeapon(const std::string& weapon) {
	for (int i = 0; i < weapons.size(); ++i) {
		if (weapons[i].empty()) {
			weapons[i] = weapon;
			return;
		}
	}
	std::cout << "No hay espacio para a�adir m�s armas." << std::endl;
}
