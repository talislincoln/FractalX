#include <FractalPCH.h>
#include "scene\components\MeshComponent.h"

#include <core\EngineCore.h>

namespace fractal
{
	namespace fscene
	{
		MeshComponent::MeshComponent (fcore::MeshDataResource* meshData, fcore::ShaderResource* shaderData, fcore::ImageResource* imageData) :
			m_meshData(meshData),
			m_shaderData(shaderData),
			m_imageData(imageData)
		{
			// empty
		}

		MeshComponent::MeshComponent (const FString& meshResourceName, const FString& shaderResourceName, const FString& imageResourceName) :
			m_meshData(nullptr),
			m_shaderData(nullptr),
			m_imageData(nullptr)
		{
			fcore::ResourceManager* resourceManager = fcore::ResourceManager::Instance ();

			m_meshData = resourceManager->GetResource<fcore::MeshDataResource> (meshResourceName);
			m_shaderData = resourceManager->GetResource<fcore::ShaderResource> (shaderResourceName);
			m_imageData = resourceManager->GetResource<fcore::ImageResource> (imageResourceName);
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

			const UINT vertexStride = sizeof (VertexPosColorTexture);
			const UINT offset = 0;

			ID3D11Buffer* vertexBuffer = m_meshData->GetVertexBuffer ();
			context->IASetVertexBuffers (0, 1, &vertexBuffer, &vertexStride, &offset);
			context->IASetInputLayout (m_shaderData->GetInputLayout());

			ID3D11Buffer* indexBuffer = m_meshData->GetIndexBuffer ();
			context->IASetIndexBuffer (indexBuffer, DXGI_FORMAT_R16_UINT, 0);

			context->VSSetShader (m_shaderData->GetVertexShader(), nullptr, 0);
			context->PSSetShader (m_shaderData->GetPixelShader(), nullptr, 0);

			ID3D11ShaderResourceView* rv = m_imageData->GetResourceView ();
			context->PSSetShaderResources (0, 1, &rv);
			ID3D11SamplerState* sampler = m_imageData->GetSampler();
			context->PSSetSamplers (0, 1, &sampler);

			//D3DReflect
			

			context->DrawIndexed (m_meshData->GetIndicesCount (), 0, 0);
		}

		bool MeshComponent::Shutdown ()
		{
			return true;
		}
	}
}