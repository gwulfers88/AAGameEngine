#pragma once
#include"Renderer.h"

namespace AAEngine 
{
	class RendererManager
	{
	public:
		RendererManager();
		~RendererManager();

		static void SingleTon();
		static void SortRenderer(std::string mesnType,Renderer* renderer);

		static RendererManager* instance;
		static std::map<std::string, std::vector<Renderer*>> RendererTypeByMeshType;
	};
}

