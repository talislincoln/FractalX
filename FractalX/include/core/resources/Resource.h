#pragma once
#ifndef _RESOURCE_H
#define _RESOURCE_H

namespace fractal
{
	namespace fcore
	{
		class Resource
		{
		public:
			Resource (const FString& resourcePath, ResourceType type);
			virtual ~Resource ();

			virtual bool Init () = 0;
			virtual bool Shutdown () = 0;

			const FString& GetResourcePath () const;
			const FString& GetResourceName () const;

			ResourceType GetResourceType () const;

			const unsigned __int32 GetId () const;

		protected:
			FString m_resourcePath;
			FString m_resourceName;
			ResourceType m_resourceType;

			static unsigned __int32 m_id;
		};
	}
}

#endif // !_RESOURCE_H
