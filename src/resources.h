#pragma once
#include <typeinfo>
namespace PME {
	namespace Resources {
		Texture* LoadTexture (const char* path) {
			#if IMG == 1
			return IMG_LoadTexture(renderer, path);
			#else
			return SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP(path));
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