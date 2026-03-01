#include "../include/Tileset.hpp"

Tileset::Tileset() {

	std::shared_ptr<Tileset::Group> water_water = std::make_shared<Tileset::Group>();
	water_water->SetPart(1, 32, 0);  // lewo-gora.
	water_water->SetPart(2, 48, 0);  // srodek-gora.
	water_water->SetPart(3, 16, 0);  // prawo-gora.
	water_water->SetPart(4, 40, 0);  // lewo-srodek.
	water_water->SetPart(5, 60, 0);  // srodek.
	water_water->SetPart(6, 20, 0);  // prawo-srodek.
	water_water->SetPart(7, 8, 0);  // lewo-dol.
	water_water->SetPart(8, 12, 0);  // srodek-dol.
	water_water->SetPart(9, 4, 0);   // prawo-dol.
	water_water->SetPart(11, 56, 0);  // wewnętrzny lewo-gora
	water_water->SetPart(13, 44, 0); // wewnętrzny lewo-dol
	water_water->SetPart(12, 52, 0); // wewnętrzny prawo-gora
	water_water->SetPart(14, 28, 0); // wewnętrzny prawo-dol
	groups.push_back(water_water);

	std::shared_ptr<Tileset::Group> water_sands = std::make_shared<Tileset::Group>();
	water_sands->SetPart(1, 32, 4);  // lewo-gora.
	water_sands->SetPart(2, 48, 4);  // srodek-gora.
	water_sands->SetPart(3, 16, 4);  // prawo-gora.
	water_sands->SetPart(4, 40, 4);  // lewo-srodek.
	water_sands->SetPart(5, 60, 4);  // srodek.
	water_sands->SetPart(6, 20, 4);  // prawo-srodek.
	water_sands->SetPart(7, 8, 4);  // lewo-dol.
	water_sands->SetPart(8, 12, 4);  // srodek-dol.
	water_sands->SetPart(9, 4, 4);   // prawo-dol.
	water_sands->SetPart(11, 56, 4);  // wewnętrzny lewo-gora
	water_sands->SetPart(13, 44, 4); // wewnętrzny lewo-dol
	water_sands->SetPart(12, 52, 4); // wewnętrzny prawo-gora
	water_sands->SetPart(14, 28, 4); // wewnętrzny prawo-dol
	groups.push_back(water_sands);

	std::shared_ptr<Tileset::Group> sands_grass = std::make_shared<Tileset::Group>();
	sands_grass->SetPart(1, 32, 8);  // lewo-gora.
	sands_grass->SetPart(2, 48, 8);  // srodek-gora.
	sands_grass->SetPart(3, 16, 8);  // prawo-gora.
	sands_grass->SetPart(4, 40, 8);  // lewo-srodek.
	sands_grass->SetPart(5, 60, 8);  // srodek.
	sands_grass->SetPart(6, 20, 8);  // prawo-srodek.
	sands_grass->SetPart(7, 8, 8);  // lewo-dol.
	sands_grass->SetPart(8, 12, 8);  // srodek-dol.
	sands_grass->SetPart(9, 4, 8);   // prawo-dol.
	sands_grass->SetPart(11, 56, 8);  // wewnętrzny lewo-gora
	sands_grass->SetPart(13, 44, 8); // wewnętrzny lewo-dol
	sands_grass->SetPart(12, 52, 8); // wewnętrzny prawo-gora
	sands_grass->SetPart(14, 28, 8); // wewnętrzny prawo-dol
	groups.push_back(sands_grass);

	std::shared_ptr<Tileset::Group> sands_swamp = std::make_shared<Tileset::Group>();
	sands_swamp->SetPart(1, 32, 12);  // lewo-gora.
	sands_swamp->SetPart(2, 48, 12);  // srodek-gora.
	sands_swamp->SetPart(3, 16, 12);  // prawo-gora.
	sands_swamp->SetPart(4, 40, 12);  // lewo-srodek.
	sands_swamp->SetPart(5, 60, 12);  // srodek.
	sands_swamp->SetPart(6, 20, 12);  // prawo-srodek.
	sands_swamp->SetPart(7, 8, 12);  // lewo-dol.
	sands_swamp->SetPart(8, 12, 12);  // srodek-dol.
	sands_swamp->SetPart(9, 4, 12);   // prawo-dol.
	sands_swamp->SetPart(11, 56, 12);  // wewnętrzny lewo-gora
	sands_swamp->SetPart(13, 44, 12); // wewnętrzny lewo-dol
	sands_swamp->SetPart(12, 52, 12); // wewnętrzny prawo-gora
	sands_swamp->SetPart(14, 28, 12); // wewnętrzny prawo-dol
	groups.push_back(sands_swamp);

	std::shared_ptr<Tileset::Group> grass_swamp = std::make_shared<Tileset::Group>();
	grass_swamp->SetPart(1, 32, 16);  // lewo-gora.
	grass_swamp->SetPart(2, 48, 16);  // srodek-gora.
	grass_swamp->SetPart(3, 16, 16);  // prawo-gora.
	grass_swamp->SetPart(4, 40, 16);  // lewo-srodek.
	grass_swamp->SetPart(5, 60, 16);  // srodek.
	grass_swamp->SetPart(6, 20, 16);  // prawo-srodek.
	grass_swamp->SetPart(7, 8, 16);  // lewo-dol.
	grass_swamp->SetPart(8, 12, 16);  // srodek-dol.
	grass_swamp->SetPart(9, 4, 16);   // prawo-dol.
	grass_swamp->SetPart(11, 56, 16);  // wewnętrzny lewo-gora
	grass_swamp->SetPart(13, 44, 16); // wewnętrzny lewo-dol
	grass_swamp->SetPart(12, 52, 16); // wewnętrzny prawo-gora
	grass_swamp->SetPart(14, 28, 16); // wewnętrzny prawo-dol
	groups.push_back(grass_swamp);

	water_sands->friends.push_back(sands_grass);
	water_sands->friends.push_back(sands_swamp);
	water_sands->friends.push_back(grass_swamp);

	sands_grass->friends.push_back(grass_swamp);
}

std::shared_ptr<Tileset> tileset;


bool areFriends(int group1, int group2) {
	if (group1 == -1 || group2 == -1) return true;
	if (group1 == group2) return true;
	for (auto& fr : tileset->groups[group1]->friends) {
		if (fr == tileset->groups[group2])
			return true;
	}
	return false;
}
