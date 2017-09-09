#include <FractalPCH.h>
#include <scene/objects/FObject.h>

namespace fractal
{
	namespace fscene
	{
		unsigned __int32 FObject::counter = -1;

		FObject::FObject(const FString& name) :
			m_id(++counter),
			m_name(name),
			m_isActive(false),
			m_isDestroyed(false),
			m_isInitialized(false)
		{

		}

		FObject::~FObject()
		{

		}

		void FObject::SetInitialized(bool newState) { m_isInitialized = newState; }

		inline const bool FObject::IsInitialized() const { return m_isInitialized; }

		inline void FObject::SetActive(bool newState) { m_isActive = newState; }

		inline const bool FObject::IsActive() const { return m_isActive; }

		inline void FObject::Destroy() { m_isDestroyed = true; }
		inline bool FObject::IsDestroyed() const { return m_isDestroyed; }

		inline void FObject::SetName(const FString& name) { m_name = name; }

		inline const FString& FObject::GetName() const { return m_name; }
	}
}