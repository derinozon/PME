# PME
Lightweight header only SDL2 engine that can build desktop/web games. <br>
Its in really early development stage and shouldn't be used in commercial projects.

Installation
------------

You need SDL dynamic libraries to use this engine. <br>
If you are on a mac, i recommend getting them from Homebrew.

Mac:

``` bash
$ brew install sdl2
#optional
$ brew install sdl2_image sdl2_ttf sdl2_mixer
```

Linux:
``` bash
sudo apt install libsdl2-dev
#optional
sudo apt install libsdl2-mixer-dev libsdl2-ttf-dev libsdl2-image-dev
```

Windows: <br>
You can download the development libraries [here](https://www.libsdl.org/download-2.0.php)

Usage
--------

Copy include/PME.h in your project and include it in your main script. <br>
Minimal example:

``` cpp
#include "./include/PME.h"

int main (int argv, char** args) {
	
	if (PME::init()) {
		PME::Debug::Log("Hello world");
		PME::run();
	}
	
	return 0;
}
```

Compile it on unix using the following command.
``` bash
$ g++ main.cpp -lSDL2
```
If you are compiling with mingw don't forget to include this flag: -lmingw32

Games Made With PME
--------

[Catch Me!](https://derinozon.itch.io/catchme)