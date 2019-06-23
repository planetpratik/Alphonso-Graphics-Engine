#pragma once
#include "Light.h"

namespace AlphonsoGraphicsEngine
{
	class DirectionalLight : public Light
	{
	public:
		DirectionalLight();
		DirectionalLight(const DirectionalLight&) = default;
		DirectionalLight& operator=(const DirectionalLight&) = default;
		DirectionalLight(DirectionalLight&&) = default;
		DirectionalLight& operator=(DirectionalLight&&) = default;
		virtual ~DirectionalLight() = default;

		const glm::vec3& Direction() const;
		const glm::vec3& Up() const;
		const glm::vec3& Right() const;

		void ApplyRotation(const glm::mat4& transform);

	protected:
		glm::vec3 mDirection;
		glm::vec3 mUp;
		glm::vec3 mRight;

		static const glm::vec3 upVector;
		static const glm::vec3 forwardVector;
		static const glm::vec3 rightVector;
	};
}