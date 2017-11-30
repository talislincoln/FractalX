#include <FractalPCH.h>
#include "MyScene.h"

#include <scene\objects\GameObject.h>
#include <scene\EngineScene.h>
#include <core\EngineCore.h>
#include <utils\GeometryGenerator.h>

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

#if defined(DEBUG) || defined(_DEBUG)  
	//FString vertex = L"../shaders/SimpleVertexShader.hlsl";
	FString vertex = L"../bin/SimpleVertexShader_d.cso";
	//FString pixel = L"../bin/SimplePixelShader.hlsl";
	FString pixel = L"../bin/SimplePixelShader_d.cso";
#else
	FString vertex = L"../bin/SimpleVertexShader.cso";
	FString pixel = L"../bin/SimplePixelShader.cso";
#endif

	// instance of the resource manager
	ResourceManager* resourceManager = ResourceManager::Instance ();

	// create a new mesh data resource and add it to the manager
	MeshDataResource* sphere = new fcore::MeshDataResource (FString (L"sphere"), GeometryGenerator::CreateSphere (3.0f, 10.0f, 10.0f));
	MeshDataResource* r = new fcore::MeshDataResource (FString (L"box"), GeometryGenerator::CreateBox (3.0f, 3.0f, 3.0f));
	// create a new shader resource and add it to the resource manager
	ShaderResource *s = new fcore::ShaderResource (L"SimpleShader", vertex, pixel);
	ImageResource* image = new fcore::ImageResource (L"seafloor", L"../bin/textures/lights.dds");

	// create game object
	m_player = new GameObject (__T ("First GO"));
	m_player->SetPosition (DirectX::XMFLOAT3 (0.0f, 0.0f, -10.0f));
	CameraComponent* c = new CameraComponent (L"MainCamera");
	c->SetShader (s);
	m_player->AddComponent (c);
	AddGameObject (m_player);

	m_cube = new GameObject (__T ("Cube"));
	m_cube->AddComponent (new MeshComponent (L"box", L"SimpleShader", L"lights"));
	m_cube->SetPosition (5.0f, 0.0f, 0.0f);
	AddGameObject (m_cube);

	m_sphere = new GameObject (__T ("Sphere"));
	//m_sphere->SetPosition (-5.0f, 0.0f, 0.0f);
	MeshComponent* mc = new MeshComponent(L"sphere", L"SimpleShader", L"lights");
	m_sphere->AddComponent (mc);
	AddGameObject (m_sphere);
	

	return Scene::Init ();
}

void MyScene::Update ()
{
	DirectX::XMFLOAT3 rot = m_cube->GetRotation ();

	m_cube->Rotate (rot.x + 0.01f, rot.y + 0.01f, rot.z + 0.01f);

	rot = m_sphere->GetRotation ();

	m_sphere->Rotate (rot.x + 0.01f, rot.y, rot.z);

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
