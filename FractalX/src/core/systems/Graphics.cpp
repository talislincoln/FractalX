#include <FractalPCH.h>
#include <core\systems\Graphics.h>

#include <core\systems\Window.h>

namespace fractal
{
	namespace fcore
	{
		Graphics::Graphics () :
			System (SystemType::GRAPHICS_SYSTEM),
			m_d3dDriverType (D3D_DRIVER_TYPE_HARDWARE),
			m_enable4xMsaa (false),
			m_4xMsaaQuality (0),
			m_d3dDevice (0),
			m_d3dImmediateContext (0),
			m_swapChain (0),
			m_depthStencilBuffer (0),
			m_renderTargetView (0),
			m_depthStencilView (0)
		{

		}

		Graphics::~Graphics ()
		{

		}

		bool Graphics::Init ()
		{
			UINT createDeviceFlags = 0;
		#if defined(DEBUG) || defined(_DEBUG)  
			createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
		#endif

			D3D_FEATURE_LEVEL featureLevel;
			HRESULT hr = D3D11CreateDevice (
				0,                 // default adapter
				m_d3dDriverType,
				0,                 // no software device
				createDeviceFlags,
				0, 0,              // default feature level array
				D3D11_SDK_VERSION,
				&m_d3dDevice,
				&featureLevel,
				&m_d3dImmediateContext);

			if (FAILED (hr))
			{
				MessageBox (0, L"D3D11CreateDevice Failed.", 0, 0);
				return false;
			}

			if (featureLevel != D3D_FEATURE_LEVEL_11_0)
			{
				MessageBox (0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
				return false;
			}

			// Check 4X MSAA quality support for our back buffer format.
			// All Direct3D 11 capable devices support 4X MSAA for all render 
			// target formats, so we only need to check quality support.

			HR (m_d3dDevice->CheckMultisampleQualityLevels (
				DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_4xMsaaQuality));
			assert (m_4xMsaaQuality > 0);

			// Fill out a DXGI_SWAP_CHAIN_DESC to describe our swap chain.

			DXGI_SWAP_CHAIN_DESC sd;
			sd.BufferDesc.Width = m_window->GetWindowWidth();
			sd.BufferDesc.Height = m_window->GetWindowHeight ();
			sd.BufferDesc.RefreshRate.Numerator = 60;
			sd.BufferDesc.RefreshRate.Denominator = 1;
			sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

			// Use 4X MSAA? 
			if (m_enable4xMsaa)
			{
				sd.SampleDesc.Count = 4;
				sd.SampleDesc.Quality = m_4xMsaaQuality - 1;
			}
			// No MSAA
			else
			{
				sd.SampleDesc.Count = 1;
				sd.SampleDesc.Quality = 0;
			}

			sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			sd.BufferCount = 1;
			sd.OutputWindow = m_window->GetWindowHandle();
			sd.Windowed = true;
			sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			sd.Flags = 0;

			// To correctly create the swap chain, we must use the IDXGIFactory that was
			// used to create the device.  If we tried to use a different IDXGIFactory instance
			// (by calling CreateDXGIFactory), we get an error: "IDXGIFactory::CreateSwapChain: 
			// This function is being called with a device from a different IDXGIFactory."

			IDXGIDevice* dxgiDevice = 0;
			HR (m_d3dDevice->QueryInterface (__uuidof(IDXGIDevice), (void**)&dxgiDevice));

			IDXGIAdapter* dxgiAdapter = 0;
			HR (dxgiDevice->GetParent (__uuidof(IDXGIAdapter), (void**)&dxgiAdapter));

			IDXGIFactory* dxgiFactory = 0;
			HR (dxgiAdapter->GetParent (__uuidof(IDXGIFactory), (void**)&dxgiFactory));

			HR (dxgiFactory->CreateSwapChain (m_d3dDevice, &sd, &m_swapChain));

			SafeRelease (dxgiDevice);
			SafeRelease (dxgiAdapter);
			SafeRelease (dxgiFactory);

			// The remaining steps that need to be carried out for d3d creation
			// also need to be executed every time the window is resized.  So
			// just call the OnResize method here to avoid code duplication.

			//OnResize ();

			return true;
		}

		void Graphics::Update ()
		{

		}

		bool Graphics::Shutdown ()
		{
			SafeRelease (m_renderTargetView);
			SafeRelease (m_depthStencilView);
			SafeRelease (m_swapChain);
			SafeRelease (m_depthStencilView);

			return true;
		}
	}
}