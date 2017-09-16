#pragma once
#ifndef _WINDOW_H
#define _WINDOW_H

#include "System.h"

namespace fractal
{
	namespace fcore
	{

		class Window : public System
		{
		private:
			HDC             m_hDeviceContext;
			HWND            m_hWindow;
			HINSTANCE       m_hInstance;
							
			RECT			m_windowRect;
							
			DWORD			m_extendedStyle;
			DWORD			m_style;
							
			unsigned int	m_pixelFormat;
							
			FString			m_windowClassname;
			FString			m_windowTitle;
							
			unsigned int 	m_windowWidth;
			unsigned int	m_windowHeight;

			HINSTANCE		m_hAppInst;
			HWND			m_hMainWnd;
			bool			m_appPaused;
			bool			m_minimized;
			bool			m_maximized;
			bool			m_resizing;

			FString			m_windowCaption;

		public:
			Window ();
			~Window ();

			bool Init () override;
			void Update () override;
			bool Shutdown () override;

			void OnResize ();

			static LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);

			LRESULT HandleEvent (HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

			//BYTE GetWindowBitsPerPixel () const;

			const FString GetWindowTitle () const;
			const FString GetWindowClassName () const;

			const POINT GetWindowPosition () const;

			HDC GetWindowDeviceContext () const;
			HWND GetWindowHandle () const;
			HINSTANCE GetWindowInstance () const;

			unsigned int GetWindowWidth () const;
			unsigned int GetWindowHeight () const;

			float AspectRation () const;

		private:
			const POINT GetCenterPosition () const;

			bool SetupWindow ();
			bool ErrorHandling ();

			bool SpawnWindow (); ///< CreateWindow was already taken by the windows API
			WNDCLASS CreateWindowClass () const;
			RECT CreateWindowRect () const;
			bool DestroyWindow ();
			PIXELFORMATDESCRIPTOR CreateWindowPixelFormatDescription () const;
		};
	}
}

#endif // !_WINDOW_H
