#pragma once
#ifndef _IDRAWABLE_H
#define _IDRAWABLE_H

namespace fractal
{
	namespace fcore
	{
		class IDrawable
		{
		private:
			bool m_canDraw;

		public:
			IDrawable () : m_canDraw (true) {};
			virtual ~IDrawable () {};

			virtual void Draw () const = 0;

			inline void SetCanDraw (bool draw) { m_canDraw = draw; }
			inline bool GetCanDraw () const { return m_canDraw; }

		};
	}
}

#endif // !_IDRAWABLE_H
