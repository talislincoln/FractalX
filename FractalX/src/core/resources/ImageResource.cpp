#include <FractalPCH.h>
#include <core\resources\ImageResource.h>

#include <core\EngineCore.h>

namespace fractal
{
	namespace fcore
	{
		ImageResource::ImageResource (const FString& name, const FString& path) :
			Resource (name, ResourceType::RESOURCE_IMAGE),
			m_textureResourceView (nullptr),
			m_samplerLinear (nullptr)
		{
			Window* w = dynamic_cast<Window*>(SystemManager::Instance ()->GetSystem (SystemType::WINDOW_SYSTEM));
			ID3D11Device* d = dynamic_cast<Graphics*>(SystemManager::Instance ()->GetSystem (SystemType::GRAPHICS_SYSTEM))->GetDevice();
			LogManager *log = LogManager::Instance ();
			
			HRESULT hr = S_OK;
			
			// Load the Texture
			hr = DirectX::CreateDDSTextureFromFile (d, path.c_str(), nullptr, &m_textureResourceView);
			if (FAILED (hr))
				log->LogError (L"Failed to load texture");

			// Create the sample state
			D3D11_SAMPLER_DESC sampDesc;
			ZeroMemory (&sampDesc, sizeof (sampDesc));
			sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			sampDesc.MinLOD = 0;
			sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
			hr = d->CreateSamplerState (&sampDesc, &m_samplerLinear);
			if (FAILED (hr))
				log->LogError (L"Failed to create sample state for texture " + name);
		}

		ImageResource::~ImageResource ()
		{
			// empty
		}

		bool ImageResource::Init ()
		{
			return m_textureResourceView && m_samplerLinear && Resource::Init();
		}

		bool ImageResource::Shutdown ()
		{
			SafeRelease (m_textureResourceView);
			SafeRelease (m_samplerLinear);

			return Resource::Shutdown ();
		}
	}
}