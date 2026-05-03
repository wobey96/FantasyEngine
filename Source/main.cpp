#include "Window/Window.h"
#include "Renderer/Renderer.h"


int main()
{
	Window* basicWindow = new Window({ 800, 600 }, "FantasyEngine");  

	basicWindow->Initialize();

	Renderer* basicRenderer = new Renderer(basicWindow);

	while (basicWindow->IsVisible())
	{
		basicWindow->Run();
	}

	delete basicWindow;

	return 0;
}
