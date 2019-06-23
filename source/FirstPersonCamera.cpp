#include "FirstPersonCamera.h"
using namespace glm;

namespace AlphonsoGraphicsEngine
{
	const float FirstPersonCamera::DefaultRotationRate = radians(30.0f);
	const float FirstPersonCamera::DefaultMovementRate = 2.0f;
	const float FirstPersonCamera::DefaultMouseSensitivity = 9.0f;
	float FirstPersonCamera::deltaTime = 0.0f;
	float FirstPersonCamera::lastFrame = 0.0f;

	FirstPersonCamera::FirstPersonCamera(RendererC& renderer) :
		Camera(renderer), mWindow(renderer.Window()),
		mMouseSensitivity(DefaultMouseSensitivity), mRotationRate(DefaultRotationRate), mMovementRate(DefaultMovementRate),
		mLastCursorX(0.0), mLastCursorY(0.0)
	{
	}

	FirstPersonCamera::FirstPersonCamera(RendererC& renderer, float fieldOfView, float aspectRatio, float nearPlaneDistance, float farPlaneDistance) :
		Camera(fieldOfView, aspectRatio, nearPlaneDistance, farPlaneDistance), mWindow(renderer.Window()),
		mMouseSensitivity(DefaultMouseSensitivity), mRotationRate(DefaultRotationRate), mMovementRate(DefaultMovementRate),
		mLastCursorX(0.0), mLastCursorY(0.0)
	{
	}

	float& FirstPersonCamera::MouseSensitivity()
	{
		return mMouseSensitivity;
	}

	float& FirstPersonCamera::RotationRate()
	{
		return mRotationRate;
	}

	float& FirstPersonCamera::MovementRate()
	{
		return mMovementRate;
	}

	void FirstPersonCamera::Initialize()
	{
		Camera::Initialize();

		glfwGetCursorPos(mWindow, &mLastCursorX, &mLastCursorY);
	}

	void FirstPersonCamera::Update(const GameTime& gameTime)
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		vec2 movementAmount = vec2(0.0f, 0.0f);
		if (glfwGetKey(mWindow, GLFW_KEY_W))
		{
			movementAmount.y = 1.0f;
		}

		if (glfwGetKey(mWindow, GLFW_KEY_S))
		{
			movementAmount.y = -1.0f;
		}

		if (glfwGetKey(mWindow, GLFW_KEY_A))
		{
			movementAmount.x = -1.0f;
		}

		if (glfwGetKey(mWindow, GLFW_KEY_D))
		{
			movementAmount.x = 1.0f;
		}
		
		vec2 rotationAmount = vec2(0.0f, 0.0f);

		double x, y;
		glfwGetCursorPos(mWindow, &x, &y);

		if (glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_LEFT))
		{
			rotationAmount.x = static_cast<float>(mLastCursorX - x) * mMouseSensitivity;
			rotationAmount.y = static_cast<float>(mLastCursorY - y) * mMouseSensitivity;
		}

		mLastCursorX = x;
		mLastCursorY = y;

		//float elapsedTime = gameTime.ElapsedGameTimeSeconds().count();
		//vec2 rotationVector = rotationAmount * mRotationRate * elapsedTime;
		vec2 rotationVector = rotationAmount * mRotationRate * deltaTime;


		mat4 rotationMatrix = rotate(mat4(1), rotationVector.y, mRight);
		rotationMatrix = rotate(rotationMatrix, rotationVector.x, vec3(0.0f, 0.0f, 1.0f));
		ApplyRotation(rotationMatrix);

		vec2 movement = movementAmount * mMovementRate * deltaTime;
		//vec2 movement = movementAmount * mMovementRate * elapsedTime;

		vec3 strafe = mRight * movement.x;
		mPosition += strafe;

		vec3 forward = mDirection * movement.y;
		mPosition += forward;

		Camera::Update(gameTime);
	}
}
