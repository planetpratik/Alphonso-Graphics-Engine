#pragma once
// This line is very important to compile code for 32-bit platforms Without adding explicit casts 
// As Vulkan-Hpp does not enable implicit conversion for 32-bit platforms by default.
#define  VULKAN_HPP_TYPESAFE_CONVERSION 1
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <set>
//#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>
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


		struct Vertex
		{
			glm::vec3 pos;
			glm::vec3 color;
			glm::vec2 texCoord;

			static vk::VertexInputBindingDescription getBindingDescription()
			{
				vk::VertexInputBindingDescription bindingDescription = {};
				bindingDescription.binding = 0;
				bindingDescription.stride = sizeof(Vertex);
				bindingDescription.inputRate = vk::VertexInputRate::eVertex;
				return bindingDescription;
			}

			static std::array<vk::VertexInputAttributeDescription, 3> getAttributeDescriptions() {
				std::array<vk::VertexInputAttributeDescription, 3> attributeDescriptions = {};

				attributeDescriptions[0].binding = 0;
				attributeDescriptions[0].location = 0;
				attributeDescriptions[0].format = vk::Format::eR32G32B32Sfloat;
				attributeDescriptions[0].offset = offsetof(Vertex, pos);

				attributeDescriptions[1].binding = 0;
				attributeDescriptions[1].location = 1;
				attributeDescriptions[1].format = vk::Format::eR32G32B32Sfloat;
				attributeDescriptions[1].offset = offsetof(Vertex, color);

				attributeDescriptions[2].binding = 0;
				attributeDescriptions[2].location = 2;
				attributeDescriptions[2].format = vk::Format::eR32G32B32Sfloat;
				attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

				return attributeDescriptions;
			}

			bool operator==(const Vertex& other) const {
				return pos == other.pos && color == other.color && texCoord == other.texCoord;
			}
		};

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
		uint32_t FindGraphicsQueueFamilyIndex(std::vector<vk::QueueFamilyProperties> const& queueFamilyProperties);
		std::pair<uint32_t, uint32_t> FindGraphicsAndPresentQueueFamilyIndex(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR const& surface);
		void CreateSwapChain();
		void CreateImageViews();
		void CreateRenderPass();
		void CreateDescriptorSetLayout();
		void CreateGraphicsPipeline();
		void CreateCommandPool();
		void CreateFramebuffers();
		void CreateTextureImage();
		void CreateTextureImageView();
		void CreateTextureSampler();
		void LoadModel();
		void CreateVertexBuffer();
		void CreateIndexBuffer();
		void CreateUniformBuffers();
		void CreateDescriptorPool();
		void CreateDescriptorSets();
		void CreateCommandBuffers();
		void CreateSemaphores();
		void UpdateUniformBuffer(vk::ResultValue<uint32_t> imageIndex);

		uint32_t FindMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties);
		void CreateImage(uint32_t width, uint32_t height, vk::Format format, vk::ImageTiling tiling, vk::ImageUsageFlags usageFlags, vk::MemoryPropertyFlagBits properties, vk::UniqueHandle<vk::Image, vk::DispatchLoaderDynamic>& image, vk::UniqueHandle<vk::DeviceMemory, vk::DispatchLoaderDynamic>& imageMemory);
		vk::ImageView CreateImageView(vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags);
		void TransitionImageLayout(vk::UniqueHandle<vk::Image, vk::DispatchLoaderDynamic>& image, vk::Format format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout);
		vk::CommandBuffer BeginSingleTimeCommands();
		void EndSingleTimeCommands(vk::CommandBuffer commandBuffer);
		void CopyBufferToImage(vk::Buffer buffer, vk::Image image, uint32_t width, uint32_t height);
		bool Renderer::HasStencilComponent(vk::Format format);

		GameClock mGameClock;
		GameTime mGameTime;

		static const int DefaultScreenWidth = 1024;
		static const int DefaultScreenHeight = 768;
		const std::vector<const char*> mValidationLayers = {
			"VK_LAYER_LUNARG_standard_validation"
		};
		const std::vector<const char*> mdeviceExtensions = { 
			"VK_KHR_swapchain"
		};

		const std::string MODEL_PATH = "../../Assets/Models/chalet.objs";
		const std::string TEXTURE_PATH = "../../Assets/Textures/chalet.jpg";

		std::vector<const char*> mInstanceExtensionNames;
	private:
		struct StagingBuffer {
			vk::UniqueHandle<vk::DeviceMemory, vk::DispatchLoaderDynamic> memory;
			vk::UniqueHandle<vk::Buffer, vk::DispatchLoaderDynamic> buffer;
		};

		StagingBuffer TextureImageStagingBuffer;

		struct {
			StagingBuffer vertices;
			StagingBuffer indices;
		} stagingBuffers;

		struct {
			vk::UniqueHandle<vk::DeviceMemory, vk::DispatchLoaderDynamic> memory;		// Handle to the device memory for this buffer
			vk::UniqueHandle<vk::Buffer, vk::DispatchLoaderDynamic> buffer;			// Handle to the Vulkan buffer object that the memory is bound to
			vk::PipelineVertexInputStateCreateInfo inputState;
			vk::VertexInputBindingDescription inputBinding;
			std::vector<vk::VertexInputAttributeDescription> inputAttributes;
		} VertexBufferOnGPU;

		struct
		{
			vk::UniqueHandle<vk::DeviceMemory, vk::DispatchLoaderDynamic> memory;
			vk::UniqueHandle<vk::Buffer, vk::DispatchLoaderDynamic> buffer;
			uint32_t count = 0;
		} IndexBufferOnGPU;

		struct UniformBufferObject {
			alignas(16) glm::mat4 model;
			alignas(16) glm::mat4 view;
			alignas(16) glm::mat4 proj;
		};

		uint32_t imageCount = 0U;
		GLFWwindow* mWindow = nullptr;
		vk::UniqueInstance mVulkanInstance;
		vk::UniqueHandle<vk::Device, vk::DispatchLoaderDynamic> mDevice;
		vk::DispatchLoaderDynamic DispatchLoaderDynamic;
		VkSurfaceKHR surface;
		vk::UniqueSurfaceKHR mSurface;
		size_t graphicsQueueFamilyIndex = 0;
		uint32_t graphicsQueueFamilyIndexUnsignedInt = 0;
		size_t presentQueueFamilyIndex = 0;
		vk::Queue mGraphicsQueue;
		vk::Queue mPresentQueue;
		vk::Viewport mViewport;
		vk::UniqueBuffer mVertexBuffer;
		vk::UniqueDeviceMemory mVertexBufferMemory;
		vk::UniqueBuffer mIndexBuffer;
		vk::UniqueDeviceMemory mIndexBufferMemory;
		
		vk::UniqueHandle<vk::SwapchainKHR, vk::DispatchLoaderDynamic> mSwapChain;
		vk::Format mSwapChainImageFormat;
		vk::Extent2D mSwapChainExtent;
		vk::SwapchainCreateInfoKHR mSwapChainCreateInfo;
		vk::DescriptorSetLayoutBinding mUBOLayoutBinding;
		vk::DescriptorSetLayoutBinding mSamplerLayoutBinding;
		vk::UniqueHandle<vk::ShaderModule, vk::DispatchLoaderDynamic> mVertexShaderModule;
		vk::UniqueHandle<vk::ShaderModule, vk::DispatchLoaderDynamic> mFragmentShaderModule;
		vk::UniqueHandle<vk::RenderPass, vk::DispatchLoaderDynamic> mRenderPass;
		vk::UniqueHandle<vk::Pipeline, vk::DispatchLoaderDynamic> mPipeline;
		vk::UniqueHandle<vk::PipelineLayout, vk::DispatchLoaderDynamic> mPipelineLayout;
		vk::UniqueHandle<vk::CommandPool, vk::DispatchLoaderDynamic> mCommandPool;
		vk::UniqueHandle<vk::Semaphore, vk::DispatchLoaderDynamic> mImageAvailableSemaphore;
		vk::UniqueHandle<vk::Semaphore, vk::DispatchLoaderDynamic> mRenderFinishedSemaphore;
		vk::UniqueHandle<vk::DescriptorPool, vk::DispatchLoaderDynamic> mDescriptorPool;
		vk::UniqueHandle<vk::Image, vk::DispatchLoaderDynamic> mTextureImage;
		vk::UniqueHandle<vk::Sampler, vk::DispatchLoaderDynamic> mTextureSampler;
		vk::UniqueHandle<vk::DeviceMemory, vk::DispatchLoaderDynamic> mTextureImageMemory;
		vk::UniqueHandle<vk::ImageView, vk::DispatchLoaderDynamic> mTextureImageView;
		vk::UniqueHandle<vk::DescriptorSetLayout, vk::DispatchLoaderDynamic> mDescriptorSetLayout;

		std::set<size_t> uniqueQueueFamilyIndices;
		std::vector<vk::LayerProperties> mInstanceLayerProperties;
		std::vector<vk::PhysicalDevice> mPhysicalDevices;
		std::vector<vk::QueueFamilyProperties> mQueueFamilyProperties;
		std::vector<vk::DeviceQueueCreateInfo> mdeviceQueueCreateInfo;
		std::vector<uint32_t> mFamilyIndices;
		std::vector<vk::Image> mSwapChainImages;
		std::vector<vk::UniqueImageView> mImageViews;
		std::vector<vk::UniqueHandle<vk::Framebuffer, vk::DispatchLoaderDynamic>> mFrameBuffers;
		std::vector<vk::UniqueCommandBuffer> mCommandBuffers;
		std::vector<vk::UniqueDescriptorSet> mDescriptorSets;
		std::vector<vk::UniqueHandle<vk::Buffer, vk::DispatchLoaderDynamic>> mUniformBuffers;
		std::vector<vk::UniqueHandle<vk::DeviceMemory, vk::DispatchLoaderDynamic>> mUniformBuffersMemory;
		std::vector<Vertex> mVertices;
		std::vector<uint32_t> mIndices;
	};

	static PFN_vkCreateDebugReportCallbackEXT  mPFN_vkCreateDebugReportCallbackEXT;
	static PFN_vkDestroyDebugReportCallbackEXT mPFN_vkDestroyDebugReportCallbackEXT;

	


}