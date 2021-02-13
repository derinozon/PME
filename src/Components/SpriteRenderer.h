#pragma once

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