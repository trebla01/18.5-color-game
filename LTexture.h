#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <cmath>
#include <SDL_ttf.h>

//Texture wrapper class
class LTexture
{
public:
	//constructure, initializes variables
	LTexture();

	//Deallocates memory, calls free
	~LTexture();

	//loads image at specific path
	bool loadFromFile(std::string path, SDL_Renderer* gRenderer);

	//Creates image from font string
	bool loadFromRenderedText(std::string textureText, SDL_Color textColor, SDL_Color bgColor, TTF_Font* gFont, SDL_Renderer* gRenderer);

	//Dellocates texture
	void free();

	//Renders texture at given point
	void render(int x, int y, SDL_Renderer* gRenderer, SDL_Rect* clip = NULL);

	//Gets image dimensions
	int getWidth();
	int getHeight();

private:
	//The actual hardware texture
	SDL_Texture* mTexture;

	//Image dimensions
	int mWidth;
	int mHeight;
};
