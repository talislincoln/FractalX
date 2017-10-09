#include <FractalPCH.h>
#include <core\Engine.h>

#include "TestGame.h"

#if defined (DEBUG) | defined (_DEBUG)
int _tmain (int argc, char* argv[])
{
	UNREFERENCED_PARAMETER (argc);
	UNREFERENCED_PARAMETER (argv);

	WinMain ((HINSTANCE)GetModuleHandle (NULL), 0, 0, SW_SHOW);
}
#endif


int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER (hPrevInstance);
	UNREFERENCED_PARAMETER (lpCmdLine);

	if (!DirectX::XMVerifyCPUSupport ())
	{
		MessageBox (nullptr, TEXT ("Failed to verify DirectX Math library support."), TEXT ("Error"), MB_OK);
		return -1;
	}

#if defined (DEBUG) | defined (_DEBUG)
	HeapSetInformation (NULL, HeapEnableTerminationOnCorruption, NULL, 0);

	// Enable run-time memory leak check for debug build
	_CrtSetDbgFlag (_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetBreakAlloc(0); // the 0 should be changed to the id of the memory leak that will be shown in the debug log
#endif
	
	// the engine takes care of deleting the game
	fractal::fcore::Engine *e = new fractal::fcore::Engine (new TestGame ());
	int result = e->Run ();
	delete e;
	return result;
}