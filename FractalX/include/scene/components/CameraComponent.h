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
			fcore::ShaderResource* m_shader;
			DirectX::XMFLOAT2 m_clippingPlanes;
		public:
			CameraComponent (const FString& name);
			~CameraComponent ();

			bool Init () override;
			void Update () override;
			void Draw () const override;
			bool Shutdown () override;

			void SetShader (fcore::ShaderResource* shader);

			DirectX::XMMATRIX GetViewMatrix () const;
			DirectX::XMMATRIX GetCameraProjection () const;
		};
	}
}

#endif // !_CAMERACOMPONENT_H
