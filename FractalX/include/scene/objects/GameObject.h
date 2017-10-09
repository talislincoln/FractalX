#pragma once
#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H

#include <vector>
#include "FObject.h"

namespace fractal
{
	namespace fscene
	{
		class GameObject : public FObject
		{
		private:
			std::vector<class Component*> m_components;
			std::vector<class GameObject*> m_children;

			class GameObject* m_parent;

		public:
			GameObject(FString name);
			~GameObject();

			virtual bool Init() override;
			virtual void Update () override;
			virtual void Draw () const override;
			virtual bool Shutdown() override;

			void AddComponent (class Component* comp);
			void RemoveComponent (class Component* comp);
			template <typename T> T* GetComponent () const;
			std::vector<class Component*> GetComponents () const;

			void AddChildObject (class GameObject* obj);
			void RemoveChildObject (class GameObject* obj);

			void SetParent (GameObject* obj);
			GameObject* GetParent () const;

			GameObject* GetRootParent () const;
		};
	}
}

#endif