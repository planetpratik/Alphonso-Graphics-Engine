#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "RendererC.h"

namespace AlphonsoGraphicsEngine
{
	class Projector
	{
	public:
		Projector(RendererC& renderer);
		Projector(float fieldOfView, float aspectRatio, float nearPlaneDistance, float farPlaneDistance);
		Projector(const Projector&) = delete;
		Projector& operator=(const Projector&) = delete;
		Projector(Projector&&) = delete;
		Projector& operator=(Projector&&) = delete;
		~Projector() = default;

		const glm::vec3& Position() const;
		const glm::vec3& Direction() const;
		const glm::vec3& Up() const;
		const glm::vec3& Right() const;

		float AspectRatio() const;
		float FieldOfView() const;
		float NearPlaneDistance() const;
		float FarPlaneDistance() const;

		const glm::mat4& ViewMatrix() const;
		const glm::mat4& ProjectionMatrix() const;
		glm::mat4 ViewProjectionMatrix() const;

		void SetPosition(float x, float y, float z);
		void SetPosition(const glm::vec3& position);
		void SetDirection(float x, float y, float z);
		void SetAspectRatio(float aspectRatio);

		void Reset();
		void Initialize();
		void Update(const GameTime& gameTime);
		void UpdateViewMatrix();
		void UpdateProjectionMatrix();
		void ApplyRotation(const glm::mat4& transform);

		static const float DefaultFieldOfView;
		static const float DefaultNearPlaneDistance;
		static const float DefaultFarPlaneDistance;

	protected:
		float mFieldOfView;
		float mAspectRatio;
		float mNearPlaneDistance;
		float mFarPlaneDistance;

		glm::vec3 mPosition;
		glm::vec3 mDirection;
		glm::vec3 mUp;
		glm::vec3 mRight;

		glm::mat4 mViewMatrix;
		glm::mat4 mProjectionMatrix;
	};
}