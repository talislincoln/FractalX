#include <FractalPCH.h>

#include <core\resources\MaterialResource.h>

#include <core\resources\ShaderResource.h>

namespace fractal
{
	namespace fcore
	{
		MaterialResource::MaterialResource (const FString& name, ShaderResource * shaderResource, float r, float g, float b, float a) :
			Resource (name, ResourceType::RESOURCE_MATERIAL),
			m_shaderResource(shaderResource),
			m_diffuseColor(DirectX::XMFLOAT4(r, g, b, a))
		{

		}
		MaterialResource::~MaterialResource ()
		{
			// empty
		}

		void MaterialResource::Use ()
		{
			m_shaderResource->UseShader ();
		}
	}
}