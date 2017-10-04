#include <FractalPCH.h>
#include <core\managers\ResourceManager.h>
#include <core\resources\Resource.h>

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
			/*for (std::pair<const FString&, Resource*>& pair : this->m_resources)
			{
				if (!pair.second->Init ())
					return false;
			}*/

			return true;
		}

		void ResourceManager::Update ()
		{

		}

		bool ResourceManager::Shutdown ()
		{

			return true;
		}

		void ResourceManager::AddResource (const FString& name, Resource* r)
		{
			if (!r)
				return;

			if (!r->HasInit ())
				r->Init ();

			m_resources.insert ({ name, r });

			//this->m_resources[name] = r;
		}
	}
}