#pragma once

#include <scene\Scene.h>

class MyScene : public fractal::fscene::Scene
{
public:
	MyScene ();
	~MyScene ();

	bool Init () override;
	void Update () override;
	void Draw () const override;
	bool Shutdown () override;
};

