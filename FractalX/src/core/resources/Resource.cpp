#include <FractalPCH.h>
#include <core\resources\Resource.h>

namespace fractal
{
	namespace fcore
	{
		unsigned __int32 Resource::m_id = 0;

		Resource::Resource (const FString& resourcePath, ResourceType type)
		{
			int start_index = this->m_resourcePath.find_last_of ('\\');
			if (start_index == std::string::npos)
				start_index = this->m_resourcePath.find_last_of ('/');
			++start_index;
			int end_index = this->m_resourcePath.find_last_of ('.');

			this->m_resourceName = this->m_resourcePath.substr (start_index, end_index - start_index);

			++m_id;
		}

		Resource::~Resource ()
		{
			// empty
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