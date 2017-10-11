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

namespace fractal
{
	const int INITIALIZATION_FAILED = 0x0001;
	const int SHUTDOWN_FAILED = 0x0010;

	namespace Colours
	{
		XMGLOBALCONST DirectX::XMVECTORF32 White = { 1.0f, 1.0f, 1.0f, 1.0f };
		XMGLOBALCONST DirectX::XMVECTORF32 Black = { 0.0f, 0.0f, 0.0f, 1.0f };
		XMGLOBALCONST DirectX::XMVECTORF32 Red = { 1.0f, 0.0f, 0.0f, 1.0f };
		XMGLOBALCONST DirectX::XMVECTORF32 Green = { 0.0f, 1.0f, 0.0f, 1.0f };
		XMGLOBALCONST DirectX::XMVECTORF32 Blue = { 0.0f, 0.0f, 1.0f, 1.0f };
		XMGLOBALCONST DirectX::XMVECTORF32 Yellow = { 1.0f, 1.0f, 0.0f, 1.0f };
		XMGLOBALCONST DirectX::XMVECTORF32 Cyan = { 0.0f, 1.0f, 1.0f, 1.0f };
		XMGLOBALCONST DirectX::XMVECTORF32 Magenta = { 1.0f, 0.0f, 1.0f, 1.0f };
					  
		XMGLOBALCONST DirectX::XMVECTORF32 Silver = { 0.75f, 0.75f, 0.75f, 1.0f };
		XMGLOBALCONST DirectX::XMVECTORF32 LightSteelBlue = { 0.69f, 0.77f, 0.87f, 1.0f };
	}

	struct VertexPosColor
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT3 Color;

		VertexPosColor (DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 color) : Position (position), Color (color) {}
	};

	enum class SystemType : unsigned __int8
	{
		WINDOW_SYSTEM,
		//INPUT_SYSTEM,
		GRAPHICS_SYSTEM,
		TIMER_SYSTEM,
		LOGIC_SYSTEM,
		//PHYSICS_SYSTEM,
		//HUD_SYSTEM,
		//RESOURCE_SYSTEM,
		TOTAL_SYSTEMS
	};

	enum class ResourceType : unsigned __int8
	{
		RESOURCE_MESH,
		RESOURCE_TEXTURE,
		RESOURCE_SHADER
	};

	/*FString GetNameFromPath (const FString& path)
	{
		int start_index = path.find_last_of ('\\');
		if (start_index == FString::npos)
			start_index = path.find_last_of ('/');
		++start_index;
		int end_index = path.find_last_of ('.');

		return path.substr (start_index, end_index - start_index);
	}
	
	std::wstring widen (const std::string& str)
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


// =========== TEMPLATE METAPROGRAMMING
namespace fractal 
{
#pragma region FDebug
	inline void FDebug()
	{
		std::cout << std::endl;
	}

	template <typename Tfirst, typename... Trest>
	void FDebug(Tfirst first, Trest... rest)
	{
		std::cout << first << ' ';
		FDebug(rest...);
	}
	// ^ example: fractal::FDebug("testing", 2.0);
#pragma endregion

#pragma region Math Functions
	namespace fmath {
		// incomplete declaration of the tuple template
		template <typename... Ttypes> class FTuple;

		// Empty tuple - base case for the typle
		template <> class FTuple<> { /* empty */ };

		// incomplete declaration
		template <size_t idx, typename Ttuple> struct FTupleElement;

		// access the 0th (first) element - base case for the tuple element
		template <typename Tvalue, typename... Trest>
		struct FTupleElement<0, FTuple<Tvalue, Trest...>>
		{
			using type_t = Tvalue;
			using TupleType_t = FTuple<Tvalue, Trest...>;
		};

		// recursive tuple element definition
		template<size_t idx, typename Tvalue, typename... Trest>
		struct FTupleElement<idx, FTuple<Tvalue, Trest...>> : public FTupleElement<idx - 1, FTuple<Trest...>> { /* empty */};

		// recursive tuple class definition
		template <typename Tvalue, typename... Trest>
		class FTuple<Tvalue, Trest...> : public FTuple<Trest...>
		{
		public:
			Tvalue value;
		};

		// Get values in a given tuple
		template <size_t idx, typename... Ttypes>
		typename FTupleElement<idx, FTuple<Ttypes...>>::type_t& // return type
		get(FTuple<Ttypes...>& tuple)
		{
			using TupleType_t = typename FTupleElement<idx, FTuple<Ttypes...>>::TupleType_t;
			return static_cast<TupleType_t&>(tuple).value;
		}

		//// Function to fill a tuple with values
		// done case 
		template <size_t idx, typename Ttuple>
		void fillTuple(Ttuple& toFill) {};

		// Function to fill a tuple with values recursive definition
		template <size_t idx, typename Ttuple, typename Tfirst, typename... Trest>
		void fillTuple(Ttuple& toFill, const Tfirst& first, Trest... rest)
		{
			get<idx>(toFill) = first;
			fillTuple<idx + 1>(toFill, rest...);
		}

		template <typename... Ttypes>
		FTuple<Ttypes...> makeTuple(Ttypes... args)
		{
			FTuple<Ttypes...> result;
			fillTuple<0>(result, args...);
			return result;
		}

		// example of maketupe
		/*void TestMakeTuple()
		{
		using namespace fractal;
		using namespace fmath;

		auto t2 = makeTuple(12, 'h', "talis");

		assert(get<0>(t2) == 12);
		assert(get<1>(t2) == 'h');
		assert(get<2>(t2) == "talis");
		}*/

		// usage of the tuple
		/*void TestTupleMetaprogramming()
		{
		using namespace fractal;
		using namespace fmath;

		FTuple<int, char, std::string> tuple;
		get<0>(tuple) = 10;
		get<1>(tuple) = 'b';
		get<2>(tuple) = std::string("Talis");

		auto v1 = get<0>(tuple);
		auto v2 = get<1>(tuple);
		auto v3 = get<2>(tuple);

		assert(v1 == 10);
		assert(v2 == 'b');
		assert(v3 == "Talis");
		}*/
	}
#pragma endregion
}
#endif // !_FRACTALPCH_H