#include "mylib.h"
//#include "bar.h"
#include "arkBar.h"
#include "barBlack.h"
#include <stdio.h>
#include <stdlib.h>
#include "bkgnd.h"
#include "game.h"
#include "miniPowerUp.h"
#include "minibar.h"
#include "win.h"
#include "lose.h"
#include "title.h"


unsigned short *buffer = (unsigned short *) 0x6000000;

u16 colors[] = {GREEN,
				MAGENTA,
				BLUE,
				YELLOW,
				RED};

int main() {
	REG_DISPCTL = MODE_3 | BG2_EN;
	enum GAMESTATE state = START;
	//enum GAMESTATE state = ENDOFGAME;

	
	int origstart1 = 1;
	int origbar_r = 145;
	int origbar_c = 90;
	int origbar_xdir = 0;
	int origbar_ydir = 0;
	int origfirst = 1;
	int origtemp = 0;
	int barWidth = ARKBAR_WIDTH;
	int hasPowerUp = 0;

	int origballColDelta = 57;

	int origrow = 139;
	int origcol = origbar_c + origballColDelta;
	int origwidth = 6;
	int origheight = 6;
	int origx_dir = 1;
	int origy_dir = -1;
	u16 origcolor = YELLOW;

	BALL ball;
	ball.row = 139;
	ball.col = origbar_c + origballColDelta;
	ball.width = 6;
	ball.height = 6;
	ball.x_dir = 1;
	ball.y_dir = -2;
	ball.color = YELLOW;
	BALL *bp;

	int wasDrawn = 0;
	int numRekt = 0;

	//Sets orig values for bar information
	int start1 = origstart1;
	int bar_r = origbar_r;
	int bar_c = origbar_c;
	int bar_xdir = origbar_xdir;
	int bar_ydir = origbar_ydir;
	int first = origfirst;
	int temp = origtemp;
	int ballColDelta = origballColDelta;

	int num_of_bricks = NUM_OF_BRICKS;
	int brick_height = BRICK_HEIGHT;
	int brick_width = BRICK_WIDTH;
	BRICK bricks[num_of_bricks];

	int miniCol = 0;
	int miniRow = 0;
	int miniBoost = 0;

	int won = 0;

	int lives = 3;
	char *lp = (char *) LIVESLEFT;
	int score = 0;
	char *sp = (char *) SCORE;

	while (1) {
		switch(state) {
			case START:
				drawImage3(0, 0, TITLE_WIDTH, TITLE_HEIGHT, title);

				//Sets values to their initial values
				start1 = origstart1;
				bar_r = origbar_r;
				bar_c = origbar_c;
				bar_xdir = origbar_xdir;
				bar_ydir = origbar_ydir;
				first = origfirst;
				temp = origtemp;
				hasPowerUp = 0;

				ball.row = origrow;
				ball.col = origcol;
				ball.width = origwidth;
				ball.height = origheight;
				ball.x_dir = origx_dir;
				ball.y_dir = origy_dir;
				ball.color = origcolor;

				int k = 0;
				for (int i = 0; i < 4; i++) {							//initialize values for bricks
					for (int j = 0; j < 6; j++) {
						bricks[k].row = i * brick_height + 6 + 20;
						bricks[k].col = j * brick_width + 6;
						bricks[k].height = brick_height;
						bricks[k].width = brick_width;
						bricks[k].rekt = 0;
						bricks[k].color = colors[rand()%5];
						if (k == 15 || k == 20){
							bricks[k].mini = 1;
						}
						k++;
					}
				}

				miniCol = 0;
				miniRow = 0;

				lives = 3;
				score = 0;
				won = 0;
				numRekt = 0;

				if (KEY_DOWN_NOW(BUTTON_START)) {
					state = CLEAR;
				}
			
			break;

			case CLEAR:						//Clear screen and go back to start positions
				clearScreen();  

				bar_r = origbar_r;
				bar_c = origbar_c;
				bar_xdir = origbar_xdir;
				bar_ydir = origbar_ydir;
				barWidth = ARKBAR_WIDTH;
				wasDrawn = 0;
				hasPowerUp = 0;

				ball.row = 139;
				ball.col = bar_c + ballColDelta;
				ball.width = 6;
				ball.height = 6;
				ball.x_dir = 1;
				ball.y_dir = -1;
				ball.color = YELLOW;

				miniBoost = 0;
				
				state = BEFOREBEGINNING;
			break;

			//Sets up screen with background and score/lives display
			case BEFOREBEGINNING:
				drawImage3(0, 20, BKGND_WIDTH, BKGND_HEIGHT, bkgnd);
				if (lives == 0) {
					state = ENDOFGAME;
				}
				drawString(10, 10, lp, RED);
				drawDigits(lives, 10, 10 + 42, RED);
				drawString(10, 180, sp, RED);
				drawDigits(score, 10, 180 + 42, RED);
				state = BEGINNINGGAME;
			break;

			//Phase before the player presses a, and the ball begins to move. Player can move bar with ball on it freely
			case BEGINNINGGAME:
				drawBall(ball.row, ball.col, ball.width, ball.height, ball.color);
				drawImage3(bar_c, bar_r, ARKBAR_WIDTH, ARKBAR_HEIGHT, arkBar);

				for (int i = 0; i < 24; i++) {
					if (!bricks[i].rekt) {
						drawBall(bricks[i].row, bricks[i].col, bricks[i].width, bricks[i].height, bricks[i].color);
					}
				}

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					state = START;
				}

				int left = 0;
				int right = 0;
				int oldrow = 0;
				int oldcol = 0;
				int currow = 0;
				int curcol = 0;
				bp = &ball;
	
				if ((bar_c) <= 0 + 7) {  
						bar_c = 0 + 7;
				}
				if ((bar_c + barWidth) >= (SCREEN_WIDTH - 7)) {
						bar_c = (SCREEN_WIDTH - 7 - barWidth);
				}
					
				//Moves bar to the left
				if (KEY_DOWN_NOW(BUTTON_LEFT)) {
					bar_xdir = -1;
					bar_ydir = 0;
					bar_r += bar_ydir;
					bar_c += bar_xdir;
					ball.col = bar_c + ballColDelta;
					left = 1;
				}
				//Moves bar to the right
				if (KEY_DOWN_NOW(BUTTON_RIGHT)) {
					bar_xdir = 1;
					bar_ydir = 0;
					bar_r += bar_ydir;
					bar_c += bar_xdir;
					ball.col = bar_c + ballColDelta;
					right = 1;
				}
				drawImage3(bar_c, bar_r, ARKBAR_WIDTH, ARKBAR_HEIGHT, arkBar);
				drawBall(ball.row, ball.col, ball.width, ball.height, ball.color);
				WaitForVblank();
					
				if (left) {
					drawImage3(bar_c + 1, bar_r, BARBLACK_WIDTH, BARBLACK_HEIGHT, barBlack);
					drawBall(ball.row, ball.col + 1, ball.width, ball.height, BLACK);
				}
				else if (right) {
					drawImage3(bar_c - 1, bar_r, BARBLACK_WIDTH, BARBLACK_HEIGHT, barBlack);
					drawBall(ball.row, ball.col - 1, ball.width, ball.height, BLACK);
				}
				if (KEY_DOWN_NOW(BUTTON_A)) {
					start1 = 0;
					state = GAMEPLAY;
				}
			break;

			//Begins the gameplay phase
			case GAMEPLAY:
				first = 1;
				while (state == GAMEPLAY) {
					drawBall(10, 180 + 42, 240 - (180 + 42), 8, BLACK);      //draws black bar over previous score to allow score to update
					drawDigits(score, 10, 180 + 42, RED);
					bp = &ball;
					left = 0;
					right = 0;
					//temp = 0;
					if ((bar_c) <= 0 + 7) {  
						bar_c = 0 + 7;
					}
					if ((bar_c + barWidth) >= (SCREEN_WIDTH - 7)) {
						bar_c = (SCREEN_WIDTH - 7 - barWidth);
					}
					
					if (KEY_DOWN_NOW(BUTTON_SELECT)) {
						state = CLEAR;
					}

					if (KEY_DOWN_NOW(BUTTON_LEFT)) {
						bar_xdir = -1;
						bar_ydir = 0;
						bar_r += bar_ydir;
						bar_c += bar_xdir;
						left = 1;
					}
					if (KEY_DOWN_NOW(BUTTON_RIGHT)) {
						bar_xdir = 1;
						bar_ydir = 0;
						bar_r += bar_ydir;
						bar_c += bar_xdir;
						right = 1;
					}
					oldrow = bp->row;
					oldcol = bp->col;
					currow = bp->row;
					curcol = bp->col;
					
					//ceiling collision detection
					if (currow < 0 + 7 + 20) {
						bp->y_dir = -bp->y_dir;
					}
					//walls collision detection
					if (curcol < 0 + 7 || curcol >= ((SCREEN_WIDTH - ball.width) - 7)) {
						bp->x_dir = -bp->x_dir;
					}
					//top-of-bar collision detection
					if ((curcol + ball.width) >= (bar_c + 3) && curcol <= (bar_c + barWidth - 3 - 1) && !first) {
						if ((currow + ball.width) >= bar_r) {
							bp->y_dir = -bp->y_dir;
						}
					}
					//side-of-bar collision detection
					if ((currow + bp->height) > (bar_r) && !first) {
						if ((curcol + bp->width) == bar_c || curcol == (bar_c + barWidth - 1)) {
							bp->x_dir = -bp->x_dir;
						}
					}
					first = 0;

					//brick collision detection
					for (int i = 0; i < num_of_bricks; i++) {
						
						if ((currow == (bricks[i].row + bricks[i].height)) && ((curcol + ball.width) >= bricks[i].col) && 
							   ((curcol) <= (bricks[i].col + bricks[i].width)) && (!bricks[i].rekt)) {
							bp->y_dir = -bp->y_dir;
							drawBall(bricks[i].row, bricks[i].col, bricks[i].width, bricks[i].height, BLACK);
							bricks[i].rekt = 1;
							numRekt++;
							if (hasMini(bricks[i].mini) && !hasPowerUp) {
								miniBoost = 1;
								miniRow = bricks[i].row;
								miniCol = bricks[i].col;
								drawImage3(miniCol, miniRow, MINIPOWERUP_WIDTH, MINIPOWERUP_HEIGHT, miniPowerUp);
								temp = 1;
							}
							score += calculateScore(miniBoost);
							i = num_of_bricks;
						}
						else if (currow < (bricks[i].row + bricks[i].height) && (currow >= bricks[i].row) && 
							   ((curcol) <= (bricks[i].col + bricks[i].width)) && ((curcol + ball.width) >= bricks[i].col) &&
							   (!bricks[i].rekt)) {
							bp->x_dir = -bp->x_dir;
							drawBall(bricks[i].row, bricks[i].col, bricks[i].width, bricks[i].height, BLACK);
							bricks[i].rekt = 1;
							numRekt++;
							if (hasMini(bricks[i].mini) && !hasPowerUp) {
								miniBoost = 1;
								miniRow = bricks[i].row;
								miniCol = bricks[i].col;
								drawImage3(miniCol, miniRow, MINIPOWERUP_WIDTH, MINIPOWERUP_HEIGHT, miniPowerUp);
								temp = 1;
							}
							score += calculateScore(miniBoost);
							i = num_of_bricks;
						}
						else if ((curcol < (bricks[i].col + bricks[i].width)) && ((curcol + ball.width) > bricks[i].col) &&
								((currow + ball.height) == bricks[i].row) && !bricks[i].rekt) {
							bp->y_dir = -bp->y_dir;
							drawBall(bricks[i].row, bricks[i].col, bricks[i].width, bricks[i].height, BLACK);
							bricks[i].rekt = 1;
							numRekt++;
							if (hasMini(bricks[i].mini) && !hasPowerUp) {
								miniBoost = 1;
								miniRow = bricks[i].row;
								miniCol = bricks[i].col;
								drawImage3(miniCol, miniRow, MINIPOWERUP_WIDTH, MINIPOWERUP_HEIGHT, miniPowerUp);
								temp = 1;
							}
							score += calculateScore(hasMini(bricks[i].mini));
							score = score + calculateScore(hasMini(bricks[i].mini));
							i = num_of_bricks;
						}

						if (numRekt == num_of_bricks) {
							state = ENDOFGAME;
						}
					}
					//PowerUp collision detection
					if (temp && ((curcol < (miniCol + MINIPOWERUP_WIDTH)) && ((curcol + ball.width) > miniCol) && ((currow < (miniRow + MINIPOWERUP_HEIGHT)))
						&& (currow + ball.height) > miniRow) && !wasDrawn) {
						drawBall(miniRow, miniCol, MINIPOWERUP_WIDTH, MINIPOWERUP_HEIGHT, BLACK);
						hasPowerUp = 1;
						wasDrawn = 1;
						temp = 0;
					}
					
					//If ball is beyond bar, a life is lost
					if (currow >= (149 + ball.height)) {
						//change state
						lives--;
						if (lives == 0) {
							state = ENDOFGAME;
						}
						state = CLEAR;
					}
					bp->row = currow + bp->y_dir;
					bp->col = curcol + bp->x_dir;
					//If a power-up was hit, draw a minibar
					if (wasDrawn) {
						drawImage3(bar_c, bar_r, MINIBAR_WIDTH, MINIBAR_HEIGHT, minibar);
						barWidth = MINIBAR_WIDTH; 
					} else {
						drawImage3(bar_c, bar_r, ARKBAR_WIDTH, ARKBAR_HEIGHT, arkBar);
					}
					drawBall(ball.row, ball.col, ball.width, ball.height, ball.color);
					WaitForVblank();
					drawImage3(bar_c, bar_r, barWidth, BARBLACK_HEIGHT, barBlack);
					
					if (left) {
						drawImage3(bar_c + 1, bar_r, barWidth, BARBLACK_HEIGHT, barBlack);
					}
					else if (right) {
						drawImage3(bar_c - 1, bar_r, barWidth, BARBLACK_HEIGHT, barBlack);
					}
					drawBall(oldrow, oldcol, ball.width, ball.height, BLACK);
				}
				//If no more lives, game over
				if (lives == 0) {
					clearScreen();
					state = ENDOFGAME;
				}
				//If all bricks have been broken, win game
				if (numRekt == num_of_bricks) {
					won = 1;
					clearScreen();
					state = ENDOFGAME;
				}
			break;

			case ENDOFGAME:
				WaitForVblank();
				if (won) {
					drawImage3(0, 0, WIN_WIDTH, WIN_HEIGHT, win);
					//drawDigits(score, 76, 135, GREEN);
					drawDigits(score, 100, 120, BLACK);
				} else {
					drawImage3(0, 0, LOSE_WIDTH, LOSE_HEIGHT, lose);
				}
				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					state = START;
				}
			break; 

			}
		}
	}



