#include "Timing.h"
#include <iostream>

namespace ge {

	FpsLimiter::FpsLimiter() { /* empty */ }


	FpsLimiter::~FpsLimiter() { /* empty */ }

	void FpsLimiter::setTargetFps(float targetFps)
	{
		maxFps = targetFps;
		targetTicks = (Uint32)(1000.f / maxFps);
	}

	void FpsLimiter::beginFrame()
	{
		startTicks = SDL_GetTicks(); // initial frame ticks
	}

	float FpsLimiter::endFrame()
	{
		calculateFps(); // changes the fps variable
		delayFps();

		return fps;
	}

	void FpsLimiter::calculateFps()
	{
		static const int NUM_SAMPLES = 10;
		static float frameTimes[NUM_SAMPLES];
		static int currFrame = 1; // frame counter

		static Uint32 prevTicks = SDL_GetTicks();

		Uint32 currTicks;
		currTicks = SDL_GetTicks();

		frameTime = (float)(currTicks - prevTicks); // difference in millisec
		frameTimes[currFrame % NUM_SAMPLES] = frameTime; // store it in the array

		prevTicks = currTicks;

		// how many samples to average
		int count;
		if (currFrame < NUM_SAMPLES) {
			count = currFrame;
		}
		else {
			count = NUM_SAMPLES;
		}
		currFrame++;

		// average the frame times
		float frameTimeAvg = 0;
		for (int i = 0; i < count; i++) {
			frameTimeAvg += frameTimes[i];
		}
		frameTimeAvg /= count;

		// calculate the fps
		if (0 == frameTimeAvg) { // not to divide by 0
			fps = 0.0f;
		}
		else {
			fps = 1000.0f / frameTimeAvg;
		}
	}

	void FpsLimiter::delayFps()
	{
		// final frame ticks
		Uint32 frameTicks = SDL_GetTicks() - startTicks;

		// delay
		if (targetTicks > frameTicks) {
			SDL_Delay(targetTicks - frameTicks);
		}
	}

}