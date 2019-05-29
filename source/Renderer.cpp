#include "Renderer.h"
#include <GL/gl.h>

namespace AlphonsoGraphicsEngine
{
	void Renderer::InitializeWindow()
	{
		glfwInit();
		// To Tell GLFW to not create OpenGL Context. ( Which is default behaviour )
		// These Hints need to be set before creation of Window.
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
		mWindow = glfwCreateWindow(DefaultScreenWidth, DefaultScreenHeight, "Alphonso Engine", nullptr, nullptr);
	}

	void Renderer::InitializeVulkan()
	{
		CreateVulkanInstance();
	}

	void Renderer::Shutdown()
	{
		glfwDestroyWindow(mWindow);
		glfwTerminate();
	}

	void Renderer::CreateVulkanInstance()
	{
		// Create application & Instance Information.
		vk::ApplicationInfo applicationInfo(
			"Alphonso Engine - Vulkan",
			VK_MAKE_VERSION(1, 0, 0),
			"Vulkan-HPP",
			VK_MAKE_VERSION(1, 0, 0),
			VK_API_VERSION_1_1);
		vk::InstanceCreateInfo instanceInfo({}, &applicationInfo);
		
		uint32_t glfwExtensionCount = 0;
		auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		instanceInfo.enabledExtensionCount = glfwExtensionCount;
		instanceInfo.ppEnabledExtensionNames = glfwExtensions;
		instanceInfo.enabledLayerCount = 0;

		// Create Unique instance. Being UniqueInstance, it doesn't need to be explicitly destroyed.
		mVulkanInstance = vk::createInstanceUnique(instanceInfo);
	}

	Renderer::Renderer()
	{
		Initialize();
	}

	void Renderer::Run()
	{
		while (!glfwWindowShouldClose(mWindow)) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glfwPollEvents();
			glfwSwapBuffers(mWindow);
			mGameClock.UpdateGameTime(mGameTime);
			Update(mGameTime);
			Draw(mGameTime);
		}
		Shutdown();
	}

	void Renderer::Initialize()
	{
		InitializeWindow();
		InitializeVulkan();
		mGameClock.Reset();
	}

	void Renderer::Update(const GameTime& gameTime)
	{
		gameTime;
	}

	void Renderer::Draw(const GameTime& gameTime)
	{
		gameTime;
	}
}