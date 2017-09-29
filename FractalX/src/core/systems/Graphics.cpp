#include <FractalPCH.h>
#include <core\systems\Graphics.h>
#include <core\systems\Window.h>
#include <core\managers\SystemManager.h>

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
			m_depthStencilView (0),
			m_d3dRasterizerState (nullptr)
		{
			ZeroMemory (&m_screenViewport, sizeof (D3D11_VIEWPORT));
		}

		Graphics::~Graphics ()
		{

		}

		bool Graphics::Init ()
		{
			// Get the reference for the main window
			m_window = static_cast<Window*>(SystemManager::Instance ()->GetSystem (SystemType::WINDOW_SYSTEM));

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
			ZeroMemory (&sd, sizeof (DXGI_SWAP_CHAIN_DESC));

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
			if (!LoadContent ())
				return false;
			OnResize ();

			return true;
		}

		void Graphics::Update ()
		{
			using namespace DirectX;

			DirectX::XMVECTOR eyePosition = XMVectorSet (0, 0, -10, 1);
			DirectX::XMVECTOR focusPoint = XMVectorSet (0, 0, 0, 1);
			DirectX::XMVECTOR upDirection = XMVectorSet (0, 1, 0, 0);
			g_ViewMatrix = XMMatrixLookAtLH (eyePosition, focusPoint, upDirection);
			m_d3dImmediateContext->UpdateSubresource (g_d3dConstantBuffers[CB_Frame], 0, nullptr, &g_ViewMatrix, 0, 0);


			static float angle = 0.0f;
			angle += 90.0f * 0.001f;
			XMVECTOR rotationAxis = XMVectorSet (0, 1, 1, 0);

			g_WorldMatrix = XMMatrixRotationAxis (rotationAxis, XMConvertToRadians (0));
			m_d3dImmediateContext->UpdateSubresource (g_d3dConstantBuffers[CB_Object], 0, nullptr, &g_WorldMatrix, 0, 0);

			m_d3dImmediateContext->ClearRenderTargetView (m_renderTargetView, reinterpret_cast<const float*>(&fractal::Colours::Blue));
			m_d3dImmediateContext->ClearDepthStencilView (m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		}

		void Graphics::Draw () const
		{
			assert (m_d3dImmediateContext);
			assert (m_swapChain);

			
			const UINT vertexStride = sizeof (VertexPosColor);
			const UINT offset = 0;

			m_d3dImmediateContext->IASetVertexBuffers (0, 1, &g_d3dVertexBuffer, &vertexStride, &offset);
			m_d3dImmediateContext->IASetInputLayout (g_d3dInputLayout);
			m_d3dImmediateContext->IASetIndexBuffer (g_d3dIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
			m_d3dImmediateContext->IASetPrimitiveTopology (D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			m_d3dImmediateContext->VSSetShader (g_d3dVertexShader, nullptr, 0);
			m_d3dImmediateContext->VSSetConstantBuffers (0, 3, g_d3dConstantBuffers);

			m_d3dImmediateContext->RSSetState (m_d3dRasterizerState);
			m_d3dImmediateContext->RSSetViewports (1, &m_screenViewport);

			m_d3dImmediateContext->PSSetShader (g_d3dPixelShader, nullptr, 0);

			m_d3dImmediateContext->OMSetRenderTargets (1, &m_renderTargetView, m_depthStencilView);
			m_d3dImmediateContext->OMSetDepthStencilState (g_d3dDepthStencilState, 1);
			
			m_d3dImmediateContext->DrawIndexed (_countof (g_Indicies), 0, 0);

			HR (m_swapChain->Present (0, 0));
		}

		bool Graphics::Shutdown ()
		{
			SafeRelease (m_renderTargetView);
			SafeRelease (m_depthStencilView);
			SafeRelease (m_swapChain);
			SafeRelease (m_depthStencilView);

			// Restore all default settings.
			if (m_d3dImmediateContext)
				m_d3dImmediateContext->ClearState ();

			SafeRelease (m_d3dImmediateContext);
			SafeRelease (m_d3dDevice);

			return true;
		}

		void Graphics::OnResize ()
		{
			if (m_window)
			{
				assert (m_d3dImmediateContext);
				assert (m_d3dDevice);
				assert (m_swapChain);

				// Release the old views, as they hold references to the buffers we
				// will be destroying.  Also release the old depth/stencil buffer.

				SafeRelease (m_renderTargetView);
				SafeRelease (m_depthStencilView);
				SafeRelease (m_depthStencilBuffer);

				// Resize the swap chain and recreate the render target view.
				Window *window = static_cast<Window*>(SystemManager::Instance ()->GetSystem (SystemType::WINDOW_SYSTEM));

				HR (m_swapChain->ResizeBuffers (1, window->GetWindowWidth (), window->GetWindowHeight (), DXGI_FORMAT_R8G8B8A8_UNORM, 0));
				ID3D11Texture2D* backBuffer;
				HR (m_swapChain->GetBuffer (0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
				HR (m_d3dDevice->CreateRenderTargetView (backBuffer, 0, &m_renderTargetView));
				SafeRelease (backBuffer);

				// Create the depth/stencil buffer and view
				D3D11_TEXTURE2D_DESC depthStencilDesc;

				depthStencilDesc.Width = window->GetWindowWidth ();
				depthStencilDesc.Height = window->GetWindowHeight ();
				depthStencilDesc.MipLevels = 1;
				depthStencilDesc.ArraySize = 1;
				depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

				// Use 4X MSAA? --must match swap chain MSAA values.
				if (m_enable4xMsaa)
				{
					depthStencilDesc.SampleDesc.Count = 4;
					depthStencilDesc.SampleDesc.Quality = m_4xMsaaQuality - 1;
				}
				// No MSAA
				else
				{
					depthStencilDesc.SampleDesc.Count = 1;
					depthStencilDesc.SampleDesc.Quality = 0;
				}

				depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
				depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
				depthStencilDesc.CPUAccessFlags = 0;
				depthStencilDesc.MiscFlags = 0;

				HR (m_d3dDevice->CreateTexture2D (&depthStencilDesc, 0, &m_depthStencilBuffer));
				HR (m_d3dDevice->CreateDepthStencilView (m_depthStencilBuffer, 0, &m_depthStencilView));

				D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
				ZeroMemory (&depthStencilStateDesc, sizeof (D3D11_DEPTH_STENCIL_DESC));

				depthStencilStateDesc.DepthEnable = TRUE;
				depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
				depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;
				depthStencilStateDesc.StencilEnable = FALSE;
				m_d3dDevice->CreateDepthStencilState (&depthStencilStateDesc, &g_d3dDepthStencilState);

				// Bind the render target view and depth/stencil view to the pipeline.
				m_d3dImmediateContext->OMSetRenderTargets (1, &m_renderTargetView, m_depthStencilView);

				// Set the viewport transform.
				m_screenViewport.TopLeftX = 0;
				m_screenViewport.TopLeftY = 0;
				m_screenViewport.Width = static_cast<float>(window->GetWindowWidth ());
				m_screenViewport.Height = static_cast<float>(window->GetWindowHeight ());
				m_screenViewport.MinDepth = 0.0f;
				m_screenViewport.MaxDepth = 1.0f;

				// Setup rasterizer state.
				D3D11_RASTERIZER_DESC rasterizerDesc;
				ZeroMemory (&rasterizerDesc, sizeof (D3D11_RASTERIZER_DESC));

				rasterizerDesc.AntialiasedLineEnable = FALSE;
				rasterizerDesc.CullMode = D3D11_CULL_BACK;
				rasterizerDesc.DepthBias = 0;
				rasterizerDesc.DepthBiasClamp = 0.0f;
				rasterizerDesc.DepthClipEnable = TRUE;
				rasterizerDesc.FillMode = D3D11_FILL_SOLID;
				rasterizerDesc.FrontCounterClockwise = FALSE;
				rasterizerDesc.MultisampleEnable = FALSE;
				rasterizerDesc.ScissorEnable = FALSE;
				rasterizerDesc.SlopeScaledDepthBias = 0.0f;

				// Create the rasterizer state object.
				HRESULT hr = m_d3dDevice->CreateRasterizerState (&rasterizerDesc, &m_d3dRasterizerState);
				if (FAILED (hr))
				{
					//return -1;
				}

				m_d3dImmediateContext->RSSetViewports (1, &m_screenViewport);
			}
			
		}
	}
}