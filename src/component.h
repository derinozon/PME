#pragma once

namespace PME {
	class Collider;
	
	class Component {
		
		public:
		Entity* entity = nullptr;
		std::string name = "New Component";
		bool enabled = true;

		// Copy constructor 4 cloning
		// Component(const Component& rhs) {
		// 	Internal::Log("Component COPY");
		// }

		template <class T>
		T* AddComponent ();
		template <class T>
		T* Get ();
		template <class T>
		T* GetComponent ();
		Component* GetComponent (int index);
		
		template <class T>
		T* Depends () {
			T* ret = Get<T>();
			if (ret == nullptr) {
				ret = AddComponent<T>();
			}
			return ret;
		}

		virtual void Start(){}
		virtual void Update(){}
		virtual void LateUpdate(){}
		virtual void Render(){}
		virtual void OnCollisionEnter(Collider* collider){}
		virtual void OnCollisionStay(Collider* collider){}
		virtual void OnCollisionExit(){}
		virtual void OnTriggerEnter(Collider* trigger){}
		virtual void OnTriggerStay(Collider* trigger){}
		virtual void OnTriggerExit(Collider* trigger){}
		
		virtual ~Component(){}
	};
}