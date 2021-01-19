void MovePlayer(Tilemap* map ,playerStruct*Player)
{

    //Hareket etmedikce yavaslat
	if (!IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT) && !IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT) && (GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X) == 0)) {
            Player->accel /= 1.2f;
			//Player->accel = round(Player->accel);
			//Player->accel = round(Player->accel);
	}   
	//yere dogru maks hiz
	if (Player->yvel > grav){
        Player->yvel = grav;
    }


	//saga doru git
    if(IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT) || (GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X) > 0)){
        Player->accel += 0.2f;
    }
    //sola doru git
    if(IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT) || (GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X) < 0)){
        Player->accel -= 0.2f;
    }
    //saga gidiyosa saga don
    if (Player->accel > 0){
        Player->flipped = 0;
	}
	//sola gidiyosa sola don
    if (Player->accel < 0) {
       Player->flipped = 1;
	}
        
	//maks hizi
	if (Player->accel > Player->maxSpeed) 
		Player->accel = Player->maxSpeed;
	//maks hizi
	if (Player->accel < -Player->maxSpeed) 
		Player->accel = -Player->maxSpeed;
	//yere dusmemesi icin koruma xD
	if (Player->Position.y > screenHeight+150){
		Player->Position.y = screenHeight+150;
		Player->jump = true;
	}
       
	//yer cekimi
	Player->yvel += 0.3f;
	
	
	//ziplama
	if(IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT) && Player->jump){
		Player->yvel = -Player->jumppower;
		Player->jump = false;
		//Player->jetfuel -= Player->maxfuel / 5;
	}
	if (!IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT)) {
		Player->jump = true;
	}
	else Player->jump = false;
	//jetpack
	if(IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)){
		if (Player->yvel > -grav && Player->jetfuel > 0) {
			Player->yvel = Player->yvel - Player->power;
			Player->jetfuel -= 36;
		}
	}
	else {
		Player->jetfuel += 2;
	}
	if (Player->jetfuel < 0) {
		Player->jetfuel = 0;
	}
	if (Player->jetfuel > Player->maxfuel) {
		Player->jetfuel = Player->maxfuel;
	}
	
	//pozisyonu tilelara ayirmak
	Vector2 yuvarlatirmis = {Player->Position.x / 16, Player->Position.y / 16};
	int x = (int) yuvarlatirmis.x - 2;
	int y = (int) yuvarlatirmis.y - 2;
	
	//collision detectiondan once ne yapiyodu
	if (Player->accel < 0) {
		Player->acceldegisken = 0.2f;
	}
	else {
		Player->acceldegisken = -0.2f;
	}
	if (Player->yvel < 0) {
		Player->yveldegisken = 0.1f;
	}
	else {
		Player->yveldegisken = -0.1f;
	}
	//etrafinda 3 blok kontrol et
	
	
	touchedthistime[1] = false;
	touchedthistime[0] = false;
	Player->jump = false;
	while (x<yuvarlatirmis.x+3){
		while (y<yuvarlatirmis.y+3){
			//oyuncunun kolijin boksunu aliyo
			Rectangle playerbox = {Player->Position.x + Player->accel - 6 ,Player->Position.y + Player->yvel - 28,12,28};
			Rectangle dirtbox = {x*16,y*16,16,16};
			if (x < 0) x = 0;
			if (y < 0) y = 0;
			if (x > map->width) x = map->width;
			if (y > map->height) y = map->height;
			if (yuvarlatirmis.x + 3 > map->width) yuvarlatirmis.x--;
			if (yuvarlatirmis.y + 3 > map->height) yuvarlatirmis.y--;
			//collision algilama
			if (CheckCollisionRecs(playerbox, dirtbox)){
				if(GetTile(map, x, y) != 0){
					
					if (GetTile(map, x, y) == 33) touchedthistime[1] = 1;
					if (GetTile(map, x, y) == 23) touchedthistime[0] = 1;
					if (GetTile(map, x, y) == 33 || GetTile(map, x, y) == 23) {}
					else {
						
						while (true) {
							//hangi sekilde dokunuyo
							Player->touchingground = 1;
							Rectangle playerbox = { Player->Position.x + Player->accel - 6 ,Player->Position.y - 28,12,28 };
							if (!CheckCollisionRecs(playerbox, dirtbox)) {
								break;
							}
							Player->accel = Player->accel + Player->acceldegisken;
							
						}
						//y kolijini
						while (true) {
							//hangi sekilde dokunuyo
							Player->touchingground = 2;
							Rectangle playerbox = { Player->Position.x - 6 ,Player->Position.y + Player->yvel - 28,12,28 };
							if (!CheckCollisionRecs(playerbox, dirtbox)) {
								break;
							}
							Player->yvel = Player->yvel + Player->yveldegisken;
							if(Player->yveldegisken <0) Player->jump = true;
							//Player->jump = true;
						}
					}

				}	
				else {
					Player->touchingground = 0;
				}
			}	
			//sonraki tura gec
			y += 1;
		}
		//sonraki tura gec
		y = 0;
		x += 1;
	}

	if (touchedthistime[1] == 0 && levelbuttons[1] == 1) {
		x = 0;
		for (; x < map->width; x++) {
			y = 0;
			for (; y < map->height; y++) {
				if (GetTile(map, x, y) == 28) {
					map->tiles[(y * map->width) + x] = 29;
				}
			}
		}
		levelbuttons[1] = 0;
	}
	if (touchedthistime[1] == 1 && levelbuttons[1] == 0) {
		x = 0;
		for (; x < map->width; x++) {
			y = 0;
			for (; y < map->height; y++) {
				if (GetTile(map, x, y) == 29) {
					map->tiles[(y * map->width) + x] = 28;
				}
			}
		}
		levelbuttons[1] = 1;
	}
	if (touchedthistime[0] == 0 && levelbuttons[0] == 1) {
		x = 0;
		for (; x < map->width; x++) {
			y = 0;
			for (; y < map->height; y++) {
				if (GetTile(map, x, y) == 18) {
					map->tiles[(y * map->width) + x] = 19;
				}
			}
		}
		levelbuttons[0] = 0;
	}
	if (touchedthistime[0] == 1 && levelbuttons[0] == 0) {
		x = 0;
		for (; x < map->width; x++) {
			y = 0;
			for (; y < map->height; y++) {
				if (GetTile(map, x, y) == 19) {
					map->tiles[(y * map->width) + x] = 18;
				}
			}
		}
		levelbuttons[0] = 1;
	}

	//eninde sonunda haraket edecek yavrucagiz
	
	Player->Position.x += Player->accel;
	Player->Position.y += Player->yvel;
}