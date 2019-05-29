#include <iostream>
#include "Renderer.h"

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

using namespace AlphonsoGraphicsEngine;

int main() 
{
	// Code for Memory Leak Detection.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	Renderer renderer;
	try
	{
		renderer.Run();
	}
	catch (const std::exception& error)
	{
		std::cerr << error.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}