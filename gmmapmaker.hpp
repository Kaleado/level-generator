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
	//A nice image to display behind the tiles.
	SDL_Surface* background;
	gmTile tile[MAP_H][MAP_W];
	//Function to create the map.
	int make()
	{
		SDL_Surface* sand, *gravel, *ocean, *rock, *grass;
		sand = load_image("tile/sand.png");
		gravel = load_image("tile/gravel.png" );
		ocean = load_image("tile/ocean.png" );
		rock = load_image("tile/rock.png" );
		grass = load_image("tile/grass.png");
		SDL_Surface* terrain[] = {grass, gravel, ocean, sand, rock};
		int iterations = 25;		//Good values are 40, 
		int tileData[MAP_H][MAP_W];
		for(int x = 0; x < MAP_W; x++)
		{
			for(int y = 0; y < MAP_H; y++)
			{
				tileData[y][x] = rand()%5;
			}
		}
		for(int x = 0; x < MAP_W; x++)
		{
			for(int y = 0; y < MAP_H; y++)
			{
				if(tileData[x][y] == 0){tile[y][x].image = sand;}
				else if(tileData[x][y] == 1){tile[y][x].image = gravel;}
				else if(tileData[x][y] == 2){tile[y][x].image = ocean;}
				else if(tileData[x][y] == 3){tile[y][x].image = rock;}
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
		return 0;
	}
}currentMap;

#endif
