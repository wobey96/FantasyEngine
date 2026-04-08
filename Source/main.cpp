#define GLFW_INCLUDE_NONE 
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

int main()
{
	if (!glfwInit())
	{
		abort(); 
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "FantasyEngine", nullptr, nullptr); 

	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents(); 
	}
	glfwTerminate(); 
	return 0;
}
