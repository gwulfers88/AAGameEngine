#pragma once

#include"RenderEngine.h"
#include"DirectInput.h"
#include"GameObject3D.h"

class GameEngineManager
{
public:
	GameEngineManager();
	~GameEngineManager();
	bool Init(HINSTANCE& hinstance, HWND& hw);
	bool MakeManagerSingleTons(HINSTANCE& hinstance, HWND& hw);
	void LoadGameObject();
	void Update();
	void ReleaseAll();

private:
	GameObject3D* obj1;
	unsigned int prewTime;
	DirectInput* keyBoardInput;
	std::vector<GameObject3D*> m_obj;
};
