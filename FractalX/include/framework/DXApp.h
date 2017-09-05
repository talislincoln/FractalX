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

	protected:

		bool InitWindow();
	};
}
#endif // 