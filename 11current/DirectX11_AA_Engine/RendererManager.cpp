#include "RendererManager.h"

namespace AAEngine 
{
    RendererManager* RendererManager::instance = nullptr;
	std::map<std::string, std::vector<Renderer*>> RendererManager::RendererTypeByMeshType;

	RendererManager::RendererManager()
	{

	}
	
	RendererManager::~RendererManager()
	{
		delete instance;
		instance = nullptr;
	}

	void RendererManager::SortRenderer(std::string meshType, Renderer* renderer)
	{
		if (RendererTypeByMeshType.find(meshType) != RendererTypeByMeshType.end())
		{
			RendererTypeByMeshType[meshType].push_back(renderer);
		}
		else
		{
			std::vector<Renderer*> renderers;
			renderers.push_back(renderer);
			RendererTypeByMeshType[meshType] = renderers;
		}
	}

	void RendererManager::SingleTon()
	{
		if (instance == nullptr)
		{
			instance = new RendererManager();
		}
	}
}