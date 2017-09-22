#pragma once
#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H

#include "FObject.h"

namespace fractal
{
	namespace fscene
	{
		class GameObject : public FObject
		{
		public:
			GameObject(FString name);
			~GameObject();

			virtual bool Init() override;
			virtual void Update () override;
			virtual void Draw () const override;
			virtual bool Shutdown() override;
		};
	}
}

#endif