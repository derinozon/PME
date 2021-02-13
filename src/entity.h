#pragma once

namespace PME {
	class Entity {
		private:
		std::vector<Component*> components;
		

        public:
		Entity* parent = nullptr;
		std::vector<Entity*> childs;
		std::string name;
		std::string tag;
		Uint16 id;
		bool enabled = true;

		
		Entity(const Entity& rhs) {
			for (int i = 0; i < rhs.components.size(); i++) {
				Component* comp = new Component(*rhs.components[i]);
				components.push_back(comp);
			}
			
			components = rhs.components;
			name = rhs.name;
			
			SceneManager::Get().GetActiveScene()->entityList.push_back(this);
			Internal::Log("Cloned entity " + name);
		}
  		//Entity& operator=(const Entity& rhs) {};

		Entity () : Entity("New Entity", "", nullptr) {}
		Entity (std::string name) : Entity(name, "", nullptr) {}
		Entity (Entity* parent) : Entity("New Entity", "", parent) {}
		Entity (std::string name, std::string tag) : Entity(name, tag, nullptr) {}

		Entity (std::string name, std::string tag, Entity* parent) {
			this->name = name;
			this->parent = parent;
			this->tag = tag;

			if (parent != nullptr)
				parent->childs.push_back(this);

			Scene* scn = SceneManager::Get().GetActiveScene();

			scn->entityList.push_back(this);
			id = scn->entityList.size()-1;

			Internal::Log(name + std::string(" created"));
		}
		~Entity () {
			
			for (auto comp : components) {
				if (comp != nullptr) {
					delete comp;
					Internal::Log("Deleted " + comp->name + " on " + name);
				}
			}

			Internal::Log("Deleted " + name);
		}

		static void Destroy (Entity* entity) {
			Scene* scn = SceneManager::Get().GetActiveScene();
			for (int i = 0; i < scn->entityList.size(); i++) {
				if (scn->entityList[i] == entity) {
					scn->entityList.erase(scn->entityList.begin()+i);
				}
			}
		}

		static Entity* Find (std::string name) {
			Scene* scn = SceneManager::Get().GetActiveScene();
			for (int i = 0; i < scn->entityList.size(); i++) {
				if (scn->entityList[i]->name == name) {
					return scn->entityList[i];
				}
			}
			return nullptr;
		}

		template <class T>
		T* AddComponent () {
			Component* ret = new T();
			ret->entity = this;
			components.push_back(ret);

			auto log = "Added " + ret->name + " to " + name;
			Internal::Log(log);
			return (T*) ret;
		}
		
		Component* GetComponent (int index) {
			Component* ret = components[index];
			if (ret == nullptr) {
				Debug::LogError("Component at index not found");
			}
			return ret;
		}

		template <class T>
		T* GetComponent () {
			T* ret = Get<T>();
			if (ret == nullptr)
				Debug::LogError("Component of type not found");
			return ret;
		}

		template <class T>
		std::vector<T*> GetComponentsOfType () {
			std::vector<T*> ret;
			for (int i = 0; i < components.size(); i++) {
				T* comp = dynamic_cast<T*>(components[i]);
				if (comp)
					ret.push_back(comp);
			}
			return ret;
		}

		template <class T>
		T* Get () {
			T* ret;
			for (int i = 0; i < components.size(); i++) {
				ret = dynamic_cast<T*>(components[i]);
				if (ret) return ret;
			}
			return nullptr;
		}

		template <class T>
		void RemoveComponent () {
			for (int i = 0; i < components.size(); i++) {
				if (dynamic_cast<T*>(components[i])) {
					delete components[i];
					components.erase(components.begin()+i);
				}
			}
		}

		void RemoveComponent (Component* component) {
			for (int i = 0; i < components.size(); i++) {
				std::cout << (component == components[i]) << std::endl;
				if (component == (components[i])) {
					delete components[i];
					components.erase(components.begin()+i);
				}
			}
		}

		int ComponentSize () {
			return components.size();
		}
	};

	// Component Implementations //

	template <class T>
	T* Component::AddComponent () {
		return entity->AddComponent<T>();
	}

	Component* Component::GetComponent (int index) {
		return entity->GetComponent(index);
	}

	template <class T>
	T* Component::GetComponent () {
		return entity->GetComponent<T>();
	}

	template <class T>
	T* Component::Get () {
		return entity->Get<T>();
	}
}