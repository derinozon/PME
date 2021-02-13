#pragma once

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