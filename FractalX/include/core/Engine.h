#pragma once
#ifndef _ENGINE_H
#define _ENGINE_H

namespace fractal
{
	namespace fcore
	{
		class Engine
		{
			// private variables declaration first
		private:
			class AbstractGame* m_game;
			class Graphics* m_graphicsSystem;

			//public methods
		public:

			Engine(class AbstractGame* game);
			~Engine();

			int Run();

			//private methods
		private:
			bool Init();
			bool CreateManagers();
			bool DestroyManagers();

			void Update();
			void Draw();
			void CalculateFrameStats ();

			bool ShutDown();
		};
	}
}

#endif // !_ENGINE_H