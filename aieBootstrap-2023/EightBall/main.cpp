#include "EightBallApp.h"

int main() {
	
	// allocation
	auto app = new EightBallApp();

	// initialise and loop
	app->run("AIE", 1280, 720, false);

	// deallocation
	delete app;

	return 0;
}