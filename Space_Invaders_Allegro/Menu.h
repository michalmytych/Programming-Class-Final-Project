#pragma once
#include <allegro5/allegro.h>
#include "Window.h"
#include "IView.h"

class Menu : public IView
{
private:
	Window* window;

public:
	Menu(Window* window);

	void runViewLoop();
};

