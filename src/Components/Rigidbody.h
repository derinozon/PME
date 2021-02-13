#pragma once


namespace PME {
	class Rigidbody : public Component {
		private:
			int contactsLastFrame = 0;
			int triggersLastFrame = 0;
			Collider* lastTRG;
			Transform* rbT;
			
		public:
			Collider* collider;
			Vector2 velocity = Vector2(0,0);
			std::vector<Collider*> contacts;
			std::vector<Collider*> triggers;

			Rigidbody () {
				name = "Rigidbody";
			}

			virtual ~Rigidbody () {
				
			}

			void Start () override {
				collider = Depends<Collider>();
				rbT = Depends<Transform>();
			}

			
			void Update () override {
				rbT->position += velocity * Time.deltaTime;
			}

			
			

			void CalculateCollision (Collider* trgt) {
				Rect aBounds = collider->Bounds();
				Rect bBounds = trgt->Bounds();

				Vector2 aPos = rbT->position + collider->offset;
				Vector2 aRect = Vector2(aBounds.w/2, aBounds.h/2);

				Vector2 bPos = trgt->entity->GetComponent<Transform>()->position + trgt->offset;
				Vector2 bRect = Vector2(bBounds.w/2, bBounds.h/2);

				Vector2 l1 = aPos-aRect;
				Vector2 r1 = aPos+aRect;
        		Vector2 l2 = bPos-bRect;
				Vector2 r2 = bPos+bRect;
				
				float xDist = std::min(r1.x, r2.x) - std::max(l1.x, l2.x);
    			float yDist = std::min(r1.y, r2.y) - std::max(l1.y, l2.y);

				Vector2 normal = Vector2(xDist, yDist);
				Vector2 off = aPos - bPos;
				
				if (xDist > yDist) normal.x = 0;
				if (yDist > xDist) normal.y = 0;

				if (off.x < 0) normal.x *= -1;
				if (off.y < 0) normal.y *= -1;
				
				rbT->position = rbT->position + normal;
			}
 
			void LateUpdate () override {
				
				// TRIGGERS //

				// ENTER //
				if (triggers.size() > triggersLastFrame) {
					Collider* lastContact = triggers[triggers.size()-1];
					
					for (int i = 0; i < entity->ComponentSize(); i++) {
						auto comp = entity->GetComponent(i);
						comp->OnTriggerEnter(lastContact);
					}
				}

				// STAY //

				if (triggers.size() > 0) {
					Collider* lastContact = triggers[triggers.size()-1];

					for (int i = 0; i < entity->ComponentSize(); i++) {
						entity->GetComponent(i)->OnTriggerStay(lastContact);
					}
				}

				// EXIT //
				if (triggers.size() < triggersLastFrame) {
					Collider* lastContact = lastTRG;

					for (int i = 0; i < entity->ComponentSize(); i++) {
						entity->GetComponent(i)->OnTriggerExit(lastContact);
					}
				}

				triggersLastFrame = triggers.size();
				if (triggers.size() > 0)
					lastTRG = triggers[triggers.size()-1];
					// DO SAME FOR COLLIDER //
				
				// COLLIDERS //

				if (contacts.size() > contactsLastFrame) {
					Collider* lastContact = contacts[contacts.size()-1];
					
					for (int i = 0; i < entity->ComponentSize(); i++) {
						auto comp = entity->GetComponent(i);
						comp->OnCollisionEnter(lastContact);
					}
				}

				if (contacts.size() > 0) {
					Collider* lastContact = contacts[contacts.size()-1];

					for (int i = 0; i < entity->ComponentSize(); i++) {
						entity->GetComponent(i)->OnCollisionStay(lastContact);
					}
				}

				if (contacts.size() < contactsLastFrame) {
					for (int i = 0; i < entity->ComponentSize(); i++) {
						entity->GetComponent(i)->OnCollisionExit();
					}
				}
				

				if (contacts.size() > 0) {
					for (auto col : contacts) {
						if (!col->isTrigger) {
							CalculateCollision(col);
						}
					}
				}

				contactsLastFrame = contacts.size();
			}

			#if DEBUG == 1
			void Render () override {
				SDL_Color r = {255, 0, 0, 255};
				SDL_Color g = {0, 255, 0, 255};
				SDL_Color c = contacts.size()>0 ? r : g;

				Rect rect = collider->Bounds()*ppu;
				rect = cam->PixelsRelativeToCamera(rect);
				Debug::DrawRect(rect, c);
			}
			#endif
	};
}