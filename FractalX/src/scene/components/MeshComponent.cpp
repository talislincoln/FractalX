#include <FractalPCH.h>
#include "scene\components\MeshComponent.h"

#include <core\EngineCore.h>
#include <scene\EngineScene.h>

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
			ID3D11DeviceContext* context = dynamic_cast<fractal::fcore::Graphics*>(fractal::fcore::SystemManager::Instance ()->GetSystem (fractal::SystemType::GRAPHICS_SYSTEM))->GetContext ();

			m_meshData = resourceManager->GetResource<fcore::MeshDataResource> (meshResourceName);
			m_shaderData = resourceManager->GetResource<fcore::ShaderResource> (shaderResourceName);
			m_imageData = resourceManager->GetResource<fcore::ImageResource> (imageResourceName);

			// TODO: double check if we should put the rest of this function in the draw methods
			const UINT vertexStride = sizeof (VertexPosColorTexture);
			const UINT offset = 0;

			//m_shaderData->UseShader ();

			ID3D11Buffer* vertexBuffer = m_meshData->GetVertexBuffer ();
			context->IASetVertexBuffers (0, 1, &vertexBuffer, &vertexStride, &offset);
			context->IASetInputLayout (m_shaderData->GetInputLayout ());

			ID3D11Buffer* indexBuffer = m_meshData->GetIndexBuffer ();
			context->IASetIndexBuffer (indexBuffer, DXGI_FORMAT_R16_UINT, 0);

			ID3D11ShaderResourceView* rv = m_imageData->GetResourceView ();
			context->PSSetShaderResources (0, 1, &rv);
			ID3D11SamplerState* sampler = m_imageData->GetSampler ();
			context->PSSetSamplers (0, 1, &sampler);
			context->VSSetConstantBuffers (0, 3, m_shaderData->GetConstantBuffers ());
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

			XMFLOAT3 pos = m_parent->GetPosition ();
			XMFLOAT3 scale = m_parent->GetScale ();
			XMMATRIX world = (XMMatrixTranslation(pos.x, pos.y, pos.z) * m_parent->GetRotationMatrix () * XMMatrixScaling (scale.x, scale.y, scale.z)) *
				sceneManager->GetActiveCamera ()->GetViewMatrix () *
				sceneManager->GetActiveCamera ()->GetCameraProjection ();

			context->UpdateSubresource (m_shaderData->GetConstantBuffers ()[2], 0, nullptr, &XMMatrixTranspose(world), 0, 0);

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