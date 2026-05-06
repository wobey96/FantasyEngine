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
		basicRenderer->ClearColor({ 0.0, 0.0, 1.0, 0.0 });
		basicRenderer->Present(); 
	}

	delete basicWindow;
	delete basicRenderer; 

	return 0;
}
