#include "game.h"
#include "mylib.h"
#include <stdio.h>

//Checks to see if the brick contains a mini powerup
int hasMini(int mini) {
	return mini;
}

//Calculates the score after each brick hit
int calculateScore(int mini) {
	return 20 + (10 * mini);
}

//Draws a single character
void drawChar(int row, int col, char ch, unsigned short color) {
	for (int r = 0; r < 8; r++) {
		for (int c = 0; c < 6; c++) {
			if (fontdata_6x8[r * 6 + c + (ch * 48)]) {
				setPixel(row + r, col + c, color);
			}
		}
	}
}

//Draws the char* passed in onto the screen
void drawString(int row, int col, char *string, unsigned short color) {
	while (*string) {
		drawChar(row, col, *string++, color);
		col += 6;
	}
}

//Draws the digits of a number onto the screen
void drawDigits(int number, int row, int col, unsigned short color) {
	char digs[720];										//720 is the max score
	sprintf(digs, "%d", number);
	drawString(row, col, digs, color);
}