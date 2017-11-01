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
	m_player = new GameObject (__T ("First GO"));
	m_cube = new GameObject (__T ("Cube"));
	m_cube2 = new GameObject (__T ("Cube2"));
	// add the mesh component to the game object
	m_cube->AddComponent (new MeshComponent (L"quad", L"SimpleShader", L"seafloor"));
	m_cube2->AddComponent (new MeshComponent (L"quad", L"SimpleShader", L"seafloor"));
	CameraComponent* c = new CameraComponent (L"MainCamera");
	c->SetShader (s);
	m_player->AddComponent (c);

	m_player->SetPosition (DirectX::XMFLOAT3 (0.0f, 0.0f, -10.0f));

	//m_cube->SetPosition (5.0f, 0.0f, 0.0f);
	//m_cube2->SetPosition (-5.0f, 0.0f, 0.0f);
	// add GO to the scene
	AddGameObject (m_player);
	AddGameObject (m_cube);
	AddGameObject (m_cube2);

	return Scene::Init ();
}

void MyScene::Update ()
{
	DirectX::XMFLOAT3 rot = m_cube->GetRotation ();

	m_cube->Rotate (rot.x, rot.y + 0.01f, rot.z);

	rot = m_cube2->GetRotation ();

	m_cube2->Rotate (rot.x + 0.01f, rot.y, rot.z);

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
