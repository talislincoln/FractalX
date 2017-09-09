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

			//public methods
		public:
			// for testing while there's no game
			Engine() {};
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

			bool ShutDown();
		};
	}
}

#endif // !_ENGINE_H