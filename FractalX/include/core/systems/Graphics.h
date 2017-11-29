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
			ID3D11DepthStencilState* m_d3dDepthStencilState;
			ID3D11RasterizerState*  m_d3dRasterizerState;

			D3D11_VIEWPORT			m_screenViewport;
									 
			D3D_DRIVER_TYPE			m_d3dDriverType;

			UINT					m_4xMsaaQuality;
			bool					m_enable4xMsaa;

			class Window* m_window;

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

			DirectX::XMMATRIX GetProjectionMatrix () const;
		};
	}
}
#endif // !_GRAPHICS_H