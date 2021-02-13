using namespace PME;
using namespace std;

class MarioController : public Component {
	private:
		float gravity = -15;
		float yvel = 0;
		bool grounded = false;

	public:
		Transform* player;
		Rigidbody* rb;
		SpriteRenderer* sr;
		Animator* anim;

		void Start () override {
			player = Depends<Transform>();
			rb = Depends<Rigidbody>();
			sr = Depends<SpriteRenderer>();
			anim = Depends<Animator>();
		}

		void LateUpdate () override {
			cam->pos = player->position;
		}

		void Update () override {
			
			yvel += gravity * Time.deltaTime;
			if (grounded) {
				yvel = Math::Clamp(yvel, -2, 20);
			}
			
			int xvel = Input.GetAxisRaw("Horizontal") * 5;
			
			if (xvel < 0) sr->flip = SDL_FLIP_HORIZONTAL;
			if (xvel > 0) sr->flip = SDL_FLIP_NONE;

			if (grounded && Input.GetKey(SDLK_SPACE)) {
				yvel = 8;
				grounded = false;
				anim->Play(2);
			}

			rb->velocity = Vector2(xvel, yvel);

			if (rb->contacts.size() == 0) grounded = false;

			if (grounded) {
				if (xvel != 0) {
					anim->Play(1);
				}
				else {
					anim->Play(0);
				}
			}
		}

		void OnCollisionEnter (Collider* collider) override {
			grounded = true;
		}

		void Render () override {
			//Debug.DrawLine(player->position, {5,5}, {255,0,0});
		}
};

void mario () {
	auto player = new Entity("Player");
	auto sr = player->AddComponent<SpriteRenderer>();
	sr->texture = Resources::LoadTexture("demo/res/pinky_run.png");
	sr->size = 2;

	auto anim = player->AddComponent<Animator>();
	std::vector<Animation> arr = {
		{
			Resources::LoadTexture("demo/res/pinky_idle.png"), 4, 4
		},
		{
			Resources::LoadTexture("demo/res/pinky_run.png"), 2, 8
		},
		{
			Resources::LoadTexture("demo/res/pinky_jump.png"), 6, 8, false, Vector2(64,64)
		}
	};
	anim->Add(arr);

	player->AddComponent<Collider>()->size = {0.52, 0.65};
	player->AddComponent<Rigidbody>();
	player->AddComponent<MarioController>();

	auto g0 = new Entity("g0");
	g0->AddComponent<SpriteRenderer>();
	g0->AddComponent<Collider>();
	auto g0t = g0->AddComponent<Transform>();
	g0t->position = {5,-4.5};
	g0t->scale = {10,1};

	auto g1 = new Entity("g1");
	g1->AddComponent<SpriteRenderer>();
	g1->AddComponent<Collider>();
	auto g1t = g1->AddComponent<Transform>();
	g1t->position = {5,-4};
	g1t->scale = {1,5};
}