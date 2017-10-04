#include <FractalPCH.h>
#include <core\resources\MeshDataResource.h>
#include <core\managers\SystemManager.h>
#include <core\systems\Graphics.h>

namespace fractal
{
	namespace fcore
	{
		MeshDataResource::MeshDataResource (const FString& path) :
			Resource(path, ResourceType::RESOURCE_MESH)
		{
			// TODO: Load mesh from file
		}

		MeshDataResource::MeshDataResource (const FString& name, std::vector<VertexPosColor> vertices, std::vector<WORD> indices) :
			Resource (name, ResourceType::RESOURCE_MESH),
			m_d3dVertexBuffer (nullptr),
			m_d3dIndexBuffer (nullptr)
		{
			ID3D11Device* device = dynamic_cast<fractal::fcore::Graphics*>(fractal::fcore::SystemManager::Instance ()->GetSystem (fractal::SystemType::GRAPHICS_SYSTEM))->GetDevice();
			assert (device);

			// Create and initialize the vertex buffer memory with 0.
			D3D11_BUFFER_DESC vertexBufferDesc;
			ZeroMemory (&vertexBufferDesc, sizeof (D3D11_BUFFER_DESC));

			vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertexBufferDesc.ByteWidth = sizeof (VertexPosColor) * vertices.size();
			//vertexBufferDesc.ByteWidth = sizeof (a.pos.front ()) * a.pos.size () + sizeof (a.cols.front ()) * a.cols.size ();
			vertexBufferDesc.CPUAccessFlags = 0;
			vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

			D3D11_SUBRESOURCE_DATA resourceData;
			ZeroMemory (&resourceData, sizeof (D3D11_SUBRESOURCE_DATA));

			resourceData.pSysMem = vertices.data();
			
			HRESULT hr = device->CreateBuffer (&vertexBufferDesc, &resourceData, &m_d3dVertexBuffer);
			assert (!FAILED (hr));
			/*if (FAILED (hr))
			{
				return false;
			}*/

			// Create and initialize the index buffer.
			D3D11_BUFFER_DESC indexBufferDesc;
			ZeroMemory (&indexBufferDesc, sizeof (D3D11_BUFFER_DESC));

			indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			indexBufferDesc.ByteWidth = sizeof (indices.front ()) * indices.size ();
			indexBufferDesc.CPUAccessFlags = 0;
			indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			resourceData.pSysMem = indices.data ();

			hr = device->CreateBuffer (&indexBufferDesc, &resourceData, &m_d3dIndexBuffer);
			assert (!FAILED (hr));
			/*if (FAILED (hr))
			{
				return false;
			}*/
		}

		bool MeshDataResource::Init ()
		{
			return Resource::Init () && m_d3dIndexBuffer && m_d3dVertexBuffer;
		}

		bool MeshDataResource::Shutdown ()
		{
			SafeRelease (m_d3dIndexBuffer);
			SafeRelease (m_d3dVertexBuffer);
			return Resource::Shutdown ();
		}

		ID3D11Buffer* MeshDataResource::GetVertexBuffer () const
		{
			return m_d3dVertexBuffer;
		}

		ID3D11Buffer* MeshDataResource::GetIndexBuffer () const
		{
			return m_d3dIndexBuffer;
		}
	}
}