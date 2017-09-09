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
		m_windowStyle (WS_OVERLAPPEDWINDOW),
		m_pDevice(nullptr),
		m_pImmediateContext(nullptr)
	{
		g_pApp = this;
	}

	DXApp::~DXApp()
	{
		if (m_pImmediateContext)
		{
			m_pImmediateContext->ClearState();
		}

		SafeRelease(m_pRenderTargetView);
		SafeRelease(m_pSwapChain);
		SafeRelease(m_pImmediateContext);
		SafeRelease(m_pDevice);

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
		return InitWindow() && InitDirect3D();
	}

	LRESULT DXApp::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		case WM_DESTROY:
			PostQuitMessage(0); // posts a quit message that will be handle by the main loop - ends the app
			return 0;
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
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

	bool DXApp::InitDirect3D()
	{
		UINT createDeviceFlags = 0;

#ifdef _DEBUG
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D_DRIVER_TYPE driverTypes[] =
		{
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE
		};
		UINT numDriverTypes = ARRAYSIZE(driverTypes);

		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3
		};
		UINT numFeatureLevels = ARRAYSIZE(featureLevels);

		DXGI_SWAP_CHAIN_DESC swapDesc;
		ZeroMemory(&swapDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
		swapDesc.BufferCount = 1; // double buffered
		swapDesc.BufferDesc.Width = m_clientWidth;
		swapDesc.BufferDesc.Height = m_clientHeight;
		swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapDesc.OutputWindow = m_hAppWnd;
		swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapDesc.Windowed = true;
		swapDesc.SampleDesc.Count = 1;
		swapDesc.SampleDesc.Quality = 0;
		swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // alt+enter to toggle fullscreen - we need to add a handle

		HRESULT result;
		for (unsigned int i = 0; i < numDriverTypes; ++i)
		{
			result = D3D11CreateDeviceAndSwapChain(NULL, driverTypes[i], NULL, createDeviceFlags,
				featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &swapDesc, &m_pSwapChain, &m_pDevice,
				&m_featureLevel, &m_pImmediateContext);

			if (SUCCEEDED(result))
			{
				m_driverType = driverTypes[i];
				break;
			}
		}

		if (FAILED(result))
		{
			OutputDebugString(L"FAILED TO CREATE DEVICE AND SWAPCHAIN");
			return false;
		}

		// CREATE RENDER TARGET VIEW
		ID3D11Texture2D*	m_pBackBufferTexture = nullptr;
		m_pSwapChain->GetBuffer(NULL, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&m_pBackBufferTexture));
		m_pDevice->CreateRenderTargetView(m_pBackBufferTexture, nullptr, &m_pRenderTargetView);

		SafeRelease(m_pBackBufferTexture);

		// BIND RENDER TARGET VIEW
		m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, nullptr);

		// VIEWPORT CREATION
		m_viewPort.Width = static_cast<float>(m_clientWidth);
		m_viewPort.Height = static_cast<float>(m_clientHeight);
		m_viewPort.TopLeftX = 0;
		m_viewPort.TopLeftY = 0;
		m_viewPort.MinDepth = 0.0f;
		m_viewPort.MaxDepth = 1.0f;

		// BIND VIEWPORT
		m_pImmediateContext->RSSetViewports(1, &m_viewPort);

		return true;
	}

}