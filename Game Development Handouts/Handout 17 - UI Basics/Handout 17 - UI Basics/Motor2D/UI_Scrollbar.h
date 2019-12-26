#ifndef __UI_SCROLLBAR_H__
#define __UI_SCROLLBAR_H__

#include "UI.h"
#include "UI_Image.h"

//class UI_Image;

class UI_Scrollbar : public UI
{
public:
	UI_Scrollbar(UI_Element element, int x, int y, SDL_Rect hitbox, SDL_Rect thumbSize, iPoint thumbOffset, SDL_Rect dragArea, float dragFactor, bool dragXAxis = false, bool dragYAxis = true,
		bool invertedScrolling = false, bool isVisible = true, bool isInteractible = true, bool isDraggable = false,
		UI* parent = nullptr, SDL_Rect* scrollMask = nullptr, iPoint maskOffset = iPoint(0, 0));

	bool Draw();

	void CheckInput();

public:
	void DrawScrollbarElements();
	void LinkScroll(UI* element);
	void UpdateLinkedElements();
	float GetDragFactor(UI* element);

	bool GetDragXAxis();
	bool GetDragYAxis();
	void DragThumbWithMousewheel();
	bool ThumbIsWithinScrollbarBounds();
	bool ThumbAtUpperBound();
	bool ThumbAtLowerBound();

private:
	SDL_Texture* tex;
	//UI_Image bar;
	//UI_Image thumb;
	//UI_Image scrollMask;

	UI_Image* bar;
	UI_Image* thumb;
	UI_Image* scrollMask;
	
	SDL_Rect	dragArea;
	float		dragFactor;
	iPoint		dragDisplacement;
	iPoint		mouseWheelScroll;
	bool		invertedScrolling;
	int			thumbPos;

	p2List<UI*> linkedElements;

	SDL_Rect	currentRect;
};
#endif // !__UI_SCROLLBAR_H__