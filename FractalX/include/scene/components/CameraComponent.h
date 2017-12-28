#pragma once
#ifndef _CAMERACOMPONENT_H
#define _CAMERACOMPONENT_H

#include "Component.h"

namespace fractal
{
	namespace fcore
	{
		class ShaderResource;
	}
	namespace fscene
	{
		class CameraComponent : public Component
		{
		private:
			//DirectX::XMVECTOR m_target;
			DirectX::XMMATRIX m_viewMatrix;
			DirectX::XMMATRIX m_projectionMatrix;

			DirectX::XMFLOAT2 m_clippingPlanes;
			fcore::ShaderResource* m_shader;
			float m_fov;

			bool m_isDirty;

		public:
			CameraComponent (const FString& name, float fov = 60.0f, float nearPlane = 0.3f, float farPlane = 1000.0f);
			~CameraComponent ();

			bool Init () override;
			void Update () override;
			void Draw () const override;
			bool Shutdown () override;

			void SetShader (fcore::ShaderResource* shader);

			void SetFieldOfView (float fov);
			float GetFieldOfView () const;

			float GetNearClippingPlane () const;
			float GetFarClippingPlane () const;
			void SetClippingPlanes (float nearPlane, float farPlane);

			DirectX::XMMATRIX GetViewMatrix () const;
			void SetProjectionMatrix (float fov, float nearPlane, float farPlane);
			DirectX::XMMATRIX GetProjectionMatrix () const;

		private:
			void UpdateProjectionMatrix ();
		};
	}
}

#endif // !_CAMERACOMPONENT_H
