#include <FractalPCH.h>
#include <scene\Scene.h>
#include <scene\objects\GameObject.h>

namespace fractal
{
	namespace fscene
	{
		Scene::Scene (const FString& name) :
			FObject(name)
		{
			// empty
		}

		Scene::~Scene ()
		{
			// empty
		}

		bool Scene::Init ()
		{
			for (GameObject* obj : this->m_objects)
			{
				if (obj->IsInitialized ())
					continue;

				if (!obj->Init ())
					return false;
			}

			SetInitialized (true);

			return true;
		}

		void Scene::Update ()
		{
			for (GameObject* obj : this->m_objects)
			{
				if (obj->IsActive ())
					obj->Update ();
			}
		}

		void Scene::Draw () const
		{
			for (GameObject* obj : this->m_objects)
			{
				if(obj->IsActive() && obj->CanDraw())
					obj->Draw ();
			}
		}

		bool Scene::Shutdown ()
		{
			for (GameObject* obj : this->m_objects)
			{
				if (!obj->Shutdown ())
					return false;

				SafeDelete (obj);
			}

			m_objects.clear ();
			return true;
		}

		void Scene::AddGameObject (GameObject* obj)
		{
			std::vector<GameObject*>::iterator it = std::find (this->m_objects.begin (), this->m_objects.end (), obj);
			if (it == this->m_objects.end ())
			{
				this->m_objects.push_back (obj);
				//obj->setScene (this);
			}
		}

		void Scene::RemoveGameObject (GameObject* obj)
		{
			std::vector<GameObject*>::iterator it = std::find (this->m_objects.begin (), this->m_objects.end (), obj);
			if (it != this->m_objects.end ())
			{
				(*it)->Shutdown ();
				this->m_objects.erase (it);
				SafeDelete ((*it));
			}
		}
	}
}