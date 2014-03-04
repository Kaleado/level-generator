#ifndef GMMAPMAKER_H
#define GMMAPMAKER_H

#include <ctime>
#include <cstdlib>
#include <fstream>
#include "render.hpp"
#include "gmobject.hpp"

const int MAP_W = 30;
const int MAP_H = 30;

class gmTile : public gmObject
{
	private:
	public:
	SDL_Surface* image = NULL;
	gmTile(){}
	gmTile(SDL_Surface* image)
	{
		this->image = image;
	}
};

class gmMap
{
	private:
	public:
	//The minimap generated when the map is made.
	SDL_Surface* minimap;
	//A nice image to display behind the tiles.
	SDL_Surface* background;
	gmTile tile[MAP_H][MAP_W];
	std::vector <gmGridObject> objects;
	int draw_minimap()
	{
		float scaleFactor = (1.0/((40.0 * (MAP_W+1)) / 160.0));
		apply(0,0,minimap,screen);
		rectangleRGBA(screen, cameraX*scaleFactor,cameraY*scaleFactor, 
						(160 > SCREEN_W*scaleFactor 	? SCREEN_W*scaleFactor	: 160)+cameraX*scaleFactor,
						(120 > SCREEN_H*scaleFactor 	? SCREEN_H*scaleFactor	: 120)+cameraY*scaleFactor,
						0xFF, 0xFF, 0xFF, 0xFF);
		return 0;
	}
	//Function to produce a minimap surface given the current map.
	SDL_Surface* get_minimap()
	{
		float scaleFactor = (1.0/((40.0 * (MAP_W+1)) / 160.0));
		int heightOverlapping = (20*scaleFactor)/2+1;
		int yOffset = (40*scaleFactor)-(20*scaleFactor);
		SDL_Surface* newMinimap = SDL_CreateRGBSurface(0, 160, 120, 32, 0, 0, 0, 0);
		SDL_Surface* miniTile = NULL;
		for(int x=0; x<MAP_W; x++){for(int y=0; y<MAP_H; y++){
			miniTile = rotozoomSurface(tile[y][x].image, 0, scaleFactor, 0);
			apply(x*(40*scaleFactor)+(y%2!=0)*(20*scaleFactor), y*heightOverlapping-yOffset, miniTile, newMinimap);
		}}
		SDL_FreeSurface(miniTile);
		return newMinimap;
	}
	//Function to create the map.
	int make()
	{
		int numObjects = 12;
		SDL_Surface *sand, *gravel, *ocean, *grass;
		sand = load_image("tile/sand.png");
		gravel = load_image("tile/gravel.png" );
		ocean = load_image("tile/ocean.png" );
		grass = load_image("tile/grass.png");
		SDL_Surface* terrain[] = {grass, ocean, gravel, sand};
		int iterations = 1;		//Good values are 25, 40
		int tileData[MAP_H][MAP_W];
		for(int x = 0; x < MAP_W; x++)
		{
			for(int y = 0; y < MAP_H; y++)
			{
				tileData[y][x] = rand()%4;
			}
		}
		for(int x = 0; x < MAP_W; x++)
		{
			for(int y = 0; y < MAP_H; y++)
			{
				if(tileData[x][y] == 0){tile[y][x].image = sand;}
				else if(tileData[x][y] == 1){tile[y][x].image = gravel;}
				else if(tileData[x][y] == 2){tile[y][x].image = ocean;}
				else 				 		{tile[y][x].image = grass;}
			}
		}
		for(int i = 0; i < iterations; i++)
		{
			for(int x = 1; x < MAP_W-1; x++){for(int y = 1; y < MAP_H-1; y++){
				for(int j = 0;j<4;j++)
				{
					int adjacentTiles = 0;
					if(terrain[j] == tile[y-1][x-1].image)		{adjacentTiles++;}
					if(terrain[j] == tile[y-1][x].image)		{adjacentTiles++;}
					if(terrain[j] == tile[y-1][x+1].image)		{adjacentTiles++;}
					if(terrain[j] == tile[y][x-1].image)		{adjacentTiles++;}
					if(terrain[j] == tile[y][x+1].image)		{adjacentTiles++;}
					if(terrain[j] == tile[y+1][x-1].image)		{adjacentTiles++;}
					if(terrain[j] == tile[y+1][x].image)		{adjacentTiles++;}
					if(terrain[j] == tile[y+1][x+1].image)		{adjacentTiles++;}
					if(adjacentTiles >= 4){tile[y][x].image = terrain[j];break;}
				}
			}}
		}
		if(!objects.empty()){objects = std::vector <gmGridObject>();}
		for(int i = 0; i<numObjects; i++)
		{
			objects.push_back(gmGridObject(load_image("object/object.png"), rand()%(MAP_W-1), rand()%(MAP_H-1), "daah"));
		}
		minimap = get_minimap();
		return 0;
	}
}currentMap;

#endif
