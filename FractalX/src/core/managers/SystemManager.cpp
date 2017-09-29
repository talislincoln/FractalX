#include <FractalPCH.h>
#include <core\managers\SystemManager.h>
#include <core\systems\System.h>
#include <core\systems\Window.h>
#include <core\systems\Clock.h>
#include <core\systems\Graphics.h>
#include <core\managers\LogManager.h>
#include <core\systems\Logic.h>
#include <algorithm>

// TODO: look into here for implementation in open and directx http://gameprogrammingpatterns.com/singleton.html

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
			case SystemType::LOGIC_SYSTEM: return new Logic ();
			default: return nullptr;
			}
		}

		bool SystemManager::Init ()
		{
			for (unsigned __int8 i = 0; (SystemType)i < SystemType::TOTAL_SYSTEMS; i++)
			{
				System* s = SystemFactory::CreateSystem ((SystemType)i);

				if (s && s->Init ())
				{
					this->m_systems.push_back (s);

					// check if the system is drawable, then save in a different array
					IDrawable* drawable = dynamic_cast<IDrawable*>(s);
					if (drawable)
						m_drawableSystems.push_back (drawable);
				}
				else
				{
					LogManager::Instance ()->LogError (L"Failed to init system");
					return false;
				}
			}

			return true;
		}

		void SystemManager::Draw () const
		{
			for (IDrawable* d : m_drawableSystems)
			{
				if(d->GetCanDraw())
					d->Draw ();
			}
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

		std::vector<IDrawable*> SystemManager::GetDrawableSystems () const
		{
			return m_drawableSystems;
		}
	}
}