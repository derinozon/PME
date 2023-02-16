#pragma once

using namespace PME;
using namespace std;

class PlayerController : public Component {

	public:
		Transform* player;

		PlayerController () {
			name = "PlayerController";
		}

		void Start () override {
			player = Depends<Transform>();
		}

		void Update () override {
			int xvel = Input.GetAxisRaw("Horizontal");
			int yvel = Input.GetAxisRaw("Vertical");

			auto kVel = Vector2(xvel, yvel);

			player->Translate(kVel.normalize() * 5 * Time.deltaTime);
			player->angle = Math::Lerp(player->angle, xvel*20, Time.deltaTime * 7);
		}
};

void alien () {
	Entity* player = new Entity("Player");
	auto sr = player->AddComponent<SpriteRenderer>();
	sr->texture = Resources::LoadTexture("demo/res/cute_alien.png");
	sr->zIndex = 20;
	player->AddComponent<Collider>()->size = {1, 1};
	auto rb = player->AddComponent<Rigidbody>();
	auto pc = player->AddComponent<PlayerController>();
	
	Entity* a = new Entity("A");
	a->AddComponent<Transform>()->position = Vector2(4, -4.8);
	a->AddComponent<SpriteRenderer>()->color = {0,0,0,100};
	a->AddComponent<Collider>();
	
	Entity* b = new Entity("B");
	b->AddComponent<Transform>()->position = Vector2(5, -2);
	b->GetComponent<Transform>()->scale = Vector2(2, 1);
	b->AddComponent<SpriteRenderer>()->color = {0,0,0,100};
	b->AddComponent<Collider>();
}