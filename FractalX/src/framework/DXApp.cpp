#include <FractalPCH.h>
#include "framework/DXApp.h"

namespace fractal
{
#define APP_CLASS_NAME L"DXAPPWNDCLASS"

	DXApp* g_pApp = nullptr;

	LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (g_pApp)
			return g_pApp->MsgProc(hWnd, msg, wParam, lParam);
		else
			return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	DXApp::DXApp(HINSTANCE hInstance) :
		m_hAppWnd (NULL),
		m_hAppInstance (hInstance),
		m_clientWidth (800),
		m_clientHeight (600),
		m_AppTitle (L"Fractal Game Engine"),
		m_windowStyle (WS_OVERLAPPEDWINDOW)
	{
		g_pApp = this;
	}

	DXApp::~DXApp()
	{
		// empty
	}

	int DXApp::Run()
	{
		MSG msg = { 0 };

		while (WM_QUIT != msg.message)
		{
			if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				Update(0.0f);
				Render(0.0f);
			}
		}

		return 0;
	}

	bool DXApp::Init()
	{
		return InitWindow();
	}

	LRESULT DXApp::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		case WM_DESTROY:
			PostQuitMessage(0); // posts a quit message that will be handle by the main loop - ends the app
			return 0;
		default:
			DefWindowProc(hWnd, msg, wParam, lParam);
		}
	}

	bool DXApp::InitWindow()
	{
		WNDCLASSEX wcex;
		ZeroMemory(&wcex, sizeof(WNDCLASSEX));
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;

		wcex.hInstance = m_hAppInstance;
		wcex.lpfnWndProc = fractal::MsgProc;
		wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION); // window icon
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW); // mouse cursor
		wcex.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
		wcex.lpszMenuName = NULL; // in case the app will have the menu bar at the top
		wcex.lpszClassName = APP_CLASS_NAME;
		wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

		if (!RegisterClassEx(&wcex))
		{
			OutputDebugString(L"\nFAILED TO CREATE WINDOW CLASS\n");
			return false;
		}

		RECT r = { 0, 0, m_clientWidth, m_clientHeight };
		AdjustWindowRect(&r, m_windowStyle, FALSE);
		UINT width = r.right - r.left;
		UINT height = r.bottom - r.top;

		UINT x = GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2;
		UINT y = GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2;

		m_hAppWnd = CreateWindow(APP_CLASS_NAME, m_AppTitle, m_windowStyle,
			x, y, width, height, NULL, NULL, m_hAppInstance, NULL);

		if (!m_hAppWnd)
		{
			OutputDebugString(L"\nFAILED TO CREATE WINDOW\n");
			return false;
		}

		ShowWindow(m_hAppWnd, SW_SHOW);

		return TRUE;
	}

}