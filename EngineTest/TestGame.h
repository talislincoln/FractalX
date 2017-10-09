#pragma once

#include <FractalPCH.h>
#include <core\AbstractGame.h>


class TestGame : public fractal::fcore::AbstractGame
{
public:
	TestGame ();
	~TestGame ();

	bool Init () override;
	bool Shutdown () override { return true; }
};

