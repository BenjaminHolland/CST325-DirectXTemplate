#include "Program.h"
#include "Window.h"
#include "Graphics.h"
namespace Program {
	bool ContinueMessageLoop = true;
}
HRESULT Program::RunMessageLoop(HINSTANCE hInstance)
{
	MSG currentMessage;
	while (Program::ContinueMessageLoop) {
		if (PeekMessage(&currentMessage, NULL, 0, 0, PM_REMOVE)) {
			if (currentMessage.message==WM_QUIT) {
				Program::ContinueMessageLoop = false;
				continue;
			}
			TranslateMessage(&currentMessage);
			DispatchMessage(&currentMessage);
		}
		else {
			Graphics::Render();
		}
	}
	return S_OK;
}
