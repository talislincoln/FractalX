#include <FractalPCH.h>
#include <core\EngineCore.h>
#include <scene\EngineScene.h>

namespace fractal
{
	namespace fcore
	{
		AbstractGame::AbstractGame() {}

		AbstractGame::~AbstractGame() {}

		void AbstractGame::AddScene (fscene::Scene* scene)
		{
			SceneManager::Instance ()->AddScene (scene);
 		}
	}
}