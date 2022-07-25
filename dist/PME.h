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

#ifndef USEGL
	#define USEGL 0
#endif

#ifndef IMG
	#define IMG 1
#endif

#ifndef MIX
	#define MIX 0
#endif

#ifndef TTF
	#define TTF 1
#endif

//#if defined(_WIN32) || defined(WIN32)

#if PORT == 0
	#include <emscripten.h>
#endif

#include <SDL2/SDL.h>

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <random>
#include <sstream>

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

namespace PME {
	
	namespace Internal {

		template <class T>
		void Log (T message) {
			#if DEBUG == 1
			std::cout << WHITE << message << RESET << std::endl;
			#endif
		}

		template <class T>
		void LogWarning (T message) {
			#if DEBUG == 1
			std::cout << YELLOW << message << RESET << std::endl;
			#endif
		}

		template <class T>
		void LogError (T message) {
			#if DEBUG == 1
			std::cout << RED << message << RESET << std::endl;
			#endif
		}

		void SDLError (const char* message) {
			#if DEBUG == 1
			std::cout << RED << message << std::endl << SDL_GetError() << RESET << std::endl;
			#endif
		}
	}
	
}

namespace PME {
	struct Rect {
		float x, y, w, h;

		
		
		Rect () {}

		Rect (float x, float y, float w, float h) {
			this->x = x;
			this->y = y;
			this->w = w;
			this->h = h;
		}

		operator SDL_Rect() const {
			return {(int)x, (int)y, (int)w, (int)h};
		}

		Rect operator * (const float &rhs) {
			return {x*rhs, y*rhs, w*rhs, h*rhs};
		}

		Rect operator *= (const float &rhs) {
			return {x*rhs, y*rhs, w*rhs, h*rhs};
		}
	};
}

namespace PME {
	struct Vector2 {
		float x,y;

		Vector2 () {
			x = 0;
			y = 0;
		}

		Vector2 (float X, float Y) {
			x = X;
			y = Y;
		}

		float magnitude () {
			return sqrtf(powf(x, 2) + powf(y, 2));
		}

		float sqrmagnitude () {
			return powf(x, 2) + powf(y, 2);
		}

		Vector2 normalize () {
			float mag = magnitude();
			if (mag == 0) {
				return Vector2(x, y);
			}
			return Vector2(x/mag, y/mag);
		}

		operator SDL_Point() const {
			return {(int)x, (int)y};
		}

		Vector2 operator + (Vector2 const &obj) {
			return Vector2(x + obj.x, y + obj.y);
		}

		Vector2 operator - (Vector2 const &obj) {
			return Vector2(x - obj.x, y - obj.y);
		}

		Vector2 operator * (int const &obj) {
			return Vector2(x * obj, y * obj);
		}

		Vector2 operator * (float const &obj) {
			return Vector2(x * obj, y * obj);
		}

		Vector2 operator * (double const &obj) {
			return Vector2(x * (float)obj, y * (float)obj);
		}

		Vector2 operator / (float const &obj) {
			return Vector2(x / obj, y / obj);
		}

		void operator += (Vector2 const &obj) {
			x += obj.x;
			y += obj.y;
		}

		Vector2 operator *= (const int &rhs) {
			return {x*rhs, y*rhs};
		}

		Vector2 operator *= (const float &rhs) {
			return {x*rhs, y*rhs};
		}

		Vector2 operator *= (const double &rhs) {
			return {x*(float)rhs, y*(float)rhs};
		}

		std::string print () {
			std::stringstream sstream;
			sstream << "Vector2(" << x << ", " << y << ')';
			return sstream.str();
		}

		static float Distance (Vector2 a, Vector2 b) {
			return std::sqrt(
				std::pow(a.x - b.x, 2) +
				std::pow(a.y - b.y, 2)
			);
		}

	};

	std::ostream &operator<<(std::ostream &os, Vector2 const &m) { 
    	return os << '(' << m.x << ',' << m.y << ')';
	}

	

	
}

namespace PME {
	namespace Debug {
		template <class T>
		void Log (T message) {
			std::cout << BOLDWHITE << message << RESET << std::endl;
		}

		template <class T>
		void LogWarning (T message) {
			std::cout << BOLDYELLOW << message << RESET << std::endl;
		}

		template <class T>
		void LogError (T message) {
			std::cout << BOLDRED << message << RESET << std::endl;
		}

		void DrawRay (Vector2 point, Vector2 direction) {
			
		}

		void DrawLine (Vector2 a, Vector2 b, SDL_Color color) {
			SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
			a *= ppu;
			b *= ppu;
			SDL_RenderDrawLine(renderer, a.x, a.y, b.x, b.y);
		}

		void DrawRect (SDL_Rect rectangle, SDL_Color color = {255, 0, 255, 255}) {
			rectangle.w /= 2;
			rectangle.h /= 2;
			rectangle.x += rectangle.w;
			rectangle.y += rectangle.h;
			SDL_Rect r = rectangle;
			SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
			const SDL_Point pts[] = {
				{ -r.w+r.x,  r.h+r.y },
				{  r.w+r.x,  r.h+r.y },
				{  r.w+r.x, -r.h+r.y },
				{ -r.w+r.x, -r.h+r.y },
				{ -r.w+r.x,  r.h+r.y }
			};
			SDL_RenderDrawLines(renderer, pts, 5);
		}

		void DrawRect (Rect rectangle, SDL_Color color = {255, 0, 255, 255}) {
			DrawRect((SDL_Rect)rectangle, color);
		}
	}
}

namespace PME {
	namespace Random {
		std::random_device rd; // obtain a random number from hardware
    	std::mt19937 gen(rd()); // seed the generator
    	

		template <class T>
		T Range (T a, T b) {
			std::uniform_int_distribution<T> distr(a, b);
			return distr(gen);
		}

		bool Value () {
			std::uniform_int_distribution<bool> distr;
			return distr(gen);
		}
	}
} 

namespace PME {
	namespace Math {
		float Sin (float a) {
			return std::sin(a);
		}

		float Lerp (float a, float b, float t) {
			return a + t * (b-a);
		}

		float Clamp (float a, float b) {
			return a > b ? b : a;
		}

		float Clamp (float a, float min, float max) {
			if (a > max) return max;
			if (a < min) return min;
			return a;
		}

		float Abs (float val) {
			return val < 0 ? -val : val;
		}
	}
	
}

namespace PME {
	namespace Physics {
		bool AABB (Rect a, Rect b) {
			a = a * ppu;
			b = b * ppu;
			if (a.x < b.x + b.w &&
				a.x + a.w > b.x &&
				a.y < b.y + b.h &&
				a.y + a.h > b.y
				)
				return true;
			else
				return false;
		}

		bool AABB (SDL_Rect a, SDL_Rect b) {
			if (a.x < b.x + b.w &&
				a.x + a.w > b.x &&
				a.y < b.y + b.h &&
				a.y + a.h > b.y
				)
				return true;
			else
				return false;
		}
	}
}

namespace PME {
	
	class AudioManager {
		private:
		AudioManager () {}

		static AudioManager instance;

		SDL_AudioSpec wavSpec;
		Uint32 wavLength;
		Uint8 *wavBuffer;
		SDL_AudioDeviceID deviceId = 0;

		public:
		// Removes copy constructor
		AudioManager(const AudioManager&) = delete;

		static AudioManager& Get () {
			return instance;
		}

		void PlayWAV (const char* path) {
			SDL_LoadWAV(path, &wavSpec, &wavBuffer, &wavLength);

			if (!deviceId) {
				deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
				int success = SDL_QueueAudio(deviceId, wavBuffer, wavLength);
				SDL_PauseAudioDevice(deviceId, 0);
			}
			else {
				SDL_ClearQueuedAudio(deviceId);
				int success = SDL_QueueAudio(deviceId, wavBuffer, wavLength);
				SDL_PauseAudioDevice(deviceId, 0);
			}
			
			
		}

		

	};
	AudioManager AudioManager::instance;
}

namespace PME {
	class TimeManager {
		public:
		TimeManager(const TimeManager&) = delete;

		static TimeManager& Get () {
			return instance;
		}

		float deltaTime;
		float time;
		float lastFrameTime;
		private:
		static TimeManager instance;
		
		TimeManager () {

		}
	};
	TimeManager TimeManager::instance;
}

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

namespace PME {
	
	class InputManager {
		public:
		// Removes copy constructor
		InputManager(const InputManager&) = delete;

		static InputManager& Get () {
			return instance;
		}

		/*
		struct KeyEvent {
			KeyEvent(SDL_Keycode key, void (*event)()) {
				keycode = key;
				evt = event;
			}
			SDL_Keycode keycode;
			void (*evt)();
		};

		std::vector<KeyEvent> kdown;

		void OnKeyDown (SDL_Keycode key, void (*event)()) {
			kdown.push_back(KeyEvent(key, event));
		}
		*/

		Vector2 GetMousePosition () {
			int x,y;
			SDL_GetMouseState(&x, &y);
			return Vector2(x,-y) / ppu;
		}

		bool GetButtonDown (int key) {
			for (auto dat : mousedata) {
				if (dat.mousecode == key) {
					if (dat.state == DOWN)
						return true;
					else 
						return false;
				}
			}
			return false;
		}

		bool GetButtonUp (int key) {
			for (auto dat : mousedata) {
				if (dat.mousecode == key) {
					if (dat.state == UP)
						return true;
					else 
						return false;
				}
			}
			return false;
		}

		bool GetKeyDown (SDL_Keycode key) {
			for (auto dat : keydata) {
				if (dat.keycode == key) {
					if (dat.state == DOWN)
						return true;
					else 
						return false;
				}
			}
			return false;
		}

		bool GetKeyUp (SDL_Keycode key) {
			for (auto dat : keydata) {
				if (dat.keycode == key) {
					if (dat.state == UP)
						return true;
					else 
						return false;
				}
			}
			return false;
		}

		bool GetKey (SDL_Keycode key) {
			for (KeyData dat : keydata) {
				if (dat.keycode == key) {
					return true;
				}
			}
			return false;
		}

		int GetAxisRaw (std::string axis) {
			if (axis == "Horizontal") {
				bool left = GetKey(SDLK_a)||GetKey(SDLK_LEFT);
				bool right = GetKey(SDLK_d)||GetKey(SDLK_RIGHT);

				if (left && !right) {
					return -1;
				}
				if (!left && right) {
					return 1;
				}
			}
			if (axis == "Vertical") {
				bool down = GetKey(SDLK_s)||GetKey(SDLK_DOWN);
				bool up = GetKey(SDLK_w)||GetKey(SDLK_UP);

				if (down && !up) {
					return -1;
				}
				if (!down && up) {
					return 1;
				}
			}
			return 0;
		}

		bool HandleEvents () {
			CleanEvents();

			while (SDL_PollEvent(&event)) {
				if (event.type == SDL_QUIT) {
					return false;
				}
				if (event.type == SDL_KEYDOWN && !event.key.repeat) {
					AddKey(event.key.keysym.sym);
					/*
					for (auto kevt : kdown) {
						if (kevt.keycode == event.key.keysym.sym) {
							kevt.evt();
						}
					}
					*/
				}
				if (event.type == SDL_KEYUP && !event.key.repeat) {
					//RemoveKey(event.key.keysym.sym);

					for (int i = 0; i < keydata.size(); i++) {
						if (keydata[i].keycode == event.key.keysym.sym) {
							keydata[i].state = UP;
						}
					}
				}

				// NW! //
				if (event.type == SDL_MOUSEBUTTONDOWN) {
					mousedata.push_back({event.button.button});
				}
				if (event.type == SDL_MOUSEBUTTONUP) {
					for (int i = 0; i < mousedata.size(); i++) {
						if (mousedata[i].mousecode == event.button.button) {
							mousedata[i].state = UP;
						}
					}
				}
			}
			
			return true;
		}

		private:
		InputManager () {}

		static InputManager instance;

		enum KeyState {
			DOWN, HOLD, UP
		};

		struct KeyData {
			SDL_Keycode keycode;
			KeyState state;
		};

		struct MouseData {
			MouseData (int code) {
				mousecode = code;
			}
			int mousecode;
			KeyState state;
		};

		SDL_Event event;
		std::vector<KeyData> keydata;
		std::vector<MouseData> mousedata;
		
		void CleanEvents () {
			for (int i = 0; i < keydata.size(); i++) {

				if (keydata[i].state == DOWN) {
					keydata[i].state = HOLD;
				}
				if (keydata[i].state == UP) {
					keydata.erase(keydata.begin()+i);
				}
			}

			for (int i = 0; i < mousedata.size(); i++) {

				if (mousedata[i].state == DOWN) {
					mousedata[i].state = HOLD;
				}
				if (mousedata[i].state == UP) {
					mousedata.erase(mousedata.begin()+i);
				}
			}
		}

		void AddKey (SDL_Keycode kc) {
			KeyData dat;
			dat.keycode = kc;
			keydata.push_back(dat);
		}

		void RemoveKey (SDL_Keycode kc) {
			for (int i = 0; i < keydata.size(); i++) {
				if (keydata[i].keycode == kc) {
					keydata.erase(keydata.begin()+i);
				}
			}
		}
	};
	InputManager InputManager::instance;
}

#include <vector>

namespace PME {
	class Entity;

	struct Scene {
		const char* name;
		std::vector<Entity*> entityList;

		Scene (const char* sceneName) {
			name = sceneName;
		}

		Scene () {
			name = "New Scene";
		}

		~Scene () {
			//for (auto ent : entityList) {
			//	delete ent;
			//}
			//Internal::Log(name + std::string(" destroyed"));
		}
	};

	class SceneManager {
		public:
			SceneManager(const SceneManager&) = delete;

			static SceneManager& Get () {
				return instance;
			}

			Scene* GetActiveScene () {
				if (activeScene == nullptr) {
					activeScene = new Scene();
				}
				return activeScene;
			}

			~SceneManager () {
				if (activeScene != nullptr)
					delete activeScene;
			}

		private:
			static SceneManager instance;
		
			SceneManager () {

			}

			Scene* activeScene;

			
	};
	SceneManager SceneManager::instance;
}

namespace PME {
	class Collider;
	
	class Component {
		
		public:
		Entity* entity = nullptr;
		std::string name = "New Component";
		bool enabled = true;

		// Copy constructor 4 cloning
		// Component(const Component& rhs) {
		// 	Internal::Log("Component COPY");
		// }

		template <class T>
		T* AddComponent ();
		template <class T>
		T* Get ();
		template <class T>
		T* GetComponent ();
		Component* GetComponent (int index);
		
		template <class T>
		T* Depends () {
			T* ret = Get<T>();
			if (ret == nullptr) {
				ret = AddComponent<T>();
			}
			return ret;
		}

		virtual void Start(){}
		virtual void Update(){}
		virtual void LateUpdate(){}
		virtual void Render(){}
		virtual void OnCollisionEnter(Collider* collider){}
		virtual void OnCollisionStay(Collider* collider){}
		virtual void OnCollisionExit(){}
		virtual void OnTriggerEnter(Collider* trigger){}
		virtual void OnTriggerStay(Collider* trigger){}
		virtual void OnTriggerExit(Collider* trigger){}
		
		virtual ~Component(){}
	};
}

namespace PME {
	class Entity {
		private:
		std::vector<Component*> components;
		

        public:
		Entity* parent = nullptr;
		std::vector<Entity*> childs;
		std::string name;
		std::string tag;
		Uint16 id;
		bool enabled = true;

		
		Entity(const Entity& rhs) {
			for (int i = 0; i < rhs.components.size(); i++) {
				Component* comp = new Component(*rhs.components[i]);
				components.push_back(comp);
			}
			
			components = rhs.components;
			name = rhs.name;
			
			SceneManager::Get().GetActiveScene()->entityList.push_back(this);
			Internal::Log("Cloned entity " + name);
		}
  		//Entity& operator=(const Entity& rhs) {};

		Entity () : Entity("New Entity", "", nullptr) {}
		Entity (std::string name) : Entity(name, "", nullptr) {}
		Entity (Entity* parent) : Entity("New Entity", "", parent) {}
		Entity (std::string name, std::string tag) : Entity(name, tag, nullptr) {}

		Entity (std::string name, std::string tag, Entity* parent) {
			this->name = name;
			this->parent = parent;
			this->tag = tag;

			if (parent != nullptr)
				parent->childs.push_back(this);

			Scene* scn = SceneManager::Get().GetActiveScene();

			scn->entityList.push_back(this);
			id = scn->entityList.size()-1;

			Internal::Log(name + std::string(" created"));
		}
		~Entity () {
			
			for (auto comp : components) {
				if (comp != nullptr) {
					delete comp;
					Internal::Log("Deleted " + comp->name + " on " + name);
				}
			}

			Internal::Log("Deleted " + name);
		}

		static void Destroy (Entity* entity) {
			Scene* scn = SceneManager::Get().GetActiveScene();
			for (int i = 0; i < scn->entityList.size(); i++) {
				if (scn->entityList[i] == entity) {
					scn->entityList.erase(scn->entityList.begin()+i);
				}
			}
		}

		static Entity* Find (std::string name) {
			Scene* scn = SceneManager::Get().GetActiveScene();
			for (int i = 0; i < scn->entityList.size(); i++) {
				if (scn->entityList[i]->name == name) {
					return scn->entityList[i];
				}
			}
			return nullptr;
		}

		template <class T>
		T* AddComponent () {
			Component* ret = new T();
			ret->entity = this;
			components.push_back(ret);

			auto log = "Added " + ret->name + " to " + name;
			Internal::Log(log);
			return (T*) ret;
		}
		
		Component* GetComponent (int index) {
			Component* ret = components[index];
			if (ret == nullptr) {
				Debug::LogError("Component at index not found");
			}
			return ret;
		}

		template <class T>
		T* GetComponent () {
			T* ret = Get<T>();
			if (ret == nullptr)
				Debug::LogError("Component of type not found");
			return ret;
		}

		template <class T>
		std::vector<T*> GetComponentsOfType () {
			std::vector<T*> ret;
			for (int i = 0; i < components.size(); i++) {
				T* comp = dynamic_cast<T*>(components[i]);
				if (comp)
					ret.push_back(comp);
			}
			return ret;
		}

		template <class T>
		T* Get () {
			T* ret;
			for (int i = 0; i < components.size(); i++) {
				ret = dynamic_cast<T*>(components[i]);
				if (ret) return ret;
			}
			return nullptr;
		}

		template <class T>
		void RemoveComponent () {
			for (int i = 0; i < components.size(); i++) {
				if (dynamic_cast<T*>(components[i])) {
					delete components[i];
					components.erase(components.begin()+i);
				}
			}
		}

		void RemoveComponent (Component* component) {
			for (int i = 0; i < components.size(); i++) {
				std::cout << (component == components[i]) << std::endl;
				if (component == (components[i])) {
					delete components[i];
					components.erase(components.begin()+i);
				}
			}
		}

		int ComponentSize () {
			return components.size();
		}
	};

	// Component Implementations //

	template <class T>
	T* Component::AddComponent () {
		return entity->AddComponent<T>();
	}

	Component* Component::GetComponent (int index) {
		return entity->GetComponent(index);
	}

	template <class T>
	T* Component::GetComponent () {
		return entity->GetComponent<T>();
	}

	template <class T>
	T* Component::Get () {
		return entity->Get<T>();
	}
}

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

namespace PME {
	
	class Transform : public Component {
		public:
			
			Vector2 position = Vector2(0, 0);
			Vector2 scale = Vector2(1, 1);
			double angle = 0;
			

			Transform () {
				name = "Transform";
			}

			virtual ~Transform () {

			}

			void Translate (Vector2 dir) {
				position += dir;
			}

			void Update () override {
				
			}

	};
}

namespace PME {
	class Camera : public Component {
		private:
			
			
		public:
			Vector2 pos = {0,0};

			bool IsVisible () {
				return true;
			}
			
			Camera () {
				name = "Camera";
			}

			virtual ~Camera () {
				
			}
			

			void Start () override {
				
			}
			
			Rect PixelsRelativeToCamera (Rect rect) {
				Vector2 ws = GetWindowSize();

				float cx = pos.x*ppu - ws.x/2;
				float cy = -pos.y*ppu - ws.y/2;

				rect.x -= cx;
				rect.y -= cy;
				return rect;
			}

			Vector2 ScreenToWorldPos (Vector2 pos) {
				Vector2 ws = GetWindowSize();

				float cx = pos.x*ppu - ws.x/2;
				float cy = -pos.y*ppu - ws.y/2;

				pos.x += cx;
				pos.y -= cy;
				return pos;
			}
		
	};
}
namespace PME {
	Camera* cam;
}

namespace PME {
	enum Pivot {
		CENTER, BOTTOM, TOP, TOPLEFT
	};
    class SpriteRenderer : public Component {
		private:
			Transform* spriteT = nullptr;
			

        public:
			SDL_Rect rect;
			Texture* texture = nullptr;
			
			SDL_Color color = {255, 255, 255, 255};
			SDL_RendererFlip flip = SDL_FLIP_NONE;
			
			int zIndex = 0;
			bool animate = true;
			int size = 1;
			SDL_Rect ds = {0,0,0,0};
			bool fit = true;
			Pivot pivot = CENTER;

			SpriteRenderer () {
				name = "Sprite Renderer";
			}

			virtual ~SpriteRenderer () {
				if (texture != NULL)
					SDL_DestroyTexture(texture);
			}

			Vector2 Center () {
				Vector2 r;
				float xr, yr;

				if (pivot == CENTER) {
					xr = -rect.w/2;
					yr = -rect.h/2;
				}
				if (pivot == BOTTOM) {
					xr = -rect.w/2;
					yr = -rect.h;
				}
				if (pivot == TOP) {
					xr = -rect.w/2;
					yr = rect.h;
				}
				if (pivot == TOPLEFT) {
					xr = yr = 0;
				}

				r.x = (spriteT->position.x)*ppu + xr;
				r.y = -(spriteT->position.y)*ppu + yr;

				return r;
			}

			void Start () override {
				spriteT = Depends<Transform>();
			}
			
			void Update () override {
				Rect r;

				r.w = spriteT->scale.x * ppu * size;
				r.h = spriteT->scale.y * ppu * size;

				
				if (!fit && texture != nullptr) {
					int w,h;
					SDL_QueryTexture(texture, NULL, NULL, &w, &h);
					r.w = w * ppu/10;
					r.h = h * ppu/10;
				}

				Vector2 center = Center();
				r.x = center.x;
				r.y = center.y;

				rect = (SDL_Rect) cam->PixelsRelativeToCamera(r);
            };

            void Render () override {
				//Debug.DrawRect(rect, {255,0,0,244});

				if (texture == nullptr) {
					SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    				SDL_RenderFillRect(renderer, &rect);
				}
				else {
					SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
					SDL_SetTextureAlphaMod(texture, color.a);
					
					SDL_Rect* dest = NULL;
					if (ds.h > 1 && ds.w > 1) 
						dest = &ds;
					SDL_RenderCopyEx(renderer, texture, dest, &rect, spriteT->angle, NULL, flip);
				}
            };

			// Z Index Sort //
			//bool operator < (const SpriteRenderer& sr) const {
        	//	return (zIndex < sr.zIndex);
   			//}
    };
    
}

namespace PME {

	struct Animation {
		Texture* spritesheet;
		float speed;
		bool loop;
		int frameCount;
		Vector2 frameSize;

		Animation () {}

		Animation (Texture* spritesheet, int frameCount = 0, float animationSpeed = 1, bool loop = true, Vector2 frameSize = {0,0}) {
			this->spritesheet = spritesheet;
			speed = animationSpeed;
			this->loop = loop;
			this->frameCount = frameCount;
			this->frameSize = frameSize;
		}
	};
	
    class Animator : public Component {
		private:
			
			bool stopped;
			bool paused;
			Uint16 frame = 0;
			float animTime = 0;

			std::vector<Animation> anims;
			Uint16 currentAnim = 0;
			SpriteRenderer* sr;
			
        public:
		
			Animator () {
				name = "Animator";
			}

			virtual ~Animator () {
				
			}

			void Start () override {
				sr = Depends<SpriteRenderer>();
				Update();
			}

			void Update () override {
				auto anim = anims[currentAnim];

				if (!stopped && anim.spritesheet != nullptr) {
					int x,y;
					sr->texture = anim.spritesheet;
					SDL_QueryTexture(sr->texture, NULL, NULL, &x, &y);

					if (anim.frameSize.x == 0 || anim.frameSize.y == 0) {
						Uint32 fs  = x > y ? y : x;
						anim.frameSize = Vector2(fs,fs);
					}
					
					if (!sr->fit)
						sr->rect.w /= anim.frameCount;
					
					
					sr->ds.w = (int) anim.frameSize.x;
					sr->ds.h = (int) anim.frameSize.y;
					
					
					if (!paused)
						animTime += Time.deltaTime * anim.speed;
					
					frame = (int)animTime % anim.frameCount;

					if (animTime > anim.frameCount) {
						animTime = 0;
						if (!anim.loop)
							Stop();
					}

					sr->ds.x = frame * anim.frameSize.x;
				}
            }

			int Add (Animation animation) {
				anims.push_back(animation);
				return anims.size();
			}

			int Add (std::vector<Animation> animations) {
				anims = animations;
				return anims.size();
			}

			void Play (int index) {
				if (index >= anims.size()) return;
				currentAnim = index;
				stopped = paused = false;
			}

			void Stop () {
				stopped = true;
			}

			void Pause () {
				paused = true;
			}
    };
	
}

namespace PME {
	class Collider : public Component {
		private:
			Transform* colT;
			
		public:

			Vector2 size = Vector2(1,1);
			Vector2 offset = Vector2(0,0);
			bool isTrigger = false;
			
			
			Collider () {
				name = "Collider";
			}

			virtual ~Collider () {
				
			}

			Rect Bounds () {
				Rect r;

				r.w = size.x*colT->scale.x;
				r.h = size.y*colT->scale.y;

				r.x =  (colT->position.x + offset.x) - r.w/2;
				r.y = -(colT->position.y + offset.y) - r.h/2;

				
				
				return r;
			}

			void Start () override {
				colT = Depends<Transform>();
			}

			#if DEBUG == 1
			void Render () override {
				SDL_Color g = {0, 255, 0, 255};
				SDL_Color y = {0, 255, 255, 255};
				SDL_Color c = isTrigger ? y : g;

				Rect rect = Bounds()*ppu;
				rect = cam->PixelsRelativeToCamera(rect);
				Debug::DrawRect(rect, c);
			}
			#endif
			

		
	};
}

namespace PME {
	class Rigidbody : public Component {
		private:
			int contactsLastFrame = 0;
			int triggersLastFrame = 0;
			Collider* lastTRG;
			Transform* rbT;
			
		public:
			Collider* collider;
			Vector2 velocity = Vector2(0,0);
			std::vector<Collider*> contacts;
			std::vector<Collider*> triggers;

			Rigidbody () {
				name = "Rigidbody";
			}

			virtual ~Rigidbody () {
				
			}

			void Start () override {
				collider = Depends<Collider>();
				rbT = Depends<Transform>();
			}

			
			void Update () override {
				rbT->position += velocity * Time.deltaTime;
			}

			
			

			void CalculateCollision (Collider* trgt) {
				Rect aBounds = collider->Bounds();
				Rect bBounds = trgt->Bounds();

				Vector2 aPos = rbT->position + collider->offset;
				Vector2 aRect = Vector2(aBounds.w/2, aBounds.h/2);

				Vector2 bPos = trgt->entity->GetComponent<Transform>()->position + trgt->offset;
				Vector2 bRect = Vector2(bBounds.w/2, bBounds.h/2);

				Vector2 l1 = aPos-aRect;
				Vector2 r1 = aPos+aRect;
        		Vector2 l2 = bPos-bRect;
				Vector2 r2 = bPos+bRect;
				
				float xDist = std::min(r1.x, r2.x) - std::max(l1.x, l2.x);
    			float yDist = std::min(r1.y, r2.y) - std::max(l1.y, l2.y);

				Vector2 normal = Vector2(xDist, yDist);
				Vector2 off = aPos - bPos;
				
				if (xDist > yDist) normal.x = 0;
				if (yDist > xDist) normal.y = 0;

				if (off.x < 0) normal.x *= -1;
				if (off.y < 0) normal.y *= -1;
				
				rbT->position = rbT->position + normal;
			}
 
			void LateUpdate () override {
				
				// TRIGGERS //

				// ENTER //
				if (triggers.size() > triggersLastFrame) {
					Collider* lastContact = triggers[triggers.size()-1];
					
					for (int i = 0; i < entity->ComponentSize(); i++) {
						auto comp = entity->GetComponent(i);
						comp->OnTriggerEnter(lastContact);
					}
				}

				// STAY //

				if (triggers.size() > 0) {
					Collider* lastContact = triggers[triggers.size()-1];

					for (int i = 0; i < entity->ComponentSize(); i++) {
						entity->GetComponent(i)->OnTriggerStay(lastContact);
					}
				}

				// EXIT //
				if (triggers.size() < triggersLastFrame) {
					Collider* lastContact = lastTRG;

					for (int i = 0; i < entity->ComponentSize(); i++) {
						entity->GetComponent(i)->OnTriggerExit(lastContact);
					}
				}

				triggersLastFrame = triggers.size();
				if (triggers.size() > 0)
					lastTRG = triggers[triggers.size()-1];
					// DO SAME FOR COLLIDER //
				
				// COLLIDERS //

				if (contacts.size() > contactsLastFrame) {
					Collider* lastContact = contacts[contacts.size()-1];
					
					for (int i = 0; i < entity->ComponentSize(); i++) {
						auto comp = entity->GetComponent(i);
						comp->OnCollisionEnter(lastContact);
					}
				}

				if (contacts.size() > 0) {
					Collider* lastContact = contacts[contacts.size()-1];

					for (int i = 0; i < entity->ComponentSize(); i++) {
						entity->GetComponent(i)->OnCollisionStay(lastContact);
					}
				}

				if (contacts.size() < contactsLastFrame) {
					for (int i = 0; i < entity->ComponentSize(); i++) {
						entity->GetComponent(i)->OnCollisionExit();
					}
				}
				

				if (contacts.size() > 0) {
					for (auto col : contacts) {
						if (!col->isTrigger) {
							CalculateCollision(col);
						}
					}
				}

				contactsLastFrame = contacts.size();
			}

			#if DEBUG == 1
			void Render () override {
				SDL_Color r = {255, 0, 0, 255};
				SDL_Color g = {0, 255, 0, 255};
				SDL_Color c = contacts.size()>0 ? r : g;

				Rect rect = collider->Bounds()*ppu;
				rect = cam->PixelsRelativeToCamera(rect);
				Debug::DrawRect(rect, c);
			}
			#endif
	};
}

namespace PME {
	#if MIX == 1
    class AudioSource : public Component {
		private:
			
        public:
		AudioClip* audio;
		bool loop;

		AudioSource () {
			Internal::Log("AudioSource created");
		}

		virtual ~AudioSource () {
			Mix_FreeChunk(audio);
			Internal::Log("AudioSource destroyed");
		}

		void Start () override {
			if (!Mix_PlayChannel(-1, audio, loop ? -1 : 0)) {
				Debug.LogError(Mix_GetError());
			}
		}
    };
	#endif
}
#if TTF == 1

namespace PME {
    class Text : public Component {
		private:
			SDL_Surface* surface;
			SDL_Texture* texture;
			SDL_Rect rect;
			Transform* textT;

			int qW, qH;

			std::string prevContent = "Sample Text";
			SDL_Color prevColor = { 255, 0, 255 };

			void RefreshTexture () {
				SDL_FreeSurface(surface);
				SDL_DestroyTexture(texture);
				surface = TTF_RenderText_Solid(font, content.c_str(), color);
				texture = SDL_CreateTextureFromSurface(renderer, surface);
				SDL_QueryTexture(texture, NULL, NULL, &qW, &qH);
			}

			bool ColorIsSame (SDL_Color a, SDL_Color b) {
				return (a.r == b.r && a.b == b.b && a.g == b.g); 
			}

        public:
			std::string content = "Sample Text";
			TTF_Font* font;
			SDL_Color color = { 255, 0, 255 };
			Vector2 offset = {0,0};
			Vector2 size = {1,1};

			Text () {
				Internal::Log("Text created");
			}

			virtual ~Text () {
				TTF_CloseFont(font);
				SDL_DestroyTexture(texture);
				SDL_FreeSurface(surface);
				Internal::Log("Text destroyed");
			}

			void Start () override {
				textT = Depends<Transform>();
				if (font == nullptr) {
					Internal::LogError("No Font!");
				}
				surface = TTF_RenderText_Solid(font, content.c_str(), color);
				texture = SDL_CreateTextureFromSurface(renderer, surface);
				SDL_QueryTexture(texture, NULL, NULL, &qW, &qH);
			}
			
			void Update () override {
				if (prevContent != content||!ColorIsSame(prevColor, color)) {
					RefreshTexture();
					prevContent = content;
					prevColor = color;
				}
				Rect r;
				r.w = textT->scale.x * qW * size.x;
				r.h = textT->scale.y * qH * size.y;

				r.x = (textT->position.x + offset.x) * ppu - rect.w/2;
				r.y = -(textT->position.y + offset.y) * ppu - rect.h/2;

				rect = (SDL_Rect) cam->PixelsRelativeToCamera(r);
            };

            void Render () override {
				SDL_RenderCopy(renderer, texture, NULL, &rect);
            };
    };
    
}
#endif
#if USEGL == 1

#include <fstream>
#include <stdio.h>

using namespace PME;

#if PORT == 0
const char* vpath = "res/shaders/web/vertex.glsl";
const char* fpath = "res/shaders/web/fragment.glsl";
#else
const char* vpath = "res/shaders/vertex.glsl";
const char* fpath = "res/shaders/fragment.glsl";
#endif

const char* ReadFile (const char* path) {
	std::FILE* file = fopen(path, "r");

	if (!file) {
		printf("file not found\n");
		return NULL;
	}

	fseek(file, 0L, SEEK_END);
	size_t numbytes = ftell(file);

	fseek(file, 0L, SEEK_SET);	

	char* buffer = (char*)calloc(numbytes, sizeof(char));	
	if (!buffer) {
		printf("buffer errors\n");
		return NULL;
	}
	fread(buffer, sizeof(char), numbytes, file);
	fclose(file);
	
	return buffer;
}

const char* vertexShaderSource = ReadFile(vpath);
const char* fragmentShaderSource = ReadFile(fpath);

int shaderProgram;
unsigned int VBO, VAO;

int CreateShader (GLenum shaderType, const char* shaderSource) {
	int shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::string stype = (shaderType == GL_VERTEX_SHADER ? "Vertex" : "Fragment");
		std::string msg = stype + " shader compilation failed";
        Internal::LogError(msg);
		Internal::LogError(infoLog);
    }
	return shader;
}

int CompileShaders () {
	shaderProgram = glCreateProgram();

	int vertexShader = CreateShader(GL_VERTEX_SHADER, vertexShaderSource);
	int fragmentShader = CreateShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
	glValidateProgram(shaderProgram);

	int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        Internal::LogError("Shader linking failed");
		Internal::LogError(infoLog);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
	return 0;
}

void glstart () {
	CompileShaders();
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left  
        0.5f, -0.5f, 0.0f, // right 
        0.0f,  0.5f, 0.0f  // top   
    }; 

    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void glupdate () {
	glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
#endif

namespace PME {
	class Invoke;
		std::vector<Invoke*> eventQueue;

		class Invoke {
			private:
				float startTime, endTime;
				bool rep;
				void (*ptr)();
				int id;
				
			public:
				bool canWork = true;

				Invoke (void (*callback)(), float seconds, bool repeat = false) {
					ptr = callback;
					startTime = Time.time;
					endTime = startTime + seconds;
					rep = repeat;
					eventQueue.push_back(this);
				}

				Invoke (float seconds, bool repeat = false) {
					startTime = Time.time;
					endTime = startTime + seconds;
					rep = repeat;
					eventQueue.push_back(this);
				}

				Invoke () {
				}

				~Invoke () {
					
				}

				bool Check () {
					if (!rep) {
						if (!canWork) return false;
					}
					

					if (Time.time > endTime) {
						canWork = false;
						return true;
					}
					return false;
				}

				void Check (float t) {
					if (t > endTime) {
						ptr();
						if (rep) {
							float seconds = endTime-startTime;
							startTime = Time.time;
							endTime = startTime + seconds;
						}
						else {
							//eventQueue.erase(this);
							//delete this;
						}
					}
				}

				float Progress () {
					float b = endTime - startTime;
					float c = Time.time - startTime;

					return Math::Clamp(c/b, 1);
				}
		};
}

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