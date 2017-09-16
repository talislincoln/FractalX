#pragma once
#ifndef _GRAPHICS_H
#define _GRAPHICS_H
#include "System.h"

namespace fractal
{
	namespace fcore
	{
		class Graphics : public System
		{
		private:
			ID3D11Device*			m_d3dDevice;
			ID3D11DeviceContext*	m_d3dImmediateContext;
			IDXGISwapChain*			m_swapChain;
			ID3D11Texture2D*		m_depthStencilBuffer;
			ID3D11RenderTargetView* m_renderTargetView;
			ID3D11DepthStencilView* m_depthStencilView;
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
			bool Shutdown () override;

			void OnResize ();

		private:
		};
	}
}
#endif // !_GRAPHICS_H
