#pragma once
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <vector>
#include <set>
#include <array>
#include <optional>
#include "GameClock.h"
#include "GameTime.h"

namespace AlphonsoGraphicsEngine
{
	class FirstPersonCamera;
	class Projector;
	/// <summary>
	/// RendererC class is the heart of this Rendering Engine & encapsulates main loop.
	/// </summary>
	class RendererC
	{
	public:
		/// <summary>Default Constructor for Renderer.</summary>
		RendererC();

		/// <summary>Copy Constructor for Renderer ( Deleted ).</summary>
		/// <param name="rhs">Const reference to passed Renderer.</param>
		RendererC(const RendererC& rhs) = delete;

		/// <summary>Move Constructor for Renderer ( Deleted ).</summary>
		/// <param name="rhs">R-Value reference to passed Renderer which is to be moved.</param>
		RendererC(RendererC&& rhs) = delete;

		/// <summary>Copy Assignment Operator for Renderer ( Deleted ).</summary>
		/// <param name="rhs">Const reference to passed Renderer.</param>
		/// <returns>Reference to current Renderer.</returns>
		RendererC& operator=(const RendererC& rhs) = delete;

		/// <summary>Copy Assignment Operator for Renderer ( Deleted ).</summary>
		/// <param name="rhs">R-Value reference to passed Renderer which is to be moved.</param>
		/// <returns>Reference to current Renderer.</returns>
		RendererC& operator=(RendererC&& rhs) = delete;

		/// <summary>Defaulted Destructor for Renderer ( Virtual ).</summary>
		virtual ~RendererC() = default;

		/// <summary>Runs main loop for Renderer ( Virtual ).</summary>
		virtual void Run();

		/// <summary>Initializes Window, Vulkan Instance & Timer for Renderer ( Virtual ).</summary>
		virtual void Initialize();

		/// <summary>This method performs Non-Rendering related operation like Mouse/Keyboard movement. ( Virtual ).</summary>
		/// <param name="gameTime">Const reference to passed GameTime.</param>
		virtual void Update(const GameTime& gameTime);

		float AspectRatio() const;
		GLFWwindow* Window();
		void recreateImGuiWindow();

		RendererC* mRendererInstance;

		struct Vertex
		{
			glm::vec3 pos;
			glm::vec3 color;
			glm::vec2 texCoord;
			glm::vec3 normal;

			static VkVertexInputBindingDescription getBindingDescription()
			{
				VkVertexInputBindingDescription bindingDescription = {};
				bindingDescription.binding = 0;
				bindingDescription.stride = sizeof(Vertex);
				bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

				return bindingDescription;
			}

			static std::array<VkVertexInputAttributeDescription, 4> getAttributeDescriptions()
			{
				std::array<VkVertexInputAttributeDescription, 4> attributeDescriptions = {};

				attributeDescriptions[0].binding = 0;
				attributeDescriptions[0].location = 0;
				attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
				attributeDescriptions[0].offset = offsetof(Vertex, pos);

				attributeDescriptions[1].binding = 0;
				attributeDescriptions[1].location = 1;
				attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
				attributeDescriptions[1].offset = offsetof(Vertex, color);

				attributeDescriptions[2].binding = 0;
				attributeDescriptions[2].location = 2;
				attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
				attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

				attributeDescriptions[3].binding = 0;
				attributeDescriptions[3].location = 3;
				attributeDescriptions[3].format = VK_FORMAT_R32G32B32_SFLOAT;
				attributeDescriptions[3].offset = offsetof(Vertex, normal);

				return attributeDescriptions;
			}

			bool operator==(const Vertex& other) const
			{
				return pos == other.pos && color == other.color && texCoord == other.texCoord && normal == other.normal;
			}
		};

		bool framebufferResized = false;
		bool isImGuiWindowCreated = false;

	protected:
		void InitializeWindow();
		void InitializeImgui(float WIDTH, float HEIGHT);
		void InitializeVulkan();
		virtual void Shutdown();

		void InitializeCamera();
		void InitializeProjector();

		void mainLoop();
		void cleanupSwapChain();
		void cleanup();
		void recreateSwapChain();
		void createInstance();
		void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		void setupDebugMessenger();
		void createSurface();
		void pickPhysicalDevice();
		void createLogicalDevice();
		void createSwapChain();
		void createImageViews();
		void createRenderPass();
		void createDescriptorSetLayout();
		void createGraphicsPipeline();
		void createFramebuffers();
		void createCommandPool();
		void createMSAAColorResources();
		void createDepthResources();
		VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
		VkFormat findDepthFormat();
		bool hasStencilComponent(VkFormat format);
		void createTextureImage();
		void createTextureImageView();
		void createTextureSampler();
		VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
		void createImage(uint32_t width, uint32_t height, VkSampleCountFlagBits sampleCount, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
		void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
		void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
		void loadModel();
		void createVertexBuffer();
		void createIndexBuffer();
		void createUniformBuffers();
		void createDescriptorPool();
		void createDescriptorSets();
		void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		VkCommandBuffer beginSingleTimeCommands();
		void endSingleTimeCommands(VkCommandBuffer commandBuffer);
		void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
		uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
		void createCommandBuffers();
		void createSyncObjects();
		void updateUniformBuffer(uint32_t currentImage);
		void drawFrame();
		VkShaderModule createShaderModule(const std::vector<char>& code);
		VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
		bool isDeviceSuitable(VkPhysicalDevice device);
		bool checkDeviceExtensionSupport(VkPhysicalDevice device);
		std::vector<const char*> getRequiredExtensions();
		bool checkValidationLayerSupport();
		void ImGuiSetupWindow();
		VkSampleCountFlagBits getMaximumPossibleSampleCount();

		void createShadowMap();
		void createShadowMapSampler();
		void createShadowMapRenderPass();
		void createShadowMapFrameBuffer();
		void updateLight();
		void updateUniformBufferOffscreen();

		std::shared_ptr<AlphonsoGraphicsEngine::FirstPersonCamera> mCamera;
		std::shared_ptr<AlphonsoGraphicsEngine::Projector> mProjector;

		GameClock mGameClock;
		GameTime mGameTime;

		const int WIDTH = 1024;
		const int HEIGHT = 768;

		const std::string MODEL_PATH = "../../Assets/Models/ChaletN.objs";
		const std::string TEXTURE_PATH = "../../Assets/Textures/chalet.png";
		const std::string PROJECTED_TEXTURE_PATH = "../../Assets/Textures/ProjectedTexture.png";

		const int MAX_FRAMES_IN_FLIGHT = 2;

		const std::vector<const char*> validationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};

		const std::vector<const char*> deviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

#ifdef NDEBUG
		const bool enableValidationLayers = false;
#else
		const bool enableValidationLayers = true;
#endif


		struct QueueFamilyIndices
		{
			std::optional<uint32_t> graphicsFamily;
			std::optional<uint32_t> presentFamily;

			bool isComplete()
			{
				return graphicsFamily.has_value() && presentFamily.has_value();
			}
		};

		struct SwapChainSupportDetails
		{
			VkSurfaceCapabilitiesKHR capabilities;
			std::vector<VkSurfaceFormatKHR> formats;
			std::vector<VkPresentModeKHR> presentModes;
		};

		struct UniformBufferObject
		{
			alignas(16) glm::mat4 model;
			alignas(16) glm::mat4 view;
			alignas(16) glm::mat4 proj;
			alignas(16) glm::vec3 lightDirection;
			alignas(16) glm::vec3 pointLightPosition;
			alignas(4) glm::float32 pointLightRadius;
			alignas(16) glm::mat4 projectiveTextureMatrix;
			alignas(16) glm::mat4 WorldLightViewProjection;
		};

		struct FragmentUniformBufferObject
		{
			alignas(16) glm::vec4 ambientColor;
			alignas(16) glm::vec4 lightColor;
			alignas(16) glm::vec4 pointLightColor;
			alignas(16) glm::vec3 pointLightPosition;
			alignas(16) glm::vec3 cameraPosition;
			alignas(16) glm::vec4 specularColor;
			alignas(4) glm::float32 specularPower;
		};

		struct OffscreenUniformBufferObjectVS
		{
			alignas(16) glm::mat4 WorldLightViewProjection;
		} uboOffscreenVS;

	private:

		SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
		void RendererC::InitializeProjectedTextureScalingMatrix(uint32_t textureWidth, uint32_t textureHeight);

		GLFWwindow* window;

		VkInstance instance;
		VkDebugUtilsMessengerEXT debugMessenger;
		VkSurfaceKHR surface;

		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		VkDevice device;

		VkQueue graphicsQueue;
		VkQueue presentQueue;

		VkSwapchainKHR swapChain;
		std::vector<VkImage> swapChainImages;
		VkFormat swapChainImageFormat;
		VkExtent2D swapChainExtent;
		std::vector<VkImageView> swapChainImageViews;
		std::vector<VkFramebuffer> swapChainFramebuffers;

		VkRenderPass renderPass;
		VkRenderPass uiRenderPass;
		VkDescriptorSetLayout descriptorSetLayout;
		VkPipelineLayout pipelineLayout;
		VkPipeline graphicsPipeline;

		VkCommandPool commandPool;

		VkImage msaaColorImage;
		VkDeviceMemory msaaColorImageMemory;
		VkImageView msaaColorImageView;

		VkImage depthImage;
		VkDeviceMemory depthImageMemory;
		VkImageView depthImageView;


		VkPipeline shadowMapPipeline;
		VkPipelineLayout shadowMapPipelineLayout;
		VkDescriptorSetLayout shadowMapPipelineDescriptorSetLayout;
		VkDescriptorSet shadowMapPipelineDescriptorSet;
		VkRenderPass shadowMapRenderPass;
		VkImage shadowMapImage;
		VkDeviceMemory shadowMapImageMemory;
		VkImageView shadowMapImageView;
		VkFramebuffer shadowMapFrameBuffer;

		VkSampler shadowMapSampler;

		VkImage textureImage;
		VkDeviceMemory textureImageMemory;
		VkImageView textureImageView;
		VkSampler textureSampler;

		VkImage projectedTextureImage;
		VkDeviceMemory projectedTextureImageMemory;
		VkImageView projectedTextureImageView;
		VkSampler projectedTextureSampler;

		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		/*const std::vector<Vertex> simplePlaneVertices = {
			{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
			{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
			{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
			{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
		};

		const std::vector<uint16_t> simplePlaneIndices = {
			0, 1, 2, 2, 3, 0
		};*/


		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		VkBuffer indexBuffer;
		VkDeviceMemory indexBufferMemory;

		std::vector<VkBuffer> uniformBuffers;
		std::vector<VkDeviceMemory> uniformBuffersMemory;
		std::vector<VkBuffer> fragmentUniformBuffers;
		std::vector<VkDeviceMemory> fragmentUniformBuffersMemory;
		std::vector<VkBuffer> offscreenUniformBuffers;
		std::vector<VkDeviceMemory> offscreenUniformBuffersMemory;

		VkDescriptorPool descriptorPool;
		std::vector<VkDescriptorSet> descriptorSets;

		std::vector<VkCommandBuffer> commandBuffers;

		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> inFlightFences;
		size_t currentFrame = 0;

		VkSampleCountFlagBits MSAA_Samples = VK_SAMPLE_COUNT_1_BIT;

		glm::mat4 mProjectedTextureScalingMatrix;
		uint32_t mProjectedTextureWidth = 0;
		uint32_t mProjectedTextureHeight = 0;
		float mProjectorPosition[3] = {};
		float mProjectorDirection[3] = {};

		// Keep depth range as small as possible
		// for better shadow map precision
		float zNear = 1.0f;
		float zFar = 96.0f;

		// Depth bias (and slope) are used to avoid shadowing artefacts
		// Constant depth bias factor (always applied)
		float depthBiasConstant = 1.25f;
		// Slope depth bias factor, applied depending on polygon's slope
		float depthBiasSlope = 1.75f;

		glm::vec3 lightPos = glm::vec3(-2,-2,-2);
		float lightFOV = 45.0f;
	};
}