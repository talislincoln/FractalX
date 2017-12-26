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
			DirectX::XMMATRIX m_worldMatrix;

			DirectX::XMFLOAT3 m_position;
			DirectX::XMFLOAT3 m_rotations;
			DirectX::XMFLOAT3 m_scaling;

			// in a perfect would, this would align with the above variables
			float m_padding; 

			bool m_isDirty;

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

			DirectX::XMMATRIX GetRotationMatrix () const;
			DirectX::XMMATRIX GetLookAtMatrix () const;
			DirectX::XMMATRIX GetWorldMatrix () const;
			DirectX::XMVECTOR GetForwardVector () const;
			DirectX::XMVECTOR GetUpVector () const;


			void SetPosition (const DirectX::XMFLOAT3& newPosition);
			void SetPosition (float x, float y, float z);
			const DirectX::XMFLOAT3& GetPosition () const;

			void Rotate (const DirectX::XMFLOAT3& rotations);
			void Rotate (float rotationX, float rotationY, float rotationZ);
			const DirectX::XMFLOAT3& GetRotation () const;

			DirectX::XMFLOAT3 GetScale () const;

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

		template <typename T>
		T* GameObject::GetComponent () const
		{
			for (Component* c : this->m_components)
			{
				T* comp = dynamic_cast<T*>(c);
				if (comp == nullptr)
					continue;

				return comp;
			}

			return nullptr;
		}
	}
}

#endif