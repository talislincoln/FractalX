#pragma once
#ifndef _COMPONENT_H
#define _COMPONENT_H

#include <scene\objects\FObject.h>

namespace fractal
{
	namespace fscene
	{
		class Component : public FObject
		{
		protected:
			class GameObject* m_gameObject;
		public:
			Component (const FString& name = _T(""));
			virtual ~Component ();

			void SetGameObject (class GameObject* parent);
			class GameObject* GetGameObject () const;
		};
	}
}

#endif // !_COMPONENT_H
