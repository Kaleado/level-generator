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
SDL_Surface* mouseCursor = NULL;

void get_map()
{
	currentMap.make();
	currentMap.background = load_image("background/background.png");
	return;
}

void load_tiles()
{
	SDL_Surface* cursor = load_image("tile/cursor.png");
	
	//Render the map background.
	apply(0,0, currentMap.background, screen);
	
	//Get the tile the mouse is currently over.
	gmTile* oT = get_hovering_tile();
	//Render all the tiles.
	for(int x = 0; x < MAP_W-1; x++)
	{
		for(int y = 0; y < MAP_H-1; y++)
		{
			//Place the tile.
			apply(x*40+(y%2!=0)*20-cameraX, y*(21/2+1)-19-cameraY, currentMap.tile[y][x].image, screen);
			//Place the cursor. It's very jumpy, and I'm not sure why.
			if(&currentMap.tile[y][x] == oT){apply(x*40+(y%2!=0)*20-cameraX, y*(21/2+1)-19-cameraY, cursor, screen);}
		}
	}
	SDL_FreeSurface(cursor);
	return;
}

void render_objects()
{
	for(uint i=1; i<currentMap.objects.size(); i++)
	{
		currentMap.objects[i-1].render();
		if(currentMap.objects[i-1].mouse_is_over())
		{
			currentMap.objects[i-1].show_tooltip();
		}
	}
	return;
}

void show_cursor()
{
	if(showMouse)
	{
		if(mouseCursor == NULL){mouseCursor = load_image("mouse.png");}
		apply(mouseX, mouseY, mouseCursor, screen);
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
	SDL_ShowCursor(0);
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
			if(event.type == SDL_QUIT){running = false;}
			if(event.type == SDL_KEYDOWN)
			{
				if(event.key.keysym.sym == SDLK_q){get_map(); load_tiles();}
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
		//Show the tiles.
		load_tiles();
		//Show the objects over the tiles.
		render_objects();
		//Show the minimap.
		if(showMinimap){draw_minimap();}
		//Show the mouse cursor.
		show_cursor();
		//Reset the cursor.
		showMouse = true;
		SDL_Flip(screen);
	}
	return;
}

void halt()
{
	SDL_Quit();
	TTF_Quit();
	return;
}

int main(int argc, char *argv[])
{
	init();
	run();
	halt();
	return 0;
}
