#pragma once
#include "Projector.h"

#define UNREFERENCED_PARAMETER(P) (P)

using namespace glm;

namespace AlphonsoGraphicsEngine
{
	const float Projector::DefaultFieldOfView = 360.0f;
	const float Projector::DefaultNearPlaneDistance = 0.1f;
	const float Projector::DefaultFarPlaneDistance = 10.0f;

	Projector::Projector(RendererC& renderer) :
		mFieldOfView(DefaultFieldOfView), mAspectRatio(renderer.AspectRatio()), mNearPlaneDistance(DefaultNearPlaneDistance), mFarPlaneDistance(DefaultFarPlaneDistance),
		mPosition(), mDirection(), mUp(), mRight(), mViewMatrix(), mProjectionMatrix()
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
		mPosition = vec3(0.1306f, 2.3667f, 2.1383f);
		mDirection = vec3(-0.0292, -0.8150f, -0.5786f);
		mUp = vec3(0.0f, 0.0f, 1.0f);
		mRight = vec3(1.0, 0.0f, 0.0f);
	}

	void Projector::Initialize()
	{
		UpdateProjectionMatrix();
		Reset();
	}

	void Projector::Update(const GameTime& gameTime)
	{
		UNREFERENCED_PARAMETER(gameTime);
		UpdateViewMatrix();
	}

	void Projector::UpdateViewMatrix()
	{
		vec3 target = mPosition + mDirection;
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