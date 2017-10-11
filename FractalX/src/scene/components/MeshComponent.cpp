#include <FractalPCH.h>
#include "scene\components\MeshComponent.h"

#include <core\EngineCore.h>

namespace fractal
{
	namespace fscene
	{
		MeshComponent::MeshComponent (fcore::MeshDataResource* meshData, fcore::ShaderResource* shaderData) :
			m_meshData(meshData),
			m_shaderData(shaderData)
		{

		}

		MeshComponent::~MeshComponent ()
		{

		}

		bool MeshComponent::Init ()
		{
			SetActive (true);
			SetCanDraw (true);

			return true;
		}

		void MeshComponent::Update ()
		{

		}

		void MeshComponent::Draw () const
		{
			ID3D11DeviceContext* context = dynamic_cast<fractal::fcore::Graphics*>(fractal::fcore::SystemManager::Instance ()->GetSystem (fractal::SystemType::GRAPHICS_SYSTEM))->GetContext ();

			const UINT vertexStride = sizeof (VertexPosColor);
			const UINT offset = 0;

			ID3D11Buffer* vertexBuffer = m_meshData->GetVertexBuffer ();
			context->IASetVertexBuffers (0, 1, &vertexBuffer, &vertexStride, &offset);
			context->IASetInputLayout (m_shaderData->GetInputLayout());

			ID3D11Buffer* indexBuffer = m_meshData->GetIndexBuffer ();
			context->IASetIndexBuffer (indexBuffer, DXGI_FORMAT_R16_UINT, 0);

			context->VSSetShader (m_shaderData->GetVertexShader(), nullptr, 0);
			context->PSSetShader (m_shaderData->GetPixelShader(), nullptr, 0);

			context->DrawIndexed (m_meshData->GetIndicesCount (), 0, 0);
		}

		bool MeshComponent::Shutdown ()
		{
			return true;
		}
	}
}