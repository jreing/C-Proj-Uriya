//TODO need to free widget
//TODO need to free gameoptions!

#include <stdio.h>
#include "SDL/SDL.h"
#include "SDL/SDL_video.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "TreeUtils.h"
#include "logic.h"

#define WIN_W 1000
#define WIN_H 1000
#define IMG_W 240
#define IMG_H 296

#define CELL_HEIGHT 78
#define CELL_WIDTH 88

#define BUTTON_WIDTH 268
#define BUTTON_HEIGHT 74

#define GRID_WIDTH 830
#define GRID_HEIGHT 663
#define BOARD_BUTTON_WIDTH 160
#define BOARD_BUTTON_HEIGHT 48
//#define STATUS_BAR_BUTTON_WIDTH 482
//#define STATUS_BAR_BUTTON_HEIGHT 79
#define TOP_PANEL_HEIGHT 115
#define SIDE_PANEL_WIDTH 207

#define SELECTED BUTTON_WIDTH*2+5
#define SELECTED_WORLD -BUTTON_WIDTH-5

#define MOUSE 0
#define CAT 1
#define MID_GAME 10

#define NEW_GAME 1
#define LOAD_GAME 2
#define CREATE_GAME 3
#define EDIT_GAME 4
#define QUIT 5
#define HUMAN 6
#define MACHINE 7
#define BACK 8
#define LEVEL_UP 9
#define LEVEL_DOWN 10
#define DONE 11
#define MOVE 12
#define PAUSE 13
#define UNPAUSE 14
#define SAVE_GAME 15
#define RESTART_GAME 16
#define RECONF_CAT 17
#define RECONF_MOUSE 18

#define NEW_GAME_LOCATION 200 ... 200+BUTTON_HEIGHT
#define LOAD_GAME_LOCATION 10*1+200+1*BUTTON_HEIGHT ...10*1+200+2*BUTTON_HEIGHT
#define CREATE_GAME_LOCATION 10*2+200+2*BUTTON_HEIGHT ... 10*2+200+3*BUTTON_HEIGHT
#define EDIT_GAME_LOCATION 10*3+200+3*BUTTON_HEIGHT ... 10*3+200+4*BUTTON_HEIGHT
#define QUIT_LOCATION 10*4+200+4*BUTTON_HEIGHT ... 10*4+200+5*BUTTON_HEIGHT
#define PAUSE_LOCATION_Y 10 + 3 * (10 + STATUS_CAPTION_HEIGHT)
#define PAUSE_LOCATION_X 300
#define HUMAN_LOCATION NEW_GAME_LOCATION
#define MACHINE_LOCATION LOAD_GAME_LOCATION
#define U_SELECT_BACK_LOCATION CREATE_GAME_LOCATION
#define SKILL_LOCATION NEW_GAME_LOCATION
#define S_SELECT_BACK_LOCATION LOAD_GAME_LOCATION
#define DONE_LOCATION CREATE_GAME_LOCATION
#define UP_ARROW 200 ... 237
#define DOWN_ARROW 239 ... 280
#define CAPTION_LOCATION_X 324
#define LOAD_GAME_CAPTION_LOCATION_Y 0
#define SKILL_CAT_CAPTION_LOCATION_Y 2*CAPTION_HEIGHT
#define SKILL_MOUSE_CAPTION_LOCATION_Y 3*CAPTION_HEIGHT-2
#define SELECT_MOUSE_CAPTION_LOCATION_Y 6*CAPTION_HEIGHT-10
#define SELECT_CAT_CAPTION_LOCATION_Y 5*CAPTION_HEIGHT-5
#define RECONF_MOUSE_LOCATION 130 ... 130 + BOARD_BUTTON_HEIGHT
#define RECONF_CAT_LOCATION 130 + BOARD_BUTTON_HEIGHT + 50 ... 130 + 2*BOARD_BUTTON_HEIGHT + 50
#define RESTART_GAME_LOCATION 130 + 2*BOARD_BUTTON_HEIGHT + 100 ... 130 + 3*BOARD_BUTTON_HEIGHT + 100
#define GOTO_MAIN_MENU_LOCATION 130 + 3*BOARD_BUTTON_HEIGHT + 150 ... 130 + 4*BOARD_BUTTON_HEIGHT + 150
#define QUIT_PROG_LOCATION 130 + 4*BOARD_BUTTON_HEIGHT + 200 ... 130 + 5*BOARD_BUTTON_HEIGHT + 200
#define CAPTION_HEIGHT 30
#define CAPTION_WIDTH 480
#define MOUSE_LOCATION 1627
#define CAT_LOCATION 1540
#define STATUS_CAPTIONS_X 1082
#define SPACE_BUTTON_HEIGHT 34
#define SPACE_BUTTON_WIDTH 245
#define GAME_OVER_MOUSE_Y 208
#define GAME_OVER_CAT_Y 264
#define GAME_OVER_TIMEOUT_Y 313
#define BIG_MSG_WIDTH 700
#define BIG_MSG_HEIGHT 45
#define WORLD_SELECT_X 1590
#define WORLD_SELECT_UNMARK_X 1366
#define PAUSED_SIDE_PANEL_LOCATION 1155
#define MOUSE_MOVE_NUM_HEIGHT 106
#define STATUS_CAPTION_WIDTH 300
#define STATUS_CAPTION_HEIGHT 17
#define DIGIT_X 1092
#define MOUSE_1ST_DIGIT_X 137
#define DIGIT_WIDTH 10
#define CAT_1ST_DIGIT_X 107
#define ZERO_Y 360
#define PINK_CELL_X 1467
#define PINK_CELL_Y 91

#define WindowInitMacro	 	if (SDL_Init(SDL_INIT_VIDEO) < 0) {\
								printf("ERROR: unable to init SDL: %s\n", SDL_GetError());\
								return 1;\
							} \
							atexit(SDL_Quit);

#define PauseMacro if (!gameOver) { \
						if (pause)\
							action = UNPAUSE;\
						else\
							action = PAUSE;\
					}

struct Widget {
	int srcX, srcY, width, height;
	int x, y;
	int isSelected;
	char* img_filename;
	char* caption;
};

typedef struct Widget Widget;

typedef Widget* WidgetRef;

gameOptions game;

WidgetRef mouse, cat; //Widgets for quick board updating

int humanSelect(int player);
int openGameWindow(int gameNum);
int skillSelect(int player);
int worldSelect(int isSave);
SDL_Surface * display;

WidgetRef getChildWidget(TreeRef root, int childNum) {
	int i = 0;
	ListRef cur = getChildren(root);
	for (i = 1; i < childNum; i++) {
		if (isEmpty(cur)) {
			return NULL;
		} else {
			cur = tail(cur);
		}
	}
	TreeRef temp = (TreeRef) (headData(cur));
	void** p = rootData(temp);
	return (WidgetRef) p;
}

int printWidget(void* data) {
	WidgetRef temp = (WidgetRef) data;
	printf(
			"x=%d,  y=%d, srcX=%d, srcY=%d, width=%d,  height=%d, isSelected=%d, filename equals= %s\n",
			temp->x, temp->y, temp->srcX, temp->srcY, temp->width, temp->height,
			temp->isSelected, temp->img_filename);
	//printf("isSelected equals= %d\n",((WidgetRef)rootData(temp))->isSelected);
	//printf("filename equals= %p\n",((WidgetRef)rootData(temp))->img_filename);
	return 1;
}

int displayWidget(void* data) {
	WidgetRef widget = (WidgetRef) data;
	SDL_Rect imgrect = {
			widget->isSelected ? widget->srcX + SELECTED : widget->srcX,
			widget->srcY, widget->width, widget->height };
	SDL_Rect dstrect = { widget->x, widget->y, widget->width, widget->height };

	SDL_Surface *img = SDL_LoadBMP(widget->img_filename);
	if (widget->img_filename == NULL) {
		printf("ERROR: failed to load image: %s\n", SDL_GetError());
		if (SDL_FillRect(display, &dstrect,
				SDL_MapRGB(display->format, 255, 255, 255)) != 0) {
			printf("ERROR: failed to draw rect: %s\n", SDL_GetError());
			SDL_FreeSurface(img);
			exit(0);
		}
	} else {

		puts(widget->img_filename);
		// Apply the image to the display
		if (SDL_BlitSurface(img, &imgrect, display, &dstrect) != 0) {
			SDL_FreeSurface(img);
			printf("ERROR: failed to blit image: %s\n", SDL_GetError());
			return 0;
		}
	}

	// Create window surface

	if (SDL_Flip(display) != 0) {
		printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
		SDL_FreeSurface(img);
		return 0;
	}
	SDL_FreeSurface(img);
	return 1;
}

Widget* createWidget(int x, int y, int srcX, int srcY, int width, int height,
		int isSelected, char* filename, char* caption) {
//printf ("%s\n", filename);
	Widget* temp = malloc(sizeof(Widget));
	temp->srcX = srcX;
	temp->srcY = srcY;
	temp->x = x;
	temp->y = y;
	temp->width = width;
	temp->height = height;
	temp->isSelected = isSelected;
	temp->img_filename = filename;
	temp->caption = caption;
//printf ("%s\n", temp->img_filename);
	printf("new widget address: %d\n", (int) temp);
	return temp;
}

int openMainWindow() {
	int i, action = 0, gameNum = 1, game = { -1, -1, -1, -1 };

	// Initialize SDL and make sure it quits
	/*if (SDL_Init(SDL_INIT_VIDEO) < 0) {
	 printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
	 return 1;
	 }
	 atexit(SDL_Quit);
	 display = SDL_SetVideoMode(500, 700, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
	 */
	WindowInitMacro;
	display = SDL_SetVideoMode(500, 700, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
	TreeRef t = newTree(createWidget(0, 0, 0, 0, 600, 7000, 0, NULL, "hgh"));
	i = 0;
	insertChild(t,
			createWidget(100, i * 10 + 200 + i * BUTTON_HEIGHT, 0,
					i * BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT, 1,
					"Buttons.bmp", ""));
	for (i = 1; i < 5; i++) {
		insertChild(t,
				createWidget(100, i * 10 + 200 + i * BUTTON_HEIGHT, 0,
						i * BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT, 0,
						"Buttons.bmp", ""));
	}

	nonRecDFS(t, displayWidget);
	//display caption
	displayWidget(createWidget(150, 100,
	CAPTION_LOCATION_X, CAPTION_HEIGHT * 4 - 3,
	CAPTION_WIDTH, CAPTION_HEIGHT, 0, "Buttons2.bmp", ""));

	// Poll for keyboard & mouse events
	SDL_Event e;
	WidgetRef cur;
	while (action != QUIT) {

		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
			case (SDL_QUIT):
				action = QUIT;
				break;

			case (SDL_KEYUP):
				switch (e.key.keysym.sym) {
				case SDLK_ESCAPE:
					action = QUIT;
					break;
				case SDLK_RETURN:
					puts("enter");
					i = 1;
					cur = getChildWidget(t, i);
					while (cur != NULL) {
						if (cur->isSelected == 1) {
							printWidget(cur);
							action = i;
							printf("%d", action);
							break;
						}
						i++;
						cur = getChildWidget(t, i);
					} //emd while*/
					break;
				case SDLK_TAB:
					printf("tab\n");
					i = 1;
					cur = getChildWidget(t, i);
					while (cur != NULL) {
						printWidget(cur);
						if (cur->isSelected == 1) {
							puts("1");
							cur->isSelected = 0;
							displayWidget(cur);
							i++;
							cur = getChildWidget(t, i);
							if (cur == NULL) {
								//if list is over, start from beginning
								i = 1;
								cur = getChildWidget(t, i);
								puts("over");
							}
							cur->isSelected = 1;
							displayWidget(cur);
							break;
						}
						i++;
						cur = getChildWidget(t, i);
					}
					break;
				default:
					break;
				} //end keyup switch
				break;
			case (SDL_MOUSEBUTTONUP):
				printf("%d", e.button.y);
				if ((e.button.x > 100) && (e.button.x < 100 + BUTTON_WIDTH))
					switch (e.button.y) {
					case NEW_GAME_LOCATION:
						action = NEW_GAME;
						break;
					case LOAD_GAME_LOCATION:
						action = LOAD_GAME;
						break;
					case CREATE_GAME_LOCATION:
						action = CREATE_GAME;
						break;
					case EDIT_GAME_LOCATION:
						puts("edit game");
						break;
					case QUIT_LOCATION:
						puts("quit");
						action = QUIT;
						break;
					default:
						break;
					} //end mouse location switch
				break; //event type
			} //end switch

			switch (action) {
			case NEW_GAME:
			case LOAD_GAME:
				puts("new game");
				SDL_Quit();
				if (action == LOAD_GAME) {
					gameNum = worldSelect(LOAD_GAME);
				} else {
					gameNum = 1;
				}
				if (humanSelect(CAT) == 1) {
					puts("finished game gathering");
					openGameWindow(gameNum);
					puts("back from game");
				}
				WindowInitMacro
				display = SDL_SetVideoMode(500, 700, 0,
				SDL_HWSURFACE | SDL_DOUBLEBUF);
				nonRecDFS(t, displayWidget);
				//display caption
				displayWidget(createWidget(150, 100,
				CAPTION_LOCATION_X, CAPTION_HEIGHT * 4 - 3,
				CAPTION_WIDTH, CAPTION_HEIGHT, 0, "Buttons2.bmp", ""));

				action = 0;
				break;

			case CREATE_GAME:
				puts("create game");
				action = 0;
				break;
			case QUIT:
				SDL_Quit();
				return 1;
				break;
			default:
				break;
			} //end switch
		} //end poll event
	}  //end while
//SDL_Delay(4000);

	return 1;
}

WidgetRef newWidget(int x, int y, int width, int height, int isSelected,
		char* img_filename, char* caption) {
	WidgetRef new;
	new = malloc(sizeof(struct Widget));
	new->x = x;
	new->y = y;
	new->width = width;
	new->height = height;
	new->isSelected = isSelected;
	new->img_filename = img_filename;
	new->caption = caption;

	return new;
}

int main() {
	openEditorWindow(1);
}

int main23(int argc, char* args[]) {

	if (argc > 1) {
		if (!strcmp(args[1], "-console")) {
			consoleMode();
		}
	}
	//skillSelect(1);
	/*TreeRef t1 = newTree(createWidget(1, 2, 3, 4, 5, 6, 1, "a", "b"));
	 insertChild(t1, createWidget(7, 8, 9, 10, 11, 12, 0, "c", "d"));
	 insertChild(t1, newWidget(5, 5, 5, 5, 0, "f", "f"));
	 TreeRef Two = (TreeRef) (headData(getChildren(t1)));
	 // void**  p=rootData(Two);
	 //printf("x of child is %d ", (getChild(t1,1))->x);
	 //printWidget((WidgetRef)(getChild(t1,1)));
	 //DFSPrint(t1,printWidget());
	 //DFSTwo (t1, printWidget);
	 */

	//humanSelect(CAT);
	int quit = 0;
	while (!quit) {
		quit = openMainWindow();
	}

	//openGameWindow(game);
	//worldSelect(0);
	//humanSelect(0);
	return 1;
}

TreeRef boardToTree(char** board) {
	TreeRef temp = newTree(
			createWidget(0, 0, 0, 0, 862, 662, 0, "Board.bmp", "hgh"));
	int i, j;
	for (i = 0; i < 7; i++) {
		for (j = 0; j < 7; j++) {
			switch (board[j][i]) {
			case 'M':
				mouse = createWidget(SIDE_PANEL_WIDTH + i * CELL_WIDTH,
				TOP_PANEL_HEIGHT + j * CELL_HEIGHT, 1627, 0,
				CELL_WIDTH, CELL_HEIGHT, 0, "Board.bmp", "");
				insertChild(temp, mouse);

				break;
			case 'P':
				insertChild(temp,
						createWidget(SIDE_PANEL_WIDTH + i * CELL_WIDTH,
						TOP_PANEL_HEIGHT + j * CELL_HEIGHT, 1365, 0,
						CELL_WIDTH, CELL_HEIGHT, 0, "Board.bmp", ""));
				break;
			case 'W':
				insertChild(temp,
						createWidget(SIDE_PANEL_WIDTH + i * CELL_WIDTH,
						TOP_PANEL_HEIGHT + j * CELL_HEIGHT, 1452, 0,
						CELL_WIDTH, CELL_HEIGHT, 0, "Board.bmp", ""));
				break;
			case 'C':
				cat = createWidget(SIDE_PANEL_WIDTH + i * CELL_WIDTH,
				TOP_PANEL_HEIGHT + j * CELL_HEIGHT, 1540, 0,
				CELL_WIDTH, CELL_HEIGHT, 0, "Board.bmp", "");
				insertChild(temp, cat);
				break;
			}
		}
	}
	printBoard(board);
	//nonRecDFS(temp, displayWidget);
	return temp;
}

void updateGrid(int direction) {
	WidgetRef temp = NULL;
	int imgLoc;

	if (!strcmp(turn, "mouse")) {
		temp = mouse;
		imgLoc = MOUSE_LOCATION;
	} else {
		temp = cat;
		imgLoc = CAT_LOCATION;
	}
	temp->srcY = TOP_PANEL_HEIGHT;
	temp->srcX = SIDE_PANEL_WIDTH;
	displayWidget(temp);

	temp->srcX = imgLoc;
	temp->srcY = 0;
	switch (direction) {
	case UP:
		temp->y -= CELL_HEIGHT;
		break;
	case DOWN:
		temp->y += CELL_HEIGHT;
		break;
	case LEFT:
		temp->x -= CELL_WIDTH;
		break;
	case RIGHT:
		temp->x += CELL_WIDTH;
		break;
	}
	displayWidget(temp);
}

int openGameWindow(int gameNum) {
	int i = 1, action = -1, direction = 0, pause = 0, turnCounter = 1,
			gameOver = 0;

	if (game.cat_human == -1 || game.mouse_human == -1) {
		return -1;
	}
	//loading default game world #1
	char** board = loadGame(gameNum);
	//$$$ Check including it inside
	updateGameStatus(board);

	//parsing gameOption...
	//TODO

	WindowInitMacro;
	display = SDL_SetVideoMode(826, 662, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);

	//TreeRef t2 = newTree(createWidget(0, 0, 0, 0, 826, 662, 0, NULL, "hgh"));
	//nonRecDFS(t2, displayWidget);

	TreeRef sidePanel = newTree(
			createWidget(0, TOP_PANEL_HEIGHT, 0, 0, SIDE_PANEL_WIDTH,
			GRID_HEIGHT, 0,
			NULL, "hgh"));
	for (i = 0; i < 5; i++) {
		insertChild(sidePanel,
				createWidget(20, 130 + BOARD_BUTTON_HEIGHT * i + 50 * i, 0,
						i * BOARD_BUTTON_HEIGHT,
						BOARD_BUTTON_WIDTH, BOARD_BUTTON_HEIGHT, 0,
						"Buttons2.bmp", ""));
	}

	TreeRef frozenSidePanel = newTree(
			createWidget(0, TOP_PANEL_HEIGHT, PAUSED_SIDE_PANEL_LOCATION, 0,
			SIDE_PANEL_WIDTH, GRID_HEIGHT, 0, "Board.bmp", "hgh"));
	for (i = 0; i < 5; i++) {
		insertChild(frozenSidePanel,
				createWidget(20, 130 + BOARD_BUTTON_HEIGHT * i + 50 * i,
				BOARD_BUTTON_WIDTH, i * BOARD_BUTTON_HEIGHT,
				BOARD_BUTTON_WIDTH, BOARD_BUTTON_HEIGHT, 0, "Buttons2.bmp",
						""));
	}

	TreeRef topPanel = newTree(createWidget(0, 0, 0, 0, 0, 0, 0, NULL, "hgh"));
	for (i = 0; i < 3; i++) {
		insertChild(topPanel,
				createWidget(300, 10 + i * (10 + STATUS_CAPTION_HEIGHT),
				STATUS_CAPTIONS_X, 0, STATUS_CAPTION_WIDTH,
				STATUS_CAPTION_HEIGHT, 0, "Buttons.bmp", "hgh"));
	}

	WidgetRef tens = createWidget(300 + MOUSE_1ST_DIGIT_X, 10, 2000, 2000,
	DIGIT_WIDTH,
	STATUS_CAPTION_HEIGHT, 0, "Buttons.bmp", "hgh");
	WidgetRef ones = createWidget(300 + MOUSE_1ST_DIGIT_X + DIGIT_WIDTH, 10,
	DIGIT_X, ZERO_Y, DIGIT_WIDTH,
	STATUS_CAPTION_HEIGHT, 0, "Buttons.bmp", "hgh");
	insertChild(topPanel, tens);
	insertChild(topPanel, ones);

	getChildWidget(topPanel, 3)->height = SPACE_BUTTON_HEIGHT;

	//nonRecDFS(topPanel, displayWidget);
	nonRecDFS(boardToTree(board), displayWidget);
	//nonRecDFS(sidePanel, printWidget);
	//nonRecDFS(sidePanel, displayWidget);
	nonRecDFS(frozenSidePanel, displayWidget);
	//SDL_Delay(5000);

	SDL_Event e;
	WidgetRef cur;
	SDL_Rect curPos;
	curPos.w = CELL_WIDTH;
	curPos.h = CELL_HEIGHT;

	while (action != QUIT) {

		if (action != 0 && pause == 0) {
			puts("refreshing");
			printf("turn %d, %d\n", turnCounter,
					(turnCounter / 2 - turnCounter / 2 % 10) / 10);

			if (turnCounter > 19 && tens->srcX == 2000) {
				tens->srcX = DIGIT_X;
			}
			tens->srcY =
					ZERO_Y + 4
							+ (STATUS_CAPTION_HEIGHT)
									* (((turnCounter / 2
											- (turnCounter / 2) % 10)) / 10);

			ones->srcY = ZERO_Y + 4
					+ (STATUS_CAPTION_HEIGHT) * (turnCounter / 2 % 10);
			if (!strcmp(turn, "mouse")) {
				cur = mouse;
				puts("mouse turn");
				tens->x = 300 + MOUSE_1ST_DIGIT_X;
				ones->x = 300 + MOUSE_1ST_DIGIT_X + DIGIT_WIDTH;

				getChildWidget(topPanel, 1)->srcY = MOUSE_MOVE_NUM_HEIGHT;
				if (game.mouse_human) {
					puts("human mouse");
					getChildWidget(topPanel, 2)->srcY = MOUSE_MOVE_NUM_HEIGHT
							+ STATUS_CAPTION_HEIGHT * 2;
					getChildWidget(topPanel, 3)->srcY = SPACE_BUTTON_HEIGHT;

				} else { //machine mouse
					getChildWidget(topPanel, 2)->srcY = MOUSE_MOVE_NUM_HEIGHT
							+ 4 * STATUS_CAPTION_HEIGHT;
				}

			} else {
				cur = cat;
				puts("cat turn");

				tens->x = 300 + CAT_1ST_DIGIT_X;
				ones->x = 300 + CAT_1ST_DIGIT_X + DIGIT_WIDTH;

				getChildWidget(topPanel, 1)->srcY = MOUSE_MOVE_NUM_HEIGHT
						+ STATUS_CAPTION_HEIGHT;
				if (game.cat_human) {
					puts("human cat");
					getChildWidget(topPanel, 2)->srcY = MOUSE_MOVE_NUM_HEIGHT
							+ STATUS_CAPTION_HEIGHT * 2;
					getChildWidget(topPanel, 3)->srcY = SPACE_BUTTON_HEIGHT;

				} else { //machine cat
					getChildWidget(topPanel, 2)->srcY = MOUSE_MOVE_NUM_HEIGHT
							+ 4 * STATUS_CAPTION_HEIGHT;
				}

			}
			curPos.x = cur->x;
			curPos.y = cur->y;
			//insertChild(topPanel, createWidget(300, 20, STATUS_CAPTIONS_X, 0,
			//SPACE_BUTTON_WIDTH, SPACE_BUTTON_HEIGHT, 0, "Buttons.bmp", ""));
			nonRecDFS(topPanel, displayWidget);
			action = 0;
		}
		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
			case (SDL_QUIT):
				action = QUIT;
				break;
			case (SDL_KEYUP):
				switch (e.key.keysym.sym) {

				case SDLK_DOWN:
					action = MOVE;
					direction = DOWN;
					break;
				case SDLK_UP:
					action = MOVE;
					direction = UP;
					break;
				case SDLK_LEFT:
					action = MOVE;
					direction = LEFT;
					break;
				case SDLK_RIGHT:
					action = MOVE;
					direction = RIGHT;
					break;
				case SDLK_ESCAPE:
					action = QUIT;
					break;
				case (SDLK_SPACE):
					PauseMacro

					break;
				case (SDLK_F1):
					if (pause)
						action = RECONF_MOUSE;
					break;
				case (SDLK_F2):
					if (pause)
						action = RECONF_CAT;
					break;
				case (SDLK_F3):
					if (pause)
						action = RESTART_GAME;
					break;
				case (SDLK_F4):
					if (pause)
						action = BACK;
					break;
				default:
					break;
				} //end switch-keyup
				break; //break keyup

			case (SDL_MOUSEBUTTONUP):

				if ((e.button.x > curPos.x + curPos.w)
						&& (e.button.x < curPos.x + 2 * curPos.w)
						&& (e.button.y > curPos.y)
						&& (e.button.y < curPos.y + curPos.h)) {
					action = MOVE;
					direction = RIGHT;
				} else if ((e.button.x > curPos.x - CELL_WIDTH)
						&& (e.button.x < curPos.x) && (e.button.y > curPos.y)
						&& (e.button.y < curPos.y + curPos.h)) {
					action = MOVE;
					direction = LEFT;
				} else if ((e.button.x > curPos.x)
						&& (e.button.x < curPos.x + curPos.w)
						&& (e.button.y > curPos.y - curPos.h)
						&& (e.button.y < curPos.y)) {
					action = MOVE;
					direction = UP;
				} else if ((e.button.x > curPos.x)
						&& (e.button.x < curPos.x + curPos.w)
						&& (e.button.y > curPos.y + curPos.h)
						&& (e.button.y < curPos.y + 2 * curPos.h)) {
					action = MOVE;
					direction = DOWN;
				} else if ((e.button.x > getChildWidget(topPanel, 3)->x)
						&& (e.button.x
								< getChildWidget(topPanel, 3)->x
										+ SPACE_BUTTON_WIDTH)
						&& (e.button.y > getChildWidget(topPanel, 3)->y)
						&& (e.button.y
								< getChildWidget(topPanel, 3)->y
										+ SPACE_BUTTON_HEIGHT)) {
					PauseMacro;
				} else if (pause
						&& e.button.x > 20&& e.button.x<20+BOARD_BUTTON_WIDTH) {
					switch (e.button.y) {
					case RECONF_MOUSE_LOCATION:
						action = RECONF_MOUSE;
						break;
					case RECONF_CAT_LOCATION:
						action = RECONF_CAT;
						break;
					case RESTART_GAME_LOCATION:
						action = RESTART_GAME;
						break;
					case GOTO_MAIN_MENU_LOCATION:
						action = BACK;
						break;
					case QUIT_PROG_LOCATION:
						action = QUIT;
						break;
					} //end switch
				}

				break;
			} //end switch e-type

		} //end pollevent
		switch (action) {
		case RECONF_CAT:
		case RECONF_MOUSE:
			SDL_Quit();
			if (action == RECONF_CAT) {
				puts("reconf cat");
				humanSelect(CAT + MID_GAME);
			} else {
				puts("reconf mouse");
				humanSelect(MOUSE + MID_GAME);
			}
			WindowInitMacro

			display = SDL_SetVideoMode(826, 662, 0,
			SDL_HWSURFACE | SDL_DOUBLEBUF);

			nonRecDFS(boardToTree(board), displayWidget);
			nonRecDFS(sidePanel, displayWidget);
			nonRecDFS(topPanel, displayWidget);
			action = 0;
			break;
		case QUIT:
			puts("quit");
			SDL_Quit();
			freeBoard(board);
			exit(0);
			break;
		case BACK:
			SDL_Quit();
			return 1;
			break;
		case MOVE:
			if (pause) {
				action = 0;
				direction = 0;
				break;
			} else if (move(&board, direction)) {
				puts("move");
				updateGrid(direction);
				switchTurn();
				turnCounter++;
			}
			direction = 0;
			break;

		case PAUSE:
			pause = 1;
			if (!gameOver) {
				nonRecDFS(sidePanel, displayWidget);
				if ((game.cat_human && !strcmp(turn, "cat"))
						|| (game.mouse_human && !strcmp(turn, "mouse"))) {
					getChildWidget(topPanel, 2)->srcY = MOUSE_MOVE_NUM_HEIGHT
							+ STATUS_CAPTION_HEIGHT * 3;
				} else {
					getChildWidget(topPanel, 2)->srcY = MOUSE_MOVE_NUM_HEIGHT
							+ STATUS_CAPTION_HEIGHT * 3; //TODO add machine pause
				}

				getChildWidget(topPanel, 3)->srcY = 2 * SPACE_BUTTON_HEIGHT;
				nonRecDFS(topPanel, displayWidget);
			}
			action = 0;
			break;
		case RESTART_GAME:
		case UNPAUSE:
			if (action == RESTART_GAME) {
				board = loadGame(gameNum);
				gameOver = 0;
				turnCounter = 1;
				nonRecDFS(boardToTree(board), displayWidget);
			}
			pause = 0;
			if (!gameOver) {
				nonRecDFS(frozenSidePanel, displayWidget);
				getChildWidget(topPanel, 2)->srcY = MOUSE_MOVE_NUM_HEIGHT
						+ STATUS_CAPTION_HEIGHT * 2;
				if ((game.cat_human && !strcmp(turn, "cat"))
						|| (game.mouse_human && !strcmp(turn, "mouse"))) {
					getChildWidget(topPanel, 3)->srcY = SPACE_BUTTON_HEIGHT;
				} else {
					getChildWidget(topPanel, 3)->srcY = 0;
				}
				nonRecDFS(topPanel, displayWidget);
			}

			break;
		default:
			break;
		}

		if (!gameOver) {
			if (turnCounter / 2 > max_turns) {
				puts("tie");
				gameOver = 1;
				displayWidget(createWidget(150, 10,
				STATUS_CAPTIONS_X, GAME_OVER_MOUSE_Y,
				BIG_MSG_WIDTH - 30, TOP_PANEL_HEIGHT - 20, 0, NULL, ""));

				displayWidget(createWidget(120, 10,
				STATUS_CAPTIONS_X, GAME_OVER_MOUSE_Y + 23 + BIG_MSG_HEIGHT * 2,
				BIG_MSG_WIDTH - 10, BIG_MSG_HEIGHT, 0, "Buttons.bmp", ""));

			}
			switch (updateGameStatus(board)) {
			case 1:
				puts("mouse wins");
				gameOver = 1;

				displayWidget(createWidget(80, 10,
				STATUS_CAPTIONS_X, GAME_OVER_MOUSE_Y,
				BIG_MSG_WIDTH - 30, TOP_PANEL_HEIGHT - 20, 0, NULL, ""));

				displayWidget(createWidget(80, 10,
				STATUS_CAPTIONS_X, GAME_OVER_MOUSE_Y,
				BIG_MSG_WIDTH + 30, BIG_MSG_HEIGHT, 0, "Buttons.bmp", ""));
				break;
			case 2:
				puts("cat wins");
				gameOver = 1;

				displayWidget(createWidget(120, 10,
				STATUS_CAPTIONS_X, GAME_OVER_MOUSE_Y,
				BIG_MSG_WIDTH - 30, TOP_PANEL_HEIGHT - 20, 0, NULL, ""));

				displayWidget(createWidget(120, 10,
				STATUS_CAPTIONS_X, GAME_OVER_CAT_Y,
				BIG_MSG_WIDTH - 10, BIG_MSG_HEIGHT, 0, "Buttons.bmp", ""));
				break;
			}	//end select
			if (gameOver) {
				action = PAUSE;
				pause = 1;
				nonRecDFS(sidePanel, displayWidget);
			}
		} //end gameover if
	} //while action
//SDL_Delay(5000);
	SDL_Quit();

	return 1;
}

int worldSelect(int isSave) {
	int i = 1, action = 0;
	int worldNum = 1;
	puts("worldNum select");
	WindowInitMacro;
	display = SDL_SetVideoMode(480, 500, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);

//build and display display-tree
	TreeRef t2 = newTree(createWidget(0, 0, 0, 0, 600, 800, 0, NULL, "hgh"));
	insertChild(t2, createWidget(100, 0 * 10 + 200 + 0 * BUTTON_HEIGHT, 0,
	BUTTON_HEIGHT * 9,
	BUTTON_WIDTH, BUTTON_HEIGHT - 2, 1, "Buttons.bmp", ""));
	for (i = 7; i < 9; i++) {
		Widget* b = createWidget(100,
				(i - 6) * 10 + 200 + (i - 6) * BUTTON_HEIGHT, 0,
				i * BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT, 0,
				"Buttons.bmp", "");
		insertChild(t2, b);
	}
	nonRecDFS(t2, printWidget);
	nonRecDFS(t2, displayWidget);

//display caption
	if (isSave == SAVE_GAME) {
		//TODO - replace with right caption
		displayWidget(createWidget(150, 100,
		CAPTION_LOCATION_X, LOAD_GAME_CAPTION_LOCATION_Y,
		CAPTION_WIDTH, CAPTION_HEIGHT, 0, "Buttons2.bmp", ""));
	} else {
		displayWidget(createWidget(150, 100,
		CAPTION_LOCATION_X, LOAD_GAME_CAPTION_LOCATION_Y,
		CAPTION_WIDTH, CAPTION_HEIGHT, 0, "Buttons2.bmp", ""));
	}
	SDL_Event e;
	WidgetRef cur;
	i = 1;
	while (action != QUIT) {
		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
			case (SDL_QUIT):
				action = QUIT;
				break;
			case (SDL_KEYUP):
				switch (e.key.keysym.sym) {
				case SDLK_RETURN:
					puts("enter");
					i = 1;
					cur = getChildWidget(t2, i);
					while (cur != NULL) {
						if (cur->isSelected == 1) {
							switch (i) {
							case 1:

								break;
							case 2:
								action = BACK;
								break;
							case 3:
								action = DONE;
								break;
							}

							printf("%d", action);
							break;
						}
						i++;
						cur = getChildWidget(t2, i);
					} //end while

					break;
				case SDLK_DOWN:
					if (i == 1)
						action = LEVEL_DOWN;
					break;
				case SDLK_UP:
					if (i == 1)
						action = LEVEL_UP;
					break;
				case SDLK_ESCAPE:
					action = QUIT;
					break;
				case SDLK_TAB:
					printf("tab\n");
					i = 1;
					cur = getChildWidget(t2, i);
					while (cur != NULL) {
						printWidget(cur);
						if (cur->isSelected == 1) {
							puts("1");
							cur->isSelected = 0;
							displayWidget(cur);
							i++;
							cur = getChildWidget(t2, i);
							if (cur == NULL) {
								//if list is over, start from beginning
								i = 1;
								cur = getChildWidget(t2, i);
								puts("over");
							}
							cur->isSelected = 1;
							displayWidget(cur);
							break;
						}
						i++;
						cur = getChildWidget(t2, i);
					}
					break;
				default:
					break;
				}
				break;
			case (SDL_MOUSEBUTTONUP):

				if ((e.button.x > 100) && (e.button.x < 100 + BUTTON_WIDTH)) {
					switch (e.button.y) {
					case S_SELECT_BACK_LOCATION:
						action = BACK;
						break;
					case DONE_LOCATION:
						action = DONE;
						break;
					case UP_ARROW:
						puts("up");
						if (e.button.x > 100 + BUTTON_WIDTH - 32) {
							action = LEVEL_UP;
						}
						break;
					case DOWN_ARROW:
						puts("down");
						if (e.button.x > 100 + BUTTON_WIDTH - 32) {
							action = LEVEL_DOWN;
						}
						break;
					} //end mouse-y button
				}  //end mouse-x button
				break;
			}

		} //end pollevenet
		switch (action) {
		case LEVEL_UP:
			puts("level_up");
			cur = getChildWidget(t2, 1);
			cur->srcY += (BUTTON_HEIGHT - 1);
			worldNum++;
			if (worldNum == 6) {
				cur->srcY = BUTTON_HEIGHT * 9;
				worldNum = 1;
			}
			displayWidget(cur);
			action = 0;
			break;
		case LEVEL_DOWN:
			cur = getChildWidget(t2, 1);
			cur->srcY -= (BUTTON_HEIGHT - 1);
			worldNum--;
			if (worldNum == 0) {
				cur->srcY = BUTTON_HEIGHT * 13;
				worldNum = 5;
			}
			displayWidget(cur);
			action = 0;
			break;

		case BACK:
			SDL_Quit();
			puts("back");
			return -1;
			break;
		case DONE:
			SDL_Quit();
			puts("quit");
			return worldNum;
		}
	}

//SDL_Delay(2000);
	SDL_Quit();
	return -1;
}

int skillSelect(int player) {
	int i = 1, action = 0;
	int skill = 5;
	puts("skill select");
	WindowInitMacro;
	display = SDL_SetVideoMode(480, 500, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
//TreeRef caption = newTree(createWidget(100,0,));
	TreeRef t2 = newTree(createWidget(0, 0, 0, 0, 600, 800, 0, NULL, "hgh"));
	insertChild(t2, createWidget(100, 0 * 10 + 200 + 0 * BUTTON_HEIGHT,
	BUTTON_WIDTH, BUTTON_HEIGHT * 4,
	BUTTON_WIDTH, BUTTON_HEIGHT, 1, "Buttons.bmp", ""));
	for (i = 7; i < 9; i++) {
		Widget* b = createWidget(100,
				(i - 6) * 10 + 200 + (i - 6) * BUTTON_HEIGHT, 0,
				i * BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT, 0,
				"Buttons.bmp", "");
		insertChild(t2, b);
	}
	nonRecDFS(t2, printWidget);
	nonRecDFS(t2, displayWidget);

//display caption
	if (player == CAT) {
		displayWidget(createWidget(80, 100,
		CAPTION_LOCATION_X, SKILL_CAT_CAPTION_LOCATION_Y,
		CAPTION_WIDTH, CAPTION_HEIGHT, 0, "Buttons2.bmp", ""));
	} else {
		displayWidget(createWidget(50, 100,
		CAPTION_LOCATION_X, SKILL_MOUSE_CAPTION_LOCATION_Y,
		CAPTION_WIDTH, CAPTION_HEIGHT, 0, "Buttons2.bmp", ""));
	}

	SDL_Event e;
	WidgetRef cur;
	i = 1;
	while (action != QUIT) {
		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
			case (SDL_QUIT):
				action = BACK;
				break;
			case (SDL_KEYUP):
				switch (e.key.keysym.sym) {
				case SDLK_RETURN:
					puts("enter");
					i = 1;
					cur = getChildWidget(t2, i);
					while (cur != NULL) {
						if (cur->isSelected == 1) {
							switch (i) {
							case 1:

								break;
							case 2:
								action = BACK;
								break;
							case 3:
								action = DONE;
								break;
							}

							printf("%d", action);
							break;
						}
						i++;
						cur = getChildWidget(t2, i);
					} //end while

					break;
				case SDLK_DOWN:
					if (i == 1)
						action = LEVEL_DOWN;
					break;
				case SDLK_UP:
					if (i == 1)
						action = LEVEL_UP;
					break;
				case SDLK_ESCAPE:
					action = BACK;
					break;
				case SDLK_TAB:
					printf("tab\n");
					i = 1;
					cur = getChildWidget(t2, i);
					while (cur != NULL) {
						printWidget(cur);
						if (cur->isSelected == 1) {
							puts("1");
							cur->isSelected = 0;
							displayWidget(cur);
							i++;
							cur = getChildWidget(t2, i);
							if (cur == NULL) {
								//if list is over, start from beginning
								i = 1;
								cur = getChildWidget(t2, i);
								puts("over");
							}
							cur->isSelected = 1;
							displayWidget(cur);
							break;
						}
						i++;
						cur = getChildWidget(t2, i);
					}
					break;
				default:
					break;
				}
				break;
			case (SDL_MOUSEBUTTONUP):

				if ((e.button.x > 100) && (e.button.x < 100 + BUTTON_WIDTH)) {
					switch (e.button.y) {
					case S_SELECT_BACK_LOCATION:
						action = BACK;
						break;
					case DONE_LOCATION:

						action = DONE;
						break;
					case UP_ARROW:
						puts("up");
						if (e.button.x > 100 + BUTTON_WIDTH - 32) {
							action = LEVEL_UP;
						}
						break;
					case DOWN_ARROW:
						puts("down");
						if (e.button.x > 100 + BUTTON_WIDTH - 32) {
							action = LEVEL_DOWN;
						}
						break;
					} //end mouse-y button
				}  //end mouse-x button
				break;
			}

		} //end pollevenet
		switch (action) {
		case LEVEL_UP:
			puts("level_up");
			cur = getChildWidget(t2, 1);
			cur->srcY += BUTTON_HEIGHT;
			skill++;
			if (skill == 10) {
				cur->srcY = 0;
				skill = 1;
			}
			displayWidget(cur);
			action = 0;
			break;
		case LEVEL_DOWN:
			cur = getChildWidget(t2, 1);
			cur->srcY -= BUTTON_HEIGHT;
			skill--;
			if (skill == 0) {
				cur->srcY = BUTTON_HEIGHT * 8;
				skill = 9;
			}
			displayWidget(cur);
			action = 0;
			break;

		case BACK:
			SDL_Quit();
			puts("back");
			return BACK;
			break;

		case DONE:
			SDL_Quit();
			puts("quit");
			if (player % 10 == MOUSE) {
				game.mouse_skill = skill;
				return 1;
			}
			if (player == CAT + MID_GAME) {
				game.cat_skill = skill;
				return 1;
			}
			if (player == CAT) {
				game.cat_skill = skill;
				//open window for mouse
				if (humanSelect(MOUSE) == BACK) {
					puts("back pressed");
					WindowInitMacro
					display = SDL_SetVideoMode(500, 500, 0,
					SDL_HWSURFACE | SDL_DOUBLEBUF);
					nonRecDFS(t2, displayWidget);

					//display caption
					displayWidget(createWidget(80, 100,
					CAPTION_LOCATION_X, SKILL_CAT_CAPTION_LOCATION_Y,
					CAPTION_WIDTH, CAPTION_HEIGHT, 0, "Buttons2.bmp", ""));
					action = 0;
				} else { //mouse skill select succeeded
					return 1;
				}
			}

			break;
		} //end switch
	} //end while
//SDL_Delay(2000);
	SDL_Quit();
	return -1;

}
int humanSelect(int player) {
//return 1 if human selected correctly , otherwise on fail

	printf("start player %d\n", player);
	int i, action = 0;
	WindowInitMacro;
	display = SDL_SetVideoMode(500, 500, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
	TreeRef t2 = newTree(createWidget(0, 0, 0, 0, 600, 800, 0, NULL, "hgh"));
	insertChild(t2,
			createWidget(100, 0 * 10 + 200 + 0 * BUTTON_HEIGHT, 0,
					5 * BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT, 1,
					"Buttons.bmp", ""));
	for (i = 6; i < 8; i++) {
		Widget* b = createWidget(100,
				(i - 5) * 10 + 200 + (i - 5) * BUTTON_HEIGHT, 0,
				i * BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT, 0,
				"Buttons.bmp", "");
		insertChild(t2, b);
	}
	nonRecDFS(t2, displayWidget);

//display caption
	if (player % 10 == CAT) {
		displayWidget(createWidget(100, 100,
		CAPTION_LOCATION_X, SELECT_CAT_CAPTION_LOCATION_Y,
		CAPTION_WIDTH, CAPTION_HEIGHT - 8, 0, "Buttons2.bmp", ""));
	} else {
		displayWidget(createWidget(85, 100,
		CAPTION_LOCATION_X, SELECT_MOUSE_CAPTION_LOCATION_Y,
		CAPTION_WIDTH, CAPTION_HEIGHT, 0, "Buttons2.bmp", ""));
	}

	SDL_Event e;
	WidgetRef cur;
	while (action != QUIT) {
		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
			case (SDL_QUIT):
				action = BACK;
				break;
			case (SDL_KEYUP):
				switch (e.key.keysym.sym) {
				case SDLK_RETURN:
					puts("enter");
					i = 1;
					cur = getChildWidget(t2, i);
					while (cur != NULL) {
						if (cur->isSelected == 1) {
							printWidget(cur);
							action = i + 5;
							printf("%d", action);
							break;
						}
						i++;
						cur = getChildWidget(t2, i);
					} //emd while

					break;
				case SDLK_ESCAPE:
					action = BACK;
					break;
				case SDLK_TAB:
					printf("tab\n");
					i = 1;
					cur = getChildWidget(t2, i);
					while (cur != NULL) {
						printWidget(cur);

						if (cur->isSelected == 1) {
							puts("1");
							cur->isSelected = 0;
							displayWidget(cur);
							i++;
							cur = getChildWidget(t2, i);
							if (cur == NULL) {
								//if list is over, start from beginning
								i = 1;
								cur = getChildWidget(t2, i);
								puts("over");
							}
							cur->isSelected = 1;
							displayWidget(cur);
							break;
						}
						i++;
						cur = getChildWidget(t2, i);
					}
					break;
				default:
					break;
				}
				break;
			case (SDL_MOUSEBUTTONUP):

				if ((e.button.x > 100) && (e.button.x < 100 + BUTTON_WIDTH)) {
					switch (e.button.y) {
					case HUMAN_LOCATION:
						action = HUMAN;

						break;
					case MACHINE_LOCATION:
						action = MACHINE;

						break;
					case U_SELECT_BACK_LOCATION:
						action = BACK;

						break;
					}
				}
				break;
			}

		} //end pollevenet

		switch (action) {

		case HUMAN:
			SDL_Quit();
			puts("human");

			if (player % 10 == MOUSE) { // player is mouse
				game.mouse_human = 1;
				return 1;
			}
			if (player == CAT + MID_GAME) {
				printf("player %d", player);
				game.cat_human = 1;
				return 1;
			}
			puts("try mouse");
			if (player == CAT) { //on game setup, not midgame
				game.cat_human = 1;
				if (humanSelect(MOUSE) == BACK) {
					puts("back pressed");
					WindowInitMacro
					display = SDL_SetVideoMode(500, 500, 0,
					SDL_HWSURFACE | SDL_DOUBLEBUF);
					nonRecDFS(t2, displayWidget);

					//display caption

					displayWidget(createWidget(100, 100,
					CAPTION_LOCATION_X,
					SELECT_CAT_CAPTION_LOCATION_Y,
					CAPTION_WIDTH, CAPTION_HEIGHT - 8, 0, "Buttons2.bmp", ""));

					action = 0;
				} else { // human select succeeded
					SDL_Quit();
					return 1;
				}
			}

			break;
		case MACHINE:
			SDL_Quit();
			puts("machine");

			//update gameOptions
			if (player % 10 == CAT) {
				game.cat_human = 0;
			} else {
				game.mouse_human = 0;
			}

			//open following windows
			if (skillSelect(player) == BACK) { //skill was put back
				WindowInitMacro
				display = SDL_SetVideoMode(500, 500, 0,
				SDL_HWSURFACE | SDL_DOUBLEBUF);
				nonRecDFS(t2, displayWidget);
				//display caption
				if (player % 10 == CAT) {
					displayWidget(createWidget(100, 100,
					CAPTION_LOCATION_X,
					SELECT_CAT_CAPTION_LOCATION_Y,
					CAPTION_WIDTH, CAPTION_HEIGHT - 8, 0, "Buttons2.bmp", ""));
				} else {
					displayWidget(createWidget(85, 100,
					CAPTION_LOCATION_X,
					SELECT_MOUSE_CAPTION_LOCATION_Y,
					CAPTION_WIDTH, CAPTION_HEIGHT, 0, "Buttons2.bmp", ""));
				}
				action = 0;
			} else {
				puts("success");
				SDL_Quit();
				return 1;

			}
			break;
		case BACK:
			printf("action= %d", action);
			SDL_Quit();
			puts("back");
			return BACK;
			break;
		} //end switch
	} //end poll

	SDL_Quit();
	return 0;

}

int openEditorWindow(int gameNum) {
	int i = 1, action = -1, direction = 0, selectedCellX = 0, selectedCellY = 0;

	if (game.cat_human == -1 || game.mouse_human == -1) {
		return -1;
	}
	//loading default game world #1
	char** board = loadGame(gameNum);
	//$$$ Check including it inside
	updateGameStatus(board);

	//parsing gameOption...
	//TODO

	WindowInitMacro;
	display = SDL_SetVideoMode(826, 662, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);

	//TreeRef t2 = newTree(createWidget(0, 0, 0, 0, 826, 662, 0, NULL, "hgh"));
	//nonRecDFS(t2, displayWidget);

	TreeRef sidePanel = newTree(
			createWidget(0, TOP_PANEL_HEIGHT, 0, 0, SIDE_PANEL_WIDTH,
			GRID_HEIGHT, 0,
			NULL, "hgh"));
	for (i = 0; i < 5; i++) {
		insertChild(sidePanel,
				createWidget(20, 130 + BOARD_BUTTON_HEIGHT * i + 50 * i, 0,
						(i + 5) * BOARD_BUTTON_HEIGHT,
						BOARD_BUTTON_WIDTH, BOARD_BUTTON_HEIGHT, 0,
						"Buttons2.bmp", ""));
	}
	/*
	 TreeRef frozenSidePanel = newTree(
	 createWidget(0, TOP_PANEL_HEIGHT, PAUSED_SIDE_PANEL_LOCATION, 0,
	 SIDE_PANEL_WIDTH, GRID_HEIGHT, 0, "Board.bmp", "hgh"));
	 for (i = 0; i < 5; i++) {
	 insertChild(frozenSidePanel,
	 createWidget(20, 130 + BOARD_BUTTON_HEIGHT * i + 50 * i,
	 BOARD_BUTTON_WIDTH, (i + 5) * BOARD_BUTTON_HEIGHT,
	 BOARD_BUTTON_WIDTH, BOARD_BUTTON_HEIGHT, 0, "Buttons2.bmp",
	 ""));
	 }
	 */

	TreeRef topPanel = newTree(
			createWidget(0, 310, 0, 0, 0, 0, 0, NULL, "hgh"));
	for (i = 0; i < 3; i++) {
		insertChild(topPanel, createWidget(10 + i * 300, 50, CAPTION_LOCATION_X,
		LOAD_GAME_CAPTION_LOCATION_Y + CAPTION_HEIGHT,
		BOARD_BUTTON_WIDTH - 1,
		BOARD_BUTTON_HEIGHT + 2, 0, "Buttons2.bmp", "hgh"));
	}
	getChildWidget(topPanel, 1)->srcX = 830;
	getChildWidget(topPanel, 1)->srcY = 0;
	getChildWidget(topPanel, 3)->srcX = 830 + BOARD_BUTTON_WIDTH;
	getChildWidget(topPanel, 3)->srcY = 0 + BOARD_BUTTON_HEIGHT + 2;

	//nonRecDFS(topPanel, displayWidget);
	nonRecDFS(boardToTree(board), displayWidget);
	//nonRecDFS(sidePanel, printWidget);
	//nonRecDFS(sidePanel, displayWidget);
	nonRecDFS(sidePanel, displayWidget);
	//SDL_Delay(5000);

	SDL_Event e;
	WidgetRef cur;
	SDL_Rect curPos;
	curPos.w = CELL_WIDTH;
	curPos.h = CELL_HEIGHT;

	while (action != QUIT) {

		if (action != 0) {
			puts("refreshing");

			//curPos.x = cur->x;
			//curPos.y = cur->y;
			//insertChild(topPanel, createWidget(300, 20, STATUS_CAPTIONS_X, 0,
			//SPACE_BUTTON_WIDTH, SPACE_BUTTON_HEIGHT, 0, "Buttons.bmp", ""));

			nonRecDFS(topPanel, displayWidget);
			action = 0;
		}

		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
			case (SDL_QUIT):
				action = QUIT;
				break;
			case (SDL_KEYUP):
				switch (e.key.keysym.sym) {

				case SDLK_DOWN:
					action = MOVE;
					direction = DOWN;
					break;
				case SDLK_UP:
					action = MOVE;
					direction = UP;
					break;
				case SDLK_LEFT:
					action = MOVE;
					direction = LEFT;
					break;
				case SDLK_RIGHT:
					action = MOVE;
					direction = RIGHT;
					break;
				case SDLK_ESCAPE:
					action = QUIT;
					break;
				case (SDLK_SPACE):

					break;
				case (SDLK_F1):
					action = RECONF_MOUSE;
					break;
				case (SDLK_F2):

					action = RECONF_CAT;
					break;
				case (SDLK_F3):
					action = RESTART_GAME;
					break;
				case (SDLK_F4):
					action = BACK;
					break;
				default:
					break;
				} //end switch-keyup
				break; //break keyup

			case (SDL_MOUSEBUTTONUP):

				if ((e.button.x > curPos.x + curPos.w)
						&& (e.button.x < curPos.x + 2 * curPos.w)
						&& (e.button.y > curPos.y)
						&& (e.button.y < curPos.y + curPos.h)) {
					action = MOVE;
					direction = RIGHT;
				} else if ((e.button.x > curPos.x - CELL_WIDTH)
						&& (e.button.x < curPos.x) && (e.button.y > curPos.y)
						&& (e.button.y < curPos.y + curPos.h)) {
					action = MOVE;
					direction = LEFT;
				} else if ((e.button.x > curPos.x)
						&& (e.button.x < curPos.x + curPos.w)
						&& (e.button.y > curPos.y - curPos.h)
						&& (e.button.y < curPos.y)) {
					action = MOVE;
					direction = UP;
				} else if ((e.button.x > curPos.x)
						&& (e.button.x < curPos.x + curPos.w)
						&& (e.button.y > curPos.y + curPos.h)
						&& (e.button.y < curPos.y + 2 * curPos.h)) {
					action = MOVE;
					direction = DOWN;
				} else if ((e.button.x > getChildWidget(topPanel, 3)->x)
						&& (e.button.x
								< getChildWidget(topPanel, 3)->x
										+ SPACE_BUTTON_WIDTH)
						&& (e.button.y > getChildWidget(topPanel, 3)->y)
						&& (e.button.y
								< getChildWidget(topPanel, 3)->y
										+ SPACE_BUTTON_HEIGHT)) {
				} else if (e.button.x
						> 20&& e.button.x < 20 + BOARD_BUTTON_WIDTH) {
					switch (e.button.y) {
					case RECONF_MOUSE_LOCATION:
						action = RECONF_MOUSE;
						break;
					case RECONF_CAT_LOCATION:
						action = RECONF_CAT;
						break;
					case RESTART_GAME_LOCATION:
						action = RESTART_GAME;
						break;
					case GOTO_MAIN_MENU_LOCATION:
						action = BACK;
						break;
					case QUIT_PROG_LOCATION:
						action = QUIT;
						break;
					} //end switch
				} //end if

				break; //break mouseup
			} //end switch e-type

		} //end pollevent
		switch (action) {
		case QUIT:
			puts("quit");
			SDL_Quit();
			freeBoard(board);
			exit(0);
			break;
		case BACK:
			SDL_Quit();
			return 1;
			break;
		case MOVE:

			action = 0;
			direction = 0;
			break;

		} //end switch

	} //while action
//SDL_Delay(5000);
	SDL_Quit();

	return 1;
}
