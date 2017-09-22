#pragma once
#ifndef _SCENE_H
#define _SCENE_H

#include <vector>
#include "objects\FObject.h"

namespace fractal
{
	namespace fscene
	{
		class Scene : public FObject
		{
		private:
			std::vector<class GameObject*> m_objects;

		public:
			Scene (const FString& name);
			~Scene ();

			virtual bool Init () override;
			virtual void Update () override;
			virtual void Draw () const override;
			virtual bool Shutdown () override;

			void AddGameObject (class GameObject* obj);
			void RemoveGameObject (class GameObject* obj);
		};
	}
}

#endif // !_SCENE_H
