#include "Window/Window.h"

int main()
{
	Window* glfwWindow = new Window({ 800, 600 }, "FantasyEngine"); 

	glfwWindow->Initialize();

	while (glfwWindow->IsVisible())
	{
		glfwWindow->Run();
	}

	delete glfwWindow;

	return 0;
}
