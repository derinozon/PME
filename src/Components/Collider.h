#pragma once


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