#include "gui.h"
#include "tests.cpp"

int main() {
	// if the program is compiled in pure C, then we need to initialize the library ourselves; __cplusplus is defined only if a C++ compiler is used
#ifndef __cplusplus
	Conio2_Init();
#endif
	Tests tests = Tests(); // Run tests
	tests.runTests();      // only for development
	Gui gui = Gui::Gui();  // Initialize main GUI object
	return 0;
}
