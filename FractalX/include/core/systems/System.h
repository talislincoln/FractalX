#pragma once
#ifndef _SYSTEM_H
#define _SYSTEM_H
namespace fractal
{
	namespace fcore
	{
		class System
		{
		public:
			/**
			Default constructor.
			\param type The type of the system being created.
			*/
			System(SystemType type);

			/**
			Default destructor.
			*/
			virtual ~System();

			/**
			Initialize the system.
			*/
			virtual bool Init() = 0;

			/**
			Update method is called every loop iteration.
			*/
			virtual void Update() = 0;

			/**
			Shutdown system.
			This method is called before the system is destroyed.
			*/
			virtual bool Shutdown() = 0;

			/**
			Define if the active or not.
			*/
			void SetActive(bool state);

			bool IsActive() const;

			SystemType GetType() const;

		private:
			SystemType m_systemType;
			bool m_isActive;
		};
	}
}
#endif