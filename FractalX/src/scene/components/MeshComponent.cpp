#include <FractalPCH.h>
#include "scene\components\MeshComponent.h"

#include <core\EngineCore.h>
#include <scene\EngineScene.h>

#include <scene\components\LightComponent.h>

namespace fractal
{
	namespace fscene
	{
		MeshComponent::MeshComponent (fcore::MeshDataResource* meshData, fcore::ShaderResource* shaderData, fcore::ImageResource* imageData, fcore::MaterialResource* materialData) :
			m_meshData(meshData),
			m_shaderData(shaderData),
			m_imageData(imageData),
			m_materialData(materialData)
		{
			// empty
		}

		MeshComponent::MeshComponent (const FString& meshResourceName, const FString& shaderResourceName, const FString& imageResourceName, fcore::MaterialResource* materialData) :
			m_meshData(nullptr),
			m_shaderData(nullptr),
			m_imageData(nullptr),
			m_materialData(materialData)
		{
			fcore::ResourceManager* resourceManager = fcore::ResourceManager::Instance ();
			ID3D11DeviceContext* context = dynamic_cast<fractal::fcore::Graphics*>(fractal::fcore::SystemManager::Instance ()->GetSystem (fractal::SystemType::GRAPHICS_SYSTEM))->GetContext ();

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
			fcore::SceneManager* sceneManager = fractal::fcore::SceneManager::Instance ();

			m_shaderData->UseShader ();		
			using namespace DirectX;

			//context->UpdateSubresource (m_shaderData->GetConstantBuffers ()[1], 0, nullptr, &sceneManager->GetActiveCamera ()->GetViewMatrix (), 0, 0);
			XMMATRIX aux = m_gameObject->GetWorldMatrix ();
			context->UpdateSubresource (m_shaderData->GetConstantBuffers ()[2], 0, nullptr, &aux, 0, 0);

			fcore::light ll;
			LightComponent* l = sceneManager->GetLight ();
			
			ll.m_ambientColor = l->m_ambientColor;
			ll.m_diffuseColor = l->m_diffuseColor;
			context->UpdateSubresource (m_shaderData->lightBuffer, 0, nullptr, &ll, 0, 0);
			context->VSSetConstantBuffers (0, 1, &m_shaderData->lightBuffer);
			//context->UpdateSubresource (m_shaderData->GetConstantBuffers ()[3], 0, nullptr, &ll.m_ambientColor, 0, 0);

			//context->VSSetConstantBuffers (0, 1, &m_shaderData->lightBuffer);
			//context->VSSetConstantBuffers (0, 1, m_shaderData->GetConstantBuffers ()[3]);
			const UINT vertexStride = sizeof (VertexPosColorTexture);
			const UINT offset = 0;

			ID3D11Buffer* vertexBuffer = m_meshData->GetVertexBuffer ();
			context->IASetVertexBuffers (0, 1, &vertexBuffer, &vertexStride, &offset);
			context->IASetInputLayout (m_shaderData->GetInputLayout ());
			

			ID3D11Buffer* indexBuffer = m_meshData->GetIndexBuffer ();
			context->IASetIndexBuffer (indexBuffer, DXGI_FORMAT_R16_UINT, 0);

			ID3D11ShaderResourceView* rv = m_imageData->GetResourceView ();
			context->PSSetShaderResources (0, 1, &rv);

			ID3D11SamplerState* sampler = m_imageData->GetSampler ();
			context->PSSetSamplers (0, 1, &sampler);
			context->VSSetConstantBuffers (0, 4, m_shaderData->GetConstantBuffers ());

			context->DrawIndexed (m_meshData->GetIndicesCount (), 0, 0);
		}

		bool MeshComponent::Shutdown ()
		{
			return true;
		}

		fcore::ShaderResource* MeshComponent::GetShader () const
		{
			return m_shaderData;
		}
	}
}