#pragma once
#ifndef _SYSTEMMANAGER_H
#define _SYSTEMMANAGER_H

#include "IManager.h"
#include <core\interfaces\IDrawable.h>
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

		class SystemManager : public IManager<SystemManager>, public IDrawable
		{
		private:
			std::vector<class System*> m_systems;
			std::vector<class IDrawable*> m_drawableSystems;

		public:
			SystemManager () : IManager<SystemManager> () {}
			~SystemManager () {}

			bool Init () override;
			bool Shutdown () override;
			void Update () override;
			void Draw () const override;

			class System* GetSystem (SystemType type);
			std::vector<class System*> GetSystems () const;
			std::vector<class IDrawable*> GetDrawableSystems () const;
		};
	}
}

#endif