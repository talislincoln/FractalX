#include <FractalPCH.h>
#include "MyScene.h"

#include <scene\objects\GameObject.h>

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

	GameObject *go = new GameObject (__T("First GO"));
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
