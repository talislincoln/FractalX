#pragma once
#ifndef _RESOURCEMANAGER_H
#define _RESOURCEMANAGER_H

#include "IManager.h"
namespace fractal
{
	namespace fcore
	{
		class ResourceManager : public IManager<ResourceManager>
		{
			ResourceManager ();
			~ResourceManager ();

			bool Init () override;
			bool Shutdown () override;
			void Update () override;
		};
	}
}
#endif // !_RESOURCEMANAGER_H
