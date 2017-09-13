#include <FractalPCH.h>
#include <core/systems/Window.h>

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
			m_windowHeight(600 /* TODO: change to Singleton<WorldSettings>::getInstance().getApplicationSettings().getWindowHeight(); */)
		{
			//empty
		}

		Window::~Window ()
		{
			// empty
		}

		bool Window::Init ()
		{
			if (!SpawnWindow ())
			{
				Shutdown ();
				return false;
			}

			return true;
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

			return false;
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
				if (window != nullptr)
					return window->HandleEvent (hWnd, uMsg, wParam, lParam);
			}
			return DefWindowProc (hWnd, uMsg, wParam, lParam);
		}

		LRESULT Window::HandleEvent (HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
		{
			switch (msg)
			{
			case WM_ACTIVATE:
			{
				if (!HIWORD (wparam))
					this->SetActive (true);
				else
					this->SetActive (false);

				return 0;
			}

			case WM_SYSCOMMAND:
			{
				switch (wparam)
				{
				case SC_SCREENSAVE:
				case SC_MONITORPOWER:
					return 0;
				}
				break;
			}

			case WM_SIZE:
			{
				// TODO: FIX THE RESIZE IN THE GRAPHICS
				// Graphics* graphics = dynamic_cast<Graphics*>(Singleton<SystemManager>::getInstance ().getSystem (SystemType::GRAPHICS_SYSTEM));
				// graphics->onResize (LOWORD (lParam), HIWORD (lParam));
				return 0;
			}

			case WM_CLOSE:
			{
				PostQuitMessage (0);
				return 0;
			}
			}

			return DefWindowProc (hwnd, msg, wparam, lparam);
		}

		/*BYTE GetWindowBitsPerPixel () const
		{

		}*/

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
			wc.lpfnWndProc = (WNDPROC)WndProc;
			wc.cbClsExtra = 0;
			wc.cbWndExtra = 0;
			wc.hInstance = this->m_hInstance;

			//TODO: load the cursor and application icon!
			wc.hIcon = LoadIcon (NULL, IDI_WINLOGO);
			wc.hCursor = LoadCursor (NULL, IDC_ARROW);

			wc.hbrBackground = NULL;
			wc.lpszMenuName = NULL;

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
