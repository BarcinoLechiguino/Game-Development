#ifndef __UI_SCROLLBAR_H__
#define __UI_SCROLLBAR_H__

#include "UI.h"
#include "UI_Image.h"

//class UI_Image;

class UI_Scrollbar : public UI
{
public:
	UI_Scrollbar(UI_Element element, int x, int y, SDL_Rect hitbox, SDL_Rect thumbSize, iPoint thumbOffset, SDL_Rect dragArea, float dragFactor,
			bool isVisible = true, bool isInteractible = true, bool isDraggable = false, UI* parent = nullptr);

	bool Draw();

	void CheckInput();

public:
	void DrawScrollbarElements();
	
	void LinkScroll();

public:
	SDL_Texture* tex;
	UI_Image bar;
	UI_Image thumb;
	UI_Image* bar2;
	UI_Image* thumb2;
	
	SDL_Rect	dragArea;
	float		dragFactor;

};
#endif // !__UI_SCROLLBAR_H__