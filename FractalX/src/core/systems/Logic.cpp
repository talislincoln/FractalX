#include <FractalPCH.h>
#include <core\systems\Logic.h>
#include <core\AbstractGame.h>
#include <core\managers\SceneManager.h>

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
			assert (m_game);

			if (!m_game->Init ())
				return false;

			return true;
		}

		void Logic::Update ()
		{
			SceneManager::Instance ()->Update ();
		}

		bool Logic::Shutdown ()
		{
			return true;
		}

		void Logic::Draw () const
		{
			SceneManager::Instance ()->Draw ();
		}

		void Logic::SetGame (AbstractGame* game)
		{
			m_game = game;
		}
	}
}