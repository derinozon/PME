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
$ brew install sdl2_image
$ brew install sdl2_ttf
$ brew install sdl2_mixer
```

Windows: <br>
You can download the development libraries [here](https://www.libsdl.org/download-2.0.php)

Linux:
``` bash

```

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