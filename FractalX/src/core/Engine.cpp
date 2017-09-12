#include <FractalPCH.h>
#include <core\Engine.h>
#include <core\AbstractGame.h>
#include <core\managers\SystemManager.h>
#include <core\systems\Window.h>

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
			if (!this->Init())
			{
				//Logger.Error("Failed to Init engine");
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
				// TODO ERROR MESSAGE
				return false;
			}
			
			if (!window->Init ())
				return false;

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

			return true;
		}

		void Engine::Update()
		{

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
