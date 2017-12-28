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
		class CameraComponent;
	}

	namespace fcore
	{
		class SceneManager : public IManager<SceneManager>, public IDrawable
		{
		private:
			std::vector<fscene::Scene*> m_scenes;
			std::vector<fscene::CameraComponent*> m_cameras;

			fscene::Scene* m_activeScene;
			fscene::CameraComponent* m_activeCamera;
			
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

			void AddCamera (fscene::CameraComponent* camera);
			void SetActiveCamera (fscene::CameraComponent* camera);
			fscene::CameraComponent* GetActiveCamera () const;

			DirectX::XMMATRIX GetPerspectiveMatrix () const;
		};
	}
}

#endif // !_SCENEMANAGER
