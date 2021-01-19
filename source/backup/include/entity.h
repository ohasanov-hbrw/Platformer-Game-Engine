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




void MoveEntity(Tilemap* map, EntityStruct* entity) {
	entity->yvel += 0.3f;
	if (entity->yvel > 7) {
		entity->yvel = 7;
	}
	if (entity->Position.y > screenHeight + 150) {
		entity->Position.y = screenHeight + 150;
	}
	Vector2 yuvarlatirmis = { entity->Position.x / 16, entity->Position.y / 16 };
	int x = yuvarlatirmis.x - 2;
	int y = yuvarlatirmis.y - 2;
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

	entity->Position.x += entity->xvel;
	entity->Position.y += entity->yvel;
}