#pragma once

#include <thread>

namespace PME {
	void _call (void (*ptr)(), int time) {
		SDL_Delay(time);
		ptr();
	}
	
	void Invoke (void (*ptr)(), float seconds) {
		std::thread first (_call, ptr, (int)(seconds*1000));
		first.detach();
	}	



	Uint32 callback (Uint32 interval, void (*ptr)()) {
		Debug.Log(ptr);
		
		return 0;
	}
}

