#pragma once
#include "Projector.h"

#define UNREFERENCED_PARAMETER(P) (P)

using namespace glm;

namespace AlphonsoGraphicsEngine
{
	const float Projector::DefaultFieldOfView = 800.0f;
	const float Projector::DefaultNearPlaneDistance = 0.1f;
	const float Projector::DefaultFarPlaneDistance = 100.0f;
	float Projector::deltaTime = 0.0f;
	float Projector::lastFrame = 0.0f;

	Projector::Projector(RendererC& renderer) :
		mFieldOfView(DefaultFieldOfView), mAspectRatio(renderer.AspectRatio()), mNearPlaneDistance(DefaultNearPlaneDistance), mFarPlaneDistance(DefaultFarPlaneDistance),
		mPosition(), mDirection(), mUp(), mRight(), mViewMatrix(), mProjectionMatrix(), mWindow(renderer.Window()), mLastCursorX(0.0), mLastCursorY(0.0)
	{
	}

	Projector::Projector(float fieldOfView, float aspectRatio, float nearPlaneDistance, float farPlaneDistance) :
		mFieldOfView(fieldOfView), mAspectRatio(aspectRatio), mNearPlaneDistance(nearPlaneDistance), mFarPlaneDistance(farPlaneDistance),
		mPosition(), mDirection(), mUp(), mRight(), mViewMatrix(), mProjectionMatrix()
	{
	}

	const vec3& Projector::Position() const
	{
		return mPosition;
	}

	const vec3& Projector::Direction() const
	{
		return mDirection;
	}

	const vec3& Projector::Up() const
	{
		return mUp;
	}

	const vec3& Projector::Right() const
	{
		return mRight;
	}

	float Projector::AspectRatio() const
	{
		return mAspectRatio;
	}

	float Projector::FieldOfView() const
	{
		return mFieldOfView;
	}

	float Projector::NearPlaneDistance() const
	{
		return mNearPlaneDistance;
	}

	float Projector::FarPlaneDistance() const
	{
		return mFarPlaneDistance;
	}

	const mat4& Projector::ViewMatrix() const
	{
		return mViewMatrix;
	}

	const mat4& Projector::ProjectionMatrix() const
	{
		return mProjectionMatrix;
	}

	mat4 Projector::ViewProjectionMatrix() const
	{
		return mViewMatrix * mProjectionMatrix;
	}

	void Projector::SetPosition(float x, float y, float z)
	{
		mPosition = vec3(x, y, z);
	}

	void Projector::SetDirection(float x, float y, float z)
	{
		mDirection = vec3(x, y, z);
	}

	void Projector::SetPosition(const vec3& position)
	{
		mPosition = position;
	}

	void Projector::SetAspectRatio(float aspectRatio)
	{
		mAspectRatio = aspectRatio;
	}

	void Projector::Reset()
	{
		//mPosition = vec3(-2.3, -1.3, 3);
		mPosition = vec3(-1.78, -0.963, 3);
		//mDirection = vec3(-2.0, 2.0, -2.0f);
		mDirection = vec3(-0.684, 0.325, -0.652f);
		mUp = vec3(0.0f, 0.0f, 1.0f);
		mRight = vec3(1.0f, 0.0f, 0.0f);
	}

	void Projector::Initialize()
	{
		UpdateProjectionMatrix();
		Reset();
	}

	void Projector::Update(const GameTime& gameTime)
	{
		UNREFERENCED_PARAMETER(gameTime);

		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		vec2 movementAmount = vec2(0.0f, 0.0f);
		if (glfwGetKey(mWindow, GLFW_KEY_I))
		{
			movementAmount.y = 1.0f;
		}

		if (glfwGetKey(mWindow, GLFW_KEY_K))
		{
			movementAmount.y = -1.0f;
		}

		if (glfwGetKey(mWindow, GLFW_KEY_J))
		{
			movementAmount.x = -1.0f;
		}

		if (glfwGetKey(mWindow, GLFW_KEY_L))
		{
			movementAmount.x = 1.0f;
		}

		vec2 rotationAmount = vec2(0.0f, 0.0f);

		double x, y;
		glfwGetCursorPos(mWindow, &x, &y);

		if (glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_RIGHT))
		{
			rotationAmount.x = static_cast<float>(mLastCursorX - x) * 4.0f;
			rotationAmount.y = static_cast<float>(mLastCursorY - y) * 4.0f;
		}

		mLastCursorX = x;
		mLastCursorY = y;

		vec2 rotationVector = rotationAmount * radians(30.0f) * deltaTime;


		mat4 rotationMatrix = rotate(mat4(1), rotationVector.y, mRight);
		rotationMatrix = rotate(rotationMatrix, rotationVector.x, vec3(0.0f, 0.0f, 1.0f));
		ApplyRotation(rotationMatrix);

		vec2 movement = movementAmount * (1.0f) * deltaTime;

		vec3 strafe = mRight * movement.x;
		mPosition += strafe;

		vec3 forward = mDirection * movement.y;
		mPosition += forward;

		UpdateViewMatrix();
	}

	void Projector::UpdateViewMatrix()
	{
		//vec3 target = mPosition + mDirection;
		vec3 target = glm::vec3(-0.113268f, -0.649047f, 0.9552f);
		//vec3 target = glm::vec3(0, 0, 0);
		mViewMatrix = lookAt(mPosition, target, mUp);
	}

	void Projector::UpdateProjectionMatrix()
	{
		mProjectionMatrix = perspective(mFieldOfView, mAspectRatio, mNearPlaneDistance, mFarPlaneDistance);
	}

	void Projector::ApplyRotation(const mat4& transform)
	{
		vec4 direction = transform * vec4(mDirection, 0.0f);
		mDirection = static_cast<vec3>(normalize(direction));

		vec4 up = transform * vec4(mUp, 0.0f);
		mUp = static_cast<vec3>(normalize(up));

		mRight = cross(mDirection, mUp);
		mUp = cross(mRight, mDirection);
	}
}