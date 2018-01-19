#pragma once
#ifndef _DEFINES_H
#define _DEFINES_H

namespace fractal
{
	const std::string WINDOW_SECTION_NAME = "window";
	const std::string WINDOW_WIDTH_NAME = "width";
	const std::string WINDOW_HEIGHT_NAME = "height";

	const std::string CAMERA_SECTION_NAME = "camera";
	const std::string CAMERA_NEARPLANE_NAME = "near";
	const std::string CMAERA_FARPLANE_NAME = "far";

	const int INITIALIZATION_FAILED = 0x0001;
	const int SHUTDOWN_FAILED = 0x0010;

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
#define MATH_PI			3.1415926535f

	// Shader resources
	enum class ConstanBuffer : unsigned __int8
	{
		CB_Appliation,
		CB_Frame,
		CB_Object,
		CB_LightBuffer,
		NumConstantBuffers
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
		TOTAL_SYSTEMS
	};

	enum class ResourceType : unsigned __int8
	{
		RESOURCE_MESH,
		RESOURCE_IMAGE,
		RESOURCE_SHADER,
		RESOURCE_MATERIAL
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
