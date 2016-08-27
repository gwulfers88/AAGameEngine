#pragma once

#include"DXbase.h"
#include"GameObject2D.h"
#include<vector>
#include"Renderer.h"

namespace AAEngine {
	class RenderEngine : public DXbase
	{
	public:
		RenderEngine();
		virtual ~RenderEngine();

		void Update(float dt)override;
		void Render(float dt)override;
		void CleanScreen();
		bool DrawString(char* message, float startX, float startY, ID3D11Buffer* vertexBuffer_);

		static bool MakeSingleTon();
		static RenderEngine* instance;

		float Time;
	};
}