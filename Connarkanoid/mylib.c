#include "mylib.h"

//Draws a simple object with specifications
void drawBall(int row, int col, int width, int height, unsigned short color) {
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			setPixel(row + j, col + i, color);
		}
	}
}

//Sets one pixel to a certain color
void setPixel(int row, int col, unsigned short color) {
	buffer[row * 240 + col] = color;
}

//Delays game
void delay(int n)
{
	volatile int x = 0;
	for(int i=0; i<n*10000; i++)
	{
		x++;
	}
}

//Draws an image at the given position using DMA
void drawImage3(int x, int y, int width, int height, const unsigned short *image) {
	for (int row = 0; row < height; row++) {
		DMA[DMA_CHANNEL_3].src = image + row * width;
		DMA[DMA_CHANNEL_3].dst = buffer + (y + row) * 240 + x;
		DMA[DMA_CHANNEL_3].cnt = width | DMA_ON;
	}
}

//Waits for vertical blank using the scanline counter
void WaitForVblank() {
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
}

//Clears the screen
void clearScreen() {
	u16 col = BLACK;
	DMA[DMA_CHANNEL_3].src = &col;
	DMA[DMA_CHANNEL_3].dst = buffer;
	DMA[DMA_CHANNEL_3].cnt = (SCREEN_WIDTH * SCREEN_HEIGHT) | DMA_SOURCE_FIXED | DMA_ON;
}

