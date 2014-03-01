#include <math.h>
#include <iostream>
#include <ctime>
#include <string>
#include "render.hpp"
#include "gmmapmaker.hpp"
#include "gmobject.hpp"
#include "gmminimap.hpp"
#include "gmgridcursor.hpp"

SDL_Event event;
bool running = true;
bool showMinimap = true;

void get_map()
{
	currentMap.make();
	currentMap.background = load_image("background/background.png");
	return;
}

void load_tiles()
{
	//Handy variables for the isometric tiles.
	SDL_Surface* cursor = load_image("tile/cursor.png");
	int heightOverlapping = (21)/2+1;
	int yOffset = 40-21;
	
	//Render the map background.
	apply(0,0, currentMap.background, screen);
	
	//Get the tile the mouse is currently over.
	gmTile* oT = get_hovering_tile();
	//Render all the tiles.
	for(int x = 0; x < MAP_W; x++)
	{
		for(int y = 0; y < MAP_H; y++)
		{
			int heightOverlapping = (21)/2+1;
			int yOffset = 40-21;
			apply(x*40+(y%2!=0)*20-cameraX, y*heightOverlapping-yOffset-cameraY, currentMap.tile[y][x].image, screen);
			//Place the cursor. It's very jumpy, and I'm not sure why.
			if(&currentMap.tile[y][x] == oT){apply(x*40+(y%2!=0)*20-cameraX, y*heightOverlapping-yOffset-cameraY, cursor, screen);}
		}
	}
	return;
}

void set_camera()
{
	int scrollSpeed = 2;
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	//If the mouse is at any of the borders of the screen, move the camera.
	if(mouseX == 0)			{cameraX -= scrollSpeed;}
	if(mouseX == SCREEN_W-1){cameraX += scrollSpeed;}
	if(mouseY == 0)			{cameraY -= scrollSpeed;}
	if(mouseY == SCREEN_H-1){cameraY += scrollSpeed;}
	
	if(cameraX < -SCREEN_W+40)		{cameraX = -SCREEN_W+40;}
	if(cameraY < -SCREEN_H+20)		{cameraY = -SCREEN_H+20;}
	if(cameraX > (MAP_W)*40)		{cameraX = (MAP_W)*40;}
	if(cameraY > (MAP_H)*10)		{cameraY = (MAP_H)*10;}
}

void init()
{
	srand((time(NULL)));
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	screen = SDL_SetVideoMode(SCREEN_W, SCREEN_H, 32, SDL_SWSURFACE);
	SDL_WM_GrabInput(SDL_GRAB_ON);
	SDL_Delay(2000);
	return;
}

void run()
{
	get_map();
	load_tiles();
	while(running)
	{
		while(SDL_PollEvent(&event))
		{
			if(event.type == SDL_KEYDOWN)
			{
				if(event.key.keysym.sym == SDLK_q){get_map();load_tiles();}
				if(event.key.keysym.sym == SDLK_p){running = false;}
				if(event.key.keysym.sym == SDLK_m){showMinimap ? showMinimap=false:showMinimap=true;}
			}
			if(event.type == SDL_MOUSEBUTTONDOWN)
			{
				if(event.button.button == SDL_BUTTON_RIGHT)
				{
					cameraX += event.button.x-SCREEN_W/2;
					cameraY += event.button.y-SCREEN_H/2;
				}
			}
			if (event.type == SDL_MOUSEMOTION)
			{
			}
		}
		mouseX = event.motion.x;
		mouseY = event.motion.y;
		set_camera();
		load_tiles();
		if(showMinimap){draw_minimap();}
		SDL_Flip(screen);
	}
	return;
}

void halt()
{
	return;
}

int main()
{
	init();
	run();
	halt();
	return 0;
}
