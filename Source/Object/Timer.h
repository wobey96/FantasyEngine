#pragma once

#include <GLFW/glfw3.h>

class Timer
{
public:
	Timer() : mLastTime(0.0), mElapsedTime(0.0), mDeltaTime(0.0f)
	{
		mLastTime = glfwGetTime();
	}

	void Update()
	{
		double currentTime = glfwGetTime();
		if (mLastTime == 0.0)
		{
			mDeltaTime = 0.016f;
		}
		else
		{
			mDeltaTime = static_cast<float>(currentTime - mLastTime);
		}
		mLastTime = currentTime;
		mElapsedTime += mDeltaTime;

		// update average FPS every sec 
		mFrameCount++;
		mAverageFPS = mFrameCount / static_cast<float>(mElapsedTime);
	}

	float GetDeltaTime() const { return mDeltaTime; }
	double GetElapsedTime() const { return mElapsedTime; }
	float GetFPS() const { return mDeltaTime > 0.0f ? 1.0f / mDeltaTime : 0.0f; }

	void Reset()
	{
		mElapsedTime = 0.0;
		mLastTime = glfwGetTime();
	}

	float GetAverageFPS() const
	{
		return mAverageFPS;
	}

private:
	double mLastTime;
	double mElapsedTime;
	float mDeltaTime;
	float mAverageFPS = 0.0f; 
	int mFrameCount = 0; 
};