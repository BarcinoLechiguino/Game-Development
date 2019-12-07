#include "UI.h"


class UI_Button : public UI
{
public:

	UI_Button(UI_Element element, int x, int y, SDL_Rect rect);

	bool Draw();

private:

};