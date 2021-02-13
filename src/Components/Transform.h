#pragma once


namespace PME {
	
	class Transform : public Component {
		public:
			
			Vector2 position = Vector2(0, 0);
			Vector2 scale = Vector2(1, 1);
			double angle = 0;
			

			Transform () {
				name = "Transform";
			}

			virtual ~Transform () {

			}

			void Translate (Vector2 dir) {
				position += dir;
			}

			void Update () override {
				
			}

	};
}