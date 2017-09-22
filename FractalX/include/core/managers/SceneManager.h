#pragma once
#ifndef _SCENEMANAGER_H
#define _SCENEMANAGER_H

#include "IManager.h"
#include <core\interfaces\IDrawable.h>
#include <vector>

namespace fractal
{
	// foward declaration
	namespace fscene
	{
		class Scene;
	}

	namespace fcore
	{
		class SceneManager : public IManager<SceneManager>, public IDrawable
		{
		private:
			std::vector<fscene::Scene*> m_scenes;
			fscene::Scene* m_activeScene;
		public:
			SceneManager() : IManager<SceneManager>() {}
			~SceneManager () {};

			bool Init () override;
			bool Shutdown () override;
			void Update () override;
			void Draw () const override;

			void AddScene (fscene::Scene* scene);
			void SetActiveScene (const FString& name);
			class fscene::Scene* GetActiveScene () const;
		};
	}
}

#endif // !_SCENEMANAGER
