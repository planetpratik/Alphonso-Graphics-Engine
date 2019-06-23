#pragma once
#include "Camera.h"

namespace AlphonsoGraphicsEngine
{
	class FirstPersonCamera : public Camera
	{
	public:
		FirstPersonCamera(RendererC& renderer);
		FirstPersonCamera(RendererC& renderer, float fieldOfView, float aspectRatio, float nearPlaneDistance, float farPlaneDistance);
		FirstPersonCamera(const FirstPersonCamera&) = delete;
		FirstPersonCamera& operator=(const FirstPersonCamera&) = delete;
		FirstPersonCamera(FirstPersonCamera&&) = delete;
		FirstPersonCamera& operator=(FirstPersonCamera&&) = delete;
		virtual ~FirstPersonCamera() = default;

		float& MouseSensitivity();
		float& RotationRate();
		float& MovementRate();

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;

		static const float DefaultMouseSensitivity;
		static const float DefaultRotationRate;
		static const float DefaultMovementRate;
		static float deltaTime;			// Time between current frame and last frame
		static float lastFrame;			// Time of last frame

	protected:
		float mMouseSensitivity;
		float mRotationRate;
		float mMovementRate;

	private:
		double mLastCursorX;
		double mLastCursorY;
		GLFWwindow* mWindow;
	};
}