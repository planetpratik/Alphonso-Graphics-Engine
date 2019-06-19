#pragma once
#include "Camera.h"

#define UNREFERENCED_PARAMETER(P) (P)

using namespace glm;

namespace AlphonsoGraphicsEngine
{
	const float Camera::DefaultFieldOfView = 45.0f;
	const float Camera::DefaultNearPlaneDistance = 0.01f;
	const float Camera::DefaultFarPlaneDistance = 1000.0f;

	const glm::vec3& Camera::Position() const
	{
		return mPosition;
	}

	const glm::vec3& Camera::Direction() const
	{
		return mDirection;
	}

	const glm::vec3& Camera::Up() const
	{
		return mUp;
	}

	const glm::vec3& Camera::Right() const
	{
		return mRight;
	}

	float Camera::AspectRatio() const
	{
		return mAspectRatio;
	}

	float Camera::FieldOfView() const
	{
		return mFieldOfView;
	}

	float Camera::NearPlaneDistance() const
	{
		return mNearPlaneDistance;
	}

	float Camera::FarPlaneDistance() const
	{
		return mFarPlaneDistance;
	}

	const glm::mat4& Camera::ViewMatrix() const
	{
		return mViewMatrix;
	}

	const glm::mat4& Camera::ProjectionMatrix() const
	{
		return mProjectionMatrix;
	}

	glm::mat4 Camera::ViewProjectionMatrix() const
	{
		return mViewMatrix * mProjectionMatrix;
	}

	void Camera::SetPosition(float x, float y, float z)
	{
		mPosition = vec3(x, y, z);
	}

	void Camera::SetPosition(const glm::vec3& position)
	{
		mPosition = position;
	}

	void Camera::Reset()
	{
		mPosition = vec3(0.0f, 0.0f, 0.0f);
		mDirection = vec3(0.0f, 0.0f, -1.0f);
		mUp = vec3(0.0f, 1.0f, 0.0f);
		mRight = vec3(1.0, 0.0f, 0.0f);
	}

	void Camera::Initialize()
	{
		UpdateProjectionMatrix();
		Reset();
	}

	void Camera::Update(const GameTime& gameTime)
	{
		UNREFERENCED_PARAMETER(gameTime);

	}

	void Camera::UpdateViewMatrix()
	{
	}

	void Camera::UpdateProjectionMatrix()
	{
	}

	void Camera::ApplyRotation(const glm::mat4& transform)
	{
		UNREFERENCED_PARAMETER(transform);
	}
}