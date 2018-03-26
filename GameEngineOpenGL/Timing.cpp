#include "Timing.h"
#include <iostream>

namespace ge {

	FpsLimiter::FpsLimiter() { /* empty */ }


	FpsLimiter::~FpsLimiter() { /* empty */ }

	void FpsLimiter::setTargetFps(float targetFps)
	{
		m_maxFps = targetFps;
		m_targetTicks = (Uint32)(1000.f / m_maxFps);
	}

	void FpsLimiter::beginFrame()
	{
		m_startTicks = SDL_GetTicks(); // initial frame ticks
	}

	float FpsLimiter::endFrame()
	{
		calculateFps(); // changes the fps variable
		delayFps();

		return m_fps;
	}

	void FpsLimiter::calculateFps()
	{
		static const int NUM_SAMPLES = 10;
		static float frameTimes[NUM_SAMPLES];
		static int currFrame = 1; // frame counter

		static Uint32 prevTicks = SDL_GetTicks();

		Uint32 currTicks;
		currTicks = SDL_GetTicks();

		m_frameTime = (float)(currTicks - prevTicks); // difference in millisec
		frameTimes[currFrame % NUM_SAMPLES] = m_frameTime; // store it in the array

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
			m_fps = 0.0f;
		}
		else {
			m_fps = 1000.0f / frameTimeAvg;
		}
	}

	void FpsLimiter::delayFps()
	{
		// final frame ticks
		Uint32 frameTicks = SDL_GetTicks() - m_startTicks;

		// delay
		if (m_targetTicks > frameTicks) {
			SDL_Delay(m_targetTicks - frameTicks);
		}
	}

}