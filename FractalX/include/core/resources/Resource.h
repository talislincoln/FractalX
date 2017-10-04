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

			virtual bool Init ();
			virtual bool Shutdown ();

			const FString& GetResourcePath () const;
			const FString& GetResourceName () const;

			ResourceType GetResourceType () const;

			const unsigned __int32 GetId () const;

			inline bool HasInit () const
			{
				return m_hasInit;
			}

		protected:
			FString m_resourcePath;
			FString m_resourceName;
			unsigned __int32 m_id;
			bool m_hasInit;
			bool m_hasShutdown;
			ResourceType m_resourceType;

			static unsigned __int32 m_idCounter;
		};
	}
}

#endif // !_RESOURCE_H
