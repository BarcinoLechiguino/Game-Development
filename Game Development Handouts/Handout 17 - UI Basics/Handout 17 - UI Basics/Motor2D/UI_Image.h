#ifndef __UI_IMAGE_H__
#define __UI_IMAGE_H__

//#include "j1Textures.h"
#include "UI.h"			//UI_Image is included in UI.h (?).

class UI_Image : public UI
{
public:
	UI_Image(UI_Element element, int x, int y, SDL_Rect imgRect, bool isInteractible = false, bool isDraggable = false, UI* parent = nullptr);

	bool Draw();

	void CheckInput();

private:
	
	SDL_Texture*	idleTex;			//Texture for the idle state of the text.
	SDL_Texture*	hoverTex;			//Texture for the hover state of the text.
	SDL_Texture*	focusTex;			//Texture for the focused state fo the text.
	SDL_Texture*	leftClickTex;		//Texture for the left-Clicked state of the text.
	SDL_Texture*	rightClickTex;		//Texture for the right-Clicked state of the text.
	SDL_Texture*	currentTex;			//UI image texture.
};

#endif // !__UI_IMAGE_H__