#include<stdio.h>
#include "conio2.h"
#include "gui.h"
#include "go.h"


int main() {
	// if the program is compiled in pure C, then we need to initialize the library ourselves; __cplusplus is defined only if a C++ compiler is used
#ifndef __cplusplus
	Conio2_Init();
#endif
	GUI gui =  GUI::GUI();
	return 0;
}
