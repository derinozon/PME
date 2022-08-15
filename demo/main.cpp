#include "../PME.h"

// #if TTF == 1
// 	#if DEBUG == 1
// 		#include "fpscounter.h"
// 	#endif
// #endif

// using namespace PME;
using namespace std;

#include "mario.h"
#include "alien.h"

#define SDL_MAIN_HANDLED
#include <SDL.h>

int main (int argv, char** args) {
	
	EngineSettings set = {
		"Game",
		840,
		480,
	};

	if (PME::init(set)) {
		alien();
		//mario();
		PME::run();
	}
    
	return 0;
}