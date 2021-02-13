#pragma once

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
