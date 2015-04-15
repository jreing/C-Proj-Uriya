//need to free widget
// need to free gameoptions!

#include <stdio.h>
#include "SDL/SDL.h"
#include "SDL/SDL_video.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "TreeUtils.h"

#define WIN_W 1000
#define WIN_H 1000
#define IMG_W 240
#define IMG_H 296

#define BUTTON_WIDTH 268
#define BUTTON_HEIGHT 74
#define SELECTED 813

#define NEW_GAME 1
#define LOAD_GAME 2
#define CREATE_GAME 3
#define EDIT_GAME 4
#define QUIT 5
#define HUMAN 6
#define MACHINE 7
#define BACK 8

#define NEW_GAME_LOCATION 200 ... 200+BUTTON_HEIGHT
#define LOAD_GAME_LOCATION 10*1+200+1*BUTTON_HEIGHT ...10*1+200+2*BUTTON_HEIGHT
#define CREATE_GAME_LOCATION 10*2+200+2*BUTTON_HEIGHT ... 10*2+200+3*BUTTON_HEIGHT
#define EDIT_GAME_LOCATION 10*3+200+3*BUTTON_HEIGHT ... 10*3+200+4*BUTTON_HEIGHT
#define QUIT_LOCATION 10*4+200+4*BUTTON_HEIGHT ... 10*4+200+5*BUTTON_HEIGHT
#define HUMAN_LOCATION NEW_GAME_LOCATION
#define MACHINE_LOCATION LOAD_GAME_LOCATION
#define U_SELECT_BACK_LOCATION CREATE_GAME_LOCATION

struct Widget {
	int srcX, srcY, width, height;
	int x, y;
	int isSelected;
	char* img_filename;
	char* caption;
};

struct gameOptions {
	int cat_human;
	int mouse_human;
	int cat_skill;
	int mouse_skill;
};

typedef struct Widget Widget;
typedef struct gameOptions gameOptions;
typedef Widget* WidgetRef;

gameOptions* newGame();
int humanSelect(int isCat);

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

void printWidget(void* data) {
	WidgetRef temp = (WidgetRef) data;
	printf(
			"x=%d,  y=%d, srcX=%d, srcY=%d, width=%d,  height=%d, isSelected=%d, filename equals= %s\n",
			temp->x, temp->y, temp->srcX, temp->srcY, temp->width, temp->height,
			temp->isSelected, temp->img_filename);
	//printf("isSelected equals= %d\n",((WidgetRef)rootData(temp))->isSelected);
	//printf("filename equals= %p\n",((WidgetRef)rootData(temp))->img_filename);
}
//opens a window with the widget image in it
SDL_Surface* displayWindow(Widget* widget) {
	SDL_Rect imgrect = { widget->x, widget->y, widget->width, widget->height };
	SDL_Rect rect = { 0, 0, 1400, 1400 };

	// Create window surface
	SDL_WM_SetCaption(widget->caption, widget->caption);
	SDL_Surface * display = SDL_SetVideoMode(widget->width, widget->height, 0,
	SDL_HWSURFACE | SDL_DOUBLEBUF);
	if (display == NULL) {
		printf("ERROR: failed to set video mode: %s\n", SDL_GetError());
		exit(0);
	}

	//puts(widget->img_filename);
	if (widget->img_filename == NULL) {
		puts("bg");
		if (SDL_FillRect(display, &rect,
				SDL_MapRGB(display->format, 255, 255, 255)) != 0) {
			printf("ERROR: failed to draw rect: %s\n", SDL_GetError());
			exit(0);
		} else {
			if (SDL_Flip(display) != 0) {
				printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
				return NULL;
			}
			return display;
		}
	} else {

		SDL_Surface *img = SDL_LoadBMP(widget->img_filename);

		if (img == NULL) {
			printf("ERROR: failed to load image: %s\n", SDL_GetError());
			if (SDL_FillRect(display, &imgrect,
					SDL_MapRGB(display->format, 0, 0, 0)) != 0) {
				printf("ERROR: failed to draw rect: %s\n", SDL_GetError());
				exit(0);
			}
		} else {

			// Apply the image to the display
			if (SDL_BlitSurface(img, &imgrect, display, &imgrect) != 0) {
				SDL_FreeSurface(img);
				printf("ERROR: failed to blit image: %s\n", SDL_GetError());
				return 0;
			}
		}
	}
	if (SDL_Flip(display) != 0) {
		printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
		return 0;
	}

	SDL_Delay(1000);

	return display;
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
		return 0;
	}
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
	printf("new widget address: %d\n", temp);
	return temp;
}

int openMainWindow() {
	int i, action = 0;
// Initialize SDL and make sure it quits
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return 1;
	}
	atexit(SDL_Quit);
	// Initialize SDL and make sure it quits
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return 1;
	}
	atexit(SDL_Quit);
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
	//SDL_Surface* display=displayWindow(rootData(t));
	nonRecDFS(t, displayWidget);
	//displayWidget(headData(getChildren(t)), display);

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
					} //emd while
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
					} //end muouse location switch
				break; //event type
			}
			switch (action) {
			case NEW_GAME:
				SDL_Quit();
				puts("new game");
				newGame();
				openMainWindow();
				action = 0;
				break;
			case LOAD_GAME:
				puts("load game");
				action = 0;
				break;
			case CREATE_GAME:
				puts("create game");
				action = 0;
				break;
			}
		}
	}

//SDL_Delay(4000);
	SDL_Quit();

	return 0;
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

	TreeRef t1 = newTree(createWidget(1, 2, 3, 4, 5, 6, 1, "a", "b"));
	insertChild(t1, createWidget(7, 8, 9, 10, 11, 12, 0, "c", "d"));
	insertChild(t1, newWidget(5, 5, 5, 5, 0, "f", "f"));
	TreeRef Two = (TreeRef) (headData(getChildren(t1)));
// void**  p=rootData(Two);
//printf("x of child is %d ", (getChild(t1,1))->x);
//printWidget((WidgetRef)(getChild(t1,1)));
//DFSPrint(t1,printWidget());
//DFSTwo (t1, printWidget);

	openMainWindow();

	return 1;
}
/*
 int main23() {
 putenv("SDL_VIDEO_WINDOW_POS=center");
 putenv("SDL_VIDEO_CENTERED=1");
 int quit = 0;
 openMainWindow();

 // Poll for keyboard & mouse events
 SDL_Event e;
 while (!quit) {
 while (SDL_PollEvent(&e) != 0) {
 switch (e.type) {
 case (SDL_QUIT):
 quit = 1;
 break;
 case (SDL_KEYUP):
 if (e.key.keysym.sym == SDLK_ESCAPE)
 quit = 1;
 break;
 case (SDL_MOUSEBUTTONUP):

 if ((e.button.x > 80) && (e.button.x < 340))
 switch (e.button.y) {
 case NEW_GAME_LOCATION:
 SDL_Quit();
 puts("new game");
 newGame();
 openMainWindow();
 break;
 case LOAD_GAME_LOCATION:
 puts("load game");
 break;
 case CREATE_GAME_LOCATION:
 puts("create game");
 break;
 case EDIT_GAME_LOCATION:
 puts("edit game");
 break;
 case QUIT_LOCATION:
 quit = 3;
 break;

 }
 break;
 }
 }
 }

 SDL_Quit();
 return 0;
 }
 */
int main2(int argc, char* args[]) {
///Start SDL
	SDL_Init(SDL_INIT_EVERYTHING);

// Initialize SDL and make sure it quits
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return 1;
	}
	atexit(SDL_Quit);

// Create window surface
	SDL_WM_SetCaption("Cat and Mouse", "Cat and Mouse");
	SDL_Surface *w = SDL_SetVideoMode(WIN_W, WIN_H, 0,
	SDL_HWSURFACE | SDL_DOUBLEBUF);
	if (w == NULL) {
		printf("ERROR: failed to set video mode: %s\n", SDL_GetError());
		return 1;
	}

// Define the rects we need
	SDL_Rect rect = { 10, 10, 50, 50 };
	SDL_Rect imgrect = { 0, 0, IMG_W, IMG_H };

// Load test spritesheet image
	SDL_Surface *img = SDL_LoadBMP("main_menu.bmp");
	if (img == NULL) {
		printf("ERROR: failed to load image: %s\n", SDL_GetError());
		return 1;
	}

	int quit = 0;
	while (!quit) {
		// Clear window to BLACK
		if (SDL_FillRect(w, 0, 0) != 0) {
			printf("ERROR: failed to draw rect: %s\n", SDL_GetError());
			break;
		}

		// Green rectangle button
		if (SDL_FillRect(w, &rect, SDL_MapRGB(w->format, 0, 255, 0)) != 0) {
			printf("ERROR: failed to draw rect: %s\n", SDL_GetError());
			break;
		}

		// Draw image sprite
		if (SDL_BlitSurface(img, &imgrect, w, 0) != 0) {
			SDL_FreeSurface(img);
			printf("ERROR: failed to blit image: %s\n", SDL_GetError());
			break;
		}

		/*// Advance to next sprite
		 imgrect.x += imgrect.w;
		 if (imgrect.x >= img->w) {
		 imgrect.x = 0;
		 imgrect.y += imgrect.h;
		 if (imgrect.y >= img->h) imgrect.y = 0;
		 }
		 */
		// We finished drawing
		if (SDL_Flip(w) != 0) {
			printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
			break;
		}

		// Poll for keyboard & mouse events
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
			case (SDL_QUIT):
				quit = 1;
				break;
			case (SDL_KEYUP):
				if (e.key.keysym.sym == SDLK_ESCAPE)
					quit = 1;
				break;
			case (SDL_MOUSEBUTTONUP):
				if ((e.button.x > rect.x) && (e.button.x < rect.x + rect.w)
						&& (e.button.y > rect.y)
						&& (e.button.y < rect.y + rect.h))
					quit = 1;
				break;
			default:
				break;
			}
		}

	}

	SDL_FreeSurface(img);

//Quit SDL
	SDL_Quit();

	return 0;
}

gameOptions* newGame() {
	gameOptions* game = malloc(sizeof(gameOptions));

	game->cat_human = humanSelect(1);
	if (game->cat_human == -1)
		return NULL;
	if (!game->cat_human) {
		game->cat_skill = skillSelect(1);
	} else
		game->cat_skill = 0;

	game->mouse_human = humanSelect(0);
	if (game->mouse_human == -1)
		return NULL;
	if (!game->mouse_human) {
		game->mouse_skill = skillSelect(0);
	} else
		game->mouse_skill = 0;

	return game;
}

int skillSelect(int isCat) {
	int i = 1;
	puts("skill select");

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return -1;
	}
	display = SDL_SetVideoMode(500, 600, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);

	int selection = 1;
	TreeRef t2 = newTree(createWidget(0, 0, 0, 0, 600, 600, 0, NULL, "hgh"));
	insertChild(t2, createWidget(100, 0 * 10 + 200 + 0 * BUTTON_HEIGHT,
	BUTTON_WIDTH, 0,
	BUTTON_WIDTH, BUTTON_HEIGHT, 1, "Buttons(1).bmp", ""));
	for (i = 7; i < 9; i++) {
		Widget* b = createWidget(100,
				(i - 6) * 10 + 200 + (i - 6) * BUTTON_HEIGHT, 0,
				i * BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT, 0,
				"Buttons(1).bmp", "");
		insertChild(t2, b);
	}
	nonRecDFS(t2, printWidget);
	nonRecDFS(t2, displayWidget);
	SDL_Delay(2000);
	SDL_Quit();
	return -1;
}

int humanSelect(int isCat) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return 1;
	}
	int i, action = 0;
	display = SDL_SetVideoMode(500, 600, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);

	TreeRef t2 = newTree(createWidget(0, 0, 0, 0, 600, 600, 0, NULL, "hgh"));
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

	int quit = 0;
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
				case SDLK_RETURN:
					puts("enter");
					i = 1;
					cur = getChildWidget(t2, i);
					while (cur != NULL) {
						if (cur->isSelected == 1) {
							printWidget(cur);
							action = i+5;
							printf("%d", action);
							break;
						}
						i++;
						cur = getChildWidget(t2, i);
					} //emd while

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
			return 1;

			break;
		case MACHINE:
			SDL_Quit();
			puts("machine");
			return 0;
			break;
		case BACK:
			SDL_Quit();
			puts("back");
			return -1;
			break;
		}
	}
	SDL_Quit();
	return 0;
}

