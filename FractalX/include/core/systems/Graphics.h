#pragma once
#ifndef _GRAPHICS_H
#define _GRAPHICS_H
#include "System.h"
#include <core\interfaces\IDrawable.h>

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

			void OnResize ();

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
