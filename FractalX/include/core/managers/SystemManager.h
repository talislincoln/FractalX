#pragma once
#ifndef _SYSTEMMANAGER_H
#define _SYSTEMMANAGER_H

#include "IManager.h"
#include <vector>

namespace fractal
{
	namespace fcore
	{
		class SystemFactory
		{
		public:

			SystemFactory () = delete;
			SystemFactory (SystemFactory&) = delete;
			SystemFactory (SystemFactory&&) = delete;

			static class System* CreateSystem (SystemType type);
		};

		class SystemManager : public IManager<SystemManager>
		{
		private:
			std::vector<class System*> m_systems;

		public:
			SystemManager () : IManager<SystemManager> () {}
			~SystemManager () {}

			bool Init () override;
			bool Shutdown () override;
			void Update () override;

			class System* GetSystem (SystemType type);
			std::vector<class System*> GetSystems () const;
		};
	}
}

#endif