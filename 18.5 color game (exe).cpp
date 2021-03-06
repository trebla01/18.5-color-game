/*
Color Game
Author: Albert Chen
Date: 5/29/15
*/
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
#include <cmath>
#include "LTexture.h"
#include <time.h> 
#include <SDL_ttf.h>

using namespace std;

//Starting Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int TOTAL_BUTTONS = 9;
const int BUTTON_WIDTH = SCREEN_WIDTH / 3;
const int BUTTON_HEIGHT = SCREEN_HEIGHT / 3;
const int DIFFICULTY = 8; //1 = hardest 

const int INTRO_SCREEN = 0;
const int IN_GAME  = 1;
const int GAME_OVER = 2;
const int QUIT_GAME = 3;

enum LButtonSprite
{
	BUTTON_SPRITE_MOUSE_OUT = 0,  
	BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
	BUTTON_SPRITE_MOUSE_DOWN = 2,
	BUTTON_SPRITE_MOUSE_UP = 3,
	BUTTON_SPRITE_TOTAL = 4
};



//The mouse button
class LButton
{
public:
	//Initializes internal variables
	LButton();

	//Sets top left position
	void setPosition(int x, int y);

	//Handles mouse event
	bool handleEvent(SDL_Event* e);

	//Shows button sprite
	void render();

private:
	//Top left position
	SDL_Point mPosition;

	//Currently used global sprite
	LButtonSprite mCurrentSprite;
};

//Starts up SDL and creates window
bool init();

//load media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Scene textures
LTexture gGameOverTexture;
LTexture gTextTexture;
LTexture gIntroTexture;

//Buttons objects
LButton gButtons[TOTAL_BUTTONS];

//Globally used font
TTF_Font *gFont = NULL;


LButton::LButton()
{
	mPosition.x = 0;
	mPosition.y = 0;

	mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
}

void LButton::setPosition(int x, int y)
{
	mPosition.x = x;
	mPosition.y = y;
}

bool LButton::handleEvent(SDL_Event* e)
{
	//If mouse event happened
	if (e->type == SDL_MOUSEBUTTONUP)
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState(&x, &y);

		//Check if mouse is in button
		bool inside = true;

		//Mouse is left of the button
		if (x < mPosition.x)
		{
			inside = false;
		}
		//Mouse is right of the button
		else if (x > mPosition.x + BUTTON_WIDTH)
		{
			inside = false;
		}
		//Mouse above the button
		else if (y < mPosition.y)
		{
			inside = false;
		}
		//Mouse below the button
		else if (y > mPosition.y + BUTTON_HEIGHT)
		{
			inside = false;
		}

		return inside;
	}

	else
		return false;
}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		cout << "SDL could not initialize! SDL Error: " << SDL_GetError();
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			cout << "Warning: linear texture filtering not enabled";
		}

		//Create Window
		gWindow = SDL_CreateWindow(
			"18.5 Color Game",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			cout << "Window could not be created! SDL Error: " << SDL_GetError();
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL)
			{
				cout << "Renderer could not be created! SDL_Error: " << SDL_GetError();
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					cout << "SDL_image could not initialize! SDL_image Error: " << SDL_GetError();
					success = false;
				}
				//Initialize SDL_ttf
				if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					success = false;
				}

				//Initialize gButton
				gButtons[0].setPosition(0, 0);
				gButtons[1].setPosition(SCREEN_WIDTH / 3, 0);
				gButtons[2].setPosition(2 * SCREEN_WIDTH /3 , 0 );
				gButtons[3].setPosition(0, SCREEN_HEIGHT / 3);
				gButtons[4].setPosition(SCREEN_WIDTH / 3, SCREEN_HEIGHT / 3);
				gButtons[5].setPosition(2 * SCREEN_WIDTH / 3, SCREEN_HEIGHT / 3);
				gButtons[6].setPosition(0, 2 * SCREEN_HEIGHT / 3);
				gButtons[7].setPosition(SCREEN_WIDTH / 3, 2 * SCREEN_HEIGHT / 3);
				gButtons[8].setPosition(2 * SCREEN_WIDTH / 3, 2 * SCREEN_HEIGHT / 3);
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Open the font
	gFont = TTF_OpenFont("WeLoveCuteThings.ttf", 36);
	if (gFont == NULL)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}

	//Load texture
	if (!gGameOverTexture.loadFromFile("colorgame_game_over.png", gRenderer))
	{
		printf("Failed to load front texture!\n");
		success = false;
	}

	if (!gIntroTexture.loadFromFile("colorgame_intro_screen.png", gRenderer))
	{
		printf("Failed to load front texture!\n");
		success = false;
	}

	return success;
}

void close()
{
	//Free loaded images
	gGameOverTexture.free();
	gIntroTexture.free();
	gTextTexture.free();

	//Destroy Window
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}
int main(int argc, char* args[])
{
	if (!init())
	{
		cout << "Failed to initialize!" << endl;
	}
	else
	{
		//load media
		if (!loadMedia())
		{
			cout << "Failed to load media!" << endl;
		}
		else
		{
			//main loop flag
			int game_state = INTRO_SCREEN;
			
			//Starting screen width and height
			int width = SCREEN_WIDTH;
			int height = SCREEN_HEIGHT;

			//color components
			Uint8 r = 0;
			Uint8 g = 255;
			Uint8 b = 255;
			Uint8 a = 255;

			//the selected box
			int selected = 0;
			int rgbRand = 1;
			int randAmount = 128;

			//initialize rand
			srand(time(NULL));

			//initialize score
			int score = 0;

			//Event handler
			SDL_Event e;;

			while (!(game_state == QUIT_GAME))
			{
				if (game_state == INTRO_SCREEN)
				{
					while (SDL_PollEvent(&e) != 0)
					{
						//User requests quit
						if (e.type == SDL_QUIT)
						{
							game_state = QUIT_GAME;
						}
						for (int i = 0; i < TOTAL_BUTTONS; i++)
						{
							if (gButtons[i].handleEvent(&e))
							{
								game_state = IN_GAME;
							}
						}
					}
					gIntroTexture.render(0, 0, gRenderer);
					SDL_RenderPresent(gRenderer);
				}
				else if (game_state == IN_GAME)
				{
					//Handle events on queue
					while (SDL_PollEvent(&e) != 0)
					{
						//User requests quit
						if (e.type == SDL_QUIT)
						{
							game_state = QUIT_GAME;
						}
	
						//Handle user selection
						int boxClicked = -1;
						for (int i = 0; i < TOTAL_BUTTONS; i++)
						{
							if (gButtons[i].handleEvent(&e))
							{
								boxClicked = i;
							}
						}
						if (boxClicked >= 0)
						{

							if (boxClicked == selected)
							{
								r = rand() % 256;
								g = rand() % 256;
								b = rand() % 256;
								a = rand() % 256;
								selected = rand() % 9;

								rgbRand = rand() % 3;
								randAmount = DIFFICULTY; // + rand() % 16
								score++;
							}
							else
							{
								game_state = GAME_OVER;
							}
						}
					}

					//get window sizeAE
					SDL_GetWindowSize(gWindow, &width, &height);

					//Clear screen
					SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
					SDL_RenderClear(gRenderer);

					SDL_Rect colorBox[9];
					colorBox[0] = { 0, 0, width / 3, height / 3 };
					colorBox[1] = { width / 3, 0, width / 3, height / 3 };
					colorBox[2] = { width * 2 / 3, 0, width / 3, height / 3 };
					colorBox[3] = { 0, height / 3, width / 3, height / 3 };
					colorBox[4] = { width / 3, height / 3, width / 3, height / 3 };
					colorBox[5] = { width * 2 / 3, height / 3, width / 3, height / 3 };
					colorBox[6] = { 0, 2 * height / 3, width / 3, height / 3 };
					colorBox[7] = { width / 3, 2 * height / 3, width / 3, height / 3 };
					colorBox[8] = { width * 2 / 3, 2 * height / 3, width / 3, height / 3 };

					SDL_SetRenderDrawColor(gRenderer, r, g, b, a);
					for (int i = 0; i < 9; i++)
					{
						SDL_RenderFillRect(gRenderer, &colorBox[i]);
					}

					switch (rgbRand)
					{
					case 0:
						SDL_SetRenderDrawColor(gRenderer, r - randAmount, g, b, a);
					case 1:
						SDL_SetRenderDrawColor(gRenderer, r, g - randAmount, b, a);
					case 2:
						SDL_SetRenderDrawColor(gRenderer, r, g, b - randAmount, a);
					}
					SDL_RenderFillRect(gRenderer, &colorBox[selected]);

					SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
					for (int i = 0; i < 9; i++)
					{
						SDL_RenderDrawRect(gRenderer, &colorBox[i]);
					}
					SDL_RenderPresent(gRenderer);
				}
				else if (game_state == GAME_OVER)
				{
					while (SDL_PollEvent(&e) != 0)
					{
						//User requests quit
						if (e.type == SDL_QUIT)
						{
							game_state = QUIT_GAME;
						}
						for (int i = 0; i < TOTAL_BUTTONS; i++)
						{
							if (gButtons[i].handleEvent(&e))
							{
								game_state = INTRO_SCREEN;
								score = 0;
							}
						}
					}
					gGameOverTexture.render(0, 0, gRenderer);
					//Render text
					SDL_Color textColor = { 0, 0, 0 };
					SDL_Color bgColor = { 255, 255, 255 };
					std::stringstream ss;
					ss << score;
					string scoreStr;
					ss >> scoreStr;

					gTextTexture.loadFromRenderedText("Your final score: " + scoreStr, textColor, bgColor, gFont, gRenderer);
					gTextTexture.render(0, 20, gRenderer);
					SDL_RenderPresent(gRenderer);
				}
			}
		}
	}
	return 0;
}