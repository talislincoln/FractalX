#include <FractalPCH.h>
#include <scene\components\Component.h>
#include <scene\objects\GameObject.h>

namespace fractal
{
	namespace fscene
	{
		Component::Component (const FString& name) :
			FObject (name),
			m_parent (nullptr)
		{
			//empty
		}

		Component::~Component ()
		{
			//empty
		}

		void Component::SetParent (GameObject* parent)
		{
			m_parent = parent;
		}

		GameObject* Component::GetParent () const
		{
			return m_parent;
		}
	}
}