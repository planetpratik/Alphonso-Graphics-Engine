#pragma once
// This line is very important to compile code for 32-bit platforms Without adding explicit casts 
// As Vulkan-Hpp does not enable implicit conversion for 32-bit platforms by default.
#define  VULKAN_HPP_TYPESAFE_CONVERSION 1
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <set>
#include "GameClock.h"
#include "GameTime.h"

namespace AlphonsoGraphicsEngine
{
	/// <summary>
	/// Renderer class is the heart of this Rendering Engine & encapsulates main loop.
	/// </summary>
	class Renderer
	{
	public:
		/// <summary>Default Constructor for Renderer.</summary>
		Renderer();

		/// <summary>Copy Constructor for Renderer ( Deleted ).</summary>
		/// <param name="rhs">Const reference to passed Renderer.</param>
		Renderer(const Renderer& rhs) = delete;

		/// <summary>Move Constructor for Renderer ( Deleted ).</summary>
		/// <param name="rhs">R-Value reference to passed Renderer which is to be moved.</param>
		Renderer(Renderer&& rhs) = delete;

		/// <summary>Copy Assignment Operator for Renderer ( Deleted ).</summary>
		/// <param name="rhs">Const reference to passed Renderer.</param>
		/// <returns>Reference to current Renderer.</returns>
		Renderer& operator=(const Renderer& rhs) = delete;

		/// <summary>Copy Assignment Operator for Renderer ( Deleted ).</summary>
		/// <param name="rhs">R-Value reference to passed Renderer which is to be moved.</param>
		/// <returns>Reference to current Renderer.</returns>
		Renderer& operator=(Renderer&& rhs) = delete;

		/// <summary>Defaulted Destructor for Renderer ( Virtual ).</summary>
		virtual ~Renderer() = default;

		/// <summary>Runs main loop for Renderer ( Virtual ).</summary>
		virtual void Run();

		/// <summary>Initializes Window, Vulkan Instance & Timer for Renderer ( Virtual ).</summary>
		virtual void Initialize();

		/// <summary>This method performs Non-Rendering related operation like Mouse/Keyboard movement. ( Virtual ).</summary>
		/// <param name="gameTime">Const reference to passed GameTime.</param>
		virtual void Update(const GameTime& gameTime);

		/// <summary>This method handles all Draw Calls. ( Virtual ).</summary>
		/// <param name="gameTime">Const reference to passed GameTime.</param>
		virtual void Draw(const GameTime& gameTime);

	protected:
		virtual void InitializeWindow();
		virtual void InitializeVulkan();
		virtual void Shutdown();

		std::vector<const char*> GetRequiredExtensions();
		bool CheckValidationLayers(std::vector<const char*>const& layers, std::vector<vk::LayerProperties> const& properties);
		void CreateVulkanInstance();
		void CreateDebugCallbacksForValidationLayers();
		void CreateWindowSurface();
		void SelectPhysicalDevice();
		void CreateLogicalDevice();
		void CreateSwapChain();
		void CreateImageViews();
		void CreateRenderPass();

		GameClock mGameClock;
		GameTime mGameTime;

		static const int DefaultScreenWidth = 1024;
		static const int DefaultScreenHeight = 768;
		const std::vector<const char*> mValidationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};
		const std::vector<const char*> mdeviceExtensions = { 
			"VK_KHR_swapchain"
		};
		std::vector<const char*> mInstanceExtensionNames;
	private:
		GLFWwindow* mWindow = nullptr;
		vk::UniqueInstance mVulkanInstance;
		vk::UniqueHandle<vk::Device, vk::DispatchLoaderDynamic> mDevice;
		vk::DispatchLoaderDynamic DispatchLoaderDynamic;
		VkSurfaceKHR surface;
		vk::UniqueSurfaceKHR mSurface;
		size_t graphicsQueueFamilyIndex = 0;
		size_t presentQueueFamilyIndex = 0;
		vk::UniqueSwapchainKHR mSwapChain;
		std::vector<vk::Image> mSwapChainImages;
		vk::Format mSwapChainImageFormat;
		vk::Extent2D mSwapChainExtent;
		vk::SwapchainCreateInfoKHR mSwapChainCreateInfo;
		std::set<size_t> uniqueQueueFamilyIndices;
		std::vector<vk::LayerProperties> mInstanceLayerProperties;
		std::vector<vk::PhysicalDevice> mPhysicalDevices;
		std::vector<vk::QueueFamilyProperties> mQueueFamilyProperties;
		std::vector<vk::DeviceQueueCreateInfo> mdeviceQueueCreateInfo;
		std::vector<uint32_t> mFamilyIndices;
	};

	static PFN_vkCreateDebugReportCallbackEXT  mPFN_vkCreateDebugReportCallbackEXT;
	static PFN_vkDestroyDebugReportCallbackEXT mPFN_vkDestroyDebugReportCallbackEXT;
}