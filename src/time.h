#pragma once

namespace PME {
	class TimeManager {
		public:
		TimeManager(const TimeManager&) = delete;

		static TimeManager& Get () {
			return instance;
		}

		float deltaTime;
		float time;
		float lastFrameTime;
		private:
		static TimeManager instance;
		
		TimeManager () {

		}
	};
	TimeManager TimeManager::instance;
}