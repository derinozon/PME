#pragma once

namespace PME {

	struct Animation {
		Texture* spritesheet;
		float speed;
		bool loop;
		int frameCount;
		Vector2 frameSize;

		Animation () {}

		Animation (Texture* spritesheet, int frameCount = 0, float animationSpeed = 1, bool loop = true, Vector2 frameSize = {0,0}) {
			this->spritesheet = spritesheet;
			speed = animationSpeed;
			this->loop = loop;
			this->frameCount = frameCount;
			this->frameSize = frameSize;
		}
	};
	
    class Animator : public Component {
		private:
			
			bool stopped;
			bool paused;
			Uint16 frame = 0;
			float animTime = 0;

			std::vector<Animation> anims;
			Uint16 currentAnim = 0;
			SpriteRenderer* sr;
			
        public:
		
			Animator () {
				name = "Animator";
			}

			virtual ~Animator () {
				
			}

			void Start () override {
				sr = Depends<SpriteRenderer>();
				Update();
			}

			void Update () override {
				auto anim = anims[currentAnim];

				if (!stopped && anim.spritesheet != nullptr) {
					int x,y;
					sr->texture = anim.spritesheet;
					SDL_QueryTexture(sr->texture, NULL, NULL, &x, &y);

					if (anim.frameSize.x == 0 || anim.frameSize.y == 0) {
						Uint32 fs  = x > y ? y : x;
						anim.frameSize = Vector2(fs,fs);
					}
					
					if (!sr->fit)
						sr->rect.w /= anim.frameCount;
					
					
					sr->ds.w = (int) anim.frameSize.x;
					sr->ds.h = (int) anim.frameSize.y;
					
					
					if (!paused)
						animTime += Time.deltaTime * anim.speed;
					
					frame = (int)animTime % anim.frameCount;

					if (animTime > anim.frameCount) {
						animTime = 0;
						if (!anim.loop)
							Stop();
					}

					sr->ds.x = frame * anim.frameSize.x;
				}
            }

			int Add (Animation animation) {
				anims.push_back(animation);
				return anims.size();
			}

			int Add (std::vector<Animation> animations) {
				anims = animations;
				return anims.size();
			}

			void Play (int index) {
				if (index >= anims.size()) return;
				currentAnim = index;
				stopped = paused = false;
			}

			void Stop () {
				stopped = true;
			}

			void Pause () {
				paused = true;
			}
    };
	
}