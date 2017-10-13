#pragma once
#ifndef _CAMERACOMPONENT_H
#define _CAMERACOMPONENT_H

#include "Component.h"

namespace fractal
{
	namespace fscene
	{
		class CameraComponent : public Component
		{
		private:
			DirectX::XMVECTOR m_target;
		public:
			CameraComponent (const FString& name);
			~CameraComponent ();

		};
	}
}

#endif // !_CAMERACOMPONENT_H
