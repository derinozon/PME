#pragma once

namespace PME {
	
	class AudioManager {
		private:
		AudioManager () {}

		static AudioManager instance;

		SDL_AudioSpec wavSpec;
		Uint32 wavLength;
		Uint8 *wavBuffer;
		SDL_AudioDeviceID deviceId = 0;

		public:
		// Removes copy constructor
		AudioManager(const AudioManager&) = delete;

		static AudioManager& Get () {
			return instance;
		}

		void PlayWAV (const char* path) {
			SDL_LoadWAV(path, &wavSpec, &wavBuffer, &wavLength);

			if (!deviceId) {
				deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
				int success = SDL_QueueAudio(deviceId, wavBuffer, wavLength);
				SDL_PauseAudioDevice(deviceId, 0);
			}
			else {
				SDL_ClearQueuedAudio(deviceId);
				int success = SDL_QueueAudio(deviceId, wavBuffer, wavLength);
				SDL_PauseAudioDevice(deviceId, 0);
			}
			
			
		}

		

	};
	AudioManager AudioManager::instance;
}