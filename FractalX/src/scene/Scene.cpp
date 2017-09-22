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

		}

		Scene::~Scene ()
		{

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

			return true;
		}

		void Scene::Update ()
		{

		}

		void Scene::Draw () const
		{

		}

		bool Scene::Shutdown ()
		{

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
				this->m_objects.erase (it);
				SafeDelete ((*it));
			}
		}
	}
}