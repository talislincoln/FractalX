#include <FractalPCH.h>
#include <core/systems/Window.h>
#include <core\managers\LogManager.h>
#include <core\systems\Graphics.h>
#include <core\systems\Clock.h>
#include <core\managers\SystemManager.h>

namespace fractal
{
	namespace fcore
	{
		Window::Window ()
			: System(SystemType::WINDOW_SYSTEM),
			m_hDeviceContext (0),
			m_hWindow (0),
			m_hInstance (GetModuleHandle(NULL)),
			m_windowRect(),
			m_extendedStyle(0),
			m_style(0),
			m_pixelFormat(0),
			m_windowClassname(L"Fractal"),
			m_windowTitle(L"Fractal"),
			m_windowWidth(800 /* TODO: change to Singleton<WorldSettings>::getInstance().getApplicationSettings().getWindowWidth();*/),
			m_windowHeight(600 /* TODO: change to Singleton<WorldSettings>::getInstance().getApplicationSettings().getWindowHeight(); */),
			m_windowCaption(L"Fractal Game Engine")
		{
			//empty
		}

		Window::~Window ()
		{
			// empty
		}

		bool Window::Init ()
		{
			WNDCLASS wc;
			wc.style = CS_HREDRAW | CS_VREDRAW;
			wc.lpfnWndProc = WndProc;
			wc.cbClsExtra = 0;
			wc.cbWndExtra = 0;
			wc.hInstance = m_hAppInst;
			wc.hIcon = LoadIcon (0, IDI_APPLICATION);
			wc.hCursor = LoadCursor (0, IDC_ARROW);
			wc.hbrBackground = (HBRUSH)GetStockObject (NULL_BRUSH);
			wc.lpszMenuName = 0;
			wc.lpszClassName = m_windowClassname.c_str();

			if (!RegisterClass (&wc))
			{
				MessageBox (0, L"RegisterClass Failed.", 0, 0);
				return false;
			}

			// Compute window rectangle dimensions based on requested client area dimensions.
			RECT R = { 0, 0, m_windowWidth, m_windowHeight };
			AdjustWindowRect (&R, WS_OVERLAPPEDWINDOW, false);
			int width = R.right - R.left;
			int height = R.bottom - R.top;

			m_hWindow = CreateWindow (m_windowClassname.c_str(), m_windowCaption.c_str (),
				WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, m_hAppInst, this);
			if (!m_hWindow)
			{
				MessageBox (0, L"CreateWindow Failed.", 0, 0);
				return false;
			}

			ShowWindow (m_hWindow, SW_SHOW);
			UpdateWindow (m_hWindow);

			return true;

			/*if (!SpawnWindow ())
			{
				Shutdown ();
				return false;
			}

			return true;*/
		}

		void Window::Update ()
		{

		}

		bool Window::Shutdown ()
		{
			// TODO: ONLY DO THE FOLLOWING 2 IF FULL Screen - SHOULD CHECK THE APPLICATION SETTINGS
			ChangeDisplaySettings (NULL, 0);
			ShowCursor (true);

			DestroyWindow ();

			return true;
		}

		void Window::OnResize ()
		{
			
		}

		LRESULT CALLBACK Window::WndProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			
			if (uMsg == WM_CREATE)
			{
				// if the message is WM_CREATE, the lParam contains a pointer to a CREATESTRUCT
				// the CREATESTRUCT contains the "this" pointer from the CreateWindow method
				// the "this" pointer of our app is stored in the createstruct pcs->lpCreateParams
				CREATESTRUCT *pCS = (CREATESTRUCT*)lParam;
				SetWindowLongPtr (hWnd, GWLP_USERDATA, (LONG_PTR)pCS->lpCreateParams);
			}
			else
			{
				//retrieve the stored "this" pointer
				Window* window = (Window*)(GetWindowLongPtr (hWnd, GWLP_USERDATA));
				if (window)
				{
					return window->HandleEvent (hWnd, uMsg, wParam, lParam);
				}
					
			}
			return DefWindowProc (hWnd, uMsg, wParam, lParam);
		}

		LRESULT Window::HandleEvent (HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
		{
			// TODO: Send MESSAGES BASED ON THE EVENTS
			// Send messages: OnResize(), OnButtonDown, OnButtonUp, OnPaused, and so on...


			switch (msg)
			{
				// WM_ACTIVATE is sent when the window is activated or deactivated.  
				// We pause the game when the window is deactivated and unpause it 
				// when it becomes active.  
			case WM_ACTIVATE:
			{
				Clock* c = static_cast<Clock*>(SystemManager::Instance ()->GetSystem (SystemType::TIMER_SYSTEM));
				if (c)
				{
					if (LOWORD (wparam) == WA_INACTIVE)
					{
						m_appPaused = true;
						c->Stop ();
					}
					else
					{
						m_appPaused = false;
						c->Start ();
					}
				}
				return 0;
			}
				// WM_SIZE is sent when the user resizes the window.  
			case WM_SIZE:
			{
				// Save the new client area dimensions.
				m_windowWidth = LOWORD (lparam);
				m_windowHeight = HIWORD (lparam);

				Graphics* graphics = dynamic_cast<Graphics*>(SystemManager::Instance ()->GetSystem (SystemType::GRAPHICS_SYSTEM));
				// graphics->onResize (LOWORD (lParam), HIWORD (lParam));
				if (graphics)
				{
					if (wparam == SIZE_MINIMIZED)
					{
						m_appPaused = true;
						m_minimized = true;
						m_maximized = false;
					}
					else if (wparam == SIZE_MAXIMIZED)
					{
						m_appPaused = false;
						m_minimized = false;
						m_maximized = true;
						graphics->OnResize ();
					}
					else if (wparam == SIZE_RESTORED)
					{

						// Restoring from minimized state?
						if (m_minimized)
						{
							m_appPaused = false;
							m_minimized = false;
							graphics->OnResize ();
						}

						// Restoring from maximized state?
						else if (m_maximized)
						{
							m_appPaused = false;
							m_maximized = false;
							graphics->OnResize ();
						}
						else if (m_resizing)
						{
							// If user is dragging the resize bars, we do not resize 
							// the buffers here because as the user continuously 
							// drags the resize bars, a stream of WM_SIZE messages are
							// sent to the window, and it would be pointless (and slow)
							// to resize for each WM_SIZE message received from dragging
							// the resize bars.  So instead, we reset after the user is 
							// done resizing the window and releases the resize bars, which 
							// sends a WM_EXITSIZEMOVE message.
						}
						else // API call such as SetWindowPos or mSwapChain->SetFullscreenState.
						{
							graphics->OnResize ();
						}
					}
				}
				return 0;
			}

			// WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
			case WM_ENTERSIZEMOVE:
			{
				m_appPaused = true;
				m_resizing = true;
				if(Clock* c = static_cast<Clock*>(SystemManager::Instance ()->GetSystem (SystemType::TIMER_SYSTEM)))
					c->Stop ();
				return 0;
			}
			// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
			// Here we reset everything based on the new window dimensions.
			case WM_EXITSIZEMOVE:
			{
				m_appPaused = false;
				m_resizing = false;
				if(Clock* c = static_cast<Clock*>(SystemManager::Instance ()->GetSystem (SystemType::TIMER_SYSTEM)))
					c->Start ();
				OnResize ();
				return 0;
			}
			// WM_DESTROY is sent when the window is being destroyed.
			case WM_DESTROY:
				PostQuitMessage (0);
				return 0;
			
			// The WM_MENUCHAR message is sent when a menu is active and the user presses 
			// a key that does not correspond to any mnemonic or accelerator key. 
			case WM_MENUCHAR:
				// Don't beep when we alt-enter.
				return MAKELRESULT (0, MNC_CLOSE);
				// Catch this message so to prevent the window from becoming too small.
			case WM_GETMINMAXINFO:
				((MINMAXINFO*)lparam)->ptMinTrackSize.x = 200;
				((MINMAXINFO*)lparam)->ptMinTrackSize.y = 200;
				return 0;
			case WM_LBUTTONDOWN:
			case WM_MBUTTONDOWN:
			case WM_RBUTTONDOWN:
				//OnMouseDown (wparam, GET_X_LPARAM (lparam), GET_Y_LPARAM (lparam));
				return 0;
			case WM_LBUTTONUP:
			case WM_MBUTTONUP:
			case WM_RBUTTONUP:
				//OnMouseUp (wparam, GET_X_LPARAM (lparam), GET_Y_LPARAM (lparam));
				return 0;
			case WM_MOUSEMOVE:
				//OnMouseMove (wparam, GET_X_LPARAM (lparam), GET_Y_LPARAM (lparam));
				return 0;
			}

			return DefWindowProc (hwnd, msg, wparam, lparam);
		}

		const FString Window::GetWindowTitle () const
		{
			return m_windowTitle;
		}

		const FString Window::GetWindowClassName () const
		{
			return m_windowClassname;
		}

		const POINT Window::GetWindowPosition () const
		{
			return GetCenterPosition ();
		}

		HDC Window::GetWindowDeviceContext () const
		{
			return m_hDeviceContext;
		}

		HWND Window::GetWindowHandle () const
		{
			return m_hWindow;
		}

		HINSTANCE Window::GetWindowInstance () const
		{
			return m_hInstance;
		}

		unsigned int Window::GetWindowWidth () const
		{
			return m_windowWidth;
		}

		unsigned int Window::GetWindowHeight () const
		{
			return m_windowHeight;
		}

		float Window::AspectRation () const
		{
			// convert to float before doing the calculation
			return static_cast<float>(m_windowWidth) / m_windowHeight;
		}

		const POINT Window::GetCenterPosition () const
		{
			int windowwidth = this->m_windowRect.right - this->m_windowRect.left;
			int windowheight = this->m_windowRect.bottom - this->m_windowRect.top;

			return POINT
			{
				GetSystemMetrics (SM_CXSCREEN) / 2 - windowwidth / 2,
				GetSystemMetrics (SM_CYSCREEN) / 2 - windowheight / 2
			};
		}

		bool Window::SetupWindow ()
		{
			this->m_hWindow = CreateWindowEx (this->m_extendedStyle,
				this->m_windowClassname.c_str (),
				this->m_windowTitle.c_str (),
				this->m_style |
				WS_CLIPSIBLINGS |
				WS_CLIPCHILDREN,
				GetWindowPosition ().x, GetWindowPosition ().y,
				this->m_windowRect.right - this->m_windowRect.left,
				this->m_windowRect.bottom - this->m_windowRect.top,
				NULL,
				NULL,
				this->m_hInstance,
				this);

			int error = GetLastError ();
			if (!this->m_hWindow)
			{
				// TODO: Singleton<Logger>::getInstance ().log (_T ("Creation of our window failed."), LOGTYPE_ERROR);
				return false;
			}

			return true;
		}

		bool Window::ErrorHandling ()
		{
			//Check if we have a device context
			if (!(this->m_hDeviceContext = GetDC (this->m_hWindow)))
			{
				// TODO: Singleton<Logger>::getInstance ().log (_T ("Can't create a \"GLDC\"."), LOGTYPE_ERROR);
				return false;
			}

			//Check the pixel format
			PIXELFORMATDESCRIPTOR pfd = CreateWindowPixelFormatDescription ();

			this->m_pixelFormat = ChoosePixelFormat (this->m_hDeviceContext, &pfd);
			if (!this->m_pixelFormat)
			{
				// TODO: Singleton<Logger>::getInstance ().log (_T ("Can't find a suitable \"PixelFormat\"."), LOGTYPE_ERROR);
				return false;
			}
			if (!SetPixelFormat (this->m_hDeviceContext, this->m_pixelFormat, &pfd))
			{
				// TODO: Singleton<Logger>::getInstance ().log (_T ("Can't set the \"PixelFormat\"."), LOGTYPE_ERROR);
				return false;
			}

			return true;
		}

		bool Window::SpawnWindow ()
		{
			this->m_windowRect = CreateWindowRect ();

			this->m_windowClassname = GetWindowClassName ();
			this->m_windowTitle = GetWindowTitle ();

			if (!RegisterClass (&CreateWindowClass ()))
			{
				int value = GetLastError ();
				// TODO: Singleton<Logger>::getInstance ().log (_T ("Register \"WNDCLASS\" failed."), LOGTYPE_ERROR);
				return false;
			}

			if (!SetupWindow ())
				return false;
			if (!ErrorHandling ())
				return false;

			ShowWindow (this->m_hWindow, SW_SHOW);
			UpdateWindow (this->m_hWindow);
			SetForegroundWindow (this->m_hWindow);
			SetFocus (this->m_hWindow);

			return true;
		}

		WNDCLASS Window::CreateWindowClass () const
		{
			LPCTSTR window_classname = GetWindowClassName ().c_str ();

			WNDCLASS wc;
			memset (&wc, NULL, sizeof (WNDCLASS));

			wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
			wc.lpfnWndProc = (WNDPROC)Window::WndProc;
			wc.cbClsExtra = 0;
			wc.cbWndExtra = 0;
			wc.hInstance = this->m_hInstance;

			wc.hIcon = LoadIcon (0, IDI_APPLICATION);
			wc.hCursor = LoadCursor (0, IDC_ARROW);

			wc.hbrBackground = (HBRUSH)GetStockObject (NULL_BRUSH);;
			wc.lpszMenuName = 0;

			wc.lpszClassName = this->m_windowClassname.c_str ();

			return wc;
		}

		RECT Window::CreateWindowRect () const
		{
			return RECT { 0, 0, (LONG)GetWindowWidth (), (LONG)GetWindowHeight () };
		}

		bool Window::DestroyWindow ()
		{
			LPCTSTR window_classname = GetWindowClassName ().c_str ();

			if (this->m_hDeviceContext && !ReleaseDC (this->m_hWindow, this->m_hDeviceContext))
			{
				// TODO: Singleton<Logger>::getInstance ().log (_T ("Release \"DC\" failed."), LOGTYPE_ERROR);
				this->m_hDeviceContext = nullptr;
			}
			if (this->m_hWindow && !::DestroyWindow (this->m_hWindow))
			{
				// TODO: Singleton<Logger>::getInstance ().log (_T ("Release \"HWND\" failed"), LOGTYPE_ERROR);
				this->m_hWindow = nullptr;
			}

			if (!UnregisterClass (this->m_windowClassname.c_str (), this->m_hInstance))
			{
				// TODO: Singleton<Logger>::getInstance ().log (_T ("Unregister \"WNDCLASS\" failed"), LOGTYPE_ERROR);
				this->m_hInstance = nullptr;
			}

			return true;
		}

		PIXELFORMATDESCRIPTOR Window::CreateWindowPixelFormatDescription () const
		{
			return PIXELFORMATDESCRIPTOR
			{
				sizeof (PIXELFORMATDESCRIPTOR),
				1,
				PFD_DRAW_TO_WINDOW |
				PFD_SUPPORT_OPENGL |
				PFD_DOUBLEBUFFER,
				PFD_TYPE_RGBA,
				32,
				0, 0, 0, 0, 0, 0,
				0,
				0,
				0,
				0, 0, 0, 0,
				16,
				0,
				0,
				PFD_MAIN_PLANE,
				0,
				0, 0, 0
			};
		}
	}
}
