#include <FractalPCH.h>
#include <core\resources\Resource.h>

namespace fractal
{
	namespace fcore
	{
		unsigned __int32 Resource::m_idCounter = 0;

		Resource::Resource (const FString& resourcePath, ResourceType type) :
			m_resourcePath (resourcePath),
			m_id (++m_idCounter),
			m_hasInit (false),
			m_hasShutdown (false),
			m_resourceName (resourcePath)
		{
			// TODO: Check if this function is working properly lol
			//this->m_resourceName = GetNameFromPath (resourcePath);
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