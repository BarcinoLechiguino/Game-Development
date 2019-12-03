#ifndef __j1UI_H__
#define __j1UI_H__

class UI
{
public:
	UI(int x, int y, SDL_Rect rect);
	
	virtual ~UI();

	virtual bool Update();

	virtual bool Draw();
	
private:
	SDL_Rect	rectangle;
	iPoint		position;
};

#endif // __j1UI_H__