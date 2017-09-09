#pragma once
#ifndef _SINGLETON_H
#define _SINGLETON_H

namespace fractal
{
	namespace fcore
	{
		template <typename T>
		class Singleton
		{
		public:
			static void DestroyInstance()
			{
				if (Singleton::m_instance)
					delete m_instance;
			}

			static T& Instance()
			{
				if (!Singleton::m_instance)
					Singleton::m_instance = new T;
				return *Singleton::m_instance;				
			}

		protected:
			Singleton() {}
			virtual ~Singleton() {}

		private:

			//Disable copy constructor & assignment operator
			Singleton(Singleton const&) {}
			Singleton& operator=(Singleton const&) {}

			static T* m_instance;
		};

		template<typename T>
		T * Singleton<T>::m_instance = nullptr;
	}
}

#endif // !_SINGLETON_H
