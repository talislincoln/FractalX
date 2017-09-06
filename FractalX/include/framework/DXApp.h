#ifndef _DXAPP_H
#define _DXAPP_H

#include <string>

namespace fractal
{
	class DXApp
	{
	public:
		DXApp(HINSTANCE hInstance);
		~DXApp(void);

		// MAIN APPLICATION LOOP
		int Run();

		// FRAMEWORK METHODS
		virtual bool Init();
		virtual void Update(float dt) = 0;
		virtual void Render(float dt) = 0;

		virtual LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	protected:
		// WIN32 ATTRIBUTES
		HWND		m_hAppWnd;		///< handle for the application window
		HINSTANCE	m_hAppInstance;
		UINT		m_clientWidth;
		UINT		m_clientHeight;

		LPCWSTR		m_AppTitle;

		DWORD		m_windowStyle;

		// DIRECTX ATTRIBUTES
		ID3D11Device*			m_pDevice;
		ID3D11DeviceContext*	m_pImmediateContext;
		IDXGISwapChain*			m_pSwapChain;
		ID3D11RenderTargetView* m_pRenderTargetView;
		D3D_DRIVER_TYPE			m_driverType;
		D3D_FEATURE_LEVEL		m_featureLevel;
		D3D11_VIEWPORT			m_viewPort;

	protected:

		bool InitWindow();

		bool InitDirect3D();
	};
}
#endif // 