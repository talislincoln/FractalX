#pragma once
#ifndef _DEFINES_H
#define _DEFINES_H

namespace fractal
{
	namespace Colors
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

#define VECTOR_UP		DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
#define VECTOR_FORWARD	DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f)
#define TO_RADIANS		0.0174532925f

	enum class SystemType : unsigned __int8
	{
		WINDOW_SYSTEM,
		//INPUT_SYSTEM,
		GRAPHICS_SYSTEM,
		TIMER_SYSTEM,
		LOGIC_SYSTEM,
		//PHYSICS_SYSTEM,
		//HUD_SYSTEM,
		TOTAL_SYSTEMS
	};

	enum class ResourceType : unsigned __int8
	{
		RESOURCE_MESH,
		RESOURCE_IMAGE,
		RESOURCE_SHADER
	};

	inline void FDebug ()
	{
		std::cout << std::endl;
	}

	template <typename Tfirst, typename... Trest>
	void FDebug (Tfirst first, Trest... rest)
	{
		std::cout << first << ' ';
		FDebug (rest...);
	}
}

#endif // !_DEFINES_H
