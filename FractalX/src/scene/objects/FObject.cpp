#include <FractalPCH.h>
#include <scene/objects/FObject.h>

namespace fractal
{
	namespace fscene
	{
		unsigned __int32 FObject::counter = 0;

		FObject::FObject(const FString& name) :
			m_id(++counter),
			m_name(name),
			m_isActive(false),
			m_isDestroyed(false),
			m_isInitialized(false),
			m_canDraw(false)
		{
			// empty
		}

		FObject::~FObject() 
		{
			// empty
		}

		void FObject::SetInitialized(bool newState) { m_isInitialized = newState; }

		const bool FObject::IsInitialized() const { return m_isInitialized; }

		void FObject::SetActive(bool newState) { m_isActive = newState; }
		const bool FObject::IsActive() const { return m_isActive; }

		void FObject::SetCanDraw (bool newState) { m_canDraw = newState; }
		bool FObject::CanDraw () const { return m_canDraw; }

		void FObject::Destroy() { m_isDestroyed = true; }
		bool FObject::IsDestroyed() const { return m_isDestroyed; }

		void FObject::SetName(const FString& name) { m_name = name; }

		const FString& FObject::GetName() const { return m_name; }
	}
}