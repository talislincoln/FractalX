#include <FractalPCH.h>
#include <core\resources\MeshDataResource.h>

namespace fractal
{
	namespace fcore
	{
		MeshDataResource::MeshDataResource (const FString& path) :
			Resource(path, ResourceType::RESOURCE_MESH)
		{
			// TODO: Load mesh from file
		}

		MeshDataResource::MeshDataResource (const FString& name, VertexPosColor* vertices, int* indices) :
			Resource(name, ResourceType::RESOURCE_MESH)
		{

		}
	}
}