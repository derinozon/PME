#pragma once

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