#include "Renderer.h"
#include <iostream>
#include <sstream>
#include <GL/gl.h>


namespace AlphonsoGraphicsEngine
{
	VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback)
	{
		return mPFN_vkCreateDebugReportCallbackEXT(instance, pCreateInfo, pAllocator, pCallback);
	}

	VKAPI_ATTR void VKAPI_CALL vkDestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator)
	{
		mPFN_vkDestroyDebugReportCallbackEXT(instance, callback, pAllocator);
	}

	// This Debug Function is copied from Vulkan-HPP's official documentation page.
	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugFunction(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT /*objType*/, uint64_t /*srcObject*/, size_t /*location*/, int32_t msgCode, const char* pLayerPrefix, const char* pMsg, void* /*pUserData*/)
	{
		std::ostringstream message;

		switch (flags)
		{
		case VK_DEBUG_REPORT_INFORMATION_BIT_EXT:
			message << "INFORMATION: ";
			break;
		case VK_DEBUG_REPORT_WARNING_BIT_EXT:
			message << "WARNING: ";
			break;
		case VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT:
			message << "PERFORMANCE WARNING: ";
			break;
		case VK_DEBUG_REPORT_ERROR_BIT_EXT:
			message << "ERROR: ";
			break;
		case VK_DEBUG_REPORT_DEBUG_BIT_EXT:
			message << "DEBUG: ";
			break;
		default:
			message << "Unknown Flag (" << flags << "): ";
			break;
		}
		message << "[" << pLayerPrefix << "] Code " << msgCode << " : " << pMsg;

		MessageBox(NULL, message.str().c_str(), "Alert", MB_OK);
		return VK_FALSE;
	}


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
		CreateDebugCallbacksForValidationLayers();
		CreateWindowSurface();
		SelectPhysicalDevice();
		CreateLogicalDevice();
	}

	void Renderer::Shutdown()
	{
		glfwDestroyWindow(mWindow);
		glfwTerminate();
	}

	void Renderer::CreateVulkanInstance()
	{
		// Returns implicitly enabled layers.
		 mInstanceLayerProperties = vk::enumerateInstanceLayerProperties();
		 if(!CheckValidationLayers(mValidationLayers, mInstanceLayerProperties))
		 {
			 std::cout << "Set the environment variable VK_LAYER_PATH to point to the location of your layers" << std::endl;
			 exit(1);
		 }
		// Create application & Instance Information.
		vk::ApplicationInfo applicationInfo(
			"Alphonso Engine - Vulkan",
			VK_MAKE_VERSION(1, 0, 0),
			"Vulkan-HPP",
			VK_MAKE_VERSION(1, 0, 0),
			VK_API_VERSION_1_1);

		mInstanceExtensionNames = GetRequiredExtensions();

		vk::InstanceCreateInfo instanceInfo(
			vk::InstanceCreateFlags(),
			&applicationInfo,
			mValidationLayers.size(),
			mValidationLayers.data(),
			mInstanceExtensionNames.size(),
			mInstanceExtensionNames.data());
		
		// Using Unique instance. Being UniqueInstance, it doesn't need to be explicitly destroyed.
		mVulkanInstance = vk::createInstanceUnique(instanceInfo);
	}

	void Renderer::CreateWindowSurface()
	{
		VkResult result = glfwCreateWindowSurface(*mVulkanInstance, mWindow, nullptr, &surface);
		if (result != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to Create Windows surface!");
		}

		mSurface = vk::UniqueSurfaceKHR(surface, *mVulkanInstance);

		// Iterate over each queue to learn whether it supports presenting.
		// Find a queue with present support
		// It Will be used to present the swap chain images to the windowing system
		for (size_t i = 0; i < mQueueFamilyProperties.size(); ++i)
		{
			if (mPhysicalDevices[0].getSurfaceSupportKHR(i, mSurface.get()))
			{
				presentQueueFamilyIndex = i;
			}
		}
	}

	void Renderer::SelectPhysicalDevice()
	{
		// Enumerate on Physical Devices
		mPhysicalDevices = mVulkanInstance->enumeratePhysicalDevices();
		if (mPhysicalDevices.size() == 0)
		{
			throw std::runtime_error("There is no GPU supporting Vulkan API!");
		}
		// Get QueueFamilyProperties of  first Physical Device
		mQueueFamilyProperties = mPhysicalDevices[0].getQueueFamilyProperties();
	}

	void Renderer::CreateLogicalDevice()
	{
		// Get First occurence of any Queue supporting Graphics Bits
		// std::find_if gives first element for which predicate returns true.
		graphicsQueueFamilyIndex = std::distance(mQueueFamilyProperties.begin(),
			std::find_if(mQueueFamilyProperties.begin(),
				mQueueFamilyProperties.end(),
				[](vk::QueueFamilyProperties const& queueFamilyProperties)
				{ return queueFamilyProperties.queueFlags & vk::QueueFlagBits::eGraphics; }));

		uniqueQueueFamilyIndices = { graphicsQueueFamilyIndex, presentQueueFamilyIndex };

		mFamilyIndices = { uniqueQueueFamilyIndices.begin(), uniqueQueueFamilyIndices.end() };

		// Create a Unique Device. It doesn't need to be explicitly destroyed.
		float queuePriority = 0.0f;
		for (int queueFamilyIndex : uniqueQueueFamilyIndices)
		{
			mdeviceQueueCreateInfo.push_back(vk::DeviceQueueCreateInfo{ vk::DeviceQueueCreateFlags(),
				static_cast<uint32_t>(queueFamilyIndex), 1, &queuePriority });
		}

		mDevice = mPhysicalDevices[0].createDeviceUnique(vk::DeviceCreateInfo(
			vk::DeviceCreateFlags(), mdeviceQueueCreateInfo.size(), mdeviceQueueCreateInfo.data(), 0, nullptr, mdeviceExtensions.size(), mdeviceExtensions.data()),
			nullptr, DispatchLoaderDynamic);
	}

	void Renderer::CreateSwapChain()
	{
		struct SM {
			vk::SharingMode sharingMode;
			uint32_t familyIndicesCount;
			uint32_t* familyIndicesDataPtr;
		} sharingModeUtil{ (graphicsQueueFamilyIndex != presentQueueFamilyIndex) ?
							   SM{ vk::SharingMode::eConcurrent, 2u, mFamilyIndices.data() } : SM{ vk::SharingMode::eExclusive, 0u, static_cast<uint32_t*>(nullptr) } };
		
		auto capabilities = mPhysicalDevices[0].getSurfaceCapabilitiesKHR(*mSurface);
		auto formats = mPhysicalDevices[0].getSurfaceFormatsKHR(*mSurface);

		mSwapChainImageFormat = vk::Format::eB8G8R8A8Unorm;
		mSwapChainExtent = vk::Extent2D{ DefaultScreenWidth, DefaultScreenHeight };

		uint32_t imageCount = capabilities.minImageCount + 1;
		if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount) {
			imageCount = capabilities.maxImageCount;
		}

		mSwapChainCreateInfo = vk::SwapchainCreateInfoKHR({}, mSurface.get(), imageCount, mSwapChainImageFormat,
			vk::ColorSpaceKHR::eSrgbNonlinear, mSwapChainExtent, 1, vk::ImageUsageFlagBits::eColorAttachment,
			sharingModeUtil.sharingMode, sharingModeUtil.familyIndicesCount,
			sharingModeUtil.familyIndicesDataPtr, vk::SurfaceTransformFlagBitsKHR::eIdentity,
			vk::CompositeAlphaFlagBitsKHR::eOpaque, vk::PresentModeKHR::eFifo, true, nullptr);

		mSwapChain = mDevice->createSwapchainKHRUnique(mSwapChainCreateInfo);
		mSwapChainImages = mDevice->getSwapchainImagesKHR(mSwapChain.get());
	}

	void Renderer::CreateImageViews()
	{
		mImageViews.reserve(mSwapChainImages.size());
		for (auto image : mSwapChainImages) {
			vk::ImageViewCreateInfo imageViewCreateInfo(vk::ImageViewCreateFlags(), image,
				vk::ImageViewType::e2D, mSwapChainImageFormat,
				vk::ComponentMapping{ vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG,
					vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA },
				vk::ImageSubresourceRange{ vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1 });
			mImageViews.push_back(mDevice->createImageViewUnique(imageViewCreateInfo));
		}
	}

	void Renderer::CreateRenderPass()
	{
	}

	std::vector<const char*> Renderer::GetRequiredExtensions()
	{
		uint32_t glfwExtensionCount = 0;
		auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		std::vector<const char*> allExtensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
		allExtensions.push_back("VK_EXT_debug_report");
		return allExtensions;
	}

	bool Renderer::CheckValidationLayers(std::vector<const char*> const& layers, std::vector<vk::LayerProperties> const& properties)
	{
		return std::all_of(layers.begin(), layers.end(), [&properties](const char* name)
			{
				return std::find_if(properties.begin(), properties.end(), [&name](vk::LayerProperties const& property) {
					return strcmp(property.layerName, name) == 0;
					}) != properties.end();
			});
	}

	// To use any Extension functions ( Suffixed with EXT ), we can't use Static Loaders.
	// We need to Create Dynamic Dispatch Loader & pass it as a last argument to EXT functions.
	void Renderer::CreateDebugCallbacksForValidationLayers()
	{
		DispatchLoaderDynamic = *mVulkanInstance;
		mPFN_vkCreateDebugReportCallbackEXT = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(mVulkanInstance->getProcAddr("vkCreateDebugReportCallbackEXT"));
		if (!mPFN_vkCreateDebugReportCallbackEXT)
		{
			std::cout << "GetInstanceProcAddr: Unable to find vkCreateDebugReportCallbackEXT function." << std::endl;
			exit(1);
		}
		mPFN_vkDestroyDebugReportCallbackEXT = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(mVulkanInstance->getProcAddr("vkDestroyDebugReportCallbackEXT"));
		if (!mPFN_vkDestroyDebugReportCallbackEXT)
		{
			std::cout << "GetInstanceProcAddr: Unable to find vkDestroyDebugReportCallbackEXT function." << std::endl;
			exit(1);
		}
		// We are passing Dynamic Loader as a last parameter ( Which is optional if we aren't using Extensions )
		auto mDebugReportCallback = mVulkanInstance->createDebugReportCallbackEXTUnique(vk::DebugReportCallbackCreateInfoEXT(vk::DebugReportFlagBitsEXT::eError | vk::DebugReportFlagBitsEXT::eWarning, DebugFunction), nullptr, DispatchLoaderDynamic);
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