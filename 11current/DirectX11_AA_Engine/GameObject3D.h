#pragma once
#include <vector>
#include "Component.h"
#include"Math/AAMath.h"


namespace AAEngine {

	struct Transform {
		Vector3 position;
		Vector3 rotation;
		Vector3 scale;
	};

	enum Primitive
	{
		Triangle,Quard, Cube, Sphere
	};

	class GameObject3D
	{
	public:
		GameObject3D(void);
		~GameObject3D(void);
		GameObject3D(std::string name);
		void AddChild(GameObject3D* obj);
		Component* AddComponent(std::string name);

		void Update(float dt);

		static GameObject3D* CreatePrimitive(Primitive type);
		static GameObject3D* MakeATriangle();

	public:
		GameObject3D* parent;
		std::vector<GameObject3D*> children;
		std::string  m_name;
		typedef std::pair<std::string, Component*> ComponentPair;
		std::vector<ComponentPair> m_componentVec;
		Transform transform;
	};
}

//struct ....... logicless

