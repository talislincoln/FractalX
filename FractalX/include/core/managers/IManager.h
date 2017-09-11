#pragma once
#ifndef _MANAGER_H
#define _MANAGER_H

#include "../systems/Singleton.h"

namespace fractal
{
	namespace fcore
	{
		template <typename T>
		class IManager : public Singleton<T>
		{
		public:

			IManager () : Singleton<T>() {}
			virtual ~IManager () {}

			virtual bool Init () = 0;
			virtual bool Shutdown () = 0;

		private:

		};
	}
}

#endif // !_MANAGER_H
