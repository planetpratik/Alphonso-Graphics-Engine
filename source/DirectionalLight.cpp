#pragma once
#include "DirectionalLight.h"

using namespace glm;

namespace AlphonsoGraphicsEngine
{
	const glm::vec3 DirectionalLight::upVector = glm::vec3(0.0f, 0.0f, 1.0f);
	const glm::vec3 DirectionalLight::forwardVector = glm::vec3(-2.0f, -2.0f, -2.0f);
	const glm::vec3 DirectionalLight::rightVector = glm::vec3(1.0, 0.0f, 0.0f);

	DirectionalLight::DirectionalLight() :
		Light(), mDirection(DirectionalLight::forwardVector), 
		mUp(DirectionalLight::upVector), mRight(DirectionalLight::rightVector)
	{
	}

	const vec3& DirectionalLight::Direction() const
	{
		return mDirection;
	}

	const vec3& DirectionalLight::Up() const
	{
		return mUp;
	}

	const vec3& DirectionalLight::Right() const
	{
		return mRight;
	}

	void DirectionalLight::ApplyRotation(const mat4& transform)
	{
		vec4 direction = transform * vec4(mDirection, 0.0f);
		mDirection = static_cast<vec3>(normalize(direction));

		vec4 up = transform * vec4(mUp, 0.0f);
		mUp = static_cast<vec3>(normalize(up));

		mRight = cross(mDirection, mUp);
		mUp = cross(mRight, mDirection);
	}
}

