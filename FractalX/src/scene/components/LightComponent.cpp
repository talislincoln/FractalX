#include <FractalPCH.h>

#include <scene\components\LightComponent.h>

#include <core\EngineCore.h>

namespace fractal
{
	namespace fscene
	{
		LightComponent::LightComponent (const FString& name, float r, float g, float b, float a) :
			m_diffuseColor(DirectX::XMFLOAT4(r, g, b, a)),
			m_ambientColor(DirectX::XMFLOAT4 (r, g, b, a))
		{
			fcore::SceneManager* sceneManager = fcore::SceneManager::Instance ();

			sceneManager->AddLight (this);
		}

		LightComponent::~LightComponent ()
		{
			// empty
		}

		bool LightComponent::Init ()
		{
			SetActive (true);
			SetCanDraw (true);

			return true;
		}

		void LightComponent::Update ()
		{

		}

		void LightComponent::Draw () const
		{
		}

		bool LightComponent::Shutdown ()
		{
			return true;
		}
	}
}