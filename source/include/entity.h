void MoveBox(Tilemap* map, boxStruct*Box){
	//Hareket etmedikce yavaslat
	Box->yvel += 0.3f;
	//Box->accel /= 1.2f;
	
	//yere dogru maks hiz
	if (Box->yvel > 7) {
		Box->yvel = 7;
	}

	if (Box->Position.y > screenHeight + 150) {
		Box->Position.y = screenHeight + 150;
		
	}

	//yer cekimi
	
	//pozisyonu tilelara ayirmak
	Vector2 yuvarlatirmis = { Box->Position.x / 16, Box->Position.y / 16 };
	int x = yuvarlatirmis.x - 2;
	int y = yuvarlatirmis.y - 2;

	//collision detectiondan once ne yapiyodu
	if (Box->accel < 0) {
		Box->acceldegisken = 0.5f;
	}
	else {
		Box->acceldegisken = -0.5f;
	}
	if (Box->yvel < 0) {
		Box->yveldegisken = 0.3f;
	}
	else {
		Box->yveldegisken = -0.3f;
	}
	//etrafinda 3 blok kontrol et
	while (x < yuvarlatirmis.x + 3) {
		while (y < yuvarlatirmis.y + 3) {
			//oyuncunun kolijin boksunu aliyo
			Rectangle playerbox = { Box->Position.x + Box->accel,Box->Position.y + Box->yvel,16,16 };
			Rectangle dirtbox = { x * 16,y * 16,16,16 };
			
			if (x < 0) x = 0;
			if (y < 0) y = 0;
			if (x > map->width) x = map->width;
			if (y > map->height) y = map->height;
			if (yuvarlatirmis.x + 3 > map->width) yuvarlatirmis.x--;
			if (yuvarlatirmis.y + 3 > map->height) yuvarlatirmis.y--;
			//collision algilama
			if (CheckCollisionRecs(playerbox, dirtbox)) {
				if (GetTile(map, x, y) != 0) {
					//x kolijini
					while (true) {
						//hangi sekilde dokunuyo
						
						Rectangle playerbox = { Box->Position.x + Box->accel,Box->Position.y,16,16 };
						if (!CheckCollisionRecs(playerbox, dirtbox)) {
							break;
						}
						Box->accel = Box->accel + Box->acceldegisken;
					}
					//y kolijini
					while (true) {

						Rectangle playerbox = { Box->Position.x ,Box->Position.y + Box->yvel,16,16 };
						if (!CheckCollisionRecs(playerbox, dirtbox)) {
							break;
						}
						Box->yvel = Box->yvel + Box->yveldegisken;
						
					}
				}
			}
			y += 1;
		}
		y = 0;
		x += 1;
	}
	Box->Position.x += Box->accel;
	Box->Position.y += Box->yvel;

}




void MoveEntity(Tilemap* map, entityStruct* entity, int number, entityStruct* all, int id, bool grav, int degen, playerStruct* player) {

	if (grav) entity->xvel /= 1.15f;
	if (grav) entity->yvel += 0.3f;
	if (entity->yvel > 7) {
		entity->yvel = 7;
	}
	if (entity->xvel > 7) entity->xvel = 7;
	if (entity->xvel < -7) entity->xvel = -7;
	if (entity->yvel > 7) entity->yvel = 7;
	if (entity->yvel < -7) entity->yvel = -7;

	if (entity->Position.y > screenHeight + 150) {
		entity->Position.y = screenHeight + 150;
	}

	Rectangle entitybox = { entity->Position.x + entity->xvel ,entity->Position.y + entity->yvel,entity->width,entity->height };
	Rectangle allbox;
	if (degen != -2) allbox = (Rectangle){ all[degen].Position.x,all[degen].Position.y,all[degen].width,all[degen].height };
	else allbox = (Rectangle){ player->Position.x - 6,player->Position.y - 28,12,28 };
	
		if (degen == -2) {
			Rectangle yes = GetCollisionRec(allbox, entitybox);
			int degisim = yes.width;
			if (player->accel > 0)
				entity->xvel = degisim;
			if (player->accel < 0)
				entity->xvel = -degisim;
		}
		else if (degen != -1) {
			Rectangle yes = GetCollisionRec(allbox, entitybox);
			int degisim = yes.width;
			if (all[degen].xvel > 0)
				entity->xvel = degisim;
			if (all[degen].xvel < 0)
				entity->xvel = -degisim;
		}
		if (degen == -2) {
			Rectangle yes = GetCollisionRec(allbox, entitybox);
			int degisim = yes.height;
			if (player->yvel > 0)
				entity->yvel = degisim;
			if (player->yvel < 0)
				entity->yvel = -degisim;
		}
		else if (degen != -1) {
			Rectangle yes = GetCollisionRec(allbox, entitybox);
			int degisim = yes.height;
			if (all[degen].yvel > 0)
				entity->yvel = degisim;
			if (all[degen].xvel < 0)
				entity->yvel = -degisim;
		}
	

	for (int i = 0; i < number; i++) {
		if (i != id && i != degen) {
			entitybox = (Rectangle){ entity->Position.x + entity->xvel ,entity->Position.y + entity->yvel,entity->width,entity->height };
			if(degen != -2) allbox = (Rectangle){ all[i].Position.x,all[i].Position.y,all[i].width,all[i].height };
			else allbox = (Rectangle){ player->Position.x-6,player->Position.y-28,12,28};
			if (CheckCollisionRecs(entitybox, allbox)) {

				bool checky = true;
				bool checkx = true;

				MoveEntity(map, &all[i], number, all, i, false, id, &player);

				if (entity->xvel < 0) {
					entity->xveldegisken = 0.5f;
				}
				else if (entity->xvel >= 0) {
					entity->xveldegisken = -0.5f;
				}
				else checkx = false;
				if (entity->yvel < 0) {
					entity->yveldegisken = 0.3f;
				}
				else if (entity->yvel >= 0) {
					entity->yveldegisken = -0.3f;
				}
				else checky = false;

				
				//y kolijini
				while (true) {
					if (!checky) break;
					Rectangle playerbox = { entity->Position.x ,entity->Position.y + entity->yvel,entity->width,entity->height };
					if (!CheckCollisionRecs(playerbox, allbox)) {
						break;
					}
					entity->yvel = entity->yvel + entity->yveldegisken;

				}
				while (true) {
					//hangi sekilde dokunuyo
					if (!checkx) break;
					Rectangle playerbox = { entity->Position.x + entity->xvel,entity->Position.y,entity->width,entity->height };
					if (!CheckCollisionRecs(playerbox, allbox)) {
						break;
					}
					entity->xvel = entity->xvel + entity->xveldegisken;
				}
			}
		}
	}

	/*entity->Position.x += entity->xvel;
	entity->Position.y += entity->yvel;*/

	Vector2 yuvarlatirmis = { entity->Position.x / 16, entity->Position.y / 16 };
	int x = yuvarlatirmis.x - 2;
	int y = yuvarlatirmis.y - 2;

	if (entity->xvel > 7) entity->xvel = 7;
	if (entity->xvel < -7) entity->xvel = -7;
	if (entity->yvel > 7) entity->yvel = 7;
	if (entity->yvel < -7) entity->yvel = -7;


	if (entity->xvel < 0) {
		entity->xveldegisken = 0.5f;
	}
	else {
		entity->xveldegisken = -0.5f;
	}
	if (entity->yvel < 0) {
		entity->yveldegisken = 0.3f;
	}
	else {
		entity->yveldegisken = -0.3f;
	}

	

	while (x < yuvarlatirmis.x + 3) {
		while (y < yuvarlatirmis.y + 3) {
			//oyuncunun kolijin boksunu aliyo
			Rectangle playerbox = { entity->Position.x + entity->xvel,entity->Position.y + entity->yvel,entity->width,entity->height };
			Rectangle dirtbox = { x * 16,y * 16,16,16 };

			if (x < 0) x = 0;
			if (y < 0) y = 0;
			if (x > map->width) x = map->width;
			if (y > map->height) y = map->height;
			if (yuvarlatirmis.x + 3 > map->width) yuvarlatirmis.x--;
			if (yuvarlatirmis.y + 3 > map->height) yuvarlatirmis.y--;
			//collision algilama
			if (CheckCollisionRecs(playerbox, dirtbox)) {
				if (GetTile(map, x, y) != 0) {
					//x kolijini
					while (true) {
						//hangi sekilde dokunuyo

						Rectangle playerbox = { entity->Position.x + entity->xvel,entity->Position.y,entity->width,entity->height };
						if (!CheckCollisionRecs(playerbox, dirtbox)) {
							break;
						}
						entity->xvel = entity->xvel + entity->xveldegisken;
					}
					//y kolijini
					while (true) {

						Rectangle playerbox = { entity->Position.x ,entity->Position.y + entity->yvel,entity->width,entity->height };
						if (!CheckCollisionRecs(playerbox, dirtbox)) {
							break;
						}
						entity->yvel = entity->yvel + entity->yveldegisken;

					}
				}
			}
			y += 1;
		}
		y = 0;
		x += 1;
	}

	if (entity->xvel > 7) entity->xvel = 7;
	if (entity->xvel < -7) entity->xvel = -7;
	if (entity->yvel > 7) entity->yvel = 7;
	if (entity->yvel < -7) entity->yvel = -7;

	//DrawTexture(box, Box[i].Position.x, Box[i].Position.y + 1, WHITE);
	entity->Position.x += entity->xvel;
	entity->Position.y += entity->yvel;
}

