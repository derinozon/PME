#pragma once

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