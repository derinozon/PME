#pragma once

namespace PME {
	#if MIX == 1
    class AudioSource : public Component {
		private:
			
        public:
		AudioClip* audio;
		bool loop;

		AudioSource () {
			Internal::Log("AudioSource created");
		}

		virtual ~AudioSource () {
			Mix_FreeChunk(audio);
			Internal::Log("AudioSource destroyed");
		}

		void Start () override {
			if (!Mix_PlayChannel(-1, audio, loop ? -1 : 0)) {
				Debug.LogError(Mix_GetError());
			}
		}
    };
	#endif
}