#include <FractalPCH.h>
#include <core\EngineCore.h>
#include "core\resources\ShaderResource.h"

namespace fractal
{
	namespace fcore
	{
		ShaderResource::ShaderResource (const FString& name, 
			const FString& vertexPath, const FString& pixelPath, 
			D3D11_INPUT_ELEMENT_DESC* vertexLayout /* = nullptr */, 
			int numElementsLayout /* = 0 */) :
			Resource(name, ResourceType::RESOURCE_SHADER),
			m_vertexShader (nullptr),
			m_pixelShader (nullptr),
			m_inputLayout (nullptr)
		{
			ID3D11Device* device = dynamic_cast<fractal::fcore::Graphics*>(fractal::fcore::SystemManager::Instance ()->GetSystem (fractal::SystemType::GRAPHICS_SYSTEM))->GetDevice ();
			assert (device);

			// TODO: Have to figure out a way to change the vertex and pixel shader output name to use long string
			// m_vertexShader = LoadShader<ID3D11VertexShader> (vertexPath, "SimpleVertexShader", "latest");
			// m_pixelShader = LoadShader<ID3D11PixelShader> (pixelPath, "SimplePixelShader", "latest");

			// Loading the vertex shader
			ID3DBlob* vertexShaderBlob;
			if (FAILED (D3DReadFileToBlob (vertexPath.c_str(), &vertexShaderBlob)))
			{
				fcout << L" error loading the vertex shader " << std::endl;
			}

			if (FAILED (device->CreateVertexShader (vertexShaderBlob->GetBufferPointer (), vertexShaderBlob->GetBufferSize (), nullptr, &m_vertexShader)))
			{
				fcout << L" error creating the vertex shader " << std::endl;
			}

			// Create the input layout for the vertex shader.
			if (!vertexLayout)
			{
				// TODO: improve this function to more robust version if it stops working
				CreateVertexInputLayout (vertexShaderBlob, &m_inputLayout);
			}
			else
			{
				// in case the function doesn't fully work, there's still the option to give the input layout explicitly
				if (FAILED (device->CreateInputLayout (vertexLayout, numElementsLayout, vertexShaderBlob->GetBufferPointer (), vertexShaderBlob->GetBufferSize (), &m_inputLayout)))
				{
					fcout << L" error creating the input layout " << std::endl;
				}
			}

			SafeRelease (vertexShaderBlob);

			// Loading the pixel shader
			ID3DBlob* pixelShaderBlob;
			if (FAILED (D3DReadFileToBlob (pixelPath.c_str (), &pixelShaderBlob)))
			{
				fcout << L" error loading the pixel shader " << std::endl;
			}

			if (FAILED (device->CreatePixelShader (pixelShaderBlob->GetBufferPointer (), pixelShaderBlob->GetBufferSize (), nullptr, &m_pixelShader)))
			{
				fcout << L" error creating the vertex shader " << std::endl;
			}
			SafeRelease (pixelShaderBlob);
		}

		void ShaderResource::CreateVertexInputLayout (ID3DBlob* vertexBlob, ID3D11InputLayout** inputlayout)
		{
			ID3D11Device* device = dynamic_cast<fractal::fcore::Graphics*>(fractal::fcore::SystemManager::Instance ()->GetSystem (fractal::SystemType::GRAPHICS_SYSTEM))->GetDevice ();

			// Reflect shader info
			ID3D11ShaderReflection* pVertexShaderReflection = NULL;
			if (FAILED (D3DReflect (vertexBlob->GetBufferPointer (), vertexBlob->GetBufferSize (), IID_ID3D11ShaderReflection, (void**)&pVertexShaderReflection)))
			{
				std::cout << "error" << std::endl;
				//return S_FALSE;
			}

			// Get shader info
			D3D11_SHADER_DESC shaderDesc;
			pVertexShaderReflection->GetDesc (&shaderDesc);

			// Read input layout description from shader info
			std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
			for (unsigned int i = 0; i< shaderDesc.InputParameters; i++)
			{
				D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
				pVertexShaderReflection->GetInputParameterDesc (i, &paramDesc);

				// fill out input element desc
				D3D11_INPUT_ELEMENT_DESC elementDesc;
				elementDesc.SemanticName = paramDesc.SemanticName;
				elementDesc.SemanticIndex = paramDesc.SemanticIndex;
				elementDesc.InputSlot = 0;
				elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
				elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				elementDesc.InstanceDataStepRate = 0;

				// determine DXGI format
				if (paramDesc.Mask == 1)
				{
					if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32_UINT;
					else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32_SINT;
					else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
				}
				else if (paramDesc.Mask <= 3)
				{
					if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
					else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
					else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
				}
				else if (paramDesc.Mask <= 7)
				{
					if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
					else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
					else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
				}
				else if (paramDesc.Mask <= 15)
				{
					if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
					else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
					else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
				}

				//save element desc
				inputLayoutDesc.push_back (elementDesc);
			}

			ID3D11InputLayout* pInputLayout = nullptr;

			// Try to create Input Layout
			HRESULT hr = device->CreateInputLayout (&inputLayoutDesc[0], inputLayoutDesc.size (), vertexBlob->GetBufferPointer (), vertexBlob->GetBufferSize (), inputlayout);

			//Free allocation shader reflection memory
			pVertexShaderReflection->Release ();
			//return &pInputLayout;
		}

		ShaderResource::~ShaderResource ()
		{
			// empty
		}

		template< class ShaderClass >
		ShaderClass* ShaderResource::LoadShader (const FString& fileName, const std::string& entryPoint, const std::string& _profile)
		{
			ID3DBlob* pShaderBlob = nullptr;
			ID3DBlob* pErrorBlob = nullptr;
			ShaderClass* pShader = nullptr;

			std::string profile = _profile;
			if (profile == "latest")
			{
				profile = GetLatestProfile<ShaderClass> ();
			}

			UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
		#if _DEBUG
			flags |= D3DCOMPILE_DEBUG;
		#endif

			HRESULT hr = D3DCompileFromFile (fileName.c_str (), nullptr,
				D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint.c_str(), profile.c_str (),
				flags, 0, &pShaderBlob, &pErrorBlob);

			if (FAILED (hr))
			{
				if (pErrorBlob)
				{
					std::string errorMessage = (char*)pErrorBlob->GetBufferPointer ();
					OutputDebugStringA (errorMessage.c_str ());

					SafeRelease (pShaderBlob);
					SafeRelease (pErrorBlob);
				}

				return false;
			}

			pShader = CreateShader<ShaderClass> (pShaderBlob, nullptr);

			SafeRelease (pShaderBlob);
			SafeRelease (pErrorBlob);

			return pShader;
		}

		bool ShaderResource::Shutdown ()
		{
			SafeRelease (m_vertexShader);
			SafeRelease (m_pixelShader);
			SafeRelease (m_inputLayout);

			return true;
		}

		ID3D11VertexShader* ShaderResource::GetVertexShader () const
		{
			return m_vertexShader;
		}

		ID3D11PixelShader*	ShaderResource::GetPixelShader () const
		{
			return m_pixelShader;
		}

		ID3D11InputLayout*	ShaderResource::GetInputLayout () const
		{
			return m_inputLayout;
		}

		template<>
		std::string ShaderResource::GetLatestProfile<ID3D11VertexShader> ()
		{
			ID3D11Device* device = SystemManager::Instance ()->GetGraphicsSystem ()->GetDevice ();
			assert (device);

			// Query the current feature level:
			D3D_FEATURE_LEVEL featureLevel = device->GetFeatureLevel ();

			switch (featureLevel)
			{
			case D3D_FEATURE_LEVEL_11_1:
			case D3D_FEATURE_LEVEL_11_0:
			{
				return "vs_5_0";
			}
			break;
			case D3D_FEATURE_LEVEL_10_1:
			{
				return "vs_4_1";
			}
			break;
			case D3D_FEATURE_LEVEL_10_0:
			{
				return "vs_4_0";
			}
			break;
			case D3D_FEATURE_LEVEL_9_3:
			{
				return "vs_4_0_level_9_3";
			}
			break;
			case D3D_FEATURE_LEVEL_9_2:
			case D3D_FEATURE_LEVEL_9_1:
			{
				return "vs_4_0_level_9_1";
			}
			break;
			} // switch( featureLevel )

			return "";
		}

		template<>
		std::string ShaderResource::GetLatestProfile<ID3D11PixelShader> ()
		{
			ID3D11Device* device = SystemManager::Instance ()->GetGraphicsSystem ()->GetDevice ();
			assert (device);

			// Query the current feature level:
			D3D_FEATURE_LEVEL featureLevel = device->GetFeatureLevel ();
			switch (featureLevel)
			{
			case D3D_FEATURE_LEVEL_11_1:
			case D3D_FEATURE_LEVEL_11_0:
			{
				return "ps_5_0";
			}
			break;
			case D3D_FEATURE_LEVEL_10_1:
			{
				return "ps_4_1";
			}
			break;
			case D3D_FEATURE_LEVEL_10_0:
			{
				return "ps_4_0";
			}
			break;
			case D3D_FEATURE_LEVEL_9_3:
			{
				return "ps_4_0_level_9_3";
			}
			break;
			case D3D_FEATURE_LEVEL_9_2:
			case D3D_FEATURE_LEVEL_9_1:
			{
				return "ps_4_0_level_9_1";
			}
			break;
			}
			return "";
		}

		template<>
		ID3D11VertexShader* ShaderResource::CreateShader<ID3D11VertexShader> (ID3DBlob* pShaderBlob, ID3D11ClassLinkage* pClassLinkage)
		{
			ID3D11Device* device = SystemManager::Instance ()->GetGraphicsSystem ()->GetDevice ();
			assert (device);
			assert (pShaderBlob);

			ID3D11VertexShader* pVertexShader = nullptr;
			device->CreateVertexShader (pShaderBlob->GetBufferPointer (), pShaderBlob->GetBufferSize (), pClassLinkage, &pVertexShader);

			return pVertexShader;
		}

		template<>
		ID3D11PixelShader* ShaderResource::CreateShader<ID3D11PixelShader> (ID3DBlob* pShaderBlob, ID3D11ClassLinkage* pClassLinkage)
		{
			ID3D11Device* device = SystemManager::Instance ()->GetGraphicsSystem ()->GetDevice ();
			assert (device);
			assert (pShaderBlob);

			ID3D11PixelShader* pPixelShader = nullptr;
			device->CreatePixelShader (pShaderBlob->GetBufferPointer (), pShaderBlob->GetBufferSize (), pClassLinkage, &pPixelShader);

			return pPixelShader;
		}
	}
}