#include "p2Point.h"
#include "SDL/include/SDL.h"
#include "UI.h"

class UI_Image : public UI
{
public:
	UI_Image(UI_ELEMENT element, int x, int y, SDL_Rect rect);

	bool Update();

	bool Draw();

private:

};