#pragma once
#ifndef _GRAPHICS_H
#define _GRAPHICS_H
#include "System.h"
#include <core\interfaces\IDrawable.h>
#include <core\systems\Window.h>

#include <vector>


#include <core\resources\MeshDataResource.h>
#include <core\managers\ResourceManager.h>

namespace fractal
{
	namespace fcore
	{
		class Graphics : public System, public IDrawable
		{
		private:
			ID3D11Device*			m_d3dDevice;
			ID3D11DeviceContext*	m_d3dImmediateContext;
			IDXGISwapChain*			m_swapChain;
			ID3D11Texture2D*		m_depthStencilBuffer;
			ID3D11RenderTargetView* m_renderTargetView;
			ID3D11DepthStencilView* m_depthStencilView;
			ID3D11DepthStencilState* g_d3dDepthStencilState = nullptr;
			ID3D11RasterizerState*  m_d3dRasterizerState;
			D3D11_VIEWPORT			m_screenViewport;
									 
			D3D_DRIVER_TYPE			m_d3dDriverType;

			UINT					m_4xMsaaQuality;
			bool					m_enable4xMsaa;

			class Window* m_window;


			// Shader resources
			enum ConstanBuffer
			{
				CB_Appliation,
				CB_Frame,
				CB_Object,
				NumConstantBuffers
			};

			ID3D11Buffer* g_d3dConstantBuffers[NumConstantBuffers];
			D3D11_VIEWPORT g_Viewport = { 0 };

			//shader data
			ID3D11VertexShader* g_d3dVertexShader = nullptr;
			ID3D11PixelShader* g_d3dPixelShader = nullptr;

			// Demo parameters
			DirectX::XMMATRIX g_WorldMatrix;
			DirectX::XMMATRIX g_ViewMatrix;
			DirectX::XMMATRIX g_ProjectionMatrix;

			bool LoadContent ()
			{
				assert (m_d3dDevice);

				// Create the constant buffers for the variables defined in the vertex shader.
				D3D11_BUFFER_DESC constantBufferDesc;
				ZeroMemory (&constantBufferDesc, sizeof (D3D11_BUFFER_DESC));

				constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
				constantBufferDesc.ByteWidth = sizeof (DirectX::XMMATRIX);
				constantBufferDesc.CPUAccessFlags = 0;
				constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;

				HRESULT hr = m_d3dDevice->CreateBuffer (&constantBufferDesc, nullptr, &g_d3dConstantBuffers[CB_Appliation]);
				if (FAILED (hr))
				{
					return false;
				}
				hr = m_d3dDevice->CreateBuffer (&constantBufferDesc, nullptr, &g_d3dConstantBuffers[CB_Frame]);
				if (FAILED (hr))
				{
					return false;
				}
				hr = m_d3dDevice->CreateBuffer (&constantBufferDesc, nullptr, &g_d3dConstantBuffers[CB_Object]);
				if (FAILED (hr))
				{
					return false;
				}

				// Setup the projection matrix.
				RECT clientRect;
				GetClientRect (m_window->GetWindowHandle(), &clientRect);

				// Compute the exact client dimensions.
				// This is required for a correct projection matrix.
				float clientWidth = static_cast<float>(clientRect.right - clientRect.left);
				float clientHeight = static_cast<float>(clientRect.bottom - clientRect.top);

				g_ProjectionMatrix = DirectX::XMMatrixPerspectiveFovLH (DirectX::XMConvertToRadians (45.0f), clientWidth / clientHeight, 0.1f, 100.0f);

				m_d3dImmediateContext->UpdateSubresource (g_d3dConstantBuffers[CB_Appliation], 0, nullptr, &g_ProjectionMatrix, 0, 0);
				
				return true;
			}

		public:
			Graphics ();
			~Graphics ();

			bool Init () override;
			void Update () override;
			void Draw () const override;
			bool Shutdown () override;

			void BeginDraw () const;
			void EndDraw () const;

			void OnResize ();

			inline ID3D11Device* GetDevice () const { return m_d3dDevice; }
			inline ID3D11DeviceContext* GetContext () const { return m_d3dImmediateContext; }

		private:

			ID3D11Buffer* mBoxVB;
			ID3D11Buffer* mBoxIB;

			ID3D11InputLayout* mInputLayout;

			DirectX::XMFLOAT4X4 mWorld;
			DirectX::XMFLOAT4X4 mView;
			DirectX::XMFLOAT4X4 mProj;

			float mTheta;
			float mPhi;
			float mRadius;
		};
	}
}
#endif // !_GRAPHICS_H