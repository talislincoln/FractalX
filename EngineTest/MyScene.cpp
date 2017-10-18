#include <FractalPCH.h>
#include "MyScene.h"

#include <scene\objects\GameObject.h>
#include <scene\EngineScene.h>
#include <core\EngineCore.h>

MyScene::MyScene () : fractal::fscene::Scene(__T("TestScene"))
{
}

MyScene::~MyScene ()
{
}

bool MyScene::Init ()
{
	using namespace fractal;
	using namespace fscene;
	using namespace fcore;
	
	std::vector<VertexPosColorTexture> vertices;
	vertices.emplace_back (DirectX::XMFLOAT3 (-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3 (0.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 0.0f));
	vertices.emplace_back (DirectX::XMFLOAT3 (-1.0f, +1.0f, -1.0f), DirectX::XMFLOAT3 (0.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f));
	vertices.emplace_back (DirectX::XMFLOAT3 (+1.0f, +1.0f, -1.0f), DirectX::XMFLOAT3 (1.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f));
	vertices.emplace_back (DirectX::XMFLOAT3 (+1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3 (1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 1.0f));

	std::vector<WORD> indices;
	indices.push_back (0);
	indices.push_back (1);
	indices.push_back (2);
	indices.push_back (0);
	indices.push_back (2);
	indices.push_back (3);

	// instance of the resource manager
	ResourceManager* resourceManager = ResourceManager::Instance ();

	// create a new mesh data resource and add it to the manager
	MeshDataResource* r = new fcore::MeshDataResource (FString(L"quad"), vertices, indices);
	
#if defined(DEBUG) || defined(_DEBUG)  
	FString vertex = L"../bin/SimpleVertexShader_d.cso";
	FString pixel = L"../bin/SimplePixelShader_d.cso";
#else
	FString vertex = L"../bin/SimpleVertexShader.cso";
	FString pixel = L"../bin/SimplePixelShader.cso";
#endif

	// create a new shader resource and add it to the resource manager
	ShaderResource *s = new fcore::ShaderResource (L"SimpleShader", vertex, pixel);

	ImageResource* image = new fcore::ImageResource (L"seafloor", L"../bin/textures/seafloor.dds");

	// create game object
	GameObject *go = new GameObject (__T ("First GO"));
	// add the mesh component to the game object
	go->AddComponent (new MeshComponent (L"quad", L"SimpleShader", L"seafloor"));
	// add GO to the scene
	AddGameObject (go);

	return Scene::Init ();
}

void MyScene::Update ()
{
	Scene::Update ();
}

void MyScene::Draw () const
{
	Scene::Draw ();
}

bool MyScene::Shutdown ()
{
	return Scene::Shutdown ();
}
