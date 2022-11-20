#pragma once

#ifndef COLORTERM
	#define COLORTERM 1
#endif

#ifndef DEBUG
	#define DEBUG 1
#endif
// 0 web 1 pc
#ifndef PORT
	#define PORT 1
#endif

#ifndef IMG
	#define IMG 2
#endif

#ifndef MIX
	#define MIX 0
#endif

#ifndef TTF
	#define TTF 0
#endif



//#if defined(_WIN32) || defined(WIN32)

#if PORT == 0
	#include <emscripten.h>
#endif

#include <string.h>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <random>
#include <sstream>

#define SDL_MAIN_HANDLED
#include <SDL.h>

#if IMG == 1
	#include <SDL2/SDL_image.h>
#endif

#if MIX == 1
	#include <SDL2/SDL_mixer.h>
#endif

#if TTF == 1
	#include <SDL2/SDL_ttf.h>
#endif

#if USEGL == 1
	
	#define GL_SILENCE_DEPRECATION
	#include <GL/glew.h>
	#include <glm/glm.hpp>
	
	#if PORT == 0
		//#include <GLES2/gl2.h>
		#include <GLES3/gl3.h>
	#else
		#ifdef __APPLE__
			#include <OpenGL/gl.h>
		#endif
		#ifdef __linux__
			#include <GL/gl.h>
		#endif
	#endif
	
	#include <SDL2/SDL_opengl.h>
#endif

#if MIX == 1
typedef Mix_Chunk AudioClip;
#endif

typedef SDL_Texture Texture;

// Lighweight SDL2 engine
namespace PME {

	const char* RESET   	= COLORTERM ? "\033[0m"			: "";
	const char* RED     	= COLORTERM ? "\033[31m"		: "";
	const char* GREEN   	= COLORTERM ? "\033[32m"		: "";
	const char* YELLOW  	= COLORTERM ? "\033[33m"		: "";
	const char* WHITE   	= COLORTERM ? "\033[37m"		: "";
	const char* BOLDRED     = COLORTERM ? "\033[1m\033[31m" : "";
	const char* BOLDGREEN   = COLORTERM ? "\033[1m\033[32m" : "";
	const char* BOLDYELLOW  = COLORTERM ? "\033[1m\033[33m" : "";
	const char* BOLDWHITE 	= COLORTERM ? "\033[1m\033[37m" : "";

	Uint16 maxFPS = 60;
	Uint16 ppu = 100;

	struct EngineSettings {
		Uint16 windowWidth = 640;
		Uint16 windowHeight = 480;

		std::string windowTitle = "Game";
		bool vsync;
		bool highDPI;
		bool resizable;

		EngineSettings (
			std::string windowTitle = "Game",
			Uint16 windowWidth = 640,
			Uint16 windowHeight = 480,
			bool vsync = true,
			bool highDPI = false,
			bool resizable = true
		)
		{
			this->windowTitle = windowTitle;
			this->windowWidth = windowWidth;
			this->windowHeight = windowHeight;
			this->vsync = vsync;
			this->highDPI = highDPI;
			this->resizable = resizable;
		}
	};
	

	SDL_Window* window;
	SDL_Renderer* renderer;

	
	
	SDL_Color clearColor = {128,128,128,255};
}

#include "src/internal.h"

#include "src/Structs/rect.h"
#include "src/Structs/vector2.h"

#include "src/debug.h"
#include "src/random.h"
#include "src/math.h"
#include "src/physics.h"
#include "src/audio.h"
#include "src/time.h"
#include "src/resources.h"
#include "src/input.h"
#include "src/scene.h"
#include "src/component.h"
#include "src/entity.h"

namespace PME {
	Vector2 GetWindowSize () {
		int ww, wh;
		SDL_GetWindowSize(window, &ww, &wh);
		return Vector2(ww, wh);
	}
	
	SceneManager& SceneLoader = SceneManager::Get();
	InputManager& Input = InputManager::Get();
	AudioManager& Audio = AudioManager::Get();
	TimeManager& Time = TimeManager::Get();
}

#include "src/Components/Transform.h"
#include "src/Components/Camera.h"
namespace PME {
	Camera* cam;
}

#include "src/Components/SpriteRenderer.h"
#include "src/Components/Animator.h"
#include "src/Components/Collider.h"
#include "src/Components/Rigidbody.h"
#include "src/Components/AudioSource.h"
#if TTF == 1
#include "src/Components/Text.h"
#endif
#if USEGL == 1
#include "src/gltest.h"
#endif
#include "src/invoke.h"

namespace PME {
	

	

	void DeleteScene () {
		for (auto ent : SceneLoader.GetActiveScene()->entityList) {
			if (ent != nullptr)
				delete ent;
		}
	}

	// Initializes PME with given settings. Returns 0 on failure.
	int init (EngineSettings set) {
		cam = new Camera();

		// INITIALIZE LIBS //
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
			Internal::SDLError("Error initializing SDL");
			return 0;
		}

		#if TTF == 1
		if (TTF_Init()) {
			Internal::SDLError("Error initializing SDL_ttf");
			return 0;
		}
		#endif

		#if IMG == 1
		if (!IMG_Init(IMG_INIT_PNG)) {
			Internal::SDLError("Error initializing SDL image library");
			return 0;
		}
		#endif

		// INITIALIZE AUDIO //
		#if MIX == 1
		int audio_rate = 22050;
		Uint16 audio_format = AUDIO_S16SYS;
		int audio_channels = 2;
		int audio_buffers = 4096;
		Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers);
		#endif

		// CREATE WINDOW //
		Uint32 windowFlags = 0;
		if (set.resizable) windowFlags |= SDL_WINDOW_RESIZABLE;
		if (set.highDPI) windowFlags |= SDL_WINDOW_ALLOW_HIGHDPI;
		
		window = SDL_CreateWindow(
			set.windowTitle.c_str(),
			SDL_WINDOWPOS_CENTERED, 
			SDL_WINDOWPOS_CENTERED,
			set.windowWidth,
			set.windowHeight,
			windowFlags
		);
		
		if (!window) {
			Internal::LogError("Error creating window");
			return 0;
		}

		// CREATE RENDERER //
		Uint32 rendererFlags = SDL_RENDERER_ACCELERATED;
		if (set.vsync) rendererFlags |= SDL_RENDERER_PRESENTVSYNC;
		renderer = SDL_CreateRenderer(window, -1, rendererFlags);
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

		if (!renderer) {
			Internal::LogError("Error creating renderer");
			return 0;
		}

		// INITIALIZE TIME //
		Time.time = SDL_GetTicks()/1000.0;
		Time.lastFrameTime = Time.time;

		Internal::Log("Engine initialized...");
		return 1;
	}
	// Initializes PME
	void init () {
		init({});
	}

	bool running = true;

	// Starts the engine
	void run () {

		auto loop = [] () {
			std::vector<Component*> components;
			std::vector<Collider*> colliders;
			std::vector<SpriteRenderer*> renderers;
			std::vector<Rigidbody*> rigidbodies;
			
			for (auto ent : SceneLoader.GetActiveScene()->entityList) {
				for (int i = 0; i < ent->ComponentSize(); i++) {
					if (!ent->enabled) continue;

					Component* c = ent->GetComponent(i);
					if (!c->enabled) continue;

					components.push_back(c);
					if (typeid(*c) == typeid(Collider)) {
						colliders.push_back((Collider*)c);
					}
					else if (typeid(*c) == typeid(SpriteRenderer)) {
						renderers.push_back((SpriteRenderer*)c);
					}
					else if (typeid(*c) == typeid(Rigidbody)) {
						rigidbodies.push_back((Rigidbody*)c);
					}
				}
			}
			
			// FRAME LIMITING NW! //
			Uint32 starting_tick = SDL_GetTicks();
			
			
			#if PORT == 1
			if (maxFPS > 0 && (1000/maxFPS) > SDL_GetTicks()-starting_tick) {
				SDL_Delay((1000/maxFPS)-(SDL_GetTicks()-starting_tick));
			}
			#endif
			
			// CALCULATING DELTATIME //
			
			Time.time = starting_tick/1000.0f;
			Time.deltaTime = Time.time - Time.lastFrameTime;
			Time.lastFrameTime = Time.time;

			// HANDLE INPUT //

			running = Input.HandleEvents();

			// UPDATE //
			float t = Time.time;
			// for (auto evt : eventQueue) {
			// 	evt->Check(t);
			// }

			for (auto comp : components) {
				if (comp->enabled)
					comp->Update();
			}

			// HANDLE PHYSICS //
			
			for (auto rb : rigidbodies) {
				if (!rb->enabled) continue;

				rb->contacts.clear();
				rb->triggers.clear();
				for (int i = 0; i < colliders.size(); i++) {
					bool isIdentical = rb->collider == colliders[i];
					bool isEnabled = rb->collider->enabled && colliders[i]->enabled;

					if (!isIdentical && isEnabled) {
						bool collides = Physics::AABB(rb->collider->Bounds(), colliders[i]->Bounds());

						if (collides) {
							if (colliders[i]->isTrigger) {
								rb->triggers.push_back(colliders[i]);
							}
							else {
								rb->contacts.push_back(colliders[i]);
							}
						}
							
					}
				}
			}

			// LATE UPDATE //

			for (auto comp : components) {
				if (comp->enabled)
					comp->LateUpdate();
			}

			// RENDERING //

			
			SDL_SetRenderDrawColor(renderer, clearColor.r, clearColor.g, clearColor.b, clearColor.a);
			SDL_RenderClear(renderer);
			
			
			
			auto zsort = [] (SpriteRenderer* a, SpriteRenderer* b) {
				return (a->zIndex < b->zIndex);
			};
			std::sort(renderers.begin(), renderers.end(), zsort);
			for (auto rend : renderers) {
				if (rend->enabled)
					rend->Render();
			}

			for (auto comp : components) {
				if (comp->enabled && typeid(*comp) != typeid(SpriteRenderer))
					comp->Render();
			}
			
			SDL_RenderPresent(renderer);
		};

		// START //
		for (auto ent : SceneLoader.GetActiveScene()->entityList) {
			for (int i = 0; i < ent->ComponentSize(); i++) {
				ent->GetComponent(i)->Start();
			}
		}
		
		// GAME LOOP //
		
		#if PORT == 0
		emscripten_set_main_loop(loop, 0, 1);
		#else
		while (running) {
			loop();
		}
		#endif

		// CLEANUP //

		DeleteScene();
		
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		
		#if IMG == 1
			IMG_Quit();
		#endif
		#if MIX == 1
			Mix_CloseAudio();
		#endif
		#if TTF == 1
			TTF_Quit();
		#endif
		
		SDL_Quit();
	}
}