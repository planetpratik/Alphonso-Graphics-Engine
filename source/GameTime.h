#pragma once

#include <chrono>
#include "GameClock.h"

namespace AlphonsoGraphicsEngine
{
	class GameTime final
	{
	public:
		const std::chrono::high_resolution_clock::time_point& CurrentTime() const;
		void SetCurrentTime(const std::chrono::high_resolution_clock::time_point& currentTime);

		const std::chrono::milliseconds& TotalGameTime() const;
		void SetTotalGameTime(const std::chrono::milliseconds& totalGameTime);

		const std::chrono::milliseconds& ElapsedGameTime() const;
		void SetElapsedGameTime(const std::chrono::milliseconds& elapsedGameTime);

		std::chrono::duration<float> TotalGameTimeSeconds() const;
		std::chrono::duration<float> ElapsedGameTimeSeconds() const;

	private:
		std::chrono::high_resolution_clock::time_point mCurrentTime;
		std::chrono::milliseconds mTotalGameTime{ 0 };
		std::chrono::milliseconds mElapsedGameTime{ 0 };
	};
}