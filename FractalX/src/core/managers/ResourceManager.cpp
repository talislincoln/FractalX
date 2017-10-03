#include <FractalPCH.h>
#include <core\managers\ResourceManager.h>
namespace fractal
{
	namespace fcore
	{
		ResourceManager::ResourceManager () : IManager<ResourceManager> ()
		{

		}

		ResourceManager::~ResourceManager ()
		{

		}

		bool ResourceManager::Init ()
		{
			
			return true;
		}

		void ResourceManager::Update ()
		{

		}

		bool ResourceManager::Shutdown ()
		{

			return true;
		}
	}
}