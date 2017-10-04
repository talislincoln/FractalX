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
			m_hasShutdown(false)
		{
			int start_index = this->m_resourcePath.find_last_of ('\\');
			if (start_index == FString::npos)
				start_index = this->m_resourcePath.find_last_of ('/');
			++start_index;
			int end_index = this->m_resourcePath.find_last_of ('.');

			this->m_resourceName = this->m_resourcePath.substr (start_index, end_index - start_index);
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

		inline const FString& Resource::GetResourcePath () const
		{
			return this->m_resourcePath;
		}

		inline const FString& Resource::GetResourceName () const
		{
			return this->m_resourceName;
		}

		inline ResourceType Resource::GetResourceType () const
		{
			return this->m_resourceType;
		}

		inline const unsigned __int32 Resource::GetId () const
		{
			return m_id;
		}
	}
}