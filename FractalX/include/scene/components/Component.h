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
			class GameObject* m_parent;
		public:
			Component (const FString& name = _T(""));
			virtual ~Component ();

			void SetParent (class GameObject* parent);
			class GameObject* GetParent () const;
		};
	}
}

#endif // !_COMPONENT_H
