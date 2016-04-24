//#include "mylib.h"

typedef struct {
	int row;
	int col;
	int width;
	int height;
	int rekt;
	unsigned short color;
	int mini;
} BRICK;

typedef struct {
	int row;
	int col;
	int width;
	int height;
	int x_dir;
	int y_dir;
	unsigned short color;
} BALL;

typedef struct {
	int row;
	int col;
	int width;
	int height;
	int on;
} POWERUP;

enum GAMESTATE {
	START,
	GAMEPLAY,
	BEGINNINGGAME,
	CLEAR,
	BEFOREBEGINNING,
	ENDOFGAME
};

#define SCORE "Score: "
#define LIVESLEFT "Lives: "

extern const unsigned char fontdata_6x8[12288];

int hasMini(int mini);
int calculateScore(int mini);
void drawChar(int row, int col, char ch, unsigned short color);
void drawString(int row, int col, char *str, unsigned short color);
void drawDigits(int number, int row, int col, unsigned short color);