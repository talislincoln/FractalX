#include <FractalPCH.h>
#include <core\managers\SceneManager.h>
#include <scene\Scene.h>
#include <algorithm>

namespace fractal
{
	namespace fcore
	{
		bool SceneManager::Init ()
		{
			/*
			setupManager<Renderer>();
			setupManager<CameraManager>();
			setupManager<ResourceManager>();

			if (!this->active_scene->getInitialized())
			{
				this->active_scene->setRenderer(&Singleton<Renderer>::getInstance());
				this->active_scene->setCameraManager(&Singleton<CameraManager>::getInstance());
				this->active_scene->setResourceManager(&Singleton<ResourceManager>::getInstance());

				if (!this->active_scene->initialize())
					return false;
				this->active_scene->setInitialized();
			}*/

			if (!this->m_activeScene->Init ())
				return false;


			return true;
		}

		bool SceneManager::Shutdown ()
		{
			for (fscene::Scene* scene : this->m_scenes)
			{
				if (!scene->Shutdown ())
					return false;

				SafeDelete (scene);
			}

			//destroyManager<Renderer>();
			//destroyManager<CameraManager> ();
			//destroyManager<UIManager> ();

			return true;
		}

		void SceneManager::Update ()
		{
			assert (this->m_activeScene->IsInitialized ());

			if (this->m_activeScene->IsActive ())
				m_activeScene->Update ();
		}

		void SceneManager::Draw () const
		{
			assert (this->m_activeScene->IsInitialized ());

			if (this->m_activeScene->IsActive ())
				m_activeScene->Draw ();
		}

		void SceneManager::AddScene (fscene::Scene* scene)
		{
			std::vector<fscene::Scene*>::iterator it = std::find (m_scenes.begin (), m_scenes.end (), scene);
			if (it == m_scenes.end())
				m_scenes.push_back (scene);

			if (!m_activeScene)
			{
				m_activeScene = scene;
				m_activeScene->SetActive (true);
			}
		}

		void SceneManager::SetActiveScene (const FString& name)
		{
			std::vector<fscene::Scene*>::iterator it = std::find_if (m_scenes.begin (), m_scenes.end (),
				[name](fscene::Scene* scene) -> bool
				{
					return scene->GetName () == name;
				});

			//Scene with given name was not found.
			assert (it != m_scenes.end ());

			if (this->m_activeScene)
			{
				this->m_activeScene->SetActive (false);
			}

			this->m_activeScene = (*it);
			this->m_activeScene->SetActive(true);
		}

		fscene::Scene* SceneManager::GetActiveScene () const
		{
			return this->m_activeScene;
		}

		void SceneManager::AddCamera (fscene::CameraComponent* camera)
		{
			if (!m_activeCamera)
				m_activeCamera = camera;

			m_cameras.push_back (camera);
		}

		void SceneManager::SetActiveCamera (fscene::CameraComponent* camera)
		{
			m_activeCamera = camera;
		}

		fscene::CameraComponent* SceneManager::GetActiveCamera () const
		{
			return m_activeCamera;
		}
	}
}