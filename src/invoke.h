#pragma once

namespace PME {
	class Invoke;
		std::vector<Invoke*> eventQueue;

		class Invoke {
			private:
				float startTime, endTime;
				bool rep;
				void (*ptr)();
				int id;
				
			public:
				bool canWork = true;

				Invoke (void (*callback)(), float seconds, bool repeat = false) {
					ptr = callback;
					startTime = Time.time;
					endTime = startTime + seconds;
					rep = repeat;
					eventQueue.push_back(this);
				}

				Invoke (float seconds, bool repeat = false) {
					startTime = Time.time;
					endTime = startTime + seconds;
					rep = repeat;
					eventQueue.push_back(this);
				}

				Invoke () {
				}

				~Invoke () {
					
				}

				bool Check () {
					if (!rep) {
						if (!canWork) return false;
					}
					

					if (Time.time > endTime) {
						canWork = false;
						return true;
					}
					return false;
				}

				void Check (float t) {
					if (t > endTime) {
						ptr();
						if (rep) {
							float seconds = endTime-startTime;
							startTime = Time.time;
							endTime = startTime + seconds;
						}
						else {
							//eventQueue.erase(this);
							//delete this;
						}
					}
				}

				float Progress () {
					float b = endTime - startTime;
					float c = Time.time - startTime;

					return Math::Clamp(c/b, 1);
				}
		};
}