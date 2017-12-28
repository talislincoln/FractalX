#pragma once
#ifndef _FRACTALPCH_H
#define _FRACTALPCH_H

// System includes
#ifndef _WINDOWS_
#include <Windows.h>
#endif

#define WIN32_LEAN_AND_MEAN

// DirectX includes
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXColors.h>

#include "utils\DDSTextureLoader.h"

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

#define SafeDelete(x) { if(x) {delete x; x = nullptr;}}

//C runtime Header Files
#ifndef _TCHAR_INC_
#include <tchar.h>
#endif // !_TCHAR_INC_

#ifndef _STRING_
#include <string>
#endif // !_STRING_

#include <sstream>

#ifdef UNICODE
#define FString std::wstring
#define TOSTRING(value) std::to_wstring(value)
#else
#define FString std::string
#define TOSTRING(value) std::to_string(value)
#endif // UNICODE

#ifndef _IOSTREAM_
#include <iostream>
#endif // !_IOSTREAM_

#ifdef UNICODE
#define fcout std::wcout
#define fcin std::wcin
#else
#define FCout std::cout
#define FCin  std::cin
#endif // UNICODE

// Link library dependencies
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "winmm.lib")

#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)                                              \
	{                                                          \
		HRESULT hr = (x);                                      \
		if(FAILED(hr))                                         \
		{                                                      \
			/*DXTrace(__FILE__, (DWORD)__LINE__, hr, L#x, true); */\
		}                                                      \
	}
#endif
#else
#ifndef HR
#define HR(x) (x)
#endif
#endif 

#include "utils\defines.h"
#include "utils\MathHelper.h"

namespace fractal
{

	struct VertexPosColorTexture
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT3 Color;
		DirectX::XMFLOAT2 UV;

		VertexPosColorTexture ()
		{
			// empty
		};

		VertexPosColorTexture (float x, float y, float z, float r, float g, float b, float u, float v) :
			Position(DirectX::XMFLOAT3(x, y, z)), Color (DirectX::XMFLOAT3 (r, g, b)), UV (DirectX::XMFLOAT2 (u, v))
		{
			// empty
		}

		VertexPosColorTexture (DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 color, DirectX::XMFLOAT2 uvs) :
			Position (position), Color (color), UV(uvs) {}
	};	
	
	/*std::wstring widen (const std::string& str)
	{
		std::wostringstream wstm;
		const std::ctype<wchar_t>& ctfacet =
			std::use_facet< std::ctype<wchar_t> > (wstm.getloc ());
		for (size_t i = 0; i<str.size (); ++i)
			wstm << ctfacet.widen (str[i]);
		return wstm.str ();
	}
	std::string narrow (const std::wstring& str)
	{
		std::ostringstream stm;
		const std::ctype<char>& ctfacet =
			std::use_facet< std::ctype<char> > (stm.getloc ());
		for (size_t i = 0; i<str.size (); ++i)
			stm << ctfacet.narrow (str[i], 0);
		return stm.str ();
	}*/
}

#endif // !_FRACTALPCH_H