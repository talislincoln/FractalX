#pragma once
#ifndef _SYSTEMMANAGER_H
#define _SYSTEMMANAGER_H

#include "IManager.h"

namespace fractal
{
	namespace fcore
	{
		class SystemManager : public IManager<SystemManager>
		{

		public:
			bool Init () override;
			bool Shutdown () override;

			SystemManager () : IManager<SystemManager>() {}
			~SystemManager () {}

		};
	}
}

#endif