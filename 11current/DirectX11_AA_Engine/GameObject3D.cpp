#include"GameObject3D.h"
#include"Mesh.h"
#include"MeshFilter.h"
#include"Material.h"
#include"MeshManager.h"
#include"Renderer.h"
#include"RendererManager.h"

namespace AAEngine
{
	GameObject3D::GameObject3D() : m_name("")
	{
		parent = nullptr;
	}

	GameObject3D::GameObject3D(std::string name)
	{
		m_name = name;
	}

	GameObject3D::~GameObject3D()
	{

	}

	void GameObject3D::AddChild(GameObject3D* obj)
	{
		children.push_back(obj);
		obj->parent = this;
	}

	Component* GameObject3D::AddComponent(std::string name)
	{	
		Component* instance = (Component*)Component::CreateInstance(name,this);//make a instance of the Component, also assing the point of the object

		m_componentVec.push_back(std::make_pair(name, instance));

		return instance;
	}

	void GameObject3D::Update(float dt)
	{
	
	}

	GameObject3D* GameObject3D::CreatePrimitive(Primitive type)
	{
		if (type == Triangle)
		{
			return MakeATriangle();
		}

		return 0;
	}

	GameObject3D* GameObject3D::MakeATriangle()
	{
		std::string t = "Triangle";

		GameObject3D* obj = new GameObject3D("Obj_with_Triangle"); // make a new game object to hold the object

		Renderer* renderer = (Renderer*)obj->AddComponent("Renderer");

		renderer->sharedMesh = t;

		renderer->material = new Material();

		RendererManager::instance->SortRenderer(t, renderer);

		return obj;
	}
}

