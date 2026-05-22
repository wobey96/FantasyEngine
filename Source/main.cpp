#include "Window/Window.h"
#include "Renderer/RendererManager.h"

#include "Object/Object.h"

#include <vector>

std::vector<VertexData> vertices =
{
	{ XMFLOAT3(-0.5f, -0.5f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
	{ XMFLOAT3(0.0f, 0.5f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
	{ XMFLOAT3(0.5f, -0.5f, 0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) }
};

std::vector<uint32_t> indicies =
{
	0, 1, 2
};

int main()
{
	// OBJECTS ARRAY
	std::vector<Object*> objects;

	// BASIC WINDOW INITIALIZATION
	Window* basicWindow = new Window({ 800, 600 }, "FantasyEngine");  
	basicWindow->Initialize();

	// BASIC RENDERER INITIALIZATION
	Renderer* basicRenderer = RendererManager::CreateRenderer(basicWindow);

	// BASIC TRIANGLE INITIALIZATION
	Object* triangle = new Object(vertices, indicies);
	objects.push_back(triangle);

	// BASIC RENDER LOOP
	while (basicWindow->IsVisible())
	{
		basicWindow->Run();
		basicRenderer->ClearColor({ 0.5f, 0.2f, 0.6f, 1.0f });
		basicRenderer->SetPipeline(); 

		for (auto& object : objects)
		{
			object->SetProps(); 
			basicRenderer->Draw(object->GetIndexCount());
		}

		basicRenderer->Present(); 
	}

	delete basicWindow;
	delete basicRenderer; 

	return 0;
}
