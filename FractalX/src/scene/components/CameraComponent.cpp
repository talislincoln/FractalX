#include <FractalPCH.h>
#include <scene\components\CameraComponent.h>

#include <core\EngineCore.h>
#include <scene\EngineScene.h>

namespace fractal
{
	namespace fscene
	{
		CameraComponent::CameraComponent (const FString& name, float fov /* = 60.0f */, 
			float nearPlane /* = 0.3f */, float farPlane /* = 1000.0f */) :
			m_clippingPlanes(DirectX::XMFLOAT2(nearPlane, farPlane)),
			m_fov(fov), m_isDirty(false)
		{
			// empty
			fcore::SceneManager* sceneManager = fcore::SceneManager::Instance ();
			sceneManager->AddCamera (this);

			UpdateProjectionMatrix ();
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

		void CameraComponent::SetFieldOfView (float fov)
		{
			m_fov = fov;

			UpdateProjectionMatrix ();
		}

		float CameraComponent::GetFieldOfView () const
		{
			return m_fov;
		}

		float CameraComponent::GetNearClippingPlane () const
		{
			return m_clippingPlanes.x;
		}

		float CameraComponent::GetFarClippingPlane () const
		{
			return m_clippingPlanes.y;
		}

		void CameraComponent::SetClippingPlanes (float nearPlane, float farPlane)
		{
			m_clippingPlanes = DirectX::XMFLOAT2 (nearPlane, farPlane);
			UpdateProjectionMatrix ();
		}

		DirectX::XMMATRIX CameraComponent::GetViewMatrix () const
		{
			DirectX::XMFLOAT3 pos = m_gameObject->GetPosition ();

			return DirectX::XMMatrixLookToLH (DirectX::XMVectorSet (pos.x, pos.y, pos.z, 1), m_gameObject->GetForwardVector(), m_gameObject->GetUpVector());
		}

		void CameraComponent::SetProjectionMatrix (float fov, float nearPlane, float farPlane)
		{
			m_fov = fov;
			m_clippingPlanes.x = nearPlane;
			m_clippingPlanes.y = farPlane;

			UpdateProjectionMatrix ();
		}

		DirectX::XMMATRIX CameraComponent::GetProjectionMatrix () const
		{
			return m_projectionMatrix;
		}

		void CameraComponent::UpdateProjectionMatrix ()
		{
			fcore::Window *w = fcore::SystemManager::Instance ()->GetWindowSystem ();
			m_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH (m_fov * MATH_PI / 180.0f, w->AspectRation (), m_clippingPlanes.x, m_clippingPlanes.y);
		}
	}
}