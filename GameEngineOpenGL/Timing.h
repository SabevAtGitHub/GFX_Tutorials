#pragma once
#include <SDL\SDL.h>

namespace ge {
	class FpsLimiter
	{
	public:
		FpsLimiter();
		~FpsLimiter();
		/// <summary>
		/// Setting target fps and target ticks
		/// </summary>
		/// <param name="targetFps"></param>
		void setTargetFps(float targetFps);
		void beginFrame();
		/// <summary>
		/// Returning the current Fps and ending the frme
		/// </summary>
		/// <returns>Current fps</returns>
		float endFrame();

	private:
		void calculateFps();
		void delayFps();

		float maxFps;
		Uint32 startTicks; // initial frame ticks
		Uint32 targetTicks;

		float time;
		float fps;		
		float frameTime;
	};

}
