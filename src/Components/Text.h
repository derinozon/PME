#pragma once

namespace PME {
    class Text : public Component {
		private:
			SDL_Surface* surface;
			SDL_Texture* texture;
			SDL_Rect rect;
			Transform* textT;

			int qW, qH;

			std::string prevContent = "Sample Text";
			SDL_Color prevColor = { 255, 0, 255 };

			void RefreshTexture () {
				SDL_FreeSurface(surface);
				SDL_DestroyTexture(texture);
				surface = TTF_RenderText_Solid(font, content.c_str(), color);
				texture = SDL_CreateTextureFromSurface(renderer, surface);
				SDL_QueryTexture(texture, NULL, NULL, &qW, &qH);
			}

			bool ColorIsSame (SDL_Color a, SDL_Color b) {
				return (a.r == b.r && a.b == b.b && a.g == b.g); 
			}

        public:
			std::string content = "Sample Text";
			TTF_Font* font;
			SDL_Color color = { 255, 0, 255 };
			Vector2 offset = {0,0};
			Vector2 size = {1,1};

			Text () {
				Internal::Log("Text created");
			}

			virtual ~Text () {
				TTF_CloseFont(font);
				SDL_DestroyTexture(texture);
				SDL_FreeSurface(surface);
				Internal::Log("Text destroyed");
			}

			void Start () override {
				textT = Depends<Transform>();
				if (font == nullptr) {
					Internal::LogError("No Font!");
				}
				surface = TTF_RenderText_Solid(font, content.c_str(), color);
				texture = SDL_CreateTextureFromSurface(renderer, surface);
				SDL_QueryTexture(texture, NULL, NULL, &qW, &qH);
			}
			
			void Update () override {
				if (prevContent != content||!ColorIsSame(prevColor, color)) {
					RefreshTexture();
					prevContent = content;
					prevColor = color;
				}
				Rect r;
				r.w = textT->scale.x * qW * size.x;
				r.h = textT->scale.y * qH * size.y;

				r.x = (textT->position.x + offset.x) * ppu - rect.w/2;
				r.y = -(textT->position.y + offset.y) * ppu - rect.h/2;

				rect = (SDL_Rect) cam->PixelsRelativeToCamera(r);
            };

            void Render () override {
				SDL_RenderCopy(renderer, texture, NULL, &rect);
            };
    };
    
}