#include "GameTime.h"

using namespace std::chrono;

namespace AlphonsoGraphicsEngine
{
	const high_resolution_clock::time_point& GameTime::CurrentTime() const
	{
		return mCurrentTime;
	}

	void GameTime::SetCurrentTime(const high_resolution_clock::time_point& currentTime)
	{
		mCurrentTime = currentTime;
	}

	const milliseconds& GameTime::TotalGameTime() const
	{
		return mTotalGameTime;
	}

	void GameTime::SetTotalGameTime(const milliseconds& totalGameTime)
	{
		mTotalGameTime = totalGameTime;
	}

	const milliseconds& GameTime::ElapsedGameTime() const
	{
		return mElapsedGameTime;
	}

	void GameTime::SetElapsedGameTime(const milliseconds& elapsedGameTime)
	{
		mElapsedGameTime = elapsedGameTime;
	}

	duration<float> GameTime::TotalGameTimeSeconds() const
	{
		return duration_cast<duration<float>>(mTotalGameTime);
	}

	duration<float> GameTime::ElapsedGameTimeSeconds() const
	{
		return duration_cast<duration<float>>(mElapsedGameTime);
	}
}