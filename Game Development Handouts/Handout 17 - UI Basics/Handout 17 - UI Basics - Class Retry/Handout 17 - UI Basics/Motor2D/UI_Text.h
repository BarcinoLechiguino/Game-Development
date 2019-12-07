#include "p2Point.h"
#include "SDL/include/SDL.h"
#include "UI.h"

class UI_Text : public UI
{
public:
	UI_Text(int x, int y, p2SString text, SDL_Rect rect);

	bool Update();

	bool Draw();

private:

};
