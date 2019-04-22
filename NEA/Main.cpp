#include "Window.h"
#include <iostream>
#include <Windows.h>

// Main called when the program starts
int main()
{
	// Initialises the program and runs it
	Window::Init();
	Window::Run();

	return 0;
}

// Main called when program starts without console
INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	return main();
}