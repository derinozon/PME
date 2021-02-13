#pragma once

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