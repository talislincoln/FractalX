#include <FractalPCH.h>
#include <core/systems/System.h>

namespace fractal
{
	namespace fcore
	{
		System::System(SystemType type) :
			m_systemType(type),
			m_isActive(true)
		{
		}

		System::~System()
		{
		}

		void System::SetActive(bool newState)
		{
			this->m_isActive = newState;
		}

		bool System::IsActive() const
		{
			return this->m_isActive;
		}

		SystemType System::GetType() const
		{
			return this->m_systemType;
		}
	}
}