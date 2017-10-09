#pragma once
#ifndef _LOGIC_H
#define _LOGIC_H

#include "System.h"
#include <core\interfaces\IDrawable.h>

namespace fractal
{
	namespace fcore
	{
		class Logic : public System, public IDrawable
		{
		private:
			class AbstractGame* m_game;

		public:

			Logic ();
			~Logic ();

			bool Init () override;
			void Update () override;
			bool Shutdown () override;

			void Draw () const override;

			void SetGame (class AbstractGame* game);
		};
	}
}

#endif // !_LOGIC_H
