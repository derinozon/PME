#pragma once
#include <typeinfo>

#include "../lib/stb_image.h"


namespace PME {
	namespace Resources {

		SDL_Surface *LoadImage(const char* path) {
			// Read data
			int width, height, bytesPerPixel;
			void* data = stbi_load(path, &width, &height, &bytesPerPixel, 0);

			// Calculate pitch
			int pitch;
			pitch = width * bytesPerPixel;
			pitch = (pitch + 3) & ~3;

			// Setup relevance bitmask
			uint32_t Rmask, Gmask, Bmask, Amask;
		#if SDL_BYTEORDER == SDL_LIL_ENDIAN
			Rmask = 0x000000FF;
			Gmask = 0x0000FF00;
			Bmask = 0x00FF0000;
			Amask = (bytesPerPixel == 4) ? 0xFF000000 : 0;
		#else
			int s = (bytesPerPixel == 4) ? 0 : 8;
			Rmask = 0xFF000000 >> s;
			Gmask = 0x00FF0000 >> s;
			Bmask = 0x0000FF00 >> s;
			Amask = 0x000000FF >> s;
		#endif
			SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(data, width, height, bytesPerPixel*8, pitch, Rmask, Gmask, Bmask, Amask);
			if (!surface)
			{
				// NOTE: Should free stbi_load 'data' variable here
				return NULL;
			}
			return surface;
		}

		Texture* LoadTexture (const char* path) {
			#if IMG == 1
			return IMG_LoadTexture(renderer, path);
			#else
			return SDL_CreateTextureFromSurface(renderer, LoadImage(path));
			//return SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP(path));
			#endif
		}
		
		#if MIX == 1
		AudioClip* LoadAudio (const char* path) {
			AudioClip* audio = Mix_LoadWAV(path);
			if (!audio) {
				Debug.LogError(Mix_GetError());
			}
			return audio;
		}
		#endif

		// template <typename T>
		// T* Load (const char* path) {
		// 	const char* n = typeid(T).name();
		// 	Debug.Log(n);

		// 	//if (typeid(T) == typeid(SDL_Texture)) {
		// 	//	return LoadTexture(path);
		// 	//}
			
		// 	return nullptr;
		// }
	}
}