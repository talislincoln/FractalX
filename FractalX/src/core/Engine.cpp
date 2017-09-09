#include <FractalPCH.h>
#include <core\Engine.h>
#include <core\AbstractGame.h>

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
			return true;
		}

		bool Engine::CreateManagers()
		{

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
