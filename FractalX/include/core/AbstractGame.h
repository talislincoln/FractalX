#pragma once
#ifndef _ABSTRACTGAME_H
#define _ABSTRACTGAME_H

namespace fractal
{
	namespace fscene
	{
		class Scene;
	}

	namespace fcore
	{
		class AbstractGame
		{
		public:
			AbstractGame();
			virtual ~AbstractGame();

			virtual bool Init() = 0;
			virtual bool Shutdown() = 0;

		protected:
			// this will call the system manager and add the scene there.
			// the idea is to avoid the user having to get the instance of the system manager and add the scene from there...
			void AddScene (fscene::Scene* scene);
		};
	}
}

#endif