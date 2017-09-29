#pragma once
#ifndef _GRAPHICS_H
#define _GRAPHICS_H
#include "System.h"
#include <core\interfaces\IDrawable.h>
#include <core\systems\Window.h>

#include <vector>
namespace fractal
{
	namespace fcore
	{
		

		class Graphics : public System, public IDrawable
		{
		private:
			ID3D11Device*			m_d3dDevice;
			ID3D11DeviceContext*	m_d3dImmediateContext;
			IDXGISwapChain*			m_swapChain;
			ID3D11Texture2D*		m_depthStencilBuffer;
			ID3D11RenderTargetView* m_renderTargetView;
			ID3D11DepthStencilView* m_depthStencilView;
			ID3D11DepthStencilState* g_d3dDepthStencilState = nullptr;
			ID3D11RasterizerState*  m_d3dRasterizerState;
			D3D11_VIEWPORT			m_screenViewport;
									 
			D3D_DRIVER_TYPE			m_d3dDriverType;

			UINT					m_4xMsaaQuality;
			bool					m_enable4xMsaa;

			class Window* m_window;


			// Shader resources
			enum ConstanBuffer
			{
				CB_Appliation,
				CB_Frame,
				CB_Object,
				NumConstantBuffers
			};

			ID3D11Buffer* g_d3dConstantBuffers[NumConstantBuffers];
			D3D11_VIEWPORT g_Viewport = { 0 };

			//vertex buffer data
			ID3D11InputLayout*	g_d3dInputLayout = nullptr;
			ID3D11Buffer* g_d3dVertexBuffer = nullptr;
			ID3D11Buffer* g_d3dIndexBuffer = nullptr;

			//shader data
			ID3D11VertexShader* g_d3dVertexShader = nullptr;
			ID3D11PixelShader* g_d3dPixelShader = nullptr;

			// Demo parameters
			DirectX::XMMATRIX g_WorldMatrix;
			DirectX::XMMATRIX g_ViewMatrix;
			DirectX::XMMATRIX g_ProjectionMatrix;

			// Vertex data for a colored cube.
			struct VertexPosColor
			{
				DirectX::XMFLOAT3 Position;
				DirectX::XMFLOAT3 Color;
			};

			VertexPosColor g_Vertices[3] =
			{
				{ DirectX::XMFLOAT3 (-1.0f, -1.0f, 0.0f), DirectX::XMFLOAT3 (0.0f, 1.0f, 1.0f) }, // 0
				{ DirectX::XMFLOAT3 (0.0f,  1.0f, 0.0f), DirectX::XMFLOAT3 (0.0f, 1.0f, 0.0f) }, // 1
				{ DirectX::XMFLOAT3 (1.0f,  -1.0f, 0.0f),  DirectX::XMFLOAT3 (1.0f, 1.0f, 0.0f) } // 2
			};

			WORD g_Indicies[3] =
			{
				0, 1, 2
			};

			struct mesh
			{
				std::vector<DirectX::XMFLOAT3> pos;
				std::vector<DirectX::XMFLOAT3> cols;

				//std::vector<WORD> indices;

				mesh () {}
				mesh (const std::vector<WORD>& idx, const std::vector<DirectX::XMFLOAT3>& positions, const std::vector<DirectX::XMFLOAT3>& colours) : /*indices(idx), */pos (positions), cols (colours) {}
			};
			
			template< class ShaderClass >
			ShaderClass* LoadShader (const std::wstring& fileName, const std::string& entryPoint, const std::string& profile) { 
				
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
					D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint.c_str (), profile.c_str (),
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

			bool LoadContent ()
			{
				assert (m_d3dDevice);

				mesh a;
				a.pos.emplace_back (-1.0f, -1.0f, 0.0f);
				a.pos.emplace_back (0.0f, 1.0f, 0.0f);
				a.pos.emplace_back (1.0f, -1.0f, 0.0f);
				a.pos.emplace_back (0.0f, -1.0f, 0.0f);

				a.cols.emplace_back (0.0f, 1.0f, 1.0f);
				a.cols.emplace_back (1.0f, 1.0f, 1.0f);
				a.cols.emplace_back (1.0f, 1.0f, 0.0f);
				a.cols.emplace_back (1.0f, 1.0f, 1.0f);

				std::vector<WORD> indices;
				indices.push_back (0);
				indices.push_back (1);
				indices.push_back (2);
				indices.push_back (3);


				std::vector<void*> test;
				
				for (int i = 0; i < indices.size (); ++i)
				{
					test.push_back (&a.pos[i]);
				}

				// Create an initialize the vertex buffer.
				D3D11_BUFFER_DESC vertexBufferDesc;
				ZeroMemory (&vertexBufferDesc, sizeof (D3D11_BUFFER_DESC));

				vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
				//vertexBufferDesc.ByteWidth = sizeof (VertexPosColor) * _countof (g_Vertices);
				vertexBufferDesc.ByteWidth = sizeof (a.pos.front()) * a.pos.size() + sizeof(a.cols.front()) * a.cols.size();
				vertexBufferDesc.CPUAccessFlags = 0;
				vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

				D3D11_SUBRESOURCE_DATA resourceData;
				ZeroMemory (&resourceData, sizeof (D3D11_SUBRESOURCE_DATA));
				

				resourceData.pSysMem = g_Vertices;
				//resourceData.pSysMem = &a.pos[0];




				HRESULT hr = m_d3dDevice->CreateBuffer (&vertexBufferDesc, &resourceData, &g_d3dVertexBuffer);
				if (FAILED (hr))
				{
					return false;
				}

				// Create and initialize the index buffer.
				D3D11_BUFFER_DESC indexBufferDesc;
				ZeroMemory (&indexBufferDesc, sizeof (D3D11_BUFFER_DESC));

				indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
				indexBufferDesc.ByteWidth = sizeof (indices.front()) * indices.size();
				indexBufferDesc.CPUAccessFlags = 0;
				indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
				resourceData.pSysMem = indices.data();

				hr = m_d3dDevice->CreateBuffer (&indexBufferDesc, &resourceData, &g_d3dIndexBuffer);
				if (FAILED (hr))
				{
					return false;
				}

				// Create the constant buffers for the variables defined in the vertex shader.
				D3D11_BUFFER_DESC constantBufferDesc;
				ZeroMemory (&constantBufferDesc, sizeof (D3D11_BUFFER_DESC));

				constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
				constantBufferDesc.ByteWidth = sizeof (DirectX::XMMATRIX);
				constantBufferDesc.CPUAccessFlags = 0;
				constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;

				hr = m_d3dDevice->CreateBuffer (&constantBufferDesc, nullptr, &g_d3dConstantBuffers[CB_Appliation]);
				if (FAILED (hr))
				{
					return false;
				}
				hr = m_d3dDevice->CreateBuffer (&constantBufferDesc, nullptr, &g_d3dConstantBuffers[CB_Frame]);
				if (FAILED (hr))
				{
					return false;
				}
				hr = m_d3dDevice->CreateBuffer (&constantBufferDesc, nullptr, &g_d3dConstantBuffers[CB_Object]);
				if (FAILED (hr))
				{
					return false;
				}

				// Load the shaders
				//g_d3dVertexShader = LoadShader<ID3D11VertexShader>( L"../data/shaders/SimpleVertexShader.hlsl", "SimpleVertexShader", "latest" );
				//g_d3dPixelShader = LoadShader<ID3D11PixelShader>( L"../data/shaders/SimplePixelShader.hlsl", "SimplePixelShader", "latest" );

				// Load the compiled vertex shader.
				ID3DBlob* vertexShaderBlob;
			#if _DEBUG
				LPCWSTR compiledVertexShaderObject = L"SimpleVertexShader_d.cso";
			#else
				LPCWSTR compiledVertexShaderObject = L"SimpleVertexShader.cso";
			#endif

				hr = D3DReadFileToBlob (compiledVertexShaderObject, &vertexShaderBlob);
				if (FAILED (hr))
				{
					return false;
				}

				hr = m_d3dDevice->CreateVertexShader (vertexShaderBlob->GetBufferPointer (), vertexShaderBlob->GetBufferSize (), nullptr, &g_d3dVertexShader);
				if (FAILED (hr))
				{
					return false;
				}

				// Create the input layout for the vertex shader.
				D3D11_INPUT_ELEMENT_DESC vertexLayoutDesc[] =
				{
					{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(a.pos.front()) * a.pos.size(), D3D11_INPUT_PER_VERTEX_DATA, 0 }
				};

				hr = m_d3dDevice->CreateInputLayout (vertexLayoutDesc, _countof (vertexLayoutDesc), vertexShaderBlob->GetBufferPointer (), vertexShaderBlob->GetBufferSize (), &g_d3dInputLayout);
				if (FAILED (hr))
				{
					return false;
				}

				SafeRelease (vertexShaderBlob);

				// Load the compiled pixel shader.
				ID3DBlob* pixelShaderBlob;
			#if _DEBUG
				LPCWSTR compiledPixelShaderObject = L"SimplePixelShader_d.cso";
			#else
				LPCWSTR compiledPixelShaderObject = L"SimplePixelShader.cso";
			#endif

				hr = D3DReadFileToBlob (compiledPixelShaderObject, &pixelShaderBlob);
				if (FAILED (hr))
				{
					return false;
				}

				hr = m_d3dDevice->CreatePixelShader (pixelShaderBlob->GetBufferPointer (), pixelShaderBlob->GetBufferSize (), nullptr, &g_d3dPixelShader);
				if (FAILED (hr))
				{
					return false;
				}

				SafeRelease (pixelShaderBlob);

				// Setup the projection matrix.
				RECT clientRect;
				GetClientRect (m_window->GetWindowHandle(), &clientRect);

				// Compute the exact client dimensions.
				// This is required for a correct projection matrix.
				float clientWidth = static_cast<float>(clientRect.right - clientRect.left);
				float clientHeight = static_cast<float>(clientRect.bottom - clientRect.top);

				g_ProjectionMatrix = DirectX::XMMatrixPerspectiveFovLH (DirectX::XMConvertToRadians (45.0f), clientWidth / clientHeight, 0.1f, 100.0f);

				m_d3dImmediateContext->UpdateSubresource (g_d3dConstantBuffers[CB_Appliation], 0, nullptr, &g_ProjectionMatrix, 0, 0);
				
				return true;
			}

			// Get the latest profile for the specified shader type.
			template< class ShaderClass >
			std::string GetLatestProfile ();

			template<>
			std::string GetLatestProfile<ID3D11VertexShader> ()
			{
				assert (m_d3dDevice);

				// Query the current feature level:
				D3D_FEATURE_LEVEL featureLevel = m_d3dDevice->GetFeatureLevel ();

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
			std::string GetLatestProfile<ID3D11PixelShader> ()
			{
				assert (m_d3dDevice);

				// Query the current feature level:
				D3D_FEATURE_LEVEL featureLevel = m_d3dDevice->GetFeatureLevel ();
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

			template< class ShaderClass >
			ShaderClass* CreateShader (ID3DBlob* pShaderBlob, ID3D11ClassLinkage* pClassLinkage);

			template<>
			ID3D11VertexShader* CreateShader<ID3D11VertexShader> (ID3DBlob* pShaderBlob, ID3D11ClassLinkage* pClassLinkage)
			{
				assert (m_d3dDevice);
				assert (pShaderBlob);

				ID3D11VertexShader* pVertexShader = nullptr;
				m_d3dDevice->CreateVertexShader (pShaderBlob->GetBufferPointer (), pShaderBlob->GetBufferSize (), pClassLinkage, &pVertexShader);

				return pVertexShader;
			}

			template<>
			ID3D11PixelShader* CreateShader<ID3D11PixelShader> (ID3DBlob* pShaderBlob, ID3D11ClassLinkage* pClassLinkage)
			{
				assert (m_d3dDevice);
				assert (pShaderBlob);

				ID3D11PixelShader* pPixelShader = nullptr;
				m_d3dDevice->CreatePixelShader (pShaderBlob->GetBufferPointer (), pShaderBlob->GetBufferSize (), pClassLinkage, &pPixelShader);

				return pPixelShader;
			}
		public:
			Graphics ();
			~Graphics ();

			bool Init () override;
			void Update () override;
			void Draw () const override;
			bool Shutdown () override;

			void OnResize ();

		private:

			ID3D11Buffer* mBoxVB;
			ID3D11Buffer* mBoxIB;

			ID3D11InputLayout* mInputLayout;

			DirectX::XMFLOAT4X4 mWorld;
			DirectX::XMFLOAT4X4 mView;
			DirectX::XMFLOAT4X4 mProj;

			float mTheta;
			float mPhi;
			float mRadius;
		};
	}
}
#endif // !_GRAPHICS_H


/*

template <typename... Ttypes>
struct MetaVertex
{
fmath::FTuple<Ttypes...> elements;

MetaVertex (Ttypes... elms)
{
elements = fmath::makeVertex (elms...);
}
};

MetaVertex<DirectX::XMFLOAT3, DirectX::XMFLOAT3> meta2f[3]
{
{ MetaVertex<DirectX::XMFLOAT3, DirectX::XMFLOAT3> (DirectX::XMFLOAT3 (-1.0f, -1.0f, 0.0f), DirectX::XMFLOAT3 (0.0f, 1.0f, 1.0f)) },
{ MetaVertex<DirectX::XMFLOAT3, DirectX::XMFLOAT3> (DirectX::XMFLOAT3 (0.0f,  1.0f, 0.0f), DirectX::XMFLOAT3 (0.0f, 1.0f, 0.0f)) },
{ MetaVertex<DirectX::XMFLOAT3, DirectX::XMFLOAT3> (DirectX::XMFLOAT3 (1.0f,  -1.0f, 0.0f),  DirectX::XMFLOAT3 (1.0f, 1.0f, 0.0f))}
};

MetaVertex<DirectX::XMFLOAT3> meta1f[3]
{
{ MetaVertex<DirectX::XMFLOAT3> (DirectX::XMFLOAT3 (-1.0f, -1.0f, 0.0f)) },
{ MetaVertex<DirectX::XMFLOAT3> (DirectX::XMFLOAT3 (0.0f,  1.0f, 0.0f)) },
{ MetaVertex<DirectX::XMFLOAT3> (DirectX::XMFLOAT3 (1.0f,  -1.0f, 0.0f)) }
};

auto tt = fmath::makeTuple (DirectX::XMFLOAT3 (1.0f, -1.0f, 0.0f), DirectX::XMFLOAT3 (1.0f, 1.0f, 0.0f));
tt.TupleType_t b;

//metaVertices[0] = fmath::makeVertex (DirectX::XMFLOAT3 (-1.0f, -1.0f, 0.0f), DirectX::XMFLOAT3 (0.0f, 1.0f, 1.0f));
//metaVertices[1] = fmath::makeVertex (DirectX::XMFLOAT3 (0.0f, 1.0f, 0.0f), DirectX::XMFLOAT3 (0.0f, 1.0f, 0.0f));
//metaVertices[2] = fmath::makeVertex (DirectX::XMFLOAT3 (1.0f, -1.0f, 0.0f), DirectX::XMFLOAT3 (1.0f, 1.0f, 0.0f));

*/