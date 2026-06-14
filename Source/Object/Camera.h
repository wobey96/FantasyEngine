#pragma once 

#include <GLFW/glfw3.h>
#include <DirectXMath.h>
#include <stdio.h>

using namespace DirectX;

class Camera
{
public:
	Camera(XMFLOAT3 pos, XMINT2 windowSize) : mPosition(pos), mWindowSize(windowSize)
	{
		mView = XMMatrixIdentity();
		mProjection = XMMatrixIdentity();
		//mSpeed = 10.0f;
		mSensitivity = 25.0f; 
	}

	void UpdateMatrix()
	{
		mView = XMMatrixLookAtLH(XMLoadFloat3(&mPosition), (XMLoadFloat3(&mOrientation) + XMLoadFloat3(&mPosition)), XMLoadFloat3(&mUp));
		mProjection = XMMatrixPerspectiveFovLH(XMConvertToRadians(74.0f), (float)mWindowSize.x / (float)mWindowSize.y, 0.1f, 100.0f);
	}

	XMMATRIX GetViewMatrix(){ return mView; }
	XMMATRIX GetProjectionMatrix() { return mProjection; }

	void HandleInput(GLFWwindow* window, float deltaTime)
	{
		printf("deltaTime: %f\n", deltaTime);  // Add this temporarily

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		{
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			{
				//printf("Pressing W Key"); 
				mPosition.x = mPosition.x + mSpeed * mOrientation.x * deltaTime;
				mPosition.y = mPosition.y + mSpeed * mOrientation.y * deltaTime;
				mPosition.z = mPosition.z + mSpeed * mOrientation.z * deltaTime;
			}

			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			{
				//printf("Pressing S Key");
				mPosition.x = mPosition.x - mSpeed * mOrientation.x * deltaTime;
				mPosition.y = mPosition.y - mSpeed * mOrientation.y * deltaTime;
				mPosition.z = mPosition.z - mSpeed * mOrientation.z * deltaTime;
			}

			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			{
				//printf("Pressing A Key");
				XMVECTOR right = XMVector3Normalize(XMVector3Cross(XMLoadFloat3(&mOrientation), XMLoadFloat3(&mUp)));
				XMFLOAT3 rightFloat; 
				XMStoreFloat3(&rightFloat, right);

				mPosition.x = mPosition.x + mSpeed * rightFloat.x * deltaTime;
				mPosition.y = mPosition.y + mSpeed * rightFloat.y * deltaTime;
				mPosition.z = mPosition.z + mSpeed * rightFloat.z * deltaTime;
			}

			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			{
				//printf("Pressing D Key");
				XMVECTOR right = XMVector3Normalize(XMVector3Cross(XMLoadFloat3(&mOrientation), XMLoadFloat3(&mUp)));
				XMFLOAT3 rightFloat;
				XMStoreFloat3(&rightFloat, right);

				mPosition.x = mPosition.x - mSpeed * rightFloat.x * deltaTime;
				mPosition.y = mPosition.y - mSpeed * rightFloat.y * deltaTime;
				mPosition.z = mPosition.z - mSpeed * rightFloat.z * deltaTime;
			}

			if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			{
				//printf("Pressing Space Bar Key");
				mPosition.x = mPosition.x + mSpeed * mUp.x * deltaTime;
				mPosition.y = mPosition.y + mSpeed * mUp.y * deltaTime;
				mPosition.z = mPosition.z + mSpeed * mUp.z * deltaTime;
			}

			if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
			{
				//printf("Pressing left ctrl Key");
				mPosition.x = mPosition.x - mSpeed * mUp.x * deltaTime;
				mPosition.y = mPosition.y - mSpeed * mUp.y * deltaTime;
				mPosition.z = mPosition.z - mSpeed * mUp.z * deltaTime;
			}

			if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			{
				//printf("Pressing left shift Key");
				mSpeed = 10.0f;
			}
			else
			{

				mSpeed = 5.0f; 
			}

			// Hides mouse cursor 
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

			// Prevents camera from jumping on the first click 
			if (firstMouse)
			{
				glfwSetCursorPos(window, (double)(mWindowSize.x / 2), (double)(mWindowSize.y / 2));
				firstMouse = false;
			}

			// Stores the coordinates of the cursor
			double mouseX; 
			double mouseY;
			// Fetches the coordinates of the cursor
			glfwGetCursorPos(window, &mouseX, &mouseY);	

			if(glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
			{
				// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen	
				// and then "transforms" them into degrees
				float rotX = mSensitivity * (float)(mouseY - (mWindowSize.y / 2)) / mWindowSize.y * deltaTime;
				float rotY = mSensitivity * (float)(-mouseX - (mWindowSize.x / 2)) / mWindowSize.x * deltaTime;

				XMFLOAT3 tempAxis;
				XMStoreFloat3(&tempAxis, XMVector3Normalize(XMVector3Cross(XMLoadFloat3(&mOrientation), XMLoadFloat3(&mUp))));
				// Calculates upcoming vertical change in the orientation
				XMFLOAT3 newOriendation = RotateVector(mOrientation, -rotX, tempAxis);

				// Decides whether or not the next vertical orientation is legal or not
				if (abs(XMVectorGetX(XMVector3AngleBetweenNormals(XMLoadFloat3(&newOriendation), XMLoadFloat3(&mUp))) - XMConvertToRadians(90.0f)) <= XMConvertToRadians(85.0f))
				{
					mOrientation = newOriendation;
				}

				// Rotates the Orientation left and right 
				mOrientation = RotateVector(mOrientation, -rotY, mUp);
			}
			
			// Sets mouse cursor to the middle of screen so that it doesn't end up roaming around
			glfwSetCursorPos(window, (mWindowSize.x / 2), (mWindowSize.y / 2)); 
		}
		else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
		{
			// Unhides cursor since camera is not looking around anymore 
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); 
			// Makes sure the next time the camera looks around it doesn't jump 
			firstMouse = true; 
		}
	}

private:
	XMFLOAT3 mPosition;
	XMFLOAT3 mOrientation = XMFLOAT3(0.0f, 0.0f, -1.0f);
	XMFLOAT3 mUp = XMFLOAT3(0.0f, 1.0f, 0.0f);

	XMINT2 mWindowSize;
	bool firstMouse = true;

	float mSpeed;
	float mSensitivity;
	
	XMMATRIX mView; 
	XMMATRIX mProjection;

	XMFLOAT3 RotateVector(XMFLOAT3 vector, float angle, XMFLOAT3 axis)
	{
		XMVECTOR xmVector = XMLoadFloat3(&vector);
		XMVECTOR xmAxis = XMLoadFloat3(&axis);
		XMMATRIX rotationMatrix = XMMatrixRotationAxis(xmAxis, XMConvertToRadians(angle));
		xmVector = XMVector3TransformCoord(xmVector, rotationMatrix);
		XMStoreFloat3(&vector, xmVector);
		return vector;
	}
	
	
};