#include"GameEngineManager.h"
#include"Mesh.h"
#include<memory>
#include"Timer.h"
#include"MeshFilter.h"
#include"MaterialManager.h"
#include"MeshManager.h"
#include"RendererManager.h"

GameEngineManager::GameEngineManager()
{
	obj1 = new GameObject3D();
}

GameEngineManager::~GameEngineManager()
{

}

bool GameEngineManager::Init(HINSTANCE& hinstance, HWND& hw)
{
	Timer::Inital();
	prewTime = Timer::instance->GetCurrent();

	MakeManagerSingleTons(hinstance, hw);

	LoadGameObject();

	return true;
}

bool GameEngineManager::MakeManagerSingleTons(HINSTANCE& hinstance, HWND& hw)
{
	RenderEngine::MakeSingleTon();
	if (!RenderEngine::instance->Initialize(hinstance, hw))
		return false;

	keyBoardInput = new DirectInput();
	keyBoardInput->Init(hinstance, hw);

	MaterialManager::SingleTn();

	MeshManager::SingleTon();

	RendererManager::SingleTon();

	return true;
}

void GameEngineManager::Update()
{
	unsigned int now = Timer::instance->GetCurrent();
	unsigned int dt = now - prewTime;
	float eclipse = dt / 1000;
	prewTime = now;

	obj1->Update(eclipse);

	//RenderEngine::instance->Update(eclipse);
	RenderEngine::instance->Render(eclipse);
}

void GameEngineManager::ReleaseAll()
{
	RenderEngine::instance->ReleaseAll();
}

void GameEngineManager::LoadGameObject()
{	
	obj1->transform.position = Vector3(1,1,0);
	obj1->AddComponent("RigidBody");
	obj1->AddChild(GameObject3D::CreatePrimitive(Primitive::Triangle));
}

