#include <FractalPCH.h>
#include <scene\components\CameraComponent.h>

#include <core\EngineCore.h>
#include <scene\EngineScene.h>

namespace fractal
{
	namespace fscene
	{
		CameraComponent::CameraComponent (const FString& name)
		{
			// empty
			fcore::SceneManager* sceneManager = fcore::SceneManager::Instance ();
			sceneManager->AddCamera (this);
		}

		CameraComponent::~CameraComponent ()
		{
			// empty
		}

		bool CameraComponent::Init ()
		{
			SetActive (true);
			SetCanDraw (true);

			return true;
		}

		void CameraComponent::Update ()
		{
			ID3D11DeviceContext* context = dynamic_cast<fractal::fcore::Graphics*>(fractal::fcore::SystemManager::Instance ()->GetSystem (fractal::SystemType::GRAPHICS_SYSTEM))->GetContext ();

			m_shader->UseShader ();

			context->UpdateSubresource (m_shader->GetConstantBuffers ()[1], 0, nullptr, &GetViewMatrix (), 0, 0);
		}

		void CameraComponent::Draw () const
		{
			
		}

		bool CameraComponent::Shutdown ()
		{
			return true;
		}

		void CameraComponent::SetShader (fcore::ShaderResource *shader)
		{
			m_shader = shader;
		}

		DirectX::XMMATRIX CameraComponent::GetViewMatrix () const
		{
			DirectX::XMFLOAT3 pos = m_parent->GetPosition ();

			return DirectX::XMMatrixLookToLH (DirectX::XMVectorSet (pos.x, pos.y, pos.z, 1), m_parent->GetForwardVector(), m_parent->GetUpVector());
		}

		DirectX::XMMATRIX CameraComponent::GetCameraProjection () const
		{
			return DirectX::XMMatrixPerspectiveFovLH (0.4f*3.14f, 800.0f / 600.0f, 1.0f, 1000.0f);
		}
	}
}