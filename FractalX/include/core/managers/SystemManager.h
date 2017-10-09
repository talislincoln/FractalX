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

			void CreateSystems ();

			bool Init () override;
			void Update () override;
			void Draw () const override;
			bool Shutdown () override;

			class Graphics* GetGraphicsSystem () const;
			class Window*	GetWindowSystem () const;
			class Clock*	GetClockSystem () const;

			class System* GetSystem (SystemType type) const;
			std::vector<class System*> GetSystems () const;
			std::vector<class IDrawable*> GetDrawableSystems () const;
		};
	}
}

#endif