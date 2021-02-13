#pragma once


namespace PME {
	class Camera : public Component {
		private:
			
			
		public:
			Vector2 pos = {0,0};

			bool IsVisible () {
				return true;
			}
			
			Camera () {
				name = "Camera";
			}

			virtual ~Camera () {
				
			}
			

			void Start () override {
				
			}
			
			Rect PixelsRelativeToCamera (Rect rect) {
				Vector2 ws = GetWindowSize();

				float cx = pos.x*ppu - ws.x/2;
				float cy = -pos.y*ppu - ws.y/2;

				rect.x -= cx;
				rect.y -= cy;
				return rect;
			}

			Vector2 ScreenToWorldPos (Vector2 pos) {
				Vector2 ws = GetWindowSize();

				float cx = pos.x*ppu - ws.x/2;
				float cy = -pos.y*ppu - ws.y/2;

				pos.x += cx;
				pos.y -= cy;
				return pos;
			}
		
	};
}