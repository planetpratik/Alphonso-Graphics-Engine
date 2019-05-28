#pragma once

#include <exception>
#include <chrono>

namespace AlphonsoGraphicsEngine
{
	class GameTime;

	class GameClock final
	{
	public:
		GameClock();
		GameClock(const GameClock&) = default;
		GameClock& operator=(const GameClock&) = default;
		GameClock(GameClock&&) = default;
		GameClock& operator=(GameClock&&) = default;
		~GameClock() = default;

		const std::chrono::high_resolution_clock::time_point& StartTime() const;
		const std::chrono::high_resolution_clock::time_point& CurrentTime() const;
		const std::chrono::high_resolution_clock::time_point& LastTime() const;

		void Reset();
		void UpdateGameTime(GameTime& gameTime);

	private:
		std::chrono::high_resolution_clock::time_point mStartTime;
		std::chrono::high_resolution_clock::time_point mCurrentTime;
		std::chrono::high_resolution_clock::time_point mLastTime;
	};
}