#include <FractalPCH.h>
#include <core\managers\SystemManager.h>
#include <core\systems\System.h>
#include <core\systems\Window.h>
#include <algorithm>

namespace fractal
{
	namespace fcore
	{
		bool SystemManager::Init ()
		{
			return true;
		}

		bool SystemManager::Shutdown ()
		{
			return true;
		}

		System* SystemManager::GetSystem (SystemType type)
		{
			//look for a position in the vector that satisfies the lambda function
			std::vector<System*>::const_iterator it = std::find_if (this->m_systems.begin (), this->m_systems.end (),
				[type](System* s) -> bool
				{
					return s->GetType () == type;
				});

			//if the system was found, then just return the system
			if (it != this->m_systems.end ())
				return (*it);

			//if there isn't a system of that type, then create one
			// TODO: create a factory for the systems
			//System* system = this->m_factory->createSystem (type);
			System* system = new Window ();

			//cast the new created system to check if it is a drawable system
			//if (dynamic_cast<IDrawable*>(system) != nullptr)
			//	this->m_drawableSystems.push_back (dynamic_cast<IDrawable*>(system));

			this->m_systems.push_back (system);

			return system;
		}

		std::vector<System*> SystemManager::GetSystems () const
		{
			return m_systems;
		}
	}
}