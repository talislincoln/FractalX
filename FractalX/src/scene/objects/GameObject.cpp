#include <FractalPCH.h>
#include "scene/objects/GameObject.h"
#include "scene/components/Component.h"

namespace fractal
{
	namespace fscene
	{
		GameObject::GameObject (FString name) :
			GameObject(name, nullptr)
		{
			// empty
		}

		GameObject::GameObject(FString name, GameObject* parent) :
			FObject(name),
			m_worldMatrix(DirectX::XMMatrixIdentity()),
			m_localTransform (DirectX::XMMatrixIdentity ()),
			m_parent(nullptr),
			m_position (DirectX::XMFLOAT3 (0.0f, 0.0f, 0.0f)),
			m_rotations (DirectX::XMFLOAT3 (0.0f, 0.0f, 0.0f)),
			m_scaling  (DirectX::XMFLOAT3 (1.0f, 1.0f, 1.0f)),
			m_isDirty (false)
		{
			if (parent)
			{
				parent->AddChildObject (this);
				m_worldMatrix = m_parent->GetWorldMatrix ();
			}
		}

		GameObject::~GameObject()
		{
			// empty
		}

		bool GameObject::Init()
		{
			for (Component* c : m_components)
			{
				if (c->IsInitialized ())
					continue;

				if (!c->Init ())
					return false;
			}

			for (GameObject* obj : m_children)
			{
				if (obj->IsInitialized ())
					continue;

				if (!obj->Init ())
					return false;
			}

			this->SetInitialized(true);
			this->SetActive(true);
			this->SetCanDraw (true);
			
			return true;
		}

		void GameObject::Update ()
		{
			if (m_isDirty)
			{
				m_localTransform = DirectX::XMMatrixScaling (m_scaling.x, m_scaling.y, m_scaling.z) * GetRotationMatrix() * DirectX::XMMatrixTranslation (m_position.x, m_position.y, m_position.z);
				for (GameObject* child : m_children)
				{
					child->UpdateWorldTransform ();
				}
				m_isDirty = false;
			}

			// update components
			for (Component* c : m_components)
			{
				if (c->IsActive ())
					c->Update ();
			}

			// update children game objects
			for (GameObject* obj : m_children)
			{
				if (obj->IsActive ())
					obj->Update ();
			}
		}

		void GameObject::Draw () const
		{
			// update components
			for (Component* c : m_components)
			{
				if (c->IsActive () && c->CanDraw())
					c->Draw ();
			}

			// update children game objects
			for (GameObject* obj : m_children)
			{
				if (obj->IsActive () && obj->CanDraw())
					obj->Draw ();
			}
		}

		bool GameObject::Shutdown()
		{
			this->SetActive(false);
			this->Destroy();

			// delete and clear all the components
			for (Component* c : this->m_components)
			{
				if (!c->Shutdown ())
					return false;
				SafeDelete (c);
			}
			this->m_components.clear ();

			// delete and clear all the game objects
			for (GameObject* obj : this->m_children)
			{
				if (!obj->Shutdown ())
					return false;
				SafeDelete (obj);
			}
			this->m_children.clear ();

			return true;
		}

		DirectX::XMMATRIX GameObject::GetRotationMatrix () const
		{
			using namespace DirectX;

			XMFLOAT3 rotations = GetRotation ();
			float rotationX = rotations.x;
			float rotationY = rotations.y;
			float rotationZ = rotations.z;

			float yaw, pitch, roll;
			pitch = rotationX * TO_RADIANS;
			yaw = rotationY * TO_RADIANS;
			roll = rotationZ * TO_RADIANS;

			//return m_parent ? m_parent->GetWorldMatrix() * XMMatrixRotationRollPitchYaw (roll, yaw, pitch) : XMMatrixRotationRollPitchYaw (roll, yaw, pitch);
			return XMMatrixRotationRollPitchYaw (roll, yaw, pitch);
		}

		/*DirectX::XMMATRIX GameObject::GetLookAtMatrix () const
		{
			DirectX::XMFLOAT3 pos = GetPosition ();

			return DirectX::XMMatrixLookToLH (DirectX::XMVectorSet(pos.x, pos.y, pos.z, 1), GetForwardVector (), GetUpVector ());
		}*/

		DirectX::XMMATRIX GameObject::GetWorldMatrix () const
		{
			return  m_localTransform * m_worldMatrix;
		}

		DirectX::XMVECTOR GameObject::GetForwardVector () const
		{
			return XMVector3TransformCoord (VECTOR_FORWARD, GetRotationMatrix());
		}

		DirectX::XMVECTOR GameObject::GetUpVector () const
		{
			using namespace DirectX;

			return XMVector3TransformCoord (VECTOR_UP, GetRotationMatrix());
		}

		void GameObject::Translate (const DirectX::XMFLOAT3& increment)
		{
			m_position.x += increment.x;
			m_position.y += increment.y;
			m_position.z += increment.z;
			m_isDirty = true;
		}

		void GameObject::Translate (float x, float y, float z)
		{
			m_position.x += x;
			m_position.y += y;
			m_position.z += z;
			m_isDirty = true;
		}

		void GameObject::SetPosition (const DirectX::XMFLOAT3& newPosition)
		{
			m_position = newPosition;
			m_isDirty = true;
		}

		void GameObject::SetPosition (float x, float y, float z)
		{
			m_position = DirectX::XMFLOAT3 (x, y, z);
			m_isDirty = true;
		}

		const DirectX::XMFLOAT3& GameObject::GetPosition () const
		{
			return m_position;
		}

		void GameObject::Rotate (const DirectX::XMFLOAT3 &rotations)
		{
			m_rotations = rotations;
			m_isDirty = true;
		}

		void GameObject::Rotate (float rotationX, float rotationY, float rotationZ)
		{
			m_rotations = DirectX::XMFLOAT3 (rotationX, rotationY, rotationZ);
			m_isDirty = true;
		}

		const DirectX::XMFLOAT3& GameObject::GetRotation () const
		{
			return m_rotations;
		}

		void GameObject::SetScate (float x, float y, float z)
		{
			m_scaling.x = x;
			m_scaling.y = y;
			m_scaling.z = z;
			
			m_isDirty = true;
		}

		DirectX::XMFLOAT3 GameObject::GetScale () const
		{
			return m_scaling;
		}

		void GameObject::AddComponent (Component* comp)
		{
			std::vector<Component*>::iterator it = std::find (this->m_components.begin (), this->m_components.end (), comp);
			if (it == this->m_components.end ())
			{
				this->m_components.push_back (comp);
				comp->SetGameObject (this);
			}
		}

		void GameObject::RemoveComponent (Component* comp)
		{
			std::vector<Component*>::iterator it = std::find (this->m_components.begin (), this->m_components.end (), comp);
			if (it != this->m_components.end ())
			{
				(*it)->Shutdown ();
				this->m_components.erase (it);
				SafeDelete (*it);
			}
		}

		std::vector<Component*> GameObject::GetComponents () const
		{
			return m_components;
		}

		void GameObject::AddChildObject (GameObject* obj)
		{
			std::vector<GameObject*>::iterator it = std::find (this->m_children.begin (), this->m_children.end (), obj);
			if (it == this->m_children.end ())
			{
				this->m_children.push_back (obj);
				obj->SetParent (this);
				// TODO: Add the object to the scene?
				//obj->setScene (getScene ());
			}
		}

		void GameObject::RemoveChildObject (GameObject* obj)
		{
			std::vector<GameObject*>::iterator it = std::find (this->m_children.begin (), this->m_children.end (), obj);
			if (it != this->m_children.end ())
			{
				(*it)->Shutdown ();
				this->m_children.erase (it);
				SafeDelete (*it);
			}
		}

		void GameObject::SetParent (GameObject* obj)
		{
			m_parent = obj;
			m_worldMatrix = obj->GetWorldMatrix ();
		}

		GameObject* GameObject::GetParent () const
		{
			return m_parent;
		}

		GameObject* GameObject::GetRootParent () const
		{
			return m_parent == nullptr ? const_cast<GameObject*>(this) : m_parent->GetRootParent ();
		}

		void GameObject::UpdateWorldTransform ()
		{
			m_worldMatrix = m_parent->GetWorldMatrix ();
		}
	}
}