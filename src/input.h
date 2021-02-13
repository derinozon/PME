#pragma once

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