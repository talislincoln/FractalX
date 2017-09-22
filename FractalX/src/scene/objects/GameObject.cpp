#include <FractalPCH.h>
#include "scene/objects/GameObject.h"

namespace fractal
{
	namespace fscene
	{
		GameObject::GameObject(FString name) :
			FObject(name)
		{

		}

		GameObject::~GameObject()
		{

		}

		bool GameObject::Init()
		{
			this->SetInitialized(true);
			this->SetActive(true);
			return true;
		}

		void GameObject::Update ()
		{
			// update components

			// update children game objects
		}

		void GameObject::Draw () const
		{

		}

		bool GameObject::Shutdown()
		{
			this->SetActive(false);
			this->Destroy();
			return true;
		}
	}
}