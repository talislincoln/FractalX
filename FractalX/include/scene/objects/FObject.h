#pragma once
#ifndef _UOBJECT_H
#define _UOBJECT_H

namespace fractal
{
	namespace fscene
	{
		class FObject
		{
			static unsigned __int32 counter;

		private:
			FString m_name;

			unsigned __int32 m_id;
			
			bool m_isActive;

			bool m_isInitialized;
			bool m_canDraw;
			bool m_isDestroyed;
			
		public:
			FObject(const FString& name = _T(""));
			virtual ~FObject();

			virtual bool Init() = 0;
			virtual void Update () = 0;
			virtual void Draw () const = 0;
			virtual bool Shutdown() = 0;

			void SetInitialized(bool newState);
			const bool IsInitialized() const;

			void SetActive(bool newState);
			const bool IsActive() const;

			void SetCanDraw (bool newState);
			bool CanDraw () const;

			void Destroy();
			bool IsDestroyed() const;

			void SetName(const FString& name);
			const FString& GetName() const;

		protected:
			template<typename T>
			const FString GenerateName(const FString& partialName)
			{
				return partialName + std::to_string(ObjectCounter<T>::getAmount());
			}
		};
	}
}

#endif