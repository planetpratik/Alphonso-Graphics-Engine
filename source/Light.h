#pragma once
#include <glm/glm.hpp>
#include "RendererC.h"

namespace AlphonsoGraphicsEngine
{
	class Light
	{
	public:
		Light();
		Light(const Light&) = default;
		Light& operator=(const Light&) = default;
		Light(Light&&) = default;
		Light& operator=(Light&&) = default;
		virtual ~Light() = default;

		const glm::vec4& Color() const;
		void SetColor(float r, float g, float b, float a);
		void SetColor(const glm::vec4& color);

	protected:
		glm::vec4 mColor;
		static const glm::vec4 White;
	};
}
