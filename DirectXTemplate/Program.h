#pragma once
#include <Windows.h>
namespace Program {

	//Pump messages in from the OS until the user quits the program. 
	//This also renders the graphics.
	HRESULT RunMessageLoop(HINSTANCE hInstance);
}