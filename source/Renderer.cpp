#include "Renderer.h"
//#include <iostream>
//#include <fstream>
//#include <sstream>
//#include <GL/gl.h>
//#include <shaderc/shaderc.hpp>
//#include <limits>
//#include <unordered_map>
//
//#define TINYOBJLOADER_IMPLEMENTATION
//#include <tiny_obj_loader.h>
//
//#pragma warning(push)
//#pragma warning(disable:4201)
//#define GLM_ENABLE_EXPERIMENTAL
//#include <glm/gtx/hash.hpp>
//#pragma warning(pop)
//
//#define STB_IMAGE_IMPLEMENTATION
//#include <stb_image.h>
//
//#define UNREFERENCED_PARAMETER(P) (P)
//
//using namespace std::string_literals;
//
//namespace std
//{
//	template<> struct hash<AlphonsoGraphicsEngine::Renderer::Vertex>
//	{
//		size_t operator()(AlphonsoGraphicsEngine::Renderer::Vertex const& vertex) const
//		{
//			return ((hash<glm::vec3>()(vertex.pos) ^ (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^ (hash<glm::vec2>()(vertex.texCoord) << 1);
//		}
//	};
//}
//
//namespace AlphonsoGraphicsEngine
//{
//	VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback)
//	{
//		return mPFN_vkCreateDebugReportCallbackEXT(instance, pCreateInfo, pAllocator, pCallback);
//	}
//
//	VKAPI_ATTR void VKAPI_CALL vkDestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator)
//	{
//		mPFN_vkDestroyDebugReportCallbackEXT(instance, callback, pAllocator);
//	}
//
//	// This Debug Function is copied from Vulkan-HPP's official documentation page.
//	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugFunction(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT /*objType*/, uint64_t /*srcObject*/, size_t /*location*/, int32_t msgCode, const char* pLayerPrefix, const char* pMsg, void* /*pUserData*/)
//	{
//		std::ostringstream message;
//
//		switch (flags)
//		{
//		case VK_DEBUG_REPORT_INFORMATION_BIT_EXT:
//			message << "INFORMATION: ";
//			break;
//		case VK_DEBUG_REPORT_WARNING_BIT_EXT:
//			message << "WARNING: ";
//			break;
//		case VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT:
//			message << "PERFORMANCE WARNING: ";
//			break;
//		case VK_DEBUG_REPORT_ERROR_BIT_EXT:
//			message << "ERROR: ";
//			break;
//		case VK_DEBUG_REPORT_DEBUG_BIT_EXT:
//			message << "DEBUG: ";
//			break;
//		default:
//			message << "Unknown Flag (" << flags << "): ";
//			break;
//		}
//		message << "[" << pLayerPrefix << "] Code " << msgCode << " : " << pMsg;
//
//		//MessageBox(NULL, message.str().c_str(), "Alert", MB_OK);
//		return VK_FALSE;
//	}
//
//
//	void Renderer::InitializeWindow()
//	{
//		glfwInit();
//		// To Tell GLFW to not create OpenGL Context. ( Which is default behaviour )
//		// These Hints need to be set before creation of Window.
//		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
//		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
//		glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
//		mWindow = glfwCreateWindow(DefaultScreenWidth, DefaultScreenHeight, "Alphonso Engine", nullptr, nullptr);
//	}
//
//	void Renderer::InitializeVulkan()
//	{
//		CreateVulkanInstance();
//		CreateDebugCallbacksForValidationLayers();
//		CreateWindowSurface();
//		SelectPhysicalDevice();
//		CreateLogicalDevice();
//		CreateSwapChain();
//		CreateImageViews();
//		CreateRenderPass();
//		CreateDescriptorSetLayout();
//		CreateGraphicsPipeline();
//		CreateCommandPool();
//		CreateDepthResources();
//		CreateFramebuffers();
//		CreateTextureImage();
//		CreateTextureImageView();
//		CreateTextureSampler();
//		LoadModel();
//		CreateVertexBuffer();
//		CreateIndexBuffer();
//		CreateUniformBuffers();
//		CreateDescriptorPool();
//		CreateDescriptorSets();
//		CreateCommandBuffers();
//		CreateSemaphores();
//	}
//
//	void Renderer::Shutdown()
//	{
//		glfwDestroyWindow(mWindow);
//		glfwTerminate();
//	}
//
//	void Renderer::CreateVulkanInstance()
//	{
//		// Returns implicitly enabled layers.
//		mInstanceLayerProperties = vk::enumerateInstanceLayerProperties();
//		if (!CheckValidationLayers(mValidationLayers, mInstanceLayerProperties))
//		{
//			std::cout << "Set Appropriate Supported Layer by checking vulkaninfo() log." << std::endl;
//			exit(1);
//		}
//		// Create application & Instance Information.
//		vk::ApplicationInfo applicationInfo(
//			"Alphonso Engine - Vulkan",
//			VK_MAKE_VERSION(1, 0, 0),
//			"Vulkan-HPP",
//			VK_MAKE_VERSION(1, 0, 0),
//			VK_API_VERSION_1_1);
//
//		mInstanceExtensionNames = GetRequiredExtensions();
//
//		vk::InstanceCreateInfo instanceInfo(
//			vk::InstanceCreateFlags(),
//			&applicationInfo,
//			static_cast<uint32_t>(mValidationLayers.size()),
//			mValidationLayers.data(),
//			static_cast<uint32_t>(mInstanceExtensionNames.size()),
//			mInstanceExtensionNames.data());
//
//		// Using Unique instance. Being UniqueInstance, it doesn't need to be explicitly destroyed.
//		mVulkanInstance = vk::createInstanceUnique(instanceInfo);
//	}
//
//	void Renderer::CreateWindowSurface()
//	{
//		VkResult result = glfwCreateWindowSurface(*mVulkanInstance, mWindow, nullptr, &surface);
//		if (result != VK_SUCCESS)
//		{
//			throw std::runtime_error("Failed to Create Windows surface!");
//		}
//
//		mSurface = vk::UniqueSurfaceKHR(surface, *mVulkanInstance);
//	}
//
//	void Renderer::SelectPhysicalDevice()
//	{
//		// Enumerate on Physical Devices
//		mPhysicalDevices = mVulkanInstance->enumeratePhysicalDevices();
//		if (mPhysicalDevices.size() == 0)
//		{
//			throw std::runtime_error("There is no GPU supporting Vulkan API!");
//		}
//		// Get QueueFamilyProperties of  first Physical Device
//		mQueueFamilyProperties = mPhysicalDevices[0].getQueueFamilyProperties();
//	}
//
//	void Renderer::CreateLogicalDevice()
//	{
//		graphicsQueueFamilyIndex = FindGraphicsQueueFamilyIndex(mQueueFamilyProperties);
//
//		std::pair<uint32_t, uint32_t> graphicsAndPresentQueueFamilyIndex = FindGraphicsAndPresentQueueFamilyIndex(mPhysicalDevices[0], surface);
//
//		presentQueueFamilyIndex = graphicsAndPresentQueueFamilyIndex.second;
//
//		uniqueQueueFamilyIndices = { graphicsQueueFamilyIndex, presentQueueFamilyIndex };
//
//		mFamilyIndices = { static_cast<uint32_t>(graphicsQueueFamilyIndex), static_cast<uint32_t>(presentQueueFamilyIndex) };
//
//		// Create a Unique Device. It doesn't need to be explicitly destroyed.
//		float queuePriority = 0.0f;
//		for (size_t queueFamilyIndex : uniqueQueueFamilyIndices)
//		{
//			mdeviceQueueCreateInfo.push_back(vk::DeviceQueueCreateInfo{ vk::DeviceQueueCreateFlags(),
//				static_cast<uint32_t>(queueFamilyIndex), 1, &queuePriority });
//		}
//
//		vk::PhysicalDeviceFeatures deviceFeatures = {};
//		deviceFeatures.samplerAnisotropy = VK_TRUE;
//
//		mDevice = mPhysicalDevices[0].createDeviceUnique(vk::DeviceCreateInfo(
//			vk::DeviceCreateFlags(), static_cast<uint32_t>(mdeviceQueueCreateInfo.size()), mdeviceQueueCreateInfo.data(), 0, nullptr, static_cast<uint32_t>(mdeviceExtensions.size()), mdeviceExtensions.data(), &deviceFeatures),
//			nullptr, DispatchLoaderDynamic);
//	}
//
//	void Renderer::CreateSwapChain()
//	{
//		struct SM {
//			vk::SharingMode sharingMode;
//			uint32_t familyIndicesCount;
//			uint32_t* familyIndicesDataPtr;
//		} sharingModeUtil{ (graphicsQueueFamilyIndex != presentQueueFamilyIndex) ?
//							   SM{ vk::SharingMode::eConcurrent, 2u, mFamilyIndices.data() } : SM{ vk::SharingMode::eExclusive, 0u, static_cast<uint32_t*>(nullptr) } };
//
//		auto capabilities = mPhysicalDevices[0].getSurfaceCapabilitiesKHR(*mSurface);
//		auto formats = mPhysicalDevices[0].getSurfaceFormatsKHR(*mSurface);
//
//		mSwapChainImageFormat = vk::Format::eB8G8R8A8Unorm;
//		mSwapChainExtent = vk::Extent2D{ DefaultScreenWidth, DefaultScreenHeight };
//
//		imageCount = capabilities.minImageCount + 1;
//		if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount) {
//			imageCount = capabilities.maxImageCount;
//		}
//
//		mSwapChainCreateInfo = vk::SwapchainCreateInfoKHR({}, *mSurface, imageCount, mSwapChainImageFormat,
//			vk::ColorSpaceKHR::eSrgbNonlinear, mSwapChainExtent, 1, vk::ImageUsageFlagBits::eColorAttachment,
//			sharingModeUtil.sharingMode, sharingModeUtil.familyIndicesCount,
//			sharingModeUtil.familyIndicesDataPtr, vk::SurfaceTransformFlagBitsKHR::eIdentity,
//			vk::CompositeAlphaFlagBitsKHR::eOpaque, vk::PresentModeKHR::eFifo, true, nullptr);
//
//		mSwapChain = mDevice->createSwapchainKHRUnique(mSwapChainCreateInfo, nullptr, DispatchLoaderDynamic);
//		mSwapChainImages = mDevice->getSwapchainImagesKHR(mSwapChain.get());
//	}
//
//	void Renderer::CreateImageViews()
//	{
//		mImageViews.resize(mSwapChainImages.size());
//		for (size_t i = 0; i< mSwapChainImages.size(); ++i) 
//		{
//			vk::ImageViewCreateInfo imageViewCreateInfo(vk::ImageViewCreateFlags(), mSwapChainImages[i],
//				vk::ImageViewType::e2D, mSwapChainImageFormat,
//				vk::ComponentMapping{ vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG,
//					vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA },
//				vk::ImageSubresourceRange{ vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1 });
//			mImageViews[i] = mDevice->createImageViewUnique(imageViewCreateInfo, nullptr, DispatchLoaderDynamic);
//		}
//	}
//
//	void Renderer::CreateRenderPass()
//	{
//		auto colorAttachment = vk::AttachmentDescription{ {}, mSwapChainImageFormat, vk::SampleCountFlagBits::e1,
//			vk::AttachmentLoadOp::eClear, vk::AttachmentStoreOp::eStore, {}, {}, {}, vk::ImageLayout::ePresentSrcKHR };
//
//		auto depthAttachment = vk::AttachmentDescription{ {}, FindDepthFormat(), vk::SampleCountFlagBits::e1,
//			vk::AttachmentLoadOp::eClear, vk::AttachmentStoreOp::eDontCare, vk::AttachmentLoadOp::eDontCare, vk::AttachmentStoreOp::eDontCare, {}, vk::ImageLayout::eDepthStencilAttachmentOptimal };
//		
//		auto colourAttachmentRef = vk::AttachmentReference{ 0, vk::ImageLayout::eColorAttachmentOptimal };
//
//		auto depthAttachmentRef = vk::AttachmentReference{ 1, vk::ImageLayout::eDepthStencilAttachmentOptimal };
//
//		auto subpass = vk::SubpassDescription{ {}, vk::PipelineBindPoint::eGraphics,
//			/*inAttachmentCount*/ 0, nullptr, 1, &colourAttachmentRef, nullptr, &depthAttachmentRef };
//
//		auto subpassDependency = vk::SubpassDependency{ VK_SUBPASS_EXTERNAL, 0,
//			vk::PipelineStageFlagBits::eColorAttachmentOutput, vk::PipelineStageFlagBits::eColorAttachmentOutput,
//			{}, vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite };
//
//		std::array<vk::AttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };
//
//		mRenderPass = mDevice->createRenderPassUnique(
//			vk::RenderPassCreateInfo{ {}, static_cast<uint32_t>(attachments.size()), attachments.data(), 1, &subpass, 1, &subpassDependency }, nullptr, DispatchLoaderDynamic);
//	}
//
//	void Renderer::CreateDescriptorSetLayout()
//	{
//		mUBOLayoutBinding.binding = 0;
//		mUBOLayoutBinding.descriptorCount = 1;
//		mUBOLayoutBinding.descriptorType = vk::DescriptorType::eUniformBuffer;
//		mUBOLayoutBinding.pImmutableSamplers = nullptr;
//		mUBOLayoutBinding.stageFlags = vk::ShaderStageFlagBits::eVertex;
//
//		mSamplerLayoutBinding.binding = 1;
//		mSamplerLayoutBinding.descriptorCount = 1;
//		mSamplerLayoutBinding.descriptorType = vk::DescriptorType::eCombinedImageSampler;
//		mSamplerLayoutBinding.pImmutableSamplers = nullptr;
//		mSamplerLayoutBinding.stageFlags = vk::ShaderStageFlagBits::eFragment;
//
//		std::array<vk::DescriptorSetLayoutBinding, 2> bindings = { mUBOLayoutBinding, mSamplerLayoutBinding };
//		vk::DescriptorSetLayoutCreateInfo layoutInfo = {};
//		layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
//		layoutInfo.pBindings = bindings.data();
//
//		mDescriptorSetLayout = mDevice->createDescriptorSetLayoutUnique(layoutInfo, nullptr, DispatchLoaderDynamic);
//	}
//
//	void Renderer::CreateGraphicsPipeline()
//	{
//		std::string vertexShader = R"vertexshader(
//        #version 450
//		#extension GL_ARB_separate_shader_objects : enable
//
//		layout(binding = 0) uniform UniformBufferObject {
//		mat4 model;
//		mat4 view;
//		mat4 proj;
//		} ubo;
//
//		layout(location = 0) in vec3 inPosition;
//		layout(location = 1) in vec3 inColor;
//		layout(location = 2) in vec2 inTexCoord;
//
//		layout(location = 0) out vec3 fragColor;
//		layout(location = 1) out vec2 fragTexCoord;
//
//		void main() {
//		gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);
//		fragColor = inColor;
//		fragTexCoord = inTexCoord;
//		}
//        )vertexshader";
//
//		std::string fragmentShader = R"fragmentShader(
//        #version 450
//		#extension GL_ARB_separate_shader_objects : enable
//
//		layout(binding = 1) uniform sampler2D texSampler;
//
//		layout(location = 0) in vec3 fragColor;
//		layout(location = 1) in vec2 fragTexCoord;
//
//		layout(location = 0) out vec4 outColor;
//
//		void main() {
//		outColor = texture(texSampler, fragTexCoord);
//		}
//        )fragmentShader";
//
//		shaderc::Compiler compiler;
//		shaderc::CompileOptions options;
//		options.SetOptimizationLevel(shaderc_optimization_level_performance);
//
//		// Compile GLSL Vertex Shader to SPIR-V
//		shaderc::SpvCompilationResult vertShaderModule =
//			compiler.CompileGlslToSpv(vertexShader, shaderc_glsl_vertex_shader, "vertex shader", options);
//		if (vertShaderModule.GetCompilationStatus() != shaderc_compilation_status_success)
//		{
//			std::cerr << vertShaderModule.GetErrorMessage();
//		}
//		auto vertShaderCode = std::vector<uint32_t>{ vertShaderModule.cbegin(), vertShaderModule.cend() };
//		auto vertSize = std::distance(vertShaderCode.begin(), vertShaderCode.end());
//		auto vertShaderCreateInfo =
//			vk::ShaderModuleCreateInfo{ {}, vertSize * sizeof(uint32_t), vertShaderCode.data() };
//
//		mVertexShaderModule = mDevice->createShaderModuleUnique(vertShaderCreateInfo, nullptr, DispatchLoaderDynamic);
//
//		// Compile GLSL Fragment Shader to SPIR-V
//		shaderc::SpvCompilationResult fragShaderModule = compiler.CompileGlslToSpv(
//			fragmentShader, shaderc_glsl_fragment_shader, "fragment shader", options);
//
//		if (fragShaderModule.GetCompilationStatus() != shaderc_compilation_status_success)
//		{
//			std::cerr << fragShaderModule.GetErrorMessage();
//		}
//
//		auto fragShaderCode = std::vector<uint32_t>{ fragShaderModule.cbegin(), fragShaderModule.cend() };
//		auto fragSize = std::distance(fragShaderCode.begin(), fragShaderCode.end());
//		auto fragShaderCreateInfo =
//			vk::ShaderModuleCreateInfo{ {}, fragSize * sizeof(uint32_t), fragShaderCode.data() };
//
//		mFragmentShaderModule = mDevice->createShaderModuleUnique(fragShaderCreateInfo, nullptr, DispatchLoaderDynamic);
//
//		auto vertShaderStageInfo = vk::PipelineShaderStageCreateInfo
//		{ {}, vk::ShaderStageFlagBits::eVertex, *mVertexShaderModule, "main" };
//
//		auto fragShaderStageInfo = vk::PipelineShaderStageCreateInfo
//		{ {}, vk::ShaderStageFlagBits::eFragment, *mFragmentShaderModule, "main" };
//
//		auto pipelineShaderStages = std::vector<vk::PipelineShaderStageCreateInfo>{ vertShaderStageInfo, fragShaderStageInfo };
//
//		auto bindingDescription = Vertex::getBindingDescription();
//		auto attributeDescriptions = Vertex::getAttributeDescriptions();
//
//		auto vertexInputInfo = vk::PipelineVertexInputStateCreateInfo{ {}, 1u, &bindingDescription, static_cast<uint32_t>(attributeDescriptions.size()), attributeDescriptions.data() };
//
//		auto inputAssembly = vk::PipelineInputAssemblyStateCreateInfo{ {}, vk::PrimitiveTopology::eTriangleList, false };
//
//		mViewport = vk::Viewport{ 0.0f, 0.0f, static_cast<float>(mSwapChainExtent.width), static_cast<float>(mSwapChainExtent.height), 0.0f, 1.0f };
//
//		auto scissor = vk::Rect2D{ { 0, 0 }, mSwapChainExtent };
//
//		auto viewportState = vk::PipelineViewportStateCreateInfo{ {}, 1, &mViewport, 1, &scissor };
//
//		auto rasterizer = vk::PipelineRasterizationStateCreateInfo{ {}, /*depthClamp*/ false,
//			/*rasterizeDiscard*/ false, vk::PolygonMode::eFill, vk::CullModeFlagBits::eBack,
//			/*frontFace*/ vk::FrontFace::eCounterClockwise, false, {}, {}, {}, 1.0f };
//
//		auto multisampling = vk::PipelineMultisampleStateCreateInfo{ {}, vk::SampleCountFlagBits::e1, false, 1.0 };
//
//		auto depthStencil = vk::PipelineDepthStencilStateCreateInfo{ {},true,true,vk::CompareOp::eLess,false,false };
//
//		auto colorBlendAttachment = vk::PipelineColorBlendAttachmentState{ {}, /*srcCol*/ vk::BlendFactor::eOne,
//			/*dstCol*/ vk::BlendFactor::eZero, /*colBlend*/ vk::BlendOp::eAdd,
//			/*srcAlpha*/ vk::BlendFactor::eOne, /*dstAlpha*/ vk::BlendFactor::eZero,
//			/*alphaBlend*/ vk::BlendOp::eAdd,
//			vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA };
//
//		auto colorBlending = vk::PipelineColorBlendStateCreateInfo
//		{ {}, /*logicOpEnable=*/false, vk::LogicOp::eCopy, /*attachmentCount=*/1, /*colourAttachments=*/&colorBlendAttachment };
//
//		std::vector<vk::DescriptorSetLayout> layouts(mSwapChainImages.size(), *mDescriptorSetLayout);
//
//		mPipelineLayout = mDevice->createPipelineLayoutUnique(vk::PipelineLayoutCreateInfo(
//			vk::PipelineLayoutCreateFlags(), /*static_cast<uint32_t>(1)*/static_cast<uint32_t>(layouts.size()),
//			/*&*mDescriptorSetLayout*/layouts.data(), 0, nullptr), nullptr, DispatchLoaderDynamic);
//
//		auto pipelineCreateInfo = vk::GraphicsPipelineCreateInfo{ {}, 2, pipelineShaderStages.data(),
//			&vertexInputInfo, &inputAssembly, nullptr, &viewportState, &rasterizer, &multisampling,
//			nullptr, &colorBlending, nullptr, *mPipelineLayout, *mRenderPass, 0 };
//
//		pipelineCreateInfo.pDepthStencilState = &depthStencil;
//
//		mPipeline = mDevice->createGraphicsPipelineUnique({}, pipelineCreateInfo, nullptr, DispatchLoaderDynamic);
//	}
//
//	void Renderer::CreateFramebuffers()
//	{
//		mFrameBuffers.resize(imageCount);
//		for (size_t i = 0; i < mImageViews.size(); i++)
//		{
//			vk::ImageView imgView = mImageViews[i].get();
//			std::array<vk::ImageView, 2> attachments = { imgView, *mDepthImageView };
//
//			mFrameBuffers[i] = mDevice->createFramebufferUnique(vk::FramebufferCreateInfo
//				{ {}, mRenderPass.get(), static_cast<uint32_t>(attachments.size()), attachments.data(), mSwapChainExtent.width, mSwapChainExtent.height, 1 }, nullptr, DispatchLoaderDynamic);
//		}
//	}
//
//	void Renderer::CreateTextureImage()
//	{
//		void* data;
//		graphicsQueueFamilyIndexUnsignedInt = static_cast<uint32_t>(graphicsQueueFamilyIndex);
//		int texWidth, texHeight, texChannels;
//
//		stbi_uc* pixels = stbi_load(TEXTURE_PATH.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
//		
//		vk::DeviceSize imageSize = static_cast<uint64_t>(texWidth) * static_cast<uint64_t>(texHeight) * static_cast<uint64_t>(4);
//		
//		if (!pixels) 
//		{
//			throw std::runtime_error("Failed to load Texture Image!");
//		}
//
//		// Create Texture Image Staging Buffer & Memory
//		TextureImageStagingBuffer.buffer = mDevice->createBufferUnique(
//			vk::BufferCreateInfo(vk::BufferCreateFlags(), imageSize,
//				vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferSrc, vk::SharingMode::eExclusive,
//				1U, &graphicsQueueFamilyIndexUnsignedInt), nullptr, DispatchLoaderDynamic);
//
//		vk::MemoryRequirements memRequirements;
//		memRequirements = mDevice->getBufferMemoryRequirements(*TextureImageStagingBuffer.buffer);
//
//		TextureImageStagingBuffer.memory = mDevice->allocateMemoryUnique(
//			vk::MemoryAllocateInfo(memRequirements.size, FindMemoryType(memRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent)), nullptr, DispatchLoaderDynamic);
//
//		data = mDevice->mapMemory(*TextureImageStagingBuffer.memory, 0, imageSize, vk::MemoryMapFlags());
//		memcpy(data, pixels, static_cast<size_t>(imageSize));
//		mDevice->unmapMemory(*TextureImageStagingBuffer.memory);
//		mDevice->bindBufferMemory(*TextureImageStagingBuffer.buffer, *TextureImageStagingBuffer.memory, 0);
//
//		stbi_image_free(pixels);
//
//		CreateImage(texWidth, texHeight,
//			vk::Format::eR8G8B8A8Unorm, vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
//			vk::MemoryPropertyFlagBits::eDeviceLocal, mTextureImage, mTextureImageMemory);
//
//		TransitionImageLayout(mTextureImage, vk::Format::eR8G8B8A8Unorm, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal);
//		CopyBufferToImage(*TextureImageStagingBuffer.buffer, *mTextureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
//		TransitionImageLayout(mTextureImage, vk::Format::eR8G8B8A8Unorm, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal);
//	}
//
//	void Renderer::CreateTextureImageView()
//	{
//		*mTextureImageView = CreateImageView(*mTextureImage, vk::Format::eR8G8B8A8Unorm, vk::ImageAspectFlagBits::eColor);
//	}
//
//	void Renderer::CreateTextureSampler()
//	{
//		vk::SamplerCreateInfo samplerInfo = {};
//		samplerInfo.magFilter = vk::Filter::eLinear;
//		samplerInfo.minFilter = vk::Filter::eLinear;
//		samplerInfo.addressModeU = vk::SamplerAddressMode::eRepeat;
//		samplerInfo.addressModeV = vk::SamplerAddressMode::eRepeat;
//		samplerInfo.addressModeW = vk::SamplerAddressMode::eRepeat;
//		samplerInfo.anisotropyEnable = VK_TRUE;
//		samplerInfo.maxAnisotropy = 16;
//		samplerInfo.borderColor = vk::BorderColor::eIntOpaqueBlack;
//		samplerInfo.unnormalizedCoordinates = VK_FALSE;
//		samplerInfo.compareEnable = VK_FALSE;
//		samplerInfo.compareOp = vk::CompareOp::eAlways;
//		samplerInfo.mipmapMode = vk::SamplerMipmapMode::eLinear;
//
//		mTextureSampler = mDevice->createSamplerUnique(samplerInfo, nullptr, DispatchLoaderDynamic);
//	}
//
//	void Renderer::LoadModel()
//	{
//		tinyobj::attrib_t attrib;
//		std::vector<tinyobj::shape_t> shapes;
//		std::vector<tinyobj::material_t> materials;
//		std::string warn, err;
//
//		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, MODEL_PATH.c_str())) {
//			throw std::runtime_error(warn + err);
//		}
//
//		std::unordered_map<Vertex, uint32_t> uniqueVertices = {};
//
//		for (const auto& shape : shapes) {
//			for (const auto& index : shape.mesh.indices) {
//				Vertex vertex = {};
//
//				vertex.pos = {
//					attrib.vertices[3 * index.vertex_index + 0],
//					attrib.vertices[3 * index.vertex_index + 1],
//					attrib.vertices[3 * index.vertex_index + 2]
//				};
//
//				vertex.texCoord = {
//					attrib.texcoords[2 * index.texcoord_index + 0],
//					1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
//				};
//
//				vertex.color = { 1.0f, 1.0f, 1.0f };
//
//				if (uniqueVertices.count(vertex) == 0) {
//					uniqueVertices[vertex] = static_cast<uint32_t>(mVertices.size());
//					mVertices.push_back(vertex);
//				}
//
//				mIndices.push_back(uniqueVertices[vertex]);
//			}
//		}
//	}
//
//	void Renderer::CreateVertexBuffer()
//	{
//		void* data;
//		graphicsQueueFamilyIndexUnsignedInt = static_cast<uint32_t>(graphicsQueueFamilyIndex);
//
//		// Setup Vertex Buffer Size
//		vk::DeviceSize vertexBufferSize = static_cast<vk::DeviceSize>(sizeof(mVertices[0])) * static_cast<vk::DeviceSize>(mVertices.size());
//
//
//		// Create Vertex Buffer
//		stagingBuffers.vertices.buffer = mDevice->createBufferUnique(
//			vk::BufferCreateInfo(vk::BufferCreateFlags(), vertexBufferSize,
//				vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferSrc, vk::SharingMode::eExclusive,
//				1U, &graphicsQueueFamilyIndexUnsignedInt), nullptr, DispatchLoaderDynamic);
//
//		vk::MemoryRequirements memRequirements;
//		memRequirements = mDevice->getBufferMemoryRequirements(*stagingBuffers.vertices.buffer);
//
//		// Request a host visible memory type that can be used to copy our data do
//		// Also request it to be coherent, so that writes are visible to the GPU right after unmapping the buffer
//		stagingBuffers.vertices.memory = mDevice->allocateMemoryUnique(
//			vk::MemoryAllocateInfo(memRequirements.size, FindMemoryType(memRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent)), nullptr, DispatchLoaderDynamic);
//
//		// Map and copy
//		data = mDevice->mapMemory(*stagingBuffers.vertices.memory, 0, memRequirements.size, vk::MemoryMapFlags());
//		memcpy(data, mVertices.data(), static_cast<size_t>(vertexBufferSize));
//		mDevice->unmapMemory(*stagingBuffers.vertices.memory);
//		mDevice->bindBufferMemory(*stagingBuffers.vertices.buffer, *stagingBuffers.vertices.memory, 0);
//
//		// Create a device local buffer to which the (host local) vertex data will be copied and which will be used for rendering
//		VertexBufferOnGPU.buffer = mDevice->createBufferUnique(
//			vk::BufferCreateInfo(vk::BufferCreateFlags(), vertexBufferSize,
//				vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst, vk::SharingMode::eExclusive,
//				1, &graphicsQueueFamilyIndexUnsignedInt), nullptr, DispatchLoaderDynamic);
//
//		memRequirements = mDevice->getBufferMemoryRequirements(*VertexBufferOnGPU.buffer);
//
//		VertexBufferOnGPU.memory = mDevice->allocateMemoryUnique(
//			vk::MemoryAllocateInfo(memRequirements.size, FindMemoryType(memRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal)), nullptr, DispatchLoaderDynamic);
//
//		mDevice->bindBufferMemory(*VertexBufferOnGPU.buffer, *VertexBufferOnGPU.memory, 0);
//	}
//
//	void Renderer::CreateIndexBuffer()
//	{
//		void* data;
//		graphicsQueueFamilyIndexUnsignedInt = static_cast<uint32_t>(graphicsQueueFamilyIndex);
//
//		// Setup Index Buffer Size
//		vk::DeviceSize IndexBufferSize = static_cast<vk::DeviceSize>(sizeof(mIndices[0])) * static_cast<vk::DeviceSize>(mIndices.size());
//
//		// Create Index buffer
//		// Copy index data to a buffer visible to the host (staging buffer)
//		stagingBuffers.indices.buffer = mDevice->createBufferUnique(vk::BufferCreateInfo(
//			vk::BufferCreateFlags(), IndexBufferSize,
//			vk::BufferUsageFlagBits::eTransferSrc, vk::SharingMode::eExclusive,
//			1, &graphicsQueueFamilyIndexUnsignedInt), nullptr, DispatchLoaderDynamic);
//
//		vk::MemoryRequirements memRequirements;
//		memRequirements = mDevice->getBufferMemoryRequirements(*stagingBuffers.indices.buffer);
//		stagingBuffers.indices.memory = mDevice->allocateMemoryUnique(
//			vk::MemoryAllocateInfo(memRequirements.size, FindMemoryType(memRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent)), nullptr, DispatchLoaderDynamic);
//
//		data = mDevice->mapMemory(*stagingBuffers.indices.memory, 0, IndexBufferSize, vk::MemoryMapFlags());
//		memcpy(data, mIndices.data(), static_cast<size_t>(IndexBufferSize));
//		mDevice->unmapMemory(*stagingBuffers.indices.memory);
//		mDevice->bindBufferMemory(*stagingBuffers.indices.buffer, *stagingBuffers.indices.memory, 0);
//
//		// Create destination buffer with device only visibility
//		IndexBufferOnGPU.buffer = mDevice->createBufferUnique(
//			vk::BufferCreateInfo(vk::BufferCreateFlags(), IndexBufferSize,
//				vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer, vk::SharingMode::eExclusive,
//				0, nullptr), nullptr, DispatchLoaderDynamic);
//
//		memRequirements = mDevice->getBufferMemoryRequirements(*IndexBufferOnGPU.buffer);
//		IndexBufferOnGPU.memory = mDevice->allocateMemoryUnique(
//			vk::MemoryAllocateInfo(memRequirements.size, FindMemoryType(memRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal)), nullptr, DispatchLoaderDynamic);
//
//		mDevice->bindBufferMemory(*IndexBufferOnGPU.buffer, *IndexBufferOnGPU.memory, 0);
//	}
//
//	void Renderer::CreateUniformBuffers()
//	{
//		mUniformBuffers.resize(mSwapChainImages.size());
//		mUniformBuffersMemory.resize(mSwapChainImages.size());
//
//		for (size_t i = 0; i < mSwapChainImages.size(); i++) {
//			// Vertex shader uniform buffer block
//			vk::MemoryAllocateInfo allocInfo = {};
//			allocInfo.pNext = nullptr;
//			allocInfo.allocationSize = 0;
//			allocInfo.memoryTypeIndex = 0;
//
//			// Create a new buffer
//			mUniformBuffers[i] = mDevice->createBufferUnique(
//				vk::BufferCreateInfo(vk::BufferCreateFlags(),sizeof(UniformBufferObject),
//					vk::BufferUsageFlagBits::eUniformBuffer), nullptr, DispatchLoaderDynamic);
//
//			vk::MemoryRequirements memoryRequirements;
//			// Get memory requirements including size, alignment and memory type 
//			memoryRequirements = mDevice->getBufferMemoryRequirements(*mUniformBuffers[i]);
//			allocInfo.allocationSize = memoryRequirements.size;
//			// Get the memory type index that supports host visible memory access
//			// Most implementations offer multiple memory types and selecting the correct one to allocate memory from is crucial
//			// We also want the buffer to be host coherent so we don't have to flush (or sync after every update.
//			// Note: This may affect performance so you might not want to do this in a real world application that updates buffers on a regular base
//			allocInfo.memoryTypeIndex = FindMemoryType(memoryRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
//			// Allocate memory for the uniform buffer
//			mUniformBuffersMemory[i] = mDevice->allocateMemoryUnique(allocInfo, nullptr, DispatchLoaderDynamic);
//			// Bind memory to buffer
//			mDevice->bindBufferMemory(*mUniformBuffers[i], *mUniformBuffersMemory[i], 0);
//		}
//	}
//
//	void Renderer::CreateDescriptorPool()
//	{
//		std::array<vk::DescriptorPoolSize, 2> poolSizes = {};
//		poolSizes[0].type = vk::DescriptorType::eUniformBuffer;
//		poolSizes[0].descriptorCount = static_cast<uint32_t>(mSwapChainImages.size());
//		poolSizes[1].type = vk::DescriptorType::eCombinedImageSampler;
//		poolSizes[1].descriptorCount = static_cast<uint32_t>(mSwapChainImages.size());
//
//		vk::DescriptorPoolCreateInfo poolInfo = {};
//		poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
//		poolInfo.pPoolSizes = poolSizes.data();
//		poolInfo.maxSets = static_cast<uint32_t>(mSwapChainImages.size());
//
//		mDescriptorPool = mDevice->createDescriptorPoolUnique(poolInfo, nullptr, DispatchLoaderDynamic);
//	}
//
//	void Renderer::CreateDescriptorSets()
//	{
//		mDescriptorSets.resize(mSwapChainImages.size());
//		std::vector<vk::DescriptorSetLayout> layouts(mSwapChainImages.size(), *mDescriptorSetLayout);
//
//		mDescriptorSets = mDevice->allocateDescriptorSetsUnique(
//			vk::DescriptorSetAllocateInfo(*mDescriptorPool, static_cast<uint32_t>(layouts.size()),layouts.data()));
//
//		for (size_t i = 0; i < mSwapChainImages.size(); i++)
//		{
//			vk::DescriptorBufferInfo bufferInfo = {};
//			bufferInfo.buffer = *mUniformBuffers[i];
//			bufferInfo.offset = 0;
//			bufferInfo.range = sizeof(UniformBufferObject);
//
//			vk::DescriptorImageInfo imageInfo = {};
//			imageInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
//			imageInfo.imageView = *mTextureImageView;
//			imageInfo.sampler = *mTextureSampler;
//
//			std::array<vk::WriteDescriptorSet, 2> descriptorWrites = {};
//
//			descriptorWrites[0].dstSet = *mDescriptorSets[i];
//			descriptorWrites[0].dstBinding = 0;
//			descriptorWrites[0].dstArrayElement = 0;
//			descriptorWrites[0].descriptorType = vk::DescriptorType::eUniformBuffer;
//			descriptorWrites[0].descriptorCount = 1;
//			descriptorWrites[0].pBufferInfo = &bufferInfo;
//
//			descriptorWrites[1].dstSet = *mDescriptorSets[i];
//			descriptorWrites[1].dstBinding = 1;
//			descriptorWrites[1].dstArrayElement = 0;
//			descriptorWrites[1].descriptorType = vk::DescriptorType::eCombinedImageSampler;
//			descriptorWrites[1].descriptorCount = 1;
//			descriptorWrites[1].pImageInfo = &imageInfo;
//
//			mDevice->updateDescriptorSets(static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
//		}
//	}
//
//	void Renderer::CreateCommandPool()
//	{
//		mCommandPool = mDevice->createCommandPoolUnique({ {}, static_cast<uint32_t>(graphicsQueueFamilyIndex) }, nullptr, DispatchLoaderDynamic);
//	}
//
//	void Renderer::CreateCommandBuffers()
//	{
//		mCommandBuffers.resize(mFrameBuffers.size());
//
//		mCommandBuffers = mDevice->allocateCommandBuffersUnique(vk::CommandBufferAllocateInfo(
//			mCommandPool.get(), vk::CommandBufferLevel::ePrimary, static_cast<uint32_t>(mCommandBuffers.size())));
//
//		mGraphicsQueue = mDevice->getQueue(static_cast<uint32_t>(graphicsQueueFamilyIndex), 0);
//		mPresentQueue = mDevice->getQueue(static_cast<uint32_t>(presentQueueFamilyIndex), 0);
//
//		for (size_t i = 0; i < mCommandBuffers.size(); i++)
//		{
//			auto beginInfo = vk::CommandBufferBeginInfo{};
//			beginInfo.flags = vk::CommandBufferUsageFlagBits::eSimultaneousUse;
//			mCommandBuffers[i]->begin(beginInfo);
//			std::array<vk::ClearValue, 2> clearValues = {};
//
//			clearValues[0].color = vk::ClearColorValue(std::array<float, 4>({ 0.0f, 0.0f, 0.0f, 1.0f }));
//			clearValues[1].depthStencil = vk::ClearDepthStencilValue(1.0f, 0);
//			auto renderPassBeginInfo = vk::RenderPassBeginInfo{ mRenderPass.get(), mFrameBuffers[i].get(),
//				vk::Rect2D{ { 0, 0 }, mSwapChainExtent }, static_cast<uint32_t>(clearValues.size()), clearValues.data() };
//
//			mCommandBuffers[i]->beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);
//			mCommandBuffers[i]->bindPipeline(vk::PipelineBindPoint::eGraphics, mPipeline.get());
//
//			vk::DeviceSize offsets = 0;
//			mCommandBuffers[i]->bindVertexBuffers(0, 1, &(*VertexBufferOnGPU.buffer), &offsets);
//			mCommandBuffers[i]->bindIndexBuffer(*IndexBufferOnGPU.buffer, 0, vk::IndexType::eUint32);
//			mCommandBuffers[i]->bindDescriptorSets(vk::PipelineBindPoint::eGraphics, *mPipelineLayout, 0, 1, &*mDescriptorSets[i], 0, nullptr);
//			mCommandBuffers[i]->drawIndexed(static_cast<uint32_t>(mIndices.size()), 1, 0, 0, 0);
//			mCommandBuffers[i]->endRenderPass();
//			mCommandBuffers[i]->end();
//		}
//	}
//
//	void Renderer::CreateSemaphores()
//	{
//		auto semaphoreCreateInfo = vk::SemaphoreCreateInfo{};
//		mImageAvailableSemaphore = mDevice->createSemaphoreUnique(semaphoreCreateInfo, nullptr, DispatchLoaderDynamic);
//		mRenderFinishedSemaphore = mDevice->createSemaphoreUnique(semaphoreCreateInfo, nullptr, DispatchLoaderDynamic);
//	}
//
//	void Renderer::UpdateUniformBuffer(vk::ResultValue<uint32_t> imageIndex)
//	{
//		static auto startTime = std::chrono::high_resolution_clock::now();
//
//		auto currentTime = std::chrono::high_resolution_clock::now();
//		float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
//
//		UniformBufferObject ubo = {};
//		ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//		ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//		ubo.proj = glm::perspective(glm::radians(45.0f), mSwapChainExtent.width / (float)mSwapChainExtent.height, 0.1f, 10.0f);
//		ubo.proj[1][1] *= -1;
//
//		void* data;
//		data = mDevice->mapMemory(*mUniformBuffersMemory[imageIndex.value], 0, sizeof(ubo), {}/*vk::MemoryMapFlags()*/);
//		memcpy(data, &ubo, sizeof(ubo));
//		mDevice->unmapMemory(*mUniformBuffersMemory[imageIndex.value]);
//	}
//
//	void Renderer::CreateDepthResources()
//	{
//		vk::Format depthFormat = FindDepthFormat();
//
//		CreateImage(mSwapChainExtent.width, mSwapChainExtent.height, depthFormat, vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eDepthStencilAttachment, vk::MemoryPropertyFlagBits::eDeviceLocal, mDepthImage, mDepthImageMemory);
//		*mDepthImageView = CreateImageView(*mDepthImage, depthFormat, vk::ImageAspectFlagBits::eDepth);
//
//		TransitionImageLayout(mDepthImage, depthFormat, vk::ImageLayout::eUndefined, vk::ImageLayout::eDepthStencilAttachmentOptimal);
//	}
//
//	vk::Format Renderer::FindSupportedFormat(const std::vector<vk::Format>& candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features)
//	{
//		for (vk::Format format : candidates)
//		{
//			vk::FormatProperties properties;
//
//			mPhysicalDevices[0].getFormatProperties(format, &properties);
//
//			if (tiling == vk::ImageTiling::eLinear && (properties.linearTilingFeatures & features) == features)
//			{
//				return format;
//			}
//			else if (tiling == vk::ImageTiling::eOptimal && (properties.optimalTilingFeatures & features) == features)
//			{
//				return format;
//			}
//		}
//
//		throw std::runtime_error("failed to find supported format!");
//	}
//
//	vk::Format Renderer::FindDepthFormat()
//	{
//		return FindSupportedFormat(
//			{ vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint },
//			vk::ImageTiling::eOptimal,vk::FormatFeatureFlagBits::eDepthStencilAttachment);
//	}
//
//	std::vector<const char*> Renderer::GetRequiredExtensions()
//	{
//		uint32_t glfwExtensionCount = 0;
//		auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
//		std::vector<const char*> allExtensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
//		allExtensions.push_back("VK_EXT_debug_report");
//		return allExtensions;
//	}
//
//	bool Renderer::CheckValidationLayers(std::vector<const char*> const& layers, std::vector<vk::LayerProperties> const& properties)
//	{
//		return std::all_of(layers.begin(), layers.end(), [&properties](const char* name)
//			{
//				return std::find_if(properties.begin(), properties.end(), [&name](vk::LayerProperties const& property) {
//					return strcmp(property.layerName, name) == 0;
//					}) != properties.end();
//			});
//	}
//
//	// To use any Extension functions ( Suffixed with EXT ), we can't use Static Loaders.
//	// We need to Create Dynamic Dispatch Loader & pass it as a last argument to EXT functions.
//	void Renderer::CreateDebugCallbacksForValidationLayers()
//	{
//		DispatchLoaderDynamic = *mVulkanInstance;
//		mPFN_vkCreateDebugReportCallbackEXT = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(mVulkanInstance->getProcAddr("vkCreateDebugReportCallbackEXT"));
//		if (!mPFN_vkCreateDebugReportCallbackEXT)
//		{
//			std::cout << "GetInstanceProcAddr: Unable to find vkCreateDebugReportCallbackEXT function." << std::endl;
//			exit(1);
//		}
//		mPFN_vkDestroyDebugReportCallbackEXT = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(mVulkanInstance->getProcAddr("vkDestroyDebugReportCallbackEXT"));
//		if (!mPFN_vkDestroyDebugReportCallbackEXT)
//		{
//			std::cout << "GetInstanceProcAddr: Unable to find vkDestroyDebugReportCallbackEXT function." << std::endl;
//			exit(1);
//		}
//		// We are passing Dynamic Loader as a last parameter ( Which is optional if we aren't using Extensions )
//		auto mDebugReportCallback = mVulkanInstance->createDebugReportCallbackEXTUnique(vk::DebugReportCallbackCreateInfoEXT(vk::DebugReportFlagBitsEXT::eError | vk::DebugReportFlagBitsEXT::eWarning, DebugFunction), nullptr, DispatchLoaderDynamic);
//	}
//
//	Renderer::Renderer()
//	{
//		Initialize();
//	}
//
//	void Renderer::Run()
//	{
//		while (!glfwWindowShouldClose(mWindow)) {
//			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//			glfwPollEvents();
//			glfwSwapBuffers(mWindow);
//			mGameClock.UpdateGameTime(mGameTime);
//			Update(mGameTime);
//			Draw(mGameTime);
//		}
//		Shutdown();
//	}
//
//	void Renderer::Initialize()
//	{
//		InitializeWindow();
//		InitializeVulkan();
//		mGameClock.Reset();
//	}
//
//	void Renderer::Update(const GameTime& gameTime)
//	{
//		gameTime;
//	}
//
//	void Renderer::Draw(const GameTime& gameTime)
//	{
//		UNREFERENCED_PARAMETER(gameTime);
//		auto imageIndex = mDevice->acquireNextImageKHR(mSwapChain.get(),
//			(std::numeric_limits<uint64_t>::max)(), mImageAvailableSemaphore.get(), {});
//
//		UpdateUniformBuffer(imageIndex);
//
//		vk::PipelineStageFlags waitStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
//
//		auto submitInfo = vk::SubmitInfo{ 1, &mImageAvailableSemaphore.get(), &waitStageMask, 1,
//			&mCommandBuffers[imageIndex.value].get(), 1, &mRenderFinishedSemaphore.get() };
//
//		mGraphicsQueue.submit(submitInfo, {});
//
//		auto presentInfo = vk::PresentInfoKHR{ 1, &mRenderFinishedSemaphore.get(), 1,
//			&mSwapChain.get(), &imageIndex.value };
//		mPresentQueue.presentKHR(presentInfo);
//
//		mDevice->waitIdle();
//	}
//
//	// Utility Functions
//
//	uint32_t Renderer::FindGraphicsQueueFamilyIndex(std::vector<vk::QueueFamilyProperties> const& queueFamilyProperties)
//	{
//		// get the first index into queueFamiliyProperties which supports graphics
//		// std::find_if gives first element for which predicate returns true.
//		size_t GraphicsQueueFamilyIndex = std::distance(queueFamilyProperties.begin(), std::find_if(queueFamilyProperties.begin(), queueFamilyProperties.end(),
//			[](vk::QueueFamilyProperties const& qfp) { return qfp.queueFlags & vk::QueueFlagBits::eGraphics; }));
//		assert(graphicsQueueFamilyIndex < queueFamilyProperties.size());
//		return static_cast<uint32_t>(GraphicsQueueFamilyIndex);
//	}
//
//	std::pair<uint32_t, uint32_t> Renderer::FindGraphicsAndPresentQueueFamilyIndex(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR const& Surface)
//	{
//		std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevice.getQueueFamilyProperties();
//
//		uint32_t GraphicsQueueFamilyIndex = FindGraphicsQueueFamilyIndex(queueFamilyProperties);
//		if (physicalDevice.getSurfaceSupportKHR(GraphicsQueueFamilyIndex, Surface))
//		{
//			return std::make_pair(GraphicsQueueFamilyIndex, GraphicsQueueFamilyIndex);    // the first graphicsQueueFamilyIndex does also support presents
//		}
//
//		// the graphicsQueueFamilyIndex doesn't support present -> look for an other family index that supports both graphics and present
//		for (size_t i = 0; i < queueFamilyProperties.size(); i++)
//		{
//			if ((queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eGraphics) && physicalDevice.getSurfaceSupportKHR(static_cast<uint32_t>(i), Surface))
//			{
//				return std::make_pair(static_cast<uint32_t>(i), static_cast<uint32_t>(i));
//			}
//		}
//
//		// there's nothing like a single family index that supports both graphics and present -> look for an other family index that supports present
//		for (size_t i = 0; i < queueFamilyProperties.size(); i++)
//		{
//			if (physicalDevice.getSurfaceSupportKHR(static_cast<uint32_t>(i), Surface))
//			{
//				return std::make_pair(static_cast<uint32_t>(graphicsQueueFamilyIndex), static_cast<uint32_t>(i));
//			}
//		}
//
//		throw std::runtime_error("Could not find queues for both graphics or present -> terminating");
//	}
//
//	void Renderer::CreateImage(uint32_t width, uint32_t height, vk::Format format, vk::ImageTiling tiling, vk::ImageUsageFlags usageFlags, vk::MemoryPropertyFlagBits properties, vk::UniqueHandle<vk::Image, vk::DispatchLoaderDynamic>& image, vk::UniqueHandle<vk::DeviceMemory, vk::DispatchLoaderDynamic>& imageMemory)
//	{
//		vk::ImageCreateInfo imageInfo = {};
//		imageInfo.imageType = vk::ImageType::e2D;
//		imageInfo.extent.width = width;
//		imageInfo.extent.height = height;
//		imageInfo.extent.depth = 1;
//		imageInfo.mipLevels = 1;
//		imageInfo.arrayLayers = 1;
//		imageInfo.format = format;
//		imageInfo.tiling = tiling;
//		imageInfo.initialLayout = vk::ImageLayout::eUndefined;
//		imageInfo.usage = usageFlags;
//		imageInfo.samples = vk::SampleCountFlagBits::e1;
//		imageInfo.sharingMode = vk::SharingMode::eExclusive;
//
//		image = mDevice->createImageUnique(imageInfo, nullptr, DispatchLoaderDynamic);
//
//		vk::MemoryRequirements memRequirements;
//		memRequirements = mDevice->getImageMemoryRequirements(*image);
//
//		vk::MemoryAllocateInfo allocInfo = {};
//		allocInfo.allocationSize = memRequirements.size;
//		allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);
//
//		imageMemory = mDevice->allocateMemoryUnique(allocInfo, nullptr, DispatchLoaderDynamic);
//		mDevice->bindImageMemory(*image, *imageMemory, 0);
//	}
//
//	vk::ImageView Renderer::CreateImageView(vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags)
//	{
//		vk::ImageViewCreateInfo viewInfo = {};
//		viewInfo.image = image;
//		viewInfo.viewType = vk::ImageViewType::e2D;
//		viewInfo.format = format;
//		viewInfo.subresourceRange.aspectMask = aspectFlags;
//		viewInfo.subresourceRange.baseMipLevel = 0;
//		viewInfo.subresourceRange.levelCount = 1;
//		viewInfo.subresourceRange.baseArrayLayer = 0;
//		viewInfo.subresourceRange.layerCount = 1;
//
//		vk::ImageView imageView;
//		imageView = mDevice->createImageView(viewInfo);
//		return imageView;
//	}
//
//	uint32_t Renderer::FindMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties)
//	{
//		vk::PhysicalDeviceMemoryProperties memProperties;
//		memProperties = mPhysicalDevices[0].getMemoryProperties();
//
//		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
//			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
//				return i;
//			}
//		}
//		throw std::runtime_error("failed to find suitable memory type!");
//	}
//	void Renderer::TransitionImageLayout(vk::UniqueHandle<vk::Image, vk::DispatchLoaderDynamic>& image, vk::Format format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout)
//	{
//		format;
//		vk::CommandBuffer commandBuffer = BeginSingleTimeCommands();
//
//		vk::ImageMemoryBarrier barrier = {};
//		barrier.oldLayout = oldLayout;
//		barrier.newLayout = newLayout;
//		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
//		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
//		barrier.image = *image;
//
//		if (newLayout == vk::ImageLayout::eDepthStencilAttachmentOptimal) 
//		{
//			barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eDepth;
//			if (HasStencilComponent(format))
//			{
//				barrier.subresourceRange.aspectMask |= vk::ImageAspectFlagBits::eStencil;
//			}
//		}
//		else
//		{
//			barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
//		}
//
//		barrier.subresourceRange.baseMipLevel = 0;
//		barrier.subresourceRange.levelCount = 1;
//		barrier.subresourceRange.baseArrayLayer = 0;
//		barrier.subresourceRange.layerCount = 1;
//
//		vk::PipelineStageFlags sourceStage;
//		vk::PipelineStageFlags destinationStage;
//
//		if (oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eTransferDstOptimal)
//		{
//			barrier.srcAccessMask = static_cast<vk::AccessFlags>(0);
//			barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;
//			sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
//			destinationStage = vk::PipelineStageFlagBits::eTransfer;
//		}
//		else if (oldLayout == vk::ImageLayout::eTransferDstOptimal && newLayout == vk::ImageLayout::eShaderReadOnlyOptimal)
//		{
//			barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
//			barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;
//			sourceStage = vk::PipelineStageFlagBits::eTransfer;
//			destinationStage = vk::PipelineStageFlagBits::eFragmentShader;
//		}
//		else if (oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eDepthStencilAttachmentOptimal)
//		{
//			barrier.srcAccessMask = static_cast<vk::AccessFlags>(0);
//			barrier.dstAccessMask = vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite;
//
//			sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
//			destinationStage = vk::PipelineStageFlagBits::eEarlyFragmentTests;
//		}
//		else
//		{
//			throw std::invalid_argument("Unsupported Layout Transition!");
//		}
//		commandBuffer.pipelineBarrier(sourceStage, destinationStage,
//			vk::DependencyFlags(), static_cast<uint32_t>(0), nullptr, static_cast<uint32_t>(0), nullptr, static_cast<uint32_t>(1), &barrier);
//
//		EndSingleTimeCommands(commandBuffer);
//	}
//
//	vk::CommandBuffer Renderer::BeginSingleTimeCommands()
//	{
//		vk::CommandBufferAllocateInfo allocInfo = {};
//		allocInfo.level = vk::CommandBufferLevel::ePrimary;
//		allocInfo.commandPool = *mCommandPool;
//		allocInfo.commandBufferCount = 1;
//
//		vk::CommandBuffer commandBuffer;
//		mDevice->allocateCommandBuffers(&allocInfo, &commandBuffer);
//
//		vk::CommandBufferBeginInfo beginInfo = {};
//		beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
//
//		commandBuffer.begin(beginInfo);
//
//		return commandBuffer;
//	}
//
//	void Renderer::EndSingleTimeCommands(vk::CommandBuffer commandBuffer)
//	{
//		mGraphicsQueue = mDevice->getQueue(static_cast<uint32_t>(graphicsQueueFamilyIndex), 0);
//
//		vkEndCommandBuffer(commandBuffer);
//
//		vk::SubmitInfo submitInfo = {};
//		submitInfo.commandBufferCount = 1;
//		submitInfo.pCommandBuffers = &commandBuffer;
//
//		mGraphicsQueue.submit(submitInfo, {});
//		mGraphicsQueue.waitIdle();
//
//		mDevice->freeCommandBuffers(*mCommandPool, commandBuffer);
//	}
//
//	void Renderer::CopyBufferToImage(vk::Buffer buffer, vk::Image image, uint32_t width, uint32_t height)
//	{
//		vk::CommandBuffer commandBuffer = BeginSingleTimeCommands();
//
//		vk::BufferImageCopy region = {};
//		region.bufferOffset = 0;
//		region.bufferRowLength = 0;
//		region.bufferImageHeight = 0;
//		region.imageSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
//		region.imageSubresource.mipLevel = 0;
//		region.imageSubresource.baseArrayLayer = 0;
//		region.imageSubresource.layerCount = 1;
//		region.imageOffset = { 0, 0, 0 };
//		region.imageExtent = { width, height, 1 };
//
//		commandBuffer.copyBufferToImage(buffer, image, vk::ImageLayout::eTransferDstOptimal, 1, &region);
//
//		EndSingleTimeCommands(commandBuffer);
//	}
//
//	bool Renderer::HasStencilComponent(vk::Format format)
//	{
//		return format == vk::Format::eD32SfloatS8Uint || format == vk::Format::eD24UnormS8Uint;
//	}
//}