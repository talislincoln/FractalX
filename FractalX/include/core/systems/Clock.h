#pragma once
#ifndef _TIMER_H
#define _TIMER_H

#include <core\systems\System.h>

namespace fractal
{
	namespace fcore
	{
		class Clock : public System
		{
		private:
			__int64 m_baseTime;
			__int64 m_pausedTime;
			__int64 m_stopTime;
			__int64 m_prevTime;
			__int64 m_currTime;

			double	m_secondsPerCount;
			double	m_deltaTime;

			bool	m_stopped;

		public:

			Clock ();
			~Clock ();

			bool Init () override;
			void Update () override;
			bool Shutdown () override;
			
			void Reset (); 
			void Start (); 
			void Stop ();  

			float DeltaTime () const;
			float TotalTime () const;

		};
	}
}

#endif // !_TIMER_H
