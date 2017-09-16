#include <FractalPCH.h>
#include <core\managers\SystemManager.h>
#include <core\systems\System.h>
#include <core\systems\Window.h>
#include <core\systems\Clock.h>
#include <core\systems\Graphics.h>
#include <core\managers\LogManager.h>
#include <algorithm>

namespace fractal
{
	namespace fcore
	{
		System* SystemFactory::CreateSystem (SystemType type)
		{
			switch (type)
			{
			case SystemType::WINDOW_SYSTEM: return new Window ();
			case SystemType::TIMER_SYSTEM: return new Clock ();
			case SystemType::GRAPHICS_SYSTEM: return new Graphics ();
			default: return nullptr;
			}
		}

		bool SystemManager::Init ()
		{
			for (unsigned __int8 i = 0; (SystemType)i < SystemType::NUM_OF_SYSTEMS; i++)
			{
				System* s = SystemFactory::CreateSystem ((SystemType)i);

				if (s && s->Init ())
				{
					this->m_systems.push_back (s);
				}
				else
				{
					LogManager::Instance ()->LogError (L"Failed to init system");
					return false;
				}
			}

			return true;
		}

		bool SystemManager::Shutdown ()
		{
			for (System* s : m_systems)
			{
				if (!s->Shutdown ())
					return false;

				SafeDelete (s);
			}

			return true;
		}

		void SystemManager::Update ()
		{
			for (System* s : m_systems)
			{
				s->Update ();
			}
		}

		System* SystemManager::GetSystem (SystemType type)
		{
			if (m_systems.size ())
			{
				//look for a position in the vector that satisfies the lambda function
				std::vector<System*>::const_iterator it = std::find_if (this->m_systems.begin (), this->m_systems.end (),
					[type](System* s) -> bool
					{
						return s->GetType () == type;
					});

				return *it;
			}
			
			return nullptr;
		}

		std::vector<System*> SystemManager::GetSystems () const
		{
			return m_systems;
		}
	}
}