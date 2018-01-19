#include <FractalPCH.h>
#include "MyScene.h"

#include <cmath>

#include <scene\objects\GameObject.h>
#include <scene\EngineScene.h>
#include <scene\components\LightComponent.h>
#include <core\resources\MaterialResource.h>
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
	FString vertex = L"../resources/shaders/SimpleVertexShader.hlsl";
	//FString vertex = L"../bin/SimpleVertexShader_d.cso";
	FString pixel = L"../resources/shaders/SimplePixelShader.hlsl";
	//FString pixel = L"../bin/SimplePixelShader_d.cso";
#else
	FString vertex = L"../bin/SimpleVertexShader.cso";
	FString pixel = L"../bin/SimplePixelShader.cso";
#endif

	// instance of the resource manager
	ResourceManager* resourceManager = ResourceManager::Instance ();

	// create a new mesh data resource and add it to the manager
	//MeshDataResource* sphere = new fcore::MeshDataResource (FString (L"sphere"), GeometryGenerator::LoadObjFromFile ("../resources/models/sphere.obj"));
	MeshDataResource* sphere = new fcore::MeshDataResource (FString (L"sphere"), GeometryGenerator::LoadObjFromFile ("../resources/models/torus.obj"));
	//MeshDataResource* sphere = new fcore::MeshDataResource (FString (L"sphere"), GeometryGenerator::CreateSphere (3.0f, 20.0f, 20.0f));
	MeshDataResource* r = new fcore::MeshDataResource (FString (L"box"), GeometryGenerator::CreateBox (3.0f, 3.0f, 3.0f));
	// create a new shader resource and add it to the resource manager
	ShaderResource *s = new fcore::ShaderResource (L"SimpleShader", vertex, pixel, false);
	ImageResource* image = new fcore::ImageResource (L"lights", L"../resources/textures/earth.dds");

	// create game object
	m_player = new GameObject (__T ("First GO"));
	m_player->SetPosition (DirectX::XMFLOAT3 (0.0f, 0.0f, -10.0f));
	CameraComponent* c = new CameraComponent (L"MainCamera");
	c->SetShader (s);
	m_player->AddComponent (c);
	m_player->AddComponent (new fscene::LightComponent (L"MyLight", 1.0f, 0.0f, 0.0f, 1.0f));
	AddGameObject (m_player);
	fcore::MaterialResource* mr = new fcore::MaterialResource (L"material", s, 1.0f, 0.0f, 0.0f, 1.0f);
	m_sphere = new GameObject (__T ("Sphere"));
	MeshComponent* mc = new MeshComponent (L"sphere", L"SimpleShader", L"lights", mr);
	m_sphere->AddComponent (mc);
	//m_sphere->SetScate (0.1f, 0.1f, 0.1f);
	AddGameObject (m_sphere);

	m_cube = new GameObject (__T ("Cube"), m_sphere);
	m_cube->AddComponent (new MeshComponent (L"box", L"SimpleShader", L"lights", mr));
	m_cube->SetPosition (5.0f, 0.0f, 0.0f);
	
	AddGameObject (m_cube);

	return Scene::Init ();
}

void MyScene::Update ()
{
	DirectX::XMFLOAT3 rot = m_cube->GetRotation ();
	
	fractal::fcore::Clock *clock = fractal::fcore::SystemManager::Instance ()->GetClockSystem ();
	
	m_cube->Rotate (rot.x, rot.y - 0.01f, rot.z);

	rot = m_sphere->GetRotation ();

	m_sphere->SetPosition (sin (clock->TotalTime ())* 5.0f, 0.0f, 0.0f);
	
	m_sphere->Rotate (rot.x, rot.y + 0.01f, rot.z + 0.01f);

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
