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

			return true;
		}

		void ResourceManager::Update ()
		{

		}

		bool ResourceManager::Shutdown ()
		{
			for (auto it : this->m_resources)
			{
				if (!it.second->Shutdown ())
					return false;

				SafeDelete(it.second);
			}

			m_resources.clear ();

			return true;
		}

		void ResourceManager::AddResource (Resource* r)
		{
			if (!r)
				return;

			if (!r->HasInit ())
				r->Init ();

			m_resources.insert ({ r->GetResourceName(), r });
		}
	}
}