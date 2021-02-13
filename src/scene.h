#pragma once

#include <vector>

namespace PME {
	class Entity;

	struct Scene {
		const char* name;
		std::vector<Entity*> entityList;

		Scene (const char* sceneName) {
			name = sceneName;
		}

		Scene () {
			name = "New Scene";
		}

		~Scene () {
			//for (auto ent : entityList) {
			//	delete ent;
			//}
			//Internal::Log(name + std::string(" destroyed"));
		}
	};

	class SceneManager {
		public:
			SceneManager(const SceneManager&) = delete;

			static SceneManager& Get () {
				return instance;
			}

			Scene* GetActiveScene () {
				if (activeScene == nullptr) {
					activeScene = new Scene();
				}
				return activeScene;
			}

			~SceneManager () {
				if (activeScene != nullptr)
					delete activeScene;
			}

		private:
			static SceneManager instance;
		
			SceneManager () {

			}

			Scene* activeScene;

			
	};
	SceneManager SceneManager::instance;
}