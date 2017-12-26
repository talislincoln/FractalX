#include <FractalPCH.h>
#include <scene\components\Component.h>
#include <scene\objects\GameObject.h>

namespace fractal
{
	namespace fscene
	{
		Component::Component (const FString& name) :
			FObject (name),
			m_gameObject (nullptr)
		{
			//empty
		}

		Component::~Component ()
		{
			//empty
		}

		void Component::SetGameObject (GameObject* go)
		{
			m_gameObject = go;
		}

		GameObject* Component::GetGameObject () const
		{
			return m_gameObject;
		}
	}
}