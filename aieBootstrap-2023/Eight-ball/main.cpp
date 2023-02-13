#include "Eight_ballApp.h"

int main() {
	
	// allocation
	auto app = new Eight_ballApp();

	// initialise and loop
	app->run("AIE", 1280, 720, false);

	// deallocation
	delete app;

	return 0;
}