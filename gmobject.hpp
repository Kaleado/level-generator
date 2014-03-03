#ifndef GMOBJECT_H
#define GMOBJECT_H

#include "render.hpp"

class gmObject
{
	private:
	public:
	//X and Y values on the screen.
	int x, y;
	std::string name;
	SDL_Surface* image = NULL;
	bool mouse_is_over()
	{
		//If the mouse is over the object, return true, otherwise, return false.
		return(mouseX > x &&
		mouseX < x+image->w &&
		mouseY > y &&
		mouseY < y+image->h);
	}
	void render()
	{
		apply(x, y, image, screen);
		return;
	}
	void show_tooltip()
	{
		if(name != "")
		{
			showMouse = false;
			SDL_Rect bg;
			TTF_Font *fnt = TTF_OpenFont("FreeMonoBold.ttf", 10);
			bg.x = mouseX;
			bg.y = mouseY;
			bg.w = name.length()*8 + 2;
			bg.h = 8+2;
			SDL_Colour white = {0xff, 0xff, 0xff};
			SDL_FillRect(screen, &bg, 0x00000000);
			SDL_Surface *nameLabel = TTF_RenderText_Blended(fnt, name.c_str(), white);
			apply(bg.x, bg.y, nameLabel, screen);
			SDL_FreeSurface(nameLabel);
			TTF_CloseFont(fnt);
		}
		return;
	}
	gmObject(){}
	gmObject(SDL_Surface* image, int x, int y, std::string name = "")
	{
		this->image = image;
		this->x = x;
		this->y = y;
		this->name = name;
	}
};

class gmGridObject : public gmObject
{
	public:
	//X and Y values on the game grid.
	int gX, gY;
	void render()
	{
		//Set X and Y coordinates to line up with the grid.
		x = gX*40+(gY%2!=0)*20-cameraX;
		y = gY*(21/2+1)-19-cameraY;
		apply(x, y, image, screen);
	}
	gmGridObject(SDL_Surface* image, int gX, int gY, std::string name = "")
	{
		this->image = image;
		this->gX = gX;
		this->gY = gY;
		this->name = name;
	}
};

#endif
