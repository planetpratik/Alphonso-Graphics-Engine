#include "Light.h"

using namespace glm;

namespace AlphonsoGraphicsEngine
{
	const glm::vec4 Light::White = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	Light::Light(): 
		mColor(Light::White)
	{
	}

	const vec4& Light::Color() const
	{
		return mColor;
	}

	void Light::SetColor(float r, float g, float b, float a)
	{
		SetColor(vec4(r, g, b, a));
	}

	void Light::SetColor(const vec4& color)
	{
		mColor = color;
	}
}