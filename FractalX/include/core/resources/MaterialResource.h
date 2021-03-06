#pragma once
#ifndef _MATERIALRESOURCE_H
#define _MATERIALRESOURCE_H
#include <core\resources\Resource.h>

namespace fractal
{
	namespace fcore
	{
		class MaterialResource : Resource
		{
			DirectX::XMFLOAT4 m_diffuseColor;
			//DirectX::XMFLOAT4 m_ambientColor;
			//DirectX::XMFLOAT4 m_specularColor;
			//DirectX::XMFLOAT4 m_emissiveColor;

			float m_metallic;
			float m_smoothness;

			class ShaderResource* m_shaderResource;

		public:
			MaterialResource (const FString& name, class ShaderResource* shaderResource, float r, float g, float b, float a);
			~MaterialResource ();

			void Use ();
		};
	}
}

#endif // !_MATERIALRESOURCE_H
