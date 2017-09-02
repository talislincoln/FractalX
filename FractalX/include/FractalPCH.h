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


// =========== TEMPLATE METAPROGRAMMING

namespace fractal 
{
#pragma region Print Function
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


namespace fractal {
	
}

#endif // !_FRACTALPCH_H