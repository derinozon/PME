

using namespace PME;
using namespace std;

class FpsCounter : public Component {
	private:
		Text* text;

	public:
		
		void Start () override {
			text = Depends<Text>();
			text->font = TTF_OpenFont("demo/res/Efforts-Pro.ttf", ppu);
		}

		void Update () override {
			int fff = (1/Time.deltaTime);
			text->content = std::to_string(fff).c_str();
		}

};