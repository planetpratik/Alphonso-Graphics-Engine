#pragma once
#include "Camera.h"

#define UNREFERENCED_PARAMETER(P) (P)

using namespace glm;

namespace AlphonsoGraphicsEngine
{
	const float Camera::DefaultFieldOfView = 45.0f;
	const float Camera::DefaultNearPlaneDistance = 0.1f;
	const float Camera::DefaultFarPlaneDistance = 10.0f;

	Camera::Camera(RendererC& renderer):
		mFieldOfView(DefaultFieldOfView), mAspectRatio(renderer.AspectRatio()), mNearPlaneDistance(DefaultNearPlaneDistance), mFarPlaneDistance(DefaultFarPlaneDistance),
		mPosition(), mDirection(), mUp(), mRight(), mViewMatrix(), mProjectionMatrix()
	{
	}

	Camera::Camera(float fieldOfView, float aspectRatio, float nearPlaneDistance, float farPlaneDistance) :
		mFieldOfView(fieldOfView), mAspectRatio(aspectRatio), mNearPlaneDistance(nearPlaneDistance), mFarPlaneDistance(farPlaneDistance),
		mPosition(), mDirection(), mUp(), mRight(), mViewMatrix(), mProjectionMatrix()
	{
	}

	const vec3& Camera::Position() const
	{
		return mPosition;
	}

	const vec3& Camera::Direction() const
	{
		return mDirection;
	}

	const vec3& Camera::Up() const
	{
		return mUp;
	}

	const vec3& Camera::Right() const
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

	const mat4& Camera::ViewMatrix() const
	{
		return mViewMatrix;
	}

	const mat4& Camera::ProjectionMatrix() const
	{
		return mProjectionMatrix;
	}

	mat4 Camera::ViewProjectionMatrix() const
	{
		return mViewMatrix * mProjectionMatrix;
	}

	void Camera::SetPosition(float x, float y, float z)
	{
		mPosition = vec3(x, y, z);
	}

	void Camera::SetPosition(const vec3& position)
	{
		mPosition = position;
	}

	void Camera::SetAspectRatio(float aspectRatio)
	{
		mAspectRatio = aspectRatio;
	}

	void Camera::Reset()
	{
		mPosition = vec3(2.0f, 2.0f, 2.0f);
		mDirection = vec3(-2.0f, -2.0f, -2.0f);
		mUp = vec3(0.0f, 0.0f, 1.0f);
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
		UpdateViewMatrix();
	}

	void Camera::UpdateViewMatrix()
	{
		vec3 target = mPosition + mDirection;
		mViewMatrix = lookAt(mPosition, target, mUp);
	}

	void Camera::UpdateProjectionMatrix()
	{
		mProjectionMatrix = perspective(mFieldOfView, mAspectRatio, mNearPlaneDistance, mFarPlaneDistance);
	}

	void Camera::ApplyRotation(const mat4& transform)
	{
		vec4 direction = transform * vec4(mDirection, 0.0f);
		mDirection = static_cast<vec3>(normalize(direction));

		vec4 up = transform * vec4(mUp, 0.0f);
		mUp = static_cast<vec3>(normalize(up));

		mRight = cross(mDirection, mUp);
		mUp = cross(mRight, mDirection);
	}
}