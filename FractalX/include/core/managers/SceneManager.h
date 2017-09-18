#pragma once
#ifndef _SCENEMANAGER_H
#define _SCENEMANAGER_H

#include "IManager.h"
#include <core\interfaces\IDrawable.h>

namespace fractal
{
	namespace fcore
	{
		class SceneManager : public IManager<SceneManager>, public IDrawable
		{
		public:
			SceneManager() : IManager<SceneManager>() {}
			~SceneManager () {};

			bool Init () override;
			bool Shutdown () override;
			void Update () override;
			void Draw () const override;

			void AddScene (class Scene* scene);
			void SetActiveScene (const FString& name);
			class Scene* GetActiveScene () const;;
		};
	}
}

#endif // !_SCENEMANAGER
