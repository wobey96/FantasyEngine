#include "Camera.h"

Camera::Camera(XMFLOAT3 startPosition)
{
	mPosition = startPosition;
	mFront = XMFLOAT3(0.0f, 0.0f, -1.0f);
	mUp = XMFLOAT3(0.0f, 1.0f, 0.0f);
	mRight = XMFLOAT3(1.0f, 0.0f, 0.0f);
}

Camera::Camera()
{
	mPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);
	mFront = XMFLOAT3(0.0f, 0.0f, -1.0f);
	mUp = XMFLOAT3(0.0f, 1.0f, 0.0f);
	mRight = XMFLOAT3(1.0f, 0.0f, 0.0f);
}

XMMATRIX Camera::GetViewMatrix()
{
	XMVECTOR pos = XMLoadFloat3(&mPosition); 
	XMVECTOR target = XMLoadFloat3(&mFront); 
	XMVECTOR up = XMLoadFloat3(&mUp);
	XMVECTOR right = XMLoadFloat3(&mRight);

	XMVECTOR direction = XMVectorSubtract(target, pos); // makes sends right? final - current to get the direction from current to target
	direction = XMVector3Normalize(direction);

	return XMMatrixLookToLH(pos, direction, up);
	 
}

XMMATRIX Camera::GetProjectionMatrix()
{
	return XMMatrixPerspectiveFovLH(74.0f, 800.0f/600.0f, 0.1f, 100.0f);
}

void Camera::ProcessTransformPosition(GLFWwindow* window)
{
	float cameraSpeed = 0.1f; 

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		mPosition = { mPosition.x, mPosition.y, mPosition.z + cameraSpeed }; 
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		mPosition = { mPosition.x, mPosition.y, mPosition.z - cameraSpeed };
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		mPosition = { mPosition.x - cameraSpeed, mPosition.y, mPosition.z};
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		mPosition = { mPosition.x + cameraSpeed, mPosition.y, mPosition.z };
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		mPosition = { mPosition.x, mPosition.y + cameraSpeed, mPosition.z };
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		mPosition = { mPosition.x, mPosition.y - cameraSpeed, mPosition.z };
	}
}
