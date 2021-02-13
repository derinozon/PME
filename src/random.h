#pragma once

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