typedef struct PlayerStruct
{
	float accel, yvel, power;
	bool jump;
	int flipped;
	int maxSpeed, jumppower;
	Rectangle collisionbox;
	Vector2 Position;
	int whereineedtogoishell;
	int touchingground;
	int renderposx;
	int renderposy;
	bool kafacolliding;
	Rectangle collisionrectangle;
	bool jetpack;
	Vector2 placeholderpos;
	float acceldegisken;
	float yveldegisken;

	int jetfuel;
	int maxfuel;

} playerStruct;

typedef struct BoxStruct
{
	float xpos;
	float ypos;
	float yvel;
	float accel;
	float acceldegisken;
	float yveldegisken;
	Vector2 Position;

} boxStruct;

typedef struct s_tilemap {
	uint32_t width, height;
	uint32_t* tiles;
} Tilemap;


typedef struct EntityStruct
{
	Vector2 Position;
	float yvel;
	float xvel;
	int width;
	int height;
	float xveldegisken;
	float yveldegisken;

} EntityStruct;