#include <FractalPCH.h>
#include "scene/objects/GameObject.h"
#include "scene/components/Component.h"

namespace fractal
{
	namespace fscene
	{
		GameObject::GameObject(FString name) :
			FObject(name),
			m_parent(nullptr),
			m_position(DirectX::XMVectorSet (0.0f, 0.0f, 0.0f, 1.0f))
		{

		}

		GameObject::~GameObject()
		{

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

		void GameObject::AddComponent (Component* comp)
		{
			std::vector<Component*>::iterator it = std::find (this->m_components.begin (), this->m_components.end (), comp);
			if (it == this->m_components.end ())
			{
				this->m_components.push_back (comp);
				comp->SetParent (this);
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
		}

		GameObject* GameObject::GetParent () const
		{
			return m_parent;
		}

		GameObject* GameObject::GetRootParent () const
		{
			return m_parent == nullptr ? const_cast<GameObject*>(this) : m_parent->GetRootParent ();
		}
	}
}