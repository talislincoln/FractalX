#ifndef _FRACTALPCH_H
#define _FRACTALPCH_H

// System includes
#include <Windows.h>

// DirectX includes
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXColors.h>

// STL includes

// Safely release a COM object.
template<typename T>
inline void SafeRelease(T& ptr)
{
	if (ptr != NULL)
	{
		ptr->Release();
		ptr = NULL;
	}
}

//C runtime Header Files
#ifndef _TCHAR_INC_
#include <tchar.h>
#endif // !_TCHAR_INC_

#ifndef _STRING_
#include <string>
#endif // !_STRING_

#ifdef UNICODE
#define FString wstring
#define TOSTRING(value) std::to_wstring(value)
#else
#define FString string
#define TOSTRING(value) std::to_string(value)
#endif // UNICODE

#ifndef _IOSTREAM_
#include <iostream>
#endif // !_IOSTREAM_

#ifdef UNICODE
#define FCout wcout
#define FCin wcin
#else
#define FCout cout
#define FCin  cin
#endif // UNICODE

// Link library dependencies
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "winmm.lib")

#endif // !_FRACTALPCH_H