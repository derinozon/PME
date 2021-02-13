#pragma once

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