#include <FractalPCH.h>
#include <core\Engine.h>
#include <core\AbstractGame.h>
#include <core\managers\SystemManager.h>
#include <core\systems\Window.h>
#include <core\systems\Clock.h>
#include <core\managers\Logger.h>

namespace fractal
{
	namespace fcore
	{
		Engine::Engine(AbstractGame* game) :
			m_game(game)
		{
		}

		Engine::~Engine()
		{
		}

		int Engine::Run()
		{
			//Logger::Instance ()->SetLogLevel (LOG_TYPES::LOG_WARNING);
			Logger::Instance ()->SetLogLevel (LOG_TYPES::LOG_INFO);
		/*#if (DEBUG) || (_DEBUG)
			Logger::Instance ()->SetLogLevel (LOG_TYPES::LOG_INFO);
		#endif*/

			Logger::Instance ()->LogInfo (L"Initializing the engine...");

			if (!this->Init())
			{
				Logger::Instance()->LogError(L"Failed to Init engine");
				return fractal::INITIALIZATION_FAILED;
			}

			//return 0; // remove this when done testing the initalization

			// initialize the random seed with the get tick function
			srand(GetTickCount());

			MSG msg = {};
			while (msg.message != WM_QUIT)
			{
				if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

				this->Update();
				this->Draw();
			}

			if (!this->ShutDown())
			{
				//Logger.Error("Failed to Shutdown the engine");
				return fractal::SHUTDOWN_FAILED;
			}

			return msg.wParam;
		}

		bool Engine::Init()
		{
			if (!this->CreateManagers ())
				return false;

			Window* window = static_cast<Window*>(SystemManager::Instance ()->GetSystem (SystemType::WINDOW_SYSTEM));
			if (!window)
			{
				Logger::Instance ()->LogError (L"Failed to get the Window instance.");
				return false;
			}

			Clock* clock = static_cast<Clock*>(SystemManager::Instance ()->GetSystem (SystemType::TIMER_SYSTEM));
			
			if (!window->Init ())
				return false;

			if (!clock->Init ())
			{
				Logger::Instance ()->LogError (L"Failed to init Clock system");
			}

			return true;
		}

		bool Engine::CreateManagers()
		{
			if (!SystemManager::Instance ())
			{
				//Logger.Error("Failed to create System Manager");
				fcout << "Failed to create system manager";
				return false;
			}

			return true;
		}

		bool Engine::DestroyManagers()
		{
			if (SystemManager::Instance ())
			{
				SystemManager::Instance ();
			}
			else
			{
				Logger::Instance ()->LogWarning (L"System manager was already destroyed.");
			}

			return true;
		}

		void Engine::Update()
		{
			Clock* c = static_cast<Clock*>(SystemManager::Instance ()->GetSystem (SystemType::TIMER_SYSTEM));
			c->Update ();
			fcout << c->TotalTime () << std::endl;
		}

		void Engine::Draw()
		{

		}

		bool Engine::ShutDown()
		{

			return true;
		}
	}
}
