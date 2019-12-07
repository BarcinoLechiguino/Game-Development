#ifndef __UI_IMAGE_H__
#define __UI_IMAGE_H__

//#include "p2Point.h"
#include "j1Textures.h"
#include "UI.h"			//UI_Image is included in UI.h (?).

//class iPoint;
//class SDL_Rect;
//class SDL_Texture;

class UI_Image : public UI
{
public:
	UI_Image(UI_Element element, int x, int y, SDL_Rect imgRect);

	bool Draw();

private:
	iPoint imgPosition;
	//SDL_Rect rect;
	SDL_Texture* tex;		//UI image texture.
};

#endif // !__UI_IMAGE_H__