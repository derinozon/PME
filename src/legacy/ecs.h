#pragma once

namespace PME {
	class Entity;
	

	class Component {
		
		public:
		Entity* entity = nullptr;

		Component () {
			//Debug::Log(T);
		}
		// Copy constructor 4 cloning
		// Component(const Component& rhs) {
		// 	Internal::Log("Component COPY");
		// }

		template <class T>
		T* AddComponent ();

		template <class T> 
		void depends () {
			AddComponent<T>();
		}
		
		virtual void Start(){}
		virtual void Update(){}
		virtual void LateUpdate(){}
		virtual void Render(){}

		virtual ~Component(){}
	};

	class Entity {
		private:
		std::vector<Component*> components;

        public:
		std::string name = "New Entity";

		
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

		Entity () {
			SceneManager::Get().GetActiveScene()->entityList.push_back(this);
			Internal::Log(name + std::string(" created"));
		}
		Entity (std::string entityName) {
			SceneManager::Get().GetActiveScene()->entityList.push_back(this);
			name = entityName;
			Internal::Log(name + std::string(" created"));
		}
		~Entity () {
			for (auto comp : components) {
				delete comp;
			}
			Internal::Log(name + std::string(" destroyed"));
		}

		Entity* Find (std::string entityName) {
			for (auto ent : SceneManager::Get().GetActiveScene()->entityList) {
				if (ent->name == entityName) {
					return ent;
				}
			}
			return nullptr;
		}
		
		template <class T>
		T* AddComponent () {
			Component* ret = new T();
			ret->entity = this;
			components.push_back(ret);
			return (T*) ret;
		}
		
		Component* GetComponent (int index) {
			Component* ret = components[index];
			if (ret == nullptr) {
				Debug.LogError("Component at index not found");
			}
			return ret;
		}

		template <class T>
		T* GetComponent () {
			T* ret = Get<T>();
			if (ret == nullptr)
				Debug.LogError("Component of type not found");
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


	template <class T>
	T* Component::AddComponent () {
		return entity->AddComponent<T>();
	}

}