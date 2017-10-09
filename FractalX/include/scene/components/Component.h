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
		private:
			class GameObject* m_parent;
		public:
			Component (const FString& name = _T(""));
			virtual ~Component ();

			virtual bool Init () = 0;
			virtual void Update () = 0;
			virtual void Draw () = 0;
			virtual bool Shutdown () = 0;

			void SetParent (class GameObject* parent);
			class GameObject* GetParent () const;
		};
	}
}

#endif // !_COMPONENT_H
