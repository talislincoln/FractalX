#include <FractalPCH.h>

#include <core\EngineCore.h>

#include <sstream>

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
			SafeDelete (m_game);
		}

		int Engine::Run()
		{
		#if defined(DEBUG) || defined(_DEBUG)  
			LogManager::Instance ()->SetLogLevel (LOG_TYPES::LOG_INFO);
		#else
			Logger::Instance ()->SetLogLevel (LOG_TYPES::LOG_WARNING);
		#endif

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
					CalculateFrameStats ();
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

			// create all the inner systems
			SystemManager::Instance ()->CreateSystems ();
			
			// setting the game before the logic system has initialized
			dynamic_cast<fractal::fcore::Logic*>(SystemManager::Instance ()->GetSystem (SystemType::LOGIC_SYSTEM))->SetGame(m_game);

			if (!ResourceManager::Instance ()->Init ())
			{
				LogManager::Instance ()->LogError (L"Failed to Init Resource Manager.");
				return false;
			}

			// Init all the systems inside the system manager
			if (!SystemManager::Instance ()->Init ())
			{
				LogManager::Instance ()->LogError (L"Failed to Init System Manager.");
				return false;
			}

			if (!SceneManager::Instance ()->Init ())
			{
				LogManager::Instance ()->LogError (L"Failed to Init Scene Manager.");
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

			if (!ResourceManager::Instance ())
			{
				LogManager::Instance ()->LogError (L"Failed to create system Manager");
				return false;
			}

			if (!SceneManager::Instance ())
			{
				LogManager::Instance ()->LogError (L"Failed to create Scene Manager");
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

			if (ResourceManager::Instance ())
			{
				if (!ResourceManager::Instance ()->Shutdown ())
				{
					return false;
				}
				ResourceManager::DestroyInstance ();
			}
			else
			{
				LogManager::Instance ()->LogWarning (L"Resource manager was already destroyed.");
			}

			if (SceneManager::Instance ())
			{
				if (!SceneManager::Instance ()->Shutdown ())
				{
					return false;
				}
				SceneManager::DestroyInstance ();
			}
			else
			{
				LogManager::Instance ()->LogWarning (L"Scene Manager was already destroyed.");
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
			Graphics* g = SystemManager::Instance ()->GetGraphicsSystem ();

			g->BeginDraw ();
			SystemManager::Instance ()->Draw ();
			g->EndDraw ();
		}

		void Engine::CalculateFrameStats ()
		{
			// Code computes the average frames per second, and also the 
			// average time it takes to render one frame.  These stats 
			// are appended to the window caption bar.

			static int frameCnt = 0;
			static float timeElapsed = 0.0f;

			Clock* c = static_cast<Clock*>(SystemManager::Instance ()->GetSystem (SystemType::TIMER_SYSTEM));
			Window* w = static_cast<Window*>(SystemManager::Instance ()->GetSystem (SystemType::WINDOW_SYSTEM));

			if (c && w)
			{
				frameCnt++;

				// Compute averages over one second period.
				if ((c->TotalTime () - timeElapsed) >= 1.0f)
				{
					float fps = (float)frameCnt; // fps = frameCnt / 1
					float mspf = 1000.0f / fps;

					std::wostringstream outs;
					outs.precision (6);
					outs << L"Fractal" << L"    "
						<< L"FPS: " << fps << L"    "
						<< L"Frame Time: " << mspf << L" (ms)";

					SetWindowText (w->GetWindowHandle (), outs.str().c_str());

					// Reset for next average.
					frameCnt = 0;
					timeElapsed += 1.0f;
				}
			}
		}

		bool Engine::ShutDown()
		{
			DestroyManagers ();
			return true;
		}
	}
}
