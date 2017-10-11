#pragma once
#ifndef _RESOURCEMANAGER_H
#define _RESOURCEMANAGER_H

#include "IManager.h"
#include <unordered_map>

namespace fractal
{
	namespace fcore
	{
		class ResourceManager : public IManager<ResourceManager>
		{
		private:
			std::unordered_map<FString, class Resource*> m_resources;

		public:

			ResourceManager ();
			~ResourceManager ();

			bool Init () override;
			bool Shutdown () override;
			void Update () override;

			void AddResource (class Resource* r);

			template<typename T> T* GetResource (const FString& path);
		};

		template<typename T>
		T* ResourceManager::GetResource (const FString& path)
		{
			if (this->m_resources[path] != nullptr)
				return dynamic_cast<T*>(this->m_resources[path]);

			// if didn't find the resouce, return nullptr
			return nullptr;
		}
	}
}
#endif // !_RESOURCEMANAGER_H
