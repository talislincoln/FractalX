
#include "TestGame.h"
#include "MyScene.h"

TestGame::TestGame ()
{
}

TestGame::~TestGame ()
{
}

bool TestGame::Init ()
{
	//add scenes
	AddScene (new MyScene ());

	return true;
 }