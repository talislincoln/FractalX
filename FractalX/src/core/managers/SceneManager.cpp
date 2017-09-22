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

		}

		void SceneManager::Draw () const
		{

		}

		void SceneManager::AddScene (fscene::Scene* scene)
		{
			std::vector<fscene::Scene*>::iterator it = std::find (m_scenes.begin (), m_scenes.end (), scene);
			if (it == m_scenes.end())
				m_scenes.push_back (scene);
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
	}
}