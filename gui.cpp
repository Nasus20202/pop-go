#include "gui.h"
#include "go.h"
#include "conio2.h"

#define TITLE "Krzysztof, Nasuta, 193328"

Gui::Gui() {
	settitle(TITLE);
	_setcursortype(_NOCURSOR);
}

Gui::~Gui()
{
	_setcursortype(_NORMALCURSOR);
	cputs("\n\nProgram terminated\n");
}