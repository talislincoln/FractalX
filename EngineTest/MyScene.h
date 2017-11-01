#pragma once

#include <scene\Scene.h>

class MyScene : public fractal::fscene::Scene
{
private:
	class fractal::fscene::GameObject* m_player;
	class fractal::fscene::GameObject* m_cube;
	class fractal::fscene::GameObject* m_cube2;

public:
	MyScene ();
	~MyScene ();

	bool Init () override;
	void Update () override;
	void Draw () const override;
	bool Shutdown () override;
};

