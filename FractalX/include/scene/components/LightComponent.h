#pragma once
#ifndef _LIGHTCOMPONENT_H
#define _LIGHTCOMPONENT_H

#include "Component.h"

namespace fractal
{
	namespace fscene
	{
		class LightComponent : public Component
		{
		public:
			DirectX::XMFLOAT4 m_ambientColor;
			DirectX::XMFLOAT4 m_diffuseColor;

		public:

			LightComponent (const FString& name, float r, float g, float b, float a);
			~LightComponent ();

			bool Init () override;
			void Update () override;
			void Draw () const override;
			bool Shutdown () override;
		};
	}
}

#endif // !_LIGHTCOMPONENT_H
