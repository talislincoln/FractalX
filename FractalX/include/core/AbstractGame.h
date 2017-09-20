#pragma once
#ifndef _ABSTRACTGAME_H
#define _ABSTRACTGAME_H

namespace fractal
{
	namespace fcore
	{
		class AbstractGame
		{
		public:
			AbstractGame();
			virtual ~AbstractGame();

			virtual bool Init() = 0;
			virtual bool Shutdown() = 0;
		};
	}
}

#endif