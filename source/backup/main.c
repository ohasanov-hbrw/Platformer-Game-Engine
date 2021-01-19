//u32 __ctru_linear_heap_size = 8 * 1024 * 1024;



#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <GL/picaGL.h>

#include <3ds/romfs.h>
#include <3ds/types.h>
#include <3ds/services/fs.h>


const int grav = 7;
const int screenWidth = 400;
const int screenHeight = 240;
int renderposx;
int xd;
int tileset[3] = { 0,0,1 };
int currentlevel = 0;
void osSetSpeedupEnable(bool enable);
int __stacksize__ = 2 * 1024 * 1024;
bool levelbuttons[2] = { 0,0 };
bool lastlevelbuttons[2] = { 0,0 };
bool touchedthistime[2];

float round(float var)
{
	// 37.66666 * 100 =3766.66 
	// 3766.66 + .5 =3767.16    for rounding off value 
	// then type cast to int so value is 3767 
	// then divided by 100 so the value converted into 37.67 
	float value = (int)(var * 100 + .5);
	return (float)value / 100;
}


#include "include/structs.h"
#include "include/tilemap.h"
#include "include/entity.h"
#include "include/player.h"





#define MAX_TOUCH_POINTS 1

int ButtonTouched(int x, int y, int x2, int y2, Vector2 touch, bool touched) {
	Rectangle buton = { x, y ,x2, y2};
	if (CheckCollisionPointRec(touch, buton) && touched) { 
		return 1; 
	}
	else {
		return 0;
	}
}

int main(void)
{
	//chdir("sdmc:/");
	
	//romfsMountSelf("romfs");
	int boxnumber = 2;
	romfsInit();
    playerStruct Player;
	boxStruct Box[50];
	for(int i = 0;i<boxnumber;i++){
		Box[i].accel = 0;
		Box[i].yvel = 0;
		Box[i].Position.x = 0;
		Box[i].Position.y = 0;
	}
	Box[1].Position.x = 268;
    Player.Position.x = 280.0;
    Player.Position.y = 100.0;
    Player.power = 0.7f;
    Player.maxSpeed = 4.5;
    Player.jumppower = 6;
	bool paused = false;
	bool startbutton = false;
	//Vector2 playerpos;
	osSetSpeedupEnable(true);
    //map--------------------------------------------------------------------------
    Player.jetpack = true;
    //int loadedlevel = 0;
    Player.flipped = 0;
	Player.jetfuel = 1500;
	Player.maxfuel = 1500;
    //map--------------------------------------------------------------------------
    Rectangle sourceRec = { 0.0f, 0.0f, 32, 32 };
    Vector2 origin = { 16, 32 };

	Vector2 LastPlayerPos = { (int)(Player.Position.x / 16) - 1,(int)(Player.Position.y / 16) - 1};

    
    InitWindow(screenWidth, screenHeight, "Platformer Test by OHASANOV");

    Image govde = LoadImage("romfs:/platformer_data/gfx/Govde1.png"); 
    ImageFormat(&govde, UNCOMPRESSED_R8G8B8A8);
    Texture2D Govde[2];
    Govde[0] = LoadTextureFromImage(govde);
    ImageFlipHorizontal(&govde);
    Govde[1] = LoadTextureFromImage(govde);

	UnloadImage(govde);

    Image sImage = LoadImage("romfs:/platformer_data/gfx/shipTileSet.png");
    Texture2D Tex = LoadTextureFromImage(sImage); 
	UnloadImage(sImage);

	Image Mouse = LoadImage("romfs:/platformer_data/gfx/mouse.png");
	Texture2D mouse = LoadTextureFromImage(Mouse);
	UnloadImage(Mouse);
	
	Image boxImg = LoadImage("romfs:/platformer_data/gfx/box.png");
	Texture2D box = LoadTextureFromImage(boxImg); 
	UnloadImage(boxImg);

	Image menuImg = LoadImage("romfs:/platformer_data/gfx/Menu.png");
	Texture2D menu = LoadTextureFromImage(menuImg);
	UnloadImage(menuImg);
	
	//Image background = LoadImage("3ds/platformer_data/gfx/SPACESHIP-BACKGROUND.png");
	//Texture2D back = LoadTextureFromImage(background); 
	
    Image kafa = LoadImage("romfs:/platformer_data/gfx/Kafa.png");

	

    ImageFormat(&kafa, UNCOMPRESSED_R8G8B8A8);
    Texture2D Kafa[2];
    Kafa[0] = LoadTextureFromImage(kafa);
    ImageFlipHorizontal(&kafa);
    Kafa[1] = LoadTextureFromImage(kafa);

	UnloadImage(kafa);

    Camera2D camera = { 0 };
    
    camera.offset = (Vector2){ screenWidth/2, screenHeight/2 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    
    SetTargetFPS(60); 

	Box[0].Position.x = 128;
	Box[0].Position.y = 32;

	int color;
	int redvalue;
	int y;
	int x;
	//---------------------map-loading---------------------
	int level = 2;

	Image levelImg = LoadImage(FormatText("romfs:/platformer_data/gfx/lvl%i.png", level));
	Tilemap map = LoadMapFromImage(levelImg);
	UnloadImage(levelImg);
	Vector2 size = { 2,2 };
	while (map.width > size.x) {
		size.x *= 2;
	}
	while (map.height > size.y) {
		size.y *= 2;
	}
	
	Image minimap = GenImageColor(size.x, size.y, BLANK);

	x = 0;
	for (; x < map.width; x++) {
		y = 0;
		for (; y < map.height; y++) {
			if (GetTile(&map, x, y) != 0) {
				ImageDrawPixel(&minimap, x, y, GRAY);
			}
		}
	}
	Texture2D minimaprender = LoadTextureFromImage(minimap);
	UnloadImage(minimap);
	//---------------------map-loading---------------------
	Vector2 touchPosition = {0,0};
	Vector2 lasttouchPosition = { 0,0 };
	Vector2 crosshair = { 0,0 };
	
	bool touch = false;

	bool mouseinuse = false;

	Rectangle miniplayer;
	Rectangle touchscreen = { 61,68,198,118 };
	bool ingame = false;
	bool maploaded = false;
	bool home;
	bool gun = false;
	//idk what the frick the code is doing from this point
	
	
    while (!WindowShouldClose()) 
    {
		if (ingame == true && maploaded) {

			touchPosition = GetTouchPosition(0);
			if (paused == false) {
				MovePlayer(&map, &Player);
				for(int i  = 0; i<boxnumber;i++) MoveBox(&map, &Box[i]);
				if ((touchPosition.x > 0) && (touchPosition.y > 0))   // Make sure point is not (-1,-1) as this means there is no touch for it
				{
					touch = true;
					lasttouchPosition = touchPosition;

					if (CheckCollisionPointRec(lasttouchPosition, touchscreen)) {
						crosshair.x = (touchPosition.x - 61) * 2;
						crosshair.y = (touchPosition.y - 68) * 2;
						mouseinuse = true;
					}
				}
				else {
					touch = false;
					mouseinuse = false;
					home = false;
				}

				Rectangle pausebutton = { 276,143,40,40 };
				if (CheckCollisionPointRec(lasttouchPosition, pausebutton) && touch && !mouseinuse) {
					paused = true;
				}
				Rectangle homebutton = { 4,143,40,40 };
				
				if (CheckCollisionPointRec(lasttouchPosition, homebutton) && touch && !mouseinuse && !home) {
					ingame = false;
					maploaded = true;
					home = true;
				}
			}

			if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_MIDDLE_RIGHT)) {
				if (!startbutton) {
					paused = !paused;
				}
				startbutton = true;

			}
			else {
				startbutton = false;
			}


			camera.target = (Vector2){ Player.Position.x, Player.Position.y - 50 };

			// Get the touch point

		}
		else {
			touchPosition = GetTouchPosition(0);
			
			if ((touchPosition.x > 0) && (touchPosition.y > 0))   // Make sure point is not (-1,-1) as this means there is no touch for it
			{
				touch = true;
				lasttouchPosition = touchPosition;
			}
			else {
				touch = false;
				home = false;
				gun = false;
			}
			
			if (ButtonTouched(4, 4, 40, 40, touchPosition, touch) && !home && maploaded) {
				maploaded = false;
				level = 1;
			}
			if (ButtonTouched(48, 4, 40, 40, touchPosition, touch) && !home && maploaded) {
				maploaded = false;
				level = 2;
			}
			if (ButtonTouched(92, 4, 40, 40, touchPosition, touch) && !home && maploaded) {
				maploaded = false;
				level = 3;
			}
			
			
			
			if (maploaded == false) {
				if (tileset[level - 1] == 0) {
					UnloadTexture(Tex);
					sImage = LoadImage("romfs:/platformer_data/gfx/shipTileSet.png");
					Tex = LoadTextureFromImage(sImage);
					UnloadImage(sImage);
				}
				if (tileset[level - 1] == 1) {
					UnloadTexture(Tex);
					sImage = LoadImage("romfs:/platformer_data/gfx/grassTileSet.png");
					Tex = LoadTextureFromImage(sImage);
					UnloadImage(sImage);
				}
				UnloadMap(map);
				Image levelImg = LoadImage(FormatText("romfs:/platformer_data/gfx/lvl%i.png", level));
				map = LoadMapFromImage(levelImg);
				UnloadImage(levelImg);
				Vector2 size = { 2,2 };
				while (map.width > size.x) {
					size.x *= 2;
				}
				while (map.height > size.y) {
					size.y *= 2;
				}

				minimap = GenImageColor(size.x, size.y, BLANK);

				x = 0;
				for (; x < map.width; x++) {
					y = 0;
					for (; y < map.height; y++) {
						if (GetTile(&map, x, y) != 0  ) {
							if (GetTile(&map, x, y) == 29 || GetTile(&map, x, y) == 19 || GetTile(&map, x, y) == 33 || GetTile(&map, x, y) == 23 || GetTile(&map, x, y) == 28 || GetTile(&map, x, y) == 18) {
								ImageDrawPixel(&minimap, x, y, BLUE); 
							}
							else {
								ImageDrawPixel(&minimap, x, y, GRAY);
							}
						}
					}
				}
				minimaprender = LoadTextureFromImage(minimap);
				UnloadImage(minimap);

				Player.Position.x = 280.0;
				Player.Position.y = 100.0;
				for(int i = 0;i<boxnumber;i++){
					Box[i].accel = 0;
					Box[i].yvel = 0;
					Box[i].Position.x = 0;
					Box[i].Position.y = 0;
				}
				Box[1].Position.x = 268;
				Player.power = 0.7f;
				Player.maxSpeed = 4.5;
				Player.jumppower = 6;
				bool paused = false;
				bool startbutton = false;
				//Vector2 playerpos;
				osSetSpeedupEnable(true);
				//map--------------------------------------------------------------------------
				Player.jetpack = true;
				//int loadedlevel = 0;
				Player.flipped = 0;
				Player.jetfuel = 1500;
				Player.maxfuel = 1500;
				//map--------------------------------------------------------------------------
				Box[0].Position.x = 128;
				Box[0].Position.y = 32;
				ingame = true;
				paused = false;
				maploaded = true;
				gun = false;
			}
			

		}

		Rectangle destRec = { Player.Position.x, Player.Position.y, 32, 32 };
		
		SetWindowMonitor(0);
        BeginDrawing();
            ClearBackground(SKYBLUE);
            
			BeginMode2D(camera);
                //cizme
				
				//DrawTexture(back, 0, 0 , WHITE);
				if (ingame) {
					y = Player.Position.y / 16 - 11;
					if (y < 0) y = 0;
					
					for (; y < map.height; y++)
					{
						x = Player.Position.x / 16 - 13;
						if (x < 0) x = 0;
	
						for (; x < map.width; x++)
						{
							if (GetTile(&map, x, y) != 0)
							{
								Vector2 drawpos = { x * 16, y * 16 };
								Rectangle drawrec = { (GetTile(&map, x, y) % 5) * 16, (GetTile(&map, x, y) / 5) * 16, 16, 16 };
								DrawTextureRec(Tex, drawrec, drawpos, WHITE);

							}
							if (x * 16 >= Player.Position.x + 218) break;
						}
						if (y * 16 >= Player.Position.y + 172) break;
					}
					//DrawTexture(terain, 0, 0, WHITE);
					//DrawTexture(buffer, 0 - ((int)Player.Position.x % 16), 0 - ((int)Player.Position.y % 16), WHITE);
					for(int i  = 0; i<boxnumber;i++) DrawTexture(box, Box[i].Position.x, Box[i].Position.y + 1, WHITE);
					//for (int i = 0; i < boxnumber; i++) DrawRectangle(Box[i].Position.x, Box[i].Position.y, 8, 8, RED);
					
					DrawTexturePro(Govde[Player.flipped], sourceRec, destRec, origin, (int)Player.accel, WHITE);
					DrawTexturePro(Kafa[Player.flipped], sourceRec, destRec, origin, (int)Player.accel, WHITE);
				}
				
                
            EndMode2D(); 
			if (ingame) {
				DrawTexture(mouse, crosshair.x, crosshair.y, WHITE);
				if (paused == true) {
					Rectangle fuel = { 0, 0, 400, 240 };
					DrawRectangleRec(fuel, (Color) { 0, 0, 0, 128 });
					DrawText("Paused", 10, 10, 20, RED);
				}
				
			}
			else {
				Rectangle fuel = { 0, 0, 400, 240 };
				DrawRectangleRec(fuel, (Color) { 0, 0, 0, 128 });
			}
			//------------------------------------------------------------
			pglSwapBuffers();
			SetWindowMonitor(1);
			//------------------------------------------------------------
			
			
			ClearBackground(SKYBLUE);
			if (ingame) {
				y = Player.Position.y / 16 - 21;

				int realy = y * 4;
				if (y < 0) y = 0;
				int realx = 0;
				Vector2 reee = { 0,0 };
				Rectangle sourceImg = { Player.Position.x / 4,Player.Position.y / 4 ,198,118 };
				Vector2 destImg = { (int)(-Player.Position.x / 2) + 141 + 120 - 20, (int)(-Player.Position.y / 2) + 68 + 74 + 14 };
				/*if (sourceImg.x < 0) {
					destImg.x -= sourceImg.x;
				}
				if (sourceImg.y < 0) {
					destImg.y -= sourceImg.y;
				}
				if (sourceImg.width > map.width * 4)sourceImg.width = map.width * 4;
				if (sourceImg.height > map.height * 4)sourceImg.height = map.height * 4;*/

				if (Player.Position.y < 0) destImg.y++;
				if (Player.Position.x < 0) destImg.x++;

				DrawTextureEx(minimaprender, destImg, 0, 8, WHITE);
				Rectangle miniplayer = { 238, 68 + 75, 7, 14 };


				DrawRectangleRec(miniplayer, GREEN);

				Rectangle miniplayer2 = { (int)(-Box[0].Position.x / 2) + 141 + 120 - 20,(int)(-Box[0].Position.y / 2) + 68 + 74 + 14 , 8, 8 };
				//DrawRectangleRec(miniplayer2, RED);

				DrawTexture(menu, 80, 0, WHITE);
				
				DrawText(FormatText("Accel [%f]:", (float)Player.accel), 90, 30, 20, RED);
				//DrawText(FormatText("Touch Y [%i]:", (int)crosshair.y), 90, 50, 20, RED);
				redvalue = GetTile(&map, destRec.x / 16, destRec.y / 16);
				//DrawText(FormatText("3ds/platformer_data/gfx/lvl%i.png", level), 80, 110, 20, RED);
				//DrawText(FormatText("Joy X [%f]:", GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X)), 90, 130, 20, RED);
				//DrawText(FormatText("Joy Y [%f]:", GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y)), 90, 150, 20, RED);
				Rectangle fuel = { 90, 210, Player.jetfuel / 5, 20 };
				color = (int)(Player.maxfuel / 250);
				DrawRectangleRec(fuel, (Color) { 255 - (int)(Player.jetfuel / color), (int)(Player.jetfuel / color), 0, 255 });
				DrawFPS(290, 10);

				if (paused == true) {
					Rectangle fuel = { 80, 0, 320, 240 };
					DrawRectangleRec(fuel, (Color) { 0, 0, 0, 192 });
					DrawText("Paused", 90, 10, 20, RED);
				}
			}
			else {
				Rectangle rektangil = { 84,4,40,40 };
				DrawRectangleRec(rektangil, BLACK);
				DrawText("1", 94, 14, 20, WHITE);
				rektangil.x = 128;
				DrawRectangleRec(rektangil, BLACK);
				DrawText("2", 138, 14, 20, WHITE);
				rektangil.x = 172;
				DrawRectangleRec(rektangil, BLACK);
				DrawText("3", 182, 14, 20, WHITE);
			}
			//DrawText(FormatText("Touch? [%i]", ButtonTouched(4, 4, 40, 40, touchPosition, touch)), 90, 10, 20, RED);
			//DrawText(FormatText("TouchedThisTime1 [%i]:", (int)touchedthistime[1]), 90, 30, 20, RED);
			//DrawText(FormatText("LevelButon1 [%i]:", (int)levelbuttons[1]), 90, 50, 20, RED);
			
        EndDrawing(); 
		}
    CloseWindow();

    UnloadTexture(Govde[0]);
    UnloadTexture(Govde[1]);
    UnloadTexture(Kafa[0]);
    UnloadTexture(menu);
	UnloadTexture(mouse);
	UnloadTexture(box);
	UnloadTexture(Tex);
	UnloadTexture(minimaprender);
	UnloadMap(map);
    return 0;
}

