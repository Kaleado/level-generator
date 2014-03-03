#ifndef RENDER_H
#define RENDER_H

#include <vector>
#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_rotozoom.h>
#include <SDL/SDL_gfxPrimitives.h>

int cameraX = 0; int cameraY = 0;
int mouseX, mouseY;
SDL_Surface* screen;
const int SCREEN_W = 640;
const int SCREEN_H = 480;
bool showMouse = true;

//Gets the colour of the pixel at location (x, y) in the given surface, surf.
Uint32 get_pixel32(SDL_Surface* surf, int x, int y)
{
	Uint32 *pixels = (Uint32 *)surf->pixels;
	return pixels[(y*surf->w)+x];
}

SDL_Color translate_color(Uint32 int_color)                           

      //Change from an "int color" to an SDL_Color
{
    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        SDL_Color color={(int_color & 0x00ff0000)/0x10000,(int_color &
0x0000ff00)/0x100,(int_color & 0x000000ff),0};
    #else
        SDL_Color color={(int_color & 0x000000ff),(int_color &
0x0000ff00)/0x100,(int_color & 0x00ff0000)/0x10000,0};    
    #endif
    return color;
}

SDL_Surface* load_text(std::string text, std::string fontPath="FreeMonoBold.ttf", int fontSize=11, SDL_Colour fontColour = {255,255,255}, bool isBlended=false)
{
	TTF_Font *font = TTF_OpenFont(fontPath.c_str(), fontSize);
	SDL_Surface* words = NULL;
	if(font == NULL){std::cout << "Could not open font."; return NULL;}
	if(!isBlended){words=TTF_RenderText_Solid(font, text.c_str(), fontColour);}
	else{words=TTF_RenderText_Blended(font, text.c_str(), fontColour);}
	return words;
}

SDL_Surface* load_image(std::string filename)
{
	//Load an image file.
	SDL_Surface* loadedImage = IMG_Load(filename.c_str());
	SDL_Surface* optimisedImage = NULL;
	//If there was NO problem loading the image file.
	if (loadedImage != NULL)
	{
		optimisedImage = SDL_DisplayFormat(loadedImage);
		//Set colour key, which is cyan.
		Uint32 colourkey = SDL_MapRGB(optimisedImage->format, 0x00, 0xFF, 0xFF);
		SDL_SetColorKey(optimisedImage, SDL_SRCCOLORKEY, colourkey);
	}
	//If there WAS a problem loading the image file.
	else
	{
		std::cout << "Error: Unable to load image - " << SDL_GetError();
		return NULL;
	}
	//Free up memory allocated to the original, loaded image.
	SDL_FreeSurface(loadedImage);
	return optimisedImage;
}

void apply(int x, int y, SDL_Surface* source, SDL_Surface* dest, SDL_Rect* clip = NULL){
	if (source != NULL)
	{
		SDL_Rect offset;
		offset.x = x;
		offset.y = y;
		SDL_BlitSurface(source, clip, dest, &offset);
	}
	else{std::cout << "Error: source is NULL.";}
	return;
}

int initialise_graphics()
{
	srand((time(NULL)));
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	screen = SDL_SetVideoMode(SCREEN_W, SCREEN_H, 32, SDL_SWSURFACE);
	SDL_WM_GrabInput(SDL_GRAB_ON);
	SDL_Delay(2000);
	return 0;
}

#endif
