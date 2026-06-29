#include "Window/Window.h"
#include "Renderer/RendererManager.h"

#include "Object/Cube.h"
#include "Object/Camera.h"
#include "Object/Timer.h"

#include <vector>


int main()
{
	// OBJECTS ARRAY
	std::vector<Object*> objects;

	// BASIC WINDOW INITIALIZATION
	Window* basicWindow = new Window({ 1280, 720 }, "FantasyEngine");  
	basicWindow->Initialize();

	// BASIC RENDERER INITIALIZATION
	Renderer* basicRenderer = RendererManager::CreateRenderer(basicWindow);

	Camera* basicCamera = new Camera(XMFLOAT3(0.0f, 0.0f, 3.0f), { basicWindow->GetSize().X, basicWindow->GetSize().Y });

	// BASIC CUBE INITIALIZATION
	Object* cube = new Cube();
	objects.push_back(cube);

	// Timer for calc delta time and elapsed time 
	Timer gameTimer; 

	// BASIC RENDER LOOP
	while (basicWindow->IsVisible())
	{
		gameTimer.Update();

		// Diplay FPS every second 
		static double fpsTimer = 0.0;
		static double avgFPS = 0.0;

		fpsTimer += gameTimer.GetDeltaTime();
		if (fpsTimer >= 1.0)
		{
			avgFPS = gameTimer.GetAverageFPS();
			printf("FPS: %f\n", avgFPS);
			fpsTimer = 0.0;
		}
		else
		{
			printf("FPS: %f\n", avgFPS); 
		}

		basicWindow->Run();
		basicRenderer->ClearColor({ 0.5f, 0.2f, 0.6f, 1.0f });

		basicCamera->HandleInput(basicWindow->GetWindowHandler(), gameTimer.GetDeltaTime());
		basicCamera->UpdateMatrix(); 

		basicRenderer->SetPipeline(); 

		for (auto& object : objects)
		{
			object->SetProps(); 
			object->UpdateMatrix(basicCamera->GetViewMatrix(), basicCamera->GetProjectionMatrix());
			basicRenderer->Draw(object->GetIndexCount());
		}

		basicRenderer->Present(); 
	}

	delete basicCamera;
	delete basicWindow;
	delete basicRenderer; 

	return 0;
}
