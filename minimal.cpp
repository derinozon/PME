#include "./include/PME.h"

int main (int argv, char** args) {
	
	if (PME::init()) {
		Debug::Log("Hello world");
		PME::run();
	}
	
	return 0;
}