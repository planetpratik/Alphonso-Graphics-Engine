#include "Renderer.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/gl.h>
#include <shaderc/shaderc.hpp>
#include <limits>
#include <unordered_map>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#pragma warning(push)
#pragma warning(disable:4201)
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#pragma warning(pop)

using namespace std::string_literals;

namespace std
{
	template<> struct hash<AlphonsoGraphicsEngine::Renderer::Vertex>
	{
		size_t operator()(AlphonsoGraphicsEngine::Renderer::Vertex const& vertex) const
		{
			return ((hash<glm::vec3>()(vertex.pos) ^ (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^ (hash<glm::vec2>()(vertex.texCoord) << 1);
		}
	};
}

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
		CreateSwapChain();
		CreateImageViews();
		CreateRenderPass();
		CreateDescriptorSetLayout();
		CreateGraphicsPipeline();
		CreateCommandPool();
		CreateFramebuffers();
		LoadModel();
		CreateVertexBuffer();
		CreateIndexBuffer();
		CreateUniformBuffers();
		CreateDescriptorPool();
		CreateDescriptorSets();
		CreateCommandBuffers();
		CreateSemaphores();
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
			 std::cout << "Set Appropriate Supported Layer by checking vulkaninfo() log." << std::endl;
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
			static_cast<uint32_t>(mValidationLayers.size()),
			mValidationLayers.data(),
			static_cast<uint32_t>(mInstanceExtensionNames.size()),
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
		graphicsQueueFamilyIndex = FindGraphicsQueueFamilyIndex(mQueueFamilyProperties);

		std::pair<uint32_t, uint32_t> graphicsAndPresentQueueFamilyIndex = FindGraphicsAndPresentQueueFamilyIndex(mPhysicalDevices[0], surface);

		presentQueueFamilyIndex = graphicsAndPresentQueueFamilyIndex.second;

		uniqueQueueFamilyIndices = { graphicsQueueFamilyIndex, presentQueueFamilyIndex };

		mFamilyIndices = { static_cast<uint32_t>(graphicsQueueFamilyIndex), static_cast<uint32_t>(presentQueueFamilyIndex) };

		//mFamilyIndices = { uniqueQueueFamilyIndices.begin(), uniqueQueueFamilyIndices.end() };

		// Create a Unique Device. It doesn't need to be explicitly destroyed.
		float queuePriority = 0.0f;
		for (size_t queueFamilyIndex : uniqueQueueFamilyIndices)
		{
			mdeviceQueueCreateInfo.push_back(vk::DeviceQueueCreateInfo{ vk::DeviceQueueCreateFlags(),
				static_cast<uint32_t>(queueFamilyIndex), 1, &queuePriority });
		}

		mDevice = mPhysicalDevices[0].createDeviceUnique(vk::DeviceCreateInfo(
			vk::DeviceCreateFlags(), static_cast<uint32_t>(mdeviceQueueCreateInfo.size()), mdeviceQueueCreateInfo.data(), 0, nullptr, static_cast<uint32_t>(mdeviceExtensions.size()), mdeviceExtensions.data()),
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

		imageCount = capabilities.minImageCount + 1;
		if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount) {
			imageCount = capabilities.maxImageCount;
		}

		mSwapChainCreateInfo = vk::SwapchainCreateInfoKHR({}, *mSurface, imageCount, mSwapChainImageFormat,
			vk::ColorSpaceKHR::eSrgbNonlinear, mSwapChainExtent, 1, vk::ImageUsageFlagBits::eColorAttachment,
			sharingModeUtil.sharingMode, sharingModeUtil.familyIndicesCount,
			sharingModeUtil.familyIndicesDataPtr, vk::SurfaceTransformFlagBitsKHR::eIdentity,
			vk::CompositeAlphaFlagBitsKHR::eOpaque, vk::PresentModeKHR::eFifo, true, nullptr);

		mSwapChain = mDevice->createSwapchainKHRUnique(mSwapChainCreateInfo, nullptr, DispatchLoaderDynamic);
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
		auto colorAttachment = vk::AttachmentDescription{ {}, mSwapChainImageFormat, vk::SampleCountFlagBits::e1,
			vk::AttachmentLoadOp::eClear, vk::AttachmentStoreOp::eStore, {}, {}, {}, vk::ImageLayout::ePresentSrcKHR };

		auto colourAttachmentRef = vk::AttachmentReference{ 0, vk::ImageLayout::eColorAttachmentOptimal };

		auto subpass = vk::SubpassDescription{ {}, vk::PipelineBindPoint::eGraphics,
			/*inAttachmentCount*/ 0, nullptr, 1, &colourAttachmentRef };

		auto subpassDependency = vk::SubpassDependency{ VK_SUBPASS_EXTERNAL, 0,
			vk::PipelineStageFlagBits::eColorAttachmentOutput, vk::PipelineStageFlagBits::eColorAttachmentOutput,
			{}, vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite };

		mRenderPass = mDevice->createRenderPassUnique(
			vk::RenderPassCreateInfo{ {}, 1, &colorAttachment, 1, &subpass, 1, &subpassDependency }, nullptr, DispatchLoaderDynamic);
		
		/*

		auto bindingDescription = Vertex::getBindingDescription();
		auto attributeDescriptions = Vertex::getAttributeDescriptions();

		vertexInputInfo.vertexBindingDescriptionCount = 1;
		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
		vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
		vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

		VkPipelineDepthStencilStateCreateInfo depthStencil = {};
		depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depthStencil.depthTestEnable = VK_TRUE;
		depthStencil.depthWriteEnable = VK_TRUE;
		depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
		depthStencil.depthBoundsTestEnable = VK_FALSE;
		depthStencil.stencilTestEnable = VK_FALSE;

	*/
	}

	void Renderer::CreateDescriptorSetLayout()
	{
	}

	void Renderer::CreateGraphicsPipeline()
	{
		std::string vertexShader = R"vertexshader(
        #version 450
        #extension GL_ARB_separate_shader_objects : enable
        out gl_PerVertex {
            vec4 gl_Position;
        };
        layout(location = 0) out vec3 fragColor;
        vec2 positions[3] = vec2[](
            vec2(0.0, -0.5),
            vec2(0.5, 0.5),
            vec2(-0.5, 0.5)
        );
        vec3 colors[3] = vec3[](
            vec3(1.0, 0.0, 0.0),
            vec3(0.0, 1.0, 0.0),
            vec3(0.0, 0.0, 1.0)
        );
        void main() {
            gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
            fragColor = colors[gl_VertexIndex];
        }
        )vertexshader";

		std::string fragmentShader = R"fragmentShader(
        #version 450
        #extension GL_ARB_separate_shader_objects : enable
        layout(location = 0) in vec3 fragColor;
        layout(location = 0) out vec4 outColor;
        void main() {
            outColor = vec4(fragColor, 1.0);
        }
        )fragmentShader";

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetOptimizationLevel(shaderc_optimization_level_performance);

		// Compile GLSL Vertex Shader to SPIR-V
		shaderc::SpvCompilationResult vertShaderModule =
			compiler.CompileGlslToSpv(vertexShader, shaderc_glsl_vertex_shader, "vertex shader", options);
		if (vertShaderModule.GetCompilationStatus() != shaderc_compilation_status_success)
		{
			std::cerr << vertShaderModule.GetErrorMessage();
		}
		auto vertShaderCode = std::vector<uint32_t>{ vertShaderModule.cbegin(), vertShaderModule.cend() };
		auto vertSize = std::distance(vertShaderCode.begin(), vertShaderCode.end());
		auto vertShaderCreateInfo =
			vk::ShaderModuleCreateInfo{ {}, vertSize * sizeof(uint32_t), vertShaderCode.data() };

		mVertexShaderModule = mDevice->createShaderModuleUnique(vertShaderCreateInfo, nullptr, DispatchLoaderDynamic);

		// Compile GLSL Fragment Shader to SPIR-V
		shaderc::SpvCompilationResult fragShaderModule = compiler.CompileGlslToSpv(
			fragmentShader, shaderc_glsl_fragment_shader, "fragment shader", options);

		if (fragShaderModule.GetCompilationStatus() != shaderc_compilation_status_success)
		{
			std::cerr << fragShaderModule.GetErrorMessage();
		}

		auto fragShaderCode = std::vector<uint32_t>{ fragShaderModule.cbegin(), fragShaderModule.cend() };
		auto fragSize = std::distance(fragShaderCode.begin(), fragShaderCode.end());
		auto fragShaderCreateInfo =
			vk::ShaderModuleCreateInfo{ {}, fragSize * sizeof(uint32_t), fragShaderCode.data() };

		mFragmentShaderModule = mDevice->createShaderModuleUnique(fragShaderCreateInfo, nullptr, DispatchLoaderDynamic);

		auto vertShaderStageInfo = vk::PipelineShaderStageCreateInfo
		{ {}, vk::ShaderStageFlagBits::eVertex, *mVertexShaderModule, "main" };

		auto fragShaderStageInfo = vk::PipelineShaderStageCreateInfo
		{ {}, vk::ShaderStageFlagBits::eFragment, *mFragmentShaderModule, "main" };

		auto pipelineShaderStages = std::vector<vk::PipelineShaderStageCreateInfo>{ vertShaderStageInfo, fragShaderStageInfo };

		auto vertexInputInfo = vk::PipelineVertexInputStateCreateInfo{ {}, 0u, nullptr, 0u, nullptr };

		auto inputAssembly = vk::PipelineInputAssemblyStateCreateInfo{ {}, vk::PrimitiveTopology::eTriangleList, false };

		mViewport = vk::Viewport{ 0.0f, 0.0f, static_cast<float>(mSwapChainExtent.width), static_cast<float>(mSwapChainExtent.height), 0.0f, 1.0f };

		auto scissor = vk::Rect2D{ { 0, 0 }, mSwapChainExtent };

		auto viewportState = vk::PipelineViewportStateCreateInfo{ {}, 1, &mViewport, 1, &scissor };

		auto rasterizer = vk::PipelineRasterizationStateCreateInfo{ {}, /*depthClamp*/ false,
			/*rasterizeDiscard*/ false, vk::PolygonMode::eFill, {},
			/*frontFace*/ vk::FrontFace::eCounterClockwise, {}, {}, {}, {}, 1.0f };

		auto multisampling = vk::PipelineMultisampleStateCreateInfo{ {}, vk::SampleCountFlagBits::e1, false, 1.0 };

		auto colorBlendAttachment = vk::PipelineColorBlendAttachmentState{ {}, /*srcCol*/ vk::BlendFactor::eOne,
			/*dstCol*/ vk::BlendFactor::eZero, /*colBlend*/ vk::BlendOp::eAdd,
			/*srcAlpha*/ vk::BlendFactor::eOne, /*dstAlpha*/ vk::BlendFactor::eZero,
			/*alphaBlend*/ vk::BlendOp::eAdd,
			vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA };

		auto colorBlending = vk::PipelineColorBlendStateCreateInfo
		{ {}, /*logicOpEnable=*/false, vk::LogicOp::eCopy, /*attachmentCount=*/1, /*colourAttachments=*/&colorBlendAttachment };

		auto pipelineLayout = mDevice->createPipelineLayoutUnique({}, nullptr, DispatchLoaderDynamic);

		auto pipelineCreateInfo = vk::GraphicsPipelineCreateInfo{ {}, 2, pipelineShaderStages.data(),
			&vertexInputInfo, &inputAssembly, nullptr, &viewportState, &rasterizer, &multisampling,
			nullptr, &colorBlending, nullptr, *pipelineLayout, *mRenderPass, 0 };

		mPipeline = mDevice->createGraphicsPipelineUnique({}, pipelineCreateInfo, nullptr, DispatchLoaderDynamic);
	}

	void Renderer::CreateFramebuffers()
	{
		mFrameBuffers.resize(imageCount);
		for (size_t i = 0; i < mImageViews.size(); i++)
		{
			mFrameBuffers[i] = mDevice->createFramebufferUnique(vk::FramebufferCreateInfo
				{ {}, mRenderPass.get(), 1, &(*mImageViews[i]), mSwapChainExtent.width, mSwapChainExtent.height, 1 }, nullptr, DispatchLoaderDynamic);
		}
	}

	void Renderer::LoadModel()
	{
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string warn, err;

		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, MODEL_PATH.c_str())) {
			throw std::runtime_error(warn + err);
		}

		std::unordered_map<Vertex, uint32_t> uniqueVertices = {};

		for (const auto& shape : shapes) {
			for (const auto& index : shape.mesh.indices) {
				Vertex vertex = {};

				vertex.pos = {
					attrib.vertices[3 * index.vertex_index + 0],
					attrib.vertices[3 * index.vertex_index + 1],
					attrib.vertices[3 * index.vertex_index + 2]
				};

				vertex.texCoord = {
					attrib.texcoords[2 * index.texcoord_index + 0],
					1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
				};

				vertex.color = { 1.0f, 1.0f, 1.0f };

				if (uniqueVertices.count(vertex) == 0) {
					uniqueVertices[vertex] = static_cast<uint32_t>(mVertices.size());
					mVertices.push_back(vertex);
				}

				mIndices.push_back(uniqueVertices[vertex]);
			}
		}
	}

	void Renderer::CreateVertexBuffer()
	{
		void* data;
		graphicsQueueFamilyIndexUnsignedInt = static_cast<uint32_t>(graphicsQueueFamilyIndex);

		// Setup Vertex Buffer Size
		vk::DeviceSize vertexBufferSize = static_cast<vk::DeviceSize>(sizeof(mVertices[0])) * static_cast<vk::DeviceSize>(mVertices.size());


		// Create Vertex Buffer
		stagingBuffers.vertices.buffer = mDevice->createBufferUnique(
			vk::BufferCreateInfo(vk::BufferCreateFlags(), vertexBufferSize,
				vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferSrc, vk::SharingMode::eExclusive,
				1U, &graphicsQueueFamilyIndexUnsignedInt), nullptr, DispatchLoaderDynamic);

		vk::MemoryRequirements memRequirements;
		memRequirements = mDevice->getBufferMemoryRequirements(*stagingBuffers.vertices.buffer);

		// Request a host visible memory type that can be used to copy our data do
		// Also request it to be coherent, so that writes are visible to the GPU right after unmapping the buffer
		stagingBuffers.vertices.memory = mDevice->allocateMemoryUnique(
			vk::MemoryAllocateInfo(memRequirements.size, FindMemoryType(memRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent)), nullptr, DispatchLoaderDynamic);

		// Map and copy
		data = mDevice->mapMemory(*stagingBuffers.vertices.memory, 0, memRequirements.size, vk::MemoryMapFlags());
		memcpy(data, mVertices.data(), static_cast<size_t>(vertexBufferSize));
		mDevice->unmapMemory(*stagingBuffers.vertices.memory);
		mDevice->bindBufferMemory(*stagingBuffers.vertices.buffer, *stagingBuffers.vertices.memory, 0);

		// Create a device local buffer to which the (host local) vertex data will be copied and which will be used for rendering
		VertexBufferOnGPU.buffer = mDevice->createBufferUnique(
			vk::BufferCreateInfo(vk::BufferCreateFlags(), vertexBufferSize,
				vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst, vk::SharingMode::eExclusive,
				1, &graphicsQueueFamilyIndexUnsignedInt), nullptr, DispatchLoaderDynamic);

		memRequirements = mDevice->getBufferMemoryRequirements(*VertexBufferOnGPU.buffer);

		VertexBufferOnGPU.memory = mDevice->allocateMemoryUnique(
			vk::MemoryAllocateInfo(memRequirements.size, FindMemoryType(memRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal)), nullptr, DispatchLoaderDynamic);

		mDevice->bindBufferMemory(*VertexBufferOnGPU.buffer, *VertexBufferOnGPU.memory, 0);
	}

	void Renderer::CreateIndexBuffer()
	{
		void* data;
		graphicsQueueFamilyIndexUnsignedInt = static_cast<uint32_t>(graphicsQueueFamilyIndex);

		// Setup Index Buffer Size
		vk::DeviceSize IndexBufferSize = static_cast<vk::DeviceSize>(sizeof(mIndices[0])) * static_cast<vk::DeviceSize>(mIndices.size());

		// Create Index buffer
		// Copy index data to a buffer visible to the host (staging buffer)
		stagingBuffers.indices.buffer = mDevice->createBufferUnique(vk::BufferCreateInfo(
			vk::BufferCreateFlags(),IndexBufferSize,
			vk::BufferUsageFlagBits::eTransferSrc,vk::SharingMode::eExclusive,
			1,&graphicsQueueFamilyIndexUnsignedInt), nullptr, DispatchLoaderDynamic);

		vk::MemoryRequirements memRequirements;
		memRequirements = mDevice->getBufferMemoryRequirements(*stagingBuffers.indices.buffer);
		stagingBuffers.indices.memory = mDevice->allocateMemoryUnique(
			vk::MemoryAllocateInfo(memRequirements.size,FindMemoryType(memRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent)), nullptr, DispatchLoaderDynamic);

		data = mDevice->mapMemory(*stagingBuffers.indices.memory, 0, IndexBufferSize, vk::MemoryMapFlags());
		memcpy(data, mIndices.data(), static_cast<size_t>(IndexBufferSize));
		mDevice->unmapMemory(*stagingBuffers.indices.memory);
		mDevice->bindBufferMemory(*stagingBuffers.indices.buffer, *stagingBuffers.indices.memory, 0);

		// Create destination buffer with device only visibility
		IndexBufferOnGPU.buffer = mDevice->createBufferUnique(
			vk::BufferCreateInfo(vk::BufferCreateFlags(),IndexBufferSize,
				vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer, vk::SharingMode::eExclusive,
				0,nullptr), nullptr, DispatchLoaderDynamic);

		memRequirements = mDevice->getBufferMemoryRequirements(*IndexBufferOnGPU.buffer);
		IndexBufferOnGPU.memory = mDevice->allocateMemoryUnique(
			vk::MemoryAllocateInfo(memRequirements.size, FindMemoryType(memRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal)), nullptr, DispatchLoaderDynamic);

		mDevice->bindBufferMemory(*IndexBufferOnGPU.buffer, *IndexBufferOnGPU.memory, 0);
	}

	void Renderer::CreateUniformBuffers()
	{
	}

	void Renderer::CreateDescriptorPool()
	{
	}

	void Renderer::CreateDescriptorSets()
	{
	}

	void Renderer::CreateCommandPool()
	{
		mCommandPool = mDevice->createCommandPoolUnique({ {}, static_cast<uint32_t>(graphicsQueueFamilyIndex) }, nullptr, DispatchLoaderDynamic);
	}

	void Renderer::CreateCommandBuffers()
	{
		mCommandBuffers.resize(mFrameBuffers.size());

		mCommandBuffers = mDevice->allocateCommandBuffersUnique(vk::CommandBufferAllocateInfo(
				mCommandPool.get(), vk::CommandBufferLevel::ePrimary, static_cast<uint32_t>(mFrameBuffers.size())));

		mGraphicsQueue = mDevice->getQueue(static_cast<uint32_t>(graphicsQueueFamilyIndex), 0);
		mPresentQueue = mDevice->getQueue(static_cast<uint32_t>(presentQueueFamilyIndex), 0);

		for (size_t i = 0; i < mCommandBuffers.size(); i++)
		{
			auto beginInfo = vk::CommandBufferBeginInfo{};
			beginInfo.flags = vk::CommandBufferUsageFlagBits::eSimultaneousUse;
			mCommandBuffers[i]->begin(beginInfo);
			std::array<vk::ClearValue, 2> clearValues = {};

			clearValues[0].color = vk::ClearColorValue(std::array<float, 4>({ 0.0f, 0.0f, 0.0f, 1.0f }));
			clearValues[1].depthStencil = vk::ClearDepthStencilValue(1.0f, 0);
			auto renderPassBeginInfo = vk::RenderPassBeginInfo{ mRenderPass.get(), mFrameBuffers[i].get(),
				vk::Rect2D{ { 0, 0 }, mSwapChainExtent }, static_cast<uint32_t>(clearValues.size()), clearValues.data() };

			mCommandBuffers[i]->beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);
			mCommandBuffers[i]->bindPipeline(vk::PipelineBindPoint::eGraphics, mPipeline.get()); 
			//TODO: Bind Vertex Buffers
			//TODO: Bind Index Buffers
			//TODO: Bind Descriptor Sets
			// Use DrawIndexed rather than draw.
			mCommandBuffers[i]->draw(3, 1, 0, 0);
			mCommandBuffers[i]->endRenderPass();
			mCommandBuffers[i]->end();
		}
	}

	void Renderer::CreateSemaphores()
	{
		auto semaphoreCreateInfo = vk::SemaphoreCreateInfo{};
		mImageAvailableSemaphore = mDevice->createSemaphoreUnique(semaphoreCreateInfo, nullptr, DispatchLoaderDynamic);
		mRenderFinishedSemaphore = mDevice->createSemaphoreUnique(semaphoreCreateInfo, nullptr, DispatchLoaderDynamic);
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
		UNREFERENCED_PARAMETER(gameTime);
		auto imageIndex = mDevice->acquireNextImageKHR(mSwapChain.get(),
			(std::numeric_limits<uint64_t>::max)(), mImageAvailableSemaphore.get(), {});

		vk::PipelineStageFlags waitStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;

		auto submitInfo = vk::SubmitInfo{ 1, &mImageAvailableSemaphore.get(), &waitStageMask, 1,
			&mCommandBuffers[imageIndex.value].get(), 1, &mRenderFinishedSemaphore.get() };

		mGraphicsQueue.submit(submitInfo, {});

		auto presentInfo = vk::PresentInfoKHR{ 1, &mRenderFinishedSemaphore.get(), 1,
			&mSwapChain.get(), &imageIndex.value };
		mPresentQueue.presentKHR(presentInfo);

		mDevice->waitIdle();
	}

	// Utility Functions

	uint32_t Renderer::FindGraphicsQueueFamilyIndex(std::vector<vk::QueueFamilyProperties> const& queueFamilyProperties)
	{
		// get the first index into queueFamiliyProperties which supports graphics
		// std::find_if gives first element for which predicate returns true.
		size_t GraphicsQueueFamilyIndex = std::distance(queueFamilyProperties.begin(), std::find_if(queueFamilyProperties.begin(), queueFamilyProperties.end(),
			[](vk::QueueFamilyProperties const& qfp) { return qfp.queueFlags & vk::QueueFlagBits::eGraphics; }));
		assert(graphicsQueueFamilyIndex < queueFamilyProperties.size());
		return static_cast<uint32_t>(GraphicsQueueFamilyIndex);
	}

	std::pair<uint32_t, uint32_t> Renderer::FindGraphicsAndPresentQueueFamilyIndex(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR const& Surface)
	{
		std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevice.getQueueFamilyProperties();

		uint32_t GraphicsQueueFamilyIndex = FindGraphicsQueueFamilyIndex(queueFamilyProperties);
		if (physicalDevice.getSurfaceSupportKHR(GraphicsQueueFamilyIndex, Surface))
		{
			return std::make_pair(GraphicsQueueFamilyIndex, GraphicsQueueFamilyIndex);    // the first graphicsQueueFamilyIndex does also support presents
		}

		// the graphicsQueueFamilyIndex doesn't support present -> look for an other family index that supports both graphics and present
		for (size_t i = 0; i < queueFamilyProperties.size(); i++)
		{
			if ((queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eGraphics) && physicalDevice.getSurfaceSupportKHR(static_cast<uint32_t>(i), Surface))
			{
				return std::make_pair(static_cast<uint32_t>(i), static_cast<uint32_t>(i));
			}
		}

		// there's nothing like a single family index that supports both graphics and present -> look for an other family index that supports present
		for (size_t i = 0; i < queueFamilyProperties.size(); i++)
		{
			if (physicalDevice.getSurfaceSupportKHR(static_cast<uint32_t>(i), Surface))
			{
				return std::make_pair(static_cast<uint32_t>(graphicsQueueFamilyIndex), static_cast<uint32_t>(i));
			}
		}

		throw std::runtime_error("Could not find queues for both graphics or present -> terminating");
	}

	uint32_t Renderer::FindMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties)
	{
		vk::PhysicalDeviceMemoryProperties memProperties;
		memProperties = mPhysicalDevices[0].getMemoryProperties();

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
				return i;
			}
		}
		throw std::runtime_error("failed to find suitable memory type!");
	}

	/*void Renderer::CopyBuffer(vk::UniqueBuffer& srcBuffer, vk::UniqueBuffer& dstBuffer, vk::DeviceSize size) {
		vk::UniqueCommandBuffer commandBuffer = beginSingleTimeCommands();

		vk::BufferCopy copyRegion = {};
		copyRegion.size = size;
		commandBuffer->copyBuffer(srcBuffer.get(), dstBuffer.get(), 1, copyRegion);
		vkCmdCopyBuffer(*commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

		endSingleTimeCommands(commandBuffer);
	}*/
}