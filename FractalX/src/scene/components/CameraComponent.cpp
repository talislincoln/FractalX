#include <FractalPCH.h>
#include <scene\components\CameraComponent.h>

#include <core\EngineCore.h>
#include <scene\EngineScene.h>

namespace fractal
{
	namespace fscene
	{
		CameraComponent::CameraComponent (const FString& name) :
			m_clippingPlanes(DirectX::XMFLOAT2(0.3f, 1000.0f))
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
			fcore::Window *w =fcore::SystemManager::Instance ()->GetWindowSystem();
			
			return DirectX::XMMatrixPerspectiveFovLH (45.0f * MATH_PI / 180.0f, w->AspectRation (), m_clippingPlanes.x, m_clippingPlanes.y);
		}
	}
}