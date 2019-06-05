#pragma once

// This line is very important to compile code for 32-bit platforms Without adding explicit casts 
// As Vulkan-Hpp does not enable implicit conversion for 32-bit platforms by default.
#define  VULKAN_HPP_TYPESAFE_CONVERSION 1
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
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

		/*VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugReportCallbackEXT(vk::Instance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);
		VKAPI_ATTR void VKAPI_CALL vkDestroyDebugReportCallbackEXT(vk::Instance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator);
		VKAPI_ATTR VkBool32 VKAPI_CALL DebugFunction(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t srcObject, size_t location, int32_t msgCode, const char* pLayerPrefix, const char* pMsg, void* pUserData);*/
	protected:
		virtual void InitializeWindow();
		virtual void InitializeVulkan();
		virtual void Shutdown();
		void CreateVulkanInstance();
		std::vector<const char*> GetRequiredExtensions();
		bool CheckValidationLayers(std::vector<const char*>const& layers, std::vector<vk::LayerProperties> const& properties);
		
		void CreateDebugCallbacksForValidationLayers();
		
		GameClock mGameClock;
		GameTime mGameTime;

		static const int DefaultScreenWidth = 1024;
		static const int DefaultScreenHeight = 768;
		const std::vector<const char*> mValidationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};
		std::vector<const char*> mInstanceExtensionNames;
	private:
		GLFWwindow* mWindow = nullptr;
		vk::Instance mVulkanInstance;
		std::vector<vk::LayerProperties> mInstanceLayerProperties;
		vk::DebugReportCallbackEXT mDebugReportCallback;
	};

	static PFN_vkCreateDebugReportCallbackEXT  mPFN_vkCreateDebugReportCallbackEXT;
	static PFN_vkDestroyDebugReportCallbackEXT mPFN_vkDestroyDebugReportCallbackEXT;
}