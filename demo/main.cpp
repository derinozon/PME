#include "../PME.h"

#if TTF == 1
	#if DEBUG == 1
		#include "fpscounter.h"
	#endif
#endif

using namespace PME;
using namespace std;

#include "mario.h"
#include "alien.h"

int main (int argv, char** args) {
	
	EngineSettings set = {
		"Game",
		840,
		480,
	};

	if (PME::init(set)) {
		mario();
		PME::run();
	}

	
    
	return 0;
}