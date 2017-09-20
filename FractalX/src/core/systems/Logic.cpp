#include <FractalPCH.h>
#include <core\systems\Logic.h>
#include <core\AbstractGame.h>

namespace fractal
{
	namespace fcore
	{
		Logic::Logic () :
			System(SystemType::LOGIC_SYSTEM),
			m_game(nullptr)
		{
			
		}

		Logic::~Logic ()
		{

		}

		bool Logic::Init ()
		{
			//assert (m_game);

			//if (!m_game->Init ())
			//	return false;

			return true;
		}

		void Logic::Update ()
		{

		}

		bool Logic::Shutdown ()
		{
			return true;
		}

		void Logic::Draw () const
		{

		}
	}
}