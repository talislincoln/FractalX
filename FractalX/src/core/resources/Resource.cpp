#include <FractalPCH.h>
#include <core\resources\Resource.h>

#include <core\EngineCore.h>

namespace fractal
{
	namespace fcore
	{
		unsigned __int32 Resource::m_idCounter = 0;

		Resource::Resource (const FString& resourcePath, ResourceType type) :
			m_resourcePath (resourcePath),
			m_id (++m_idCounter),
			m_hasInit (false),
			m_hasShutdown (false)
			//m_resourceName (GetNameFromPath (resourcePath))
		{
			// TODO: Check if this function is working properly lol
			int start_index = m_resourcePath.find_last_of ('\\');
			if (start_index == FString::npos)
				start_index = m_resourcePath.find_last_of ('/');
			++start_index;
			int end_index = m_resourcePath.find_last_of ('.');

			m_resourceName = m_resourcePath.substr (start_index, end_index - start_index);

			ResourceManager* resourceManager = ResourceManager::Instance ();
			resourceManager->AddResource (this);
		}

		Resource::~Resource ()
		{
			// empty
		}

		bool Resource::Init ()
		{
			m_hasInit = true;

			return true;
		}

		bool Resource::Shutdown ()
		{
			m_hasShutdown = true;

			return true;
		}

		const FString& Resource::GetResourcePath () const
		{
			return this->m_resourcePath;
		}

		const FString& Resource::GetResourceName () const
		{
			return this->m_resourceName;
		}

		ResourceType Resource::GetResourceType () const
		{
			return this->m_resourceType;
		}

		const unsigned __int32 Resource::GetId () const
		{
			return m_id;
		}
	}
}