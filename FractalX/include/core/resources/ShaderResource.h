#pragma once
#ifndef _SHADERRESOURCE_H
#define _SHADERRESOURCE_H

#include <core\resources\Resource.h>

namespace fractal
{
	namespace fcore
	{
		class ShaderResource : public Resource
		{
		private:
			// Shader data
			ID3D11Buffer*			m_constantBuffers[(int)ConstanBuffer::NumConstantBuffers];
			ID3D11VertexShader*		m_vertexShader;
			ID3D11PixelShader*		m_pixelShader;

			ID3D11InputLayout*		m_inputLayout;

		public:
			ShaderResource (const FString& name, const FString& vertexPath, const FString& pixelPath, D3D11_INPUT_ELEMENT_DESC* vertexLayout = nullptr , int numElementsLayout = 0);
			~ShaderResource ();

			bool Shutdown () override;

			void UseShader () const;

			ID3D11VertexShader* GetVertexShader () const;
			ID3D11PixelShader*	GetPixelShader () const;

			ID3D11InputLayout*	GetInputLayout () const;

			ID3D11Buffer*const* GetConstantBuffers () const;

		private:
			// TODO: make this function abstract. And create a new "BasicShader"
			virtual void ConfigureConstantBuffers ();

			void CreateVertexInputLayout (ID3DBlob* vertexBlob, ID3D11InputLayout** inputlayout);

			template< class ShaderClass >
			ShaderClass* LoadShader (const FString& fileName, const std::string& entryPoint, const std::string& profile);

			// Get the latest profile for the specified shader type.
			template< class ShaderClass >
			std::string GetLatestProfile ();

			template< class ShaderClass >
			ShaderClass* CreateShader (ID3DBlob* pShaderBlob, ID3D11ClassLinkage* pClassLinkage);
		};
	}
}

#endif // !_SHADERRESOURCE_H
