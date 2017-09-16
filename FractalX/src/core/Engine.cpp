#include <FractalPCH.h>
#include <core\Engine.h>
#include <core\AbstractGame.h>
#include <core\managers\SystemManager.h>
#include <core\systems\Window.h>
#include <core\systems\Clock.h>
#include <core\managers\LogManager.h>
#include <core\systems\Graphics.h>

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
			LogManager::Instance ()->SetLogLevel (LOG_TYPES::LOG_INFO);

			if (!this->Init())
			{
				LogManager::Instance()->LogError(L"Failed to Init engine");
				return fractal::INITIALIZATION_FAILED;
			}
			LogManager::Instance ()->LogInfo (L"Engine Initialized");

			// initialize the random seed with the get tick function
			srand(GetTickCount());

			MSG msg = {};
			while (msg.message != WM_QUIT)
			{
				if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				else
				{
					this->Update ();
					this->Draw ();
				}
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

			if (!SystemManager::Instance ()->Init ())
			{
				LogManager::Instance ()->LogError (L"Failed to Init system manager.");
				return false;
			}

			return true;
		}

		bool Engine::CreateManagers()
		{
			if (!SystemManager::Instance ())
			{
				LogManager::Instance ()->LogError (L"Failed to create system Manager");
				return false;
			}

			return true;
		}

		bool Engine::DestroyManagers()
		{
			if (SystemManager::Instance ())
			{
				if (!SystemManager::Instance ()->Shutdown ())
				{
					return false;
				}
				SystemManager::DestroyInstance ();
			}
			else
			{
				LogManager::Instance ()->LogWarning (L"System manager was already destroyed.");
			}

			if (LogManager::Instance ())
			{
				if (!LogManager::Instance ()->Shutdown ())
				{
					return false;
				}
				LogManager::DestroyInstance ();
			}
			else
			{
				LogManager::Instance ()->LogWarning (L"System manager was already destroyed.");
			}

			return true;
		}

		void Engine::Update()
		{
			SystemManager::Instance ()->Update ();
		}

		void Engine::Draw()
		{

		}

		bool Engine::ShutDown()
		{
			DestroyManagers ();
			return true;
		}
	}
}
