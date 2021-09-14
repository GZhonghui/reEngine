#include"VulkanManager.h"

bool VulkanManager::createInstance()
{
    VkApplicationInfo appInfo = {};

    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Vulkan Manager";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "None";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    uint32_t glfwExtensionCount;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    VkInstanceCreateInfo createInfo = {};

    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
    createInfo.enabledLayerCount = 0;

    VkResult result = vkCreateInstance(&createInfo, nullptr, &m_Instance);

    if (result != VK_SUCCESS)
    {
        Out::Log(pType::ERROR, "Cant Create Instance");
        return false;
    }

    uint32_t vulkanExtensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &vulkanExtensionCount, nullptr);

    std::vector<VkExtensionProperties> vulkanInstanceExtensions(vulkanExtensionCount);
    vkEnumerateInstanceExtensionProperties
    (
        nullptr, &vulkanExtensionCount, vulkanInstanceExtensions.data()
    );

    Out::Log
    (
        pType::MESSAGE,
        "Vulkan Extensions Count: %d", static_cast<int>(vulkanExtensionCount)
    );

    return true;
}

bool VulkanManager::createSurface()
{
    VkResult result;
    result = glfwCreateWindowSurface(m_Instance, m_Window, nullptr, &m_Surface);

    if (result != VK_SUCCESS)
    {
        Out::Log(pType::ERROR, "GLFW cant create surface");
        return false;
    }

    return true;
}

bool VulkanManager::createPhysicalDevice()
{
    m_PhysicalDevice = VK_NULL_HANDLE;

    uint32_t physicalDeviceCount;
    vkEnumeratePhysicalDevices(m_Instance, &physicalDeviceCount, nullptr);

    Out::Log
    (
        pType::MESSAGE,
        "Physical Device count: %d", static_cast<int>(physicalDeviceCount)
    );

    if (!physicalDeviceCount)
    {
        return false;
    }

    std::vector<VkPhysicalDevice> devices(physicalDeviceCount);
    vkEnumeratePhysicalDevices(m_Instance, &physicalDeviceCount, devices.data());

    for (auto device = devices.begin(); device != devices.end(); ++device)
    {
        if (VulkanMisc::Tools::checkDevice(*device, m_Surface, m_NeedExtensionsChar))
        {
            m_PhysicalDevice = *device;
            break;
        }
    }

    if (m_PhysicalDevice == VK_NULL_HANDLE)
    {
        Out::Log(pType::ERROR, "Cant find suitable physical device");
        return false;
    }

    VkPhysicalDeviceProperties physicalDeviceProperties;
    vkGetPhysicalDeviceProperties(m_PhysicalDevice, &physicalDeviceProperties);
    Out::Log(pType::MESSAGE, "Pick device: %s", physicalDeviceProperties.deviceName);

    //Get Vulkan Info
    VulkanMisc::Tools::checkQueueFamilies(m_PhysicalDevice, m_Surface, m_VulkanInfo.queueFamiliesIndex);
    VulkanMisc::Tools::checkSwapchainSupport(m_PhysicalDevice, m_Surface, m_VulkanInfo.swapChainDetails);

    VulkanMisc::Tools::chooseSwapSurfaceFormat
    (
        m_VulkanInfo.swapChainDetails.m_Formats,
        m_VulkanInfo.swapSurfaceFormat
    );
    VulkanMisc::Tools::chooseSwapPresentMode
    (
        m_VulkanInfo.swapChainDetails.m_PresentModes,
        m_VulkanInfo.swapPresentMode
    );

    VulkanMisc::Tools::chooseSwapExtent
    (
        m_wWidth, m_wHeight,
        m_VulkanInfo.swapChainDetails.m_Capabilities,
        m_VulkanInfo.swapChainExtent
    );

    return true;
}

bool VulkanManager::createLogicalDevice()
{
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::unordered_set<int> queueIndices;

    VulkanMisc::QueueFamiliesIndices queueFamiliesIndex = m_VulkanInfo.queueFamiliesIndex;

    queueIndices.insert(queueFamiliesIndex.m_GraphicsIndex);
    queueIndices.insert(queueFamiliesIndex.m_PresentIndex);

    float queuePriority = 1.0f;

    auto pQI = &queueIndices;
    for (auto createIndex = pQI->begin(); createIndex != pQI->end(); ++createIndex)
    {
        VkDeviceQueueCreateInfo queueCreateInfo = {};

        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = *createIndex;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;

        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures = {};

    VkDeviceCreateInfo deviceCreateInfo = {};

    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
    deviceCreateInfo.queueCreateInfoCount = (uint32_t)queueCreateInfos.size();
    deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
    deviceCreateInfo.ppEnabledExtensionNames = m_NeedExtensionsChar.data();
    deviceCreateInfo.enabledExtensionCount = (uint32_t)m_NeedExtensionsChar.size();

    VkResult result = vkCreateDevice(m_PhysicalDevice, &deviceCreateInfo, nullptr, &m_Device);

    if (result != VK_SUCCESS)
    {
        Out::Log(pType::ERROR, "Create Device Failed");
        return false;
    }

    return true;
}

bool VulkanManager::createQueue()
{
    VulkanMisc::QueueFamiliesIndices QFI = m_VulkanInfo.queueFamiliesIndex;

    vkGetDeviceQueue(m_Device, QFI.m_GraphicsIndex, 0, &m_GraphicsQueue);
    vkGetDeviceQueue(m_Device, QFI.m_PresentIndex, 0, &m_PresentQueue);

    return true;
}

bool VulkanManager::createSwapChain()
{
    VkResult result;

    VulkanMisc::QueueFamiliesIndices queueFamiliesIndex = m_VulkanInfo.queueFamiliesIndex;
    VulkanMisc::SwapChainDetails swapChainDetails = m_VulkanInfo.swapChainDetails;

    uint32_t swapChainImagesCount;
    swapChainImagesCount = swapChainDetails.m_Capabilities.minImageCount + 1;

    if (swapChainDetails.m_Capabilities.maxImageCount > 0)
    {
        swapChainImagesCount = std::min
        (
            swapChainImagesCount, swapChainDetails.m_Capabilities.maxImageCount
        );
    }

    VkSwapchainCreateInfoKHR swapChainCreateInfo = {};

    swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapChainCreateInfo.surface = m_Surface;
    swapChainCreateInfo.minImageCount = swapChainImagesCount;
    swapChainCreateInfo.imageFormat = m_VulkanInfo.swapSurfaceFormat.format;
    swapChainCreateInfo.imageColorSpace = m_VulkanInfo.swapSurfaceFormat.colorSpace;
    swapChainCreateInfo.imageExtent = m_VulkanInfo.swapChainExtent;
    swapChainCreateInfo.imageArrayLayers = 1;
    swapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    if (queueFamiliesIndex.m_GraphicsIndex != queueFamiliesIndex.m_PresentIndex)
    {
        uint32_t indices[2] = {};
        indices[0] = (uint32_t)queueFamiliesIndex.m_GraphicsIndex;
        indices[1] = (uint32_t)queueFamiliesIndex.m_PresentIndex;

        swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapChainCreateInfo.queueFamilyIndexCount = 2;
        swapChainCreateInfo.pQueueFamilyIndices = indices;
    }
    else
    {
        swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        swapChainCreateInfo.queueFamilyIndexCount = 0;
        swapChainCreateInfo.pQueueFamilyIndices = nullptr;
    }

    swapChainCreateInfo.preTransform = swapChainDetails.m_Capabilities.currentTransform;
    swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapChainCreateInfo.presentMode = m_VulkanInfo.swapPresentMode;
    swapChainCreateInfo.clipped = VK_TRUE;
    swapChainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

    result = vkCreateSwapchainKHR(m_Device, &swapChainCreateInfo, nullptr, &m_SwapChain);

    if (result != VK_SUCCESS)
    {
        Out::Log(pType::ERROR, "Create swap chain failed");
        return false;
    }

    //make sure Get swap chain images count again
    vkGetSwapchainImagesKHR(m_Device, m_SwapChain, &swapChainImagesCount, nullptr);

    Out::Log(pType::MESSAGE, "Swapchain Count: %d", (int)swapChainImagesCount);

    return true;
}

bool VulkanManager::createImages()
{
    uint32_t swapChainImagesCount;
    vkGetSwapchainImagesKHR(m_Device, m_SwapChain, &swapChainImagesCount, nullptr);

    m_SwapChainImages.resize(swapChainImagesCount);
    vkGetSwapchainImagesKHR(m_Device, m_SwapChain, &swapChainImagesCount, m_SwapChainImages.data());

    return true;
}

bool VulkanManager::createImageViews()
{
    VkResult result;

    VulkanMisc::SwapChainDetails swapChainDetails = m_VulkanInfo.swapChainDetails;
    VkSurfaceFormatKHR swapSurfaceFormat = m_VulkanInfo.swapSurfaceFormat;

    uint32_t swapChainImagesCount;
    vkGetSwapchainImagesKHR(m_Device, m_SwapChain, &swapChainImagesCount, nullptr);
    m_SwapChainImageViews.resize(swapChainImagesCount);

    for (unsigned int i = 0; i < swapChainImagesCount; i += 1)
    {
        VkImageViewCreateInfo viewCreateInfo = {};

        viewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewCreateInfo.image = m_SwapChainImages[i];
        viewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewCreateInfo.format = swapSurfaceFormat.format;
        viewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        viewCreateInfo.subresourceRange.baseMipLevel = 0;
        viewCreateInfo.subresourceRange.levelCount = 1;
        viewCreateInfo.subresourceRange.baseArrayLayer = 0;
        viewCreateInfo.subresourceRange.layerCount = 1;

        result = vkCreateImageView
        (
            m_Device, &viewCreateInfo, nullptr, &m_SwapChainImageViews[i]
        );

        if (result != VK_SUCCESS)
        {
            Out::Log(pType::ERROR, "Cant create views for image in swap chain");
            return false;
        }
    }

    return true;
}

bool VulkanManager::createShader()
{
    VkResult result;

    Shader fragmentShader("VKDefault", sType::FRAG);
    Shader vertexShader("VKDefault", sType::VERT);

    VkShaderModuleCreateInfo shaderCreateInfo = {};
    shaderCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;

    shaderCreateInfo.codeSize = fragmentShader.m_ShaderCode.size();
    shaderCreateInfo.pCode = reinterpret_cast<const uint32_t*>(fragmentShader.m_ShaderCode.data());
    result = vkCreateShaderModule(m_Device, &shaderCreateInfo, nullptr, &m_ShaderFragment);

    if (result != VK_SUCCESS)
    {
        Out::Log(pType::ERROR, "Load Shader Failed");
        return false;
    }

    shaderCreateInfo.codeSize = vertexShader.m_ShaderCode.size();
    shaderCreateInfo.pCode = reinterpret_cast<const uint32_t*>(vertexShader.m_ShaderCode.data());
    result = vkCreateShaderModule(m_Device, &shaderCreateInfo, nullptr, &m_ShaderVertex);

    if (result != VK_SUCCESS)
    {
        Out::Log(pType::ERROR, "Load Shader Failed");
        return false;
    }

    return true;
}

bool VulkanManager::createRenderPass()
{
    VkSurfaceFormatKHR swapSurfaceFormat = m_VulkanInfo.swapSurfaceFormat;

    VkAttachmentDescription colorAttachment = {};
    colorAttachment.format = swapSurfaceFormat.format;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentReference = {};
    colorAttachmentReference.attachment = 0;
    colorAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subPass = {};
    subPass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subPass.colorAttachmentCount = 1;
    subPass.pColorAttachments = &colorAttachmentReference;

    VkSubpassDependency subpassDependency = {};
    subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    subpassDependency.dstSubpass = 0;
    subpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpassDependency.srcAccessMask = 0;
    subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpassDependency.dstAccessMask = (VkFlags)VK_NULL_HANDLE;
    subpassDependency.dstAccessMask |= VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
    subpassDependency.dstAccessMask |= VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo renderPassCreateInfo = {};
    renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassCreateInfo.attachmentCount = 1;
    renderPassCreateInfo.pAttachments = &colorAttachment;
    renderPassCreateInfo.subpassCount = 1;
    renderPassCreateInfo.pSubpasses = &subPass;
    renderPassCreateInfo.pDependencies = &subpassDependency;

    VkResult result = vkCreateRenderPass(m_Device, &renderPassCreateInfo, nullptr, &m_RenderPass);

    if (result != VK_SUCCESS)
    {
        Out::Log(pType::ERROR, "Create Render Pass Failed");
        return false;
    }

    return true;
}

bool VulkanManager::createDescriptorSetLayout()
{
    VkDescriptorSetLayoutBinding UBOLayoutBinding = {};
    UBOLayoutBinding.binding = 0;
    UBOLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    UBOLayoutBinding.descriptorCount = 1;
    UBOLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    UBOLayoutBinding.pImmutableSamplers = nullptr;

    VkDescriptorSetLayoutCreateInfo layoutCreateInfo = {};
    layoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutCreateInfo.bindingCount = 1;
    layoutCreateInfo.pBindings = &UBOLayoutBinding;

    vkCreateDescriptorSetLayout(m_Device, &layoutCreateInfo, nullptr, &m_DescriptorSetLayout);

    return true;
}

bool VulkanManager::createGraphicsPipeline()
{
    VkExtent2D swapChainExtent = m_VulkanInfo.swapChainExtent;

    VkPipelineShaderStageCreateInfo vertexShaderStageCreateInfo = {};
    vertexShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertexShaderStageCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertexShaderStageCreateInfo.module = m_ShaderVertex;
    vertexShaderStageCreateInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragmentShaderStageCreateInfo = {};
    fragmentShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragmentShaderStageCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragmentShaderStageCreateInfo.module = m_ShaderFragment;
    fragmentShaderStageCreateInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStageCreateInfos[2] = {};
    shaderStageCreateInfos[0] = vertexShaderStageCreateInfo;
    shaderStageCreateInfos[1] = fragmentShaderStageCreateInfo;

    VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo = {};
    vertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

    auto bindingDescription = VulkanMisc::Vertex::getBindingDescription();
    auto attributeDescription = VulkanMisc::Vertex::getAttributeDescription();

    vertexInputStateCreateInfo.vertexBindingDescriptionCount = 1;
    vertexInputStateCreateInfo.pVertexBindingDescriptions = &bindingDescription;

    vertexInputStateCreateInfo.vertexAttributeDescriptionCount = attributeDescription.size();
    vertexInputStateCreateInfo.pVertexAttributeDescriptions = attributeDescription.data();

    VkPipelineInputAssemblyStateCreateInfo vertexInputAssemblyCreateInfo = {};
    vertexInputAssemblyCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    vertexInputAssemblyCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    vertexInputAssemblyCreateInfo.primitiveRestartEnable = VK_FALSE;

    VkViewport viewport = {};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)swapChainExtent.width;
    viewport.height = (float)swapChainExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor = {};
    scissor.offset.x = 0;
    scissor.offset.y = 0;
    scissor.extent = swapChainExtent;

    VkPipelineViewportStateCreateInfo viewportStateCreateInfo = {};
    viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportStateCreateInfo.viewportCount = 1;
    viewportStateCreateInfo.pViewports = &viewport;
    viewportStateCreateInfo.scissorCount = 1;
    viewportStateCreateInfo.pScissors = &scissor;

    VkPipelineRasterizationStateCreateInfo resterizationStateCreateInfo = {};
    resterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    resterizationStateCreateInfo.depthClampEnable = VK_FALSE;
    resterizationStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;
    resterizationStateCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
    resterizationStateCreateInfo.lineWidth = 1.0f;
    resterizationStateCreateInfo.cullMode = VK_CULL_MODE_NONE;
    resterizationStateCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
    resterizationStateCreateInfo.depthBiasEnable = VK_FALSE;
    resterizationStateCreateInfo.depthBiasConstantFactor = 0.0f;
    resterizationStateCreateInfo.depthBiasClamp = 0.0f;
    resterizationStateCreateInfo.depthBiasSlopeFactor = 0.0f;

    VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo = {};
    multisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
    multisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampleStateCreateInfo.minSampleShading = 1.0f;
    multisampleStateCreateInfo.pSampleMask = nullptr;
    multisampleStateCreateInfo.alphaToCoverageEnable = VK_FALSE;
    multisampleStateCreateInfo.alphaToOneEnable = VK_FALSE;

    VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
    colorBlendAttachment.colorWriteMask = (VkFlags)VK_NULL_HANDLE;

    colorBlendAttachment.colorWriteMask |= VK_COLOR_COMPONENT_R_BIT;
    colorBlendAttachment.colorWriteMask |= VK_COLOR_COMPONENT_G_BIT;
    colorBlendAttachment.colorWriteMask |= VK_COLOR_COMPONENT_B_BIT;
    colorBlendAttachment.colorWriteMask |= VK_COLOR_COMPONENT_A_BIT;

    colorBlendAttachment.blendEnable = VK_FALSE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

    VkPipelineColorBlendStateCreateInfo colorBlendCreateInfo = {};
    colorBlendCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlendCreateInfo.logicOpEnable = VK_FALSE;
    colorBlendCreateInfo.logicOp = VK_LOGIC_OP_COPY;
    colorBlendCreateInfo.attachmentCount = 1;
    colorBlendCreateInfo.pAttachments = &colorBlendAttachment;

    for (unsigned int i = 0; i <= 3; i += 1)
    {
        colorBlendCreateInfo.blendConstants[i] = 0.0f;
    }

    VkPipelineLayoutCreateInfo layoutCreateInfo = {};
    layoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    layoutCreateInfo.setLayoutCount = 1;
    layoutCreateInfo.pSetLayouts = &m_DescriptorSetLayout;
    layoutCreateInfo.pushConstantRangeCount = 0;
    layoutCreateInfo.pPushConstantRanges = nullptr;

    VkResult result = vkCreatePipelineLayout(m_Device, &layoutCreateInfo, nullptr, &m_PipelineLayout);

    VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
    pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineCreateInfo.stageCount = 2;
    pipelineCreateInfo.pStages = shaderStageCreateInfos;
    pipelineCreateInfo.pVertexInputState = &vertexInputStateCreateInfo;
    pipelineCreateInfo.pInputAssemblyState = &vertexInputAssemblyCreateInfo;
    pipelineCreateInfo.pViewportState = &viewportStateCreateInfo;
    pipelineCreateInfo.pRasterizationState = &resterizationStateCreateInfo;
    pipelineCreateInfo.pMultisampleState = &multisampleStateCreateInfo;
    pipelineCreateInfo.pDepthStencilState = nullptr;
    pipelineCreateInfo.pColorBlendState = &colorBlendCreateInfo;
    pipelineCreateInfo.pDynamicState = nullptr;
    pipelineCreateInfo.layout = m_PipelineLayout;
    pipelineCreateInfo.renderPass = m_RenderPass;
    pipelineCreateInfo.subpass = 0;
    pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineCreateInfo.basePipelineIndex = -1;

    result = vkCreateGraphicsPipelines
    (
        m_Device, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &m_GraphicsPipeline
    );

    if (result != VK_SUCCESS)
    {
        Out::Log(pType::ERROR, "Create Pipeline Failed");
        return false;
    }

    return true;
}

bool VulkanManager::createFrameBuffers()
{
    VkResult result;

    uint32_t swapChainImagesCount;
    vkGetSwapchainImagesKHR(m_Device, m_SwapChain, &swapChainImagesCount, nullptr);
    m_SwapChainFrameBuffers.resize(swapChainImagesCount);

    for (unsigned int i = 0; i < swapChainImagesCount; i += 1)
    {
        VkImageView attachments[1] = {};

        attachments[0] = m_SwapChainImageViews[i];

        VkFramebufferCreateInfo frameBufferCreateInfo = {};
        frameBufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        frameBufferCreateInfo.renderPass = m_RenderPass;
        frameBufferCreateInfo.attachmentCount = 1;
        frameBufferCreateInfo.pAttachments = attachments;
        frameBufferCreateInfo.width = m_VulkanInfo.swapChainExtent.width;
        frameBufferCreateInfo.height = m_VulkanInfo.swapChainExtent.height;
        frameBufferCreateInfo.layers = 1;

        result = vkCreateFramebuffer
        (
            m_Device, &frameBufferCreateInfo, nullptr, &m_SwapChainFrameBuffers[i]
        );

        if (result != VK_SUCCESS)
        {
            Out::Log(pType::ERROR, "Create Frame Buffer Failed");
            return false;
        }
    }

    return true;
}

bool VulkanManager::createVertexBuffer()
{
    VkResult result;

    VkBufferCreateInfo vertexBufferCreateInfo = {};
    vertexBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    vertexBufferCreateInfo.size = sizeof(VulkanMisc::Vertex) * m_Vertices.size();
    vertexBufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    vertexBufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    result = vkCreateBuffer(m_Device, &vertexBufferCreateInfo, nullptr, &m_VertexBuffer);

    Out::Log
    (
        pType::MESSAGE,
        "Create Vertex buffer with size of %d", (int)vertexBufferCreateInfo.size
    );

    VkMemoryRequirements memoryRequitments;
    vkGetBufferMemoryRequirements(m_Device, m_VertexBuffer, &memoryRequitments);

    VkMemoryAllocateInfo allocMemoryInfo = {};
    allocMemoryInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocMemoryInfo.allocationSize = memoryRequitments.size;

    VulkanMisc::Tools::findMemoryType
    (
        allocMemoryInfo.memoryTypeIndex, memoryRequitments.memoryTypeBits,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        m_PhysicalDevice
    );

    result = vkAllocateMemory(m_Device, &allocMemoryInfo, nullptr, &m_VertexBufferMemory);

    vkBindBufferMemory(m_Device, m_VertexBuffer, m_VertexBufferMemory, 0);

    void* vertexData;
    vkMapMemory(m_Device, m_VertexBufferMemory, 0, vertexBufferCreateInfo.size, 0, &vertexData);

    memcpy(vertexData, m_Vertices.data(), (size_t)vertexBufferCreateInfo.size);
    vkUnmapMemory(m_Device, m_VertexBufferMemory);

    return true;
}

bool VulkanManager::createUniformBuffers()
{
    VkDeviceSize bufferSize = sizeof(UniformMVP);

    uint32_t swapChainImagesCount = m_MainWindowData.ImageCount;

    m_UniformTransformMatBuffers.resize(swapChainImagesCount);
    m_UniformTransformMatBuffersMemory.resize(swapChainImagesCount);

    VkBufferUsageFlags bufferUsage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

    VkMemoryPropertyFlags memProperties = (VkMemoryPropertyFlags)VK_NULL_HANDLE;
    memProperties |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
    memProperties |= VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

    VkBufferCreateInfo bufferCreateInfo = {};
    bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCreateInfo.size = sizeof(UniformMVP);
    bufferCreateInfo.usage = bufferUsage;
    bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    for (size_t i = 0; i < swapChainImagesCount; i += 1)
    {
        vkCreateBuffer(m_Device, &bufferCreateInfo, nullptr, &m_UniformTransformMatBuffers[i]);

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(m_Device, m_UniformTransformMatBuffers[i], &memRequirements);

        VkMemoryAllocateInfo memoryAllocInfo = {};
        memoryAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        memoryAllocInfo.allocationSize = memRequirements.size;

        VulkanMisc::Tools::findMemoryType
        (
            memoryAllocInfo.memoryTypeIndex, memRequirements.memoryTypeBits,
            memProperties, m_PhysicalDevice
        );

        vkAllocateMemory
        (
            m_Device, &memoryAllocInfo, nullptr,
            &m_UniformTransformMatBuffersMemory[i]
        );

        vkBindBufferMemory
        (
            m_Device, m_UniformTransformMatBuffers[i],
            m_UniformTransformMatBuffersMemory[i], 0
        );
    }

    Out::Log(pType::MESSAGE, "Create Uniform Buffers Done");

    return true;
}

bool VulkanManager::createDescriptorPool()
{
    uint32_t swapChainImagesCount = m_MainWindowData.ImageCount;

    VkDescriptorPoolSize descriptorPoolSize = {};
    descriptorPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorPoolSize.descriptorCount = swapChainImagesCount;

    VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {};
    descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptorPoolCreateInfo.poolSizeCount = 1;
    descriptorPoolCreateInfo.pPoolSizes = &descriptorPoolSize;
    descriptorPoolCreateInfo.maxSets = swapChainImagesCount;

    vkCreateDescriptorPool(m_Device, &descriptorPoolCreateInfo, nullptr, &m_DescriptorPool);

    return true;
}

bool VulkanManager::createDescriptorSets()
{
    uint32_t swapChainImagesCount = m_MainWindowData.ImageCount;

    std::vector<VkDescriptorSetLayout> layouts(swapChainImagesCount, m_DescriptorSetLayout);

    VkDescriptorSetAllocateInfo descriptorSetAllocInfo = {};
    descriptorSetAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    descriptorSetAllocInfo.descriptorPool = m_DescriptorPool;
    descriptorSetAllocInfo.descriptorSetCount = swapChainImagesCount;
    descriptorSetAllocInfo.pSetLayouts = layouts.data();

    m_DescriptorSets.resize(swapChainImagesCount);

    vkAllocateDescriptorSets(m_Device, &descriptorSetAllocInfo, m_DescriptorSets.data());

    for (size_t i = 0; i < swapChainImagesCount; i += 1)
    {
        VkDescriptorBufferInfo bufferInfo = {};
        bufferInfo.buffer = m_UniformTransformMatBuffers[i];
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(UniformMVP);

        VkWriteDescriptorSet descriptorWrite = {};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = m_DescriptorSets[i];
        descriptorWrite.dstBinding = 0;
        descriptorWrite.dstArrayElement = 0;
        descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrite.descriptorCount = 1;
        descriptorWrite.pBufferInfo = &bufferInfo;
        descriptorWrite.pImageInfo = nullptr;
        descriptorWrite.pTexelBufferView = nullptr;

        vkUpdateDescriptorSets(m_Device, 1, &descriptorWrite, 0, nullptr);
    }

    return true;
}

bool VulkanManager::createCommandPool()
{
    VkResult result;

    VkCommandPoolCreateInfo commandPoolCreateInfo = {};
    commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    commandPoolCreateInfo.queueFamilyIndex = m_VulkanInfo.queueFamiliesIndex.m_GraphicsIndex;
    commandPoolCreateInfo.flags = 0;

    result = vkCreateCommandPool(m_Device, &commandPoolCreateInfo, nullptr, &m_CommandPool);

    return true;
}

bool VulkanManager::createCommandBuffers()
{
    VkResult result;

    uint32_t swapChainImagesCount = m_MainWindowData.ImageCount;

    m_CommandBuffers.resize(swapChainImagesCount);

    VkCommandBufferAllocateInfo commandBufferAllocInfo = {};

    commandBufferAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    commandBufferAllocInfo.commandPool = m_CommandPool;
    commandBufferAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    commandBufferAllocInfo.commandBufferCount = swapChainImagesCount;

    result = vkAllocateCommandBuffers(m_Device, &commandBufferAllocInfo, m_CommandBuffers.data());

    for (unsigned int i = 0; i < swapChainImagesCount; i += 1)
    {
        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
        beginInfo.pInheritanceInfo = nullptr;

        result = vkBeginCommandBuffer(m_CommandBuffers[i], &beginInfo);

        VkRenderPassBeginInfo renderPassBeginInfo = {};
        renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassBeginInfo.renderPass = m_RenderPass;
        renderPassBeginInfo.framebuffer = m_MainWindowData.Frames[i].Framebuffer;
        renderPassBeginInfo.renderArea.offset.x = 0;
        renderPassBeginInfo.renderArea.offset.y = 0;
        renderPassBeginInfo.renderArea.extent = m_VulkanInfo.swapChainExtent;

        VkClearValue clearColor = { colorGray.x(),colorGray.y(),colorGray.z(),1.0f };
        renderPassBeginInfo.clearValueCount = 1;
        renderPassBeginInfo.pClearValues = &clearColor;

        vkCmdBeginRenderPass(m_CommandBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(m_CommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_GraphicsPipeline);

        VkBuffer vertexBuffers[] = { m_VertexBuffer };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(m_CommandBuffers[i], 0, 1, vertexBuffers, offsets);

        vkCmdBindDescriptorSets
        (
            m_CommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS,
            m_PipelineLayout, 0, 1, &m_DescriptorSets[i], 0, nullptr
        );

        vkCmdDraw(m_CommandBuffers[i], (uint32_t)m_Vertices.size(), 1, 0, 0);

        vkCmdEndRenderPass(m_CommandBuffers[i]);

        result = vkEndCommandBuffer(m_CommandBuffers[i]);

        if (result != VK_SUCCESS)
        {
            Out::Log(pType::ERROR, "Record Command Failed");
            return false;
        }
    }

    return true;
}

bool VulkanManager::createSemaphores()
{
    VkResult result;

    uint32_t swapChainImagesCount;
    vkGetSwapchainImagesKHR(m_Device, m_SwapChain, &swapChainImagesCount, nullptr);

    m_ImageAvailableSemaphores.resize(FramesInFlightLimit);
    m_RenderFinishedSemaphores.resize(FramesInFlightLimit);

    m_InFlightFences.resize(FramesInFlightLimit);
    m_ImagesInFlight.resize(swapChainImagesCount, VK_NULL_HANDLE);

    VkSemaphoreCreateInfo semaphoreCreateInfo = {};
    semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceCreateInfo = {};
    fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (unsigned int i = 0; i < FramesInFlightLimit; i += 1)
    {
        result = vkCreateSemaphore
        (
            m_Device, &semaphoreCreateInfo, nullptr, &m_ImageAvailableSemaphores[i]
        );
        result = vkCreateSemaphore
        (
            m_Device, &semaphoreCreateInfo, nullptr, &m_RenderFinishedSemaphores[i]
        );

        result = vkCreateFence(m_Device, &fenceCreateInfo, nullptr, &m_InFlightFences[i]);
    }

    return true;
}

bool VulkanManager::Render()
{
    uint64_t max64 = std::numeric_limits<uint64_t>::max();
    vkWaitForFences(m_Device, 1, &m_InFlightFences[m_CurrentFrame], VK_TRUE, max64);

    uint32_t imageIndex;
    vkAcquireNextImageKHR
    (
        m_Device, m_SwapChain, max64, m_ImageAvailableSemaphores[m_CurrentFrame],
        VK_NULL_HANDLE, &imageIndex
    );

    UniformMVP mvp;

    auto swapChainExtent = m_VulkanInfo.swapChainExtent;
    float r = (float)swapChainExtent.width / swapChainExtent.height;

    if (Event::mainCamera.updateView(mvp.m_View))
    {
        mvp.m_Model = glm::mat4(1.0f);
        mvp.m_Project = glm::perspective(glm::radians(45.0f), r, 0.1f, 100.0f);

        void* uniformData;
        vkMapMemory
        (
            m_Device, m_UniformTransformMatBuffersMemory[imageIndex], 0,
            sizeof(UniformMVP), 0, &uniformData
        );

        memcpy(uniformData, &mvp, sizeof(UniformMVP));
        vkUnmapMemory(m_Device, m_UniformTransformMatBuffersMemory[imageIndex]);
    }

    if (m_ImagesInFlight[imageIndex] != VK_NULL_HANDLE)
    {
        vkWaitForFences(m_Device, 1, &m_ImagesInFlight[imageIndex], VK_TRUE, max64);
    }
    m_ImagesInFlight[imageIndex] = m_InFlightFences[m_CurrentFrame];

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[1] = {};
    waitSemaphores[0] = m_ImageAvailableSemaphores[m_CurrentFrame];

    VkPipelineStageFlags waitStages[1] = {};
    waitStages[0] = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_CommandBuffers[imageIndex];

    VkSemaphore signalSemaphores[1] = {};
    signalSemaphores[0] = m_RenderFinishedSemaphores[m_CurrentFrame];

    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    vkResetFences(m_Device, 1, &m_InFlightFences[m_CurrentFrame]);

    VkResult result = vkQueueSubmit(m_GraphicsQueue, 1, &submitInfo, m_InFlightFences[m_CurrentFrame]);

    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[1] = {};
    swapChains[0] = m_SwapChain;

    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = &result;

    vkQueuePresentKHR(m_PresentQueue, &presentInfo);

    vkQueueWaitIdle(m_PresentQueue);

    m_CurrentFrame = (m_CurrentFrame + 1) % FramesInFlightLimit;

    return true;
}

bool VulkanManager::RenderWithUI()
{
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Draw UI
    RenderUI();

    ImGui::Render();

    ImDrawData* drawData = ImGui::GetDrawData();

    auto pMWD = &m_MainWindowData;
    
    VkSemaphore imageAcquiredSemaphore  = pMWD->FrameSemaphores[pMWD->SemaphoreIndex].ImageAcquiredSemaphore;
    VkSemaphore renderCompleteSemaphore = pMWD->FrameSemaphores[pMWD->SemaphoreIndex].RenderCompleteSemaphore;
    auto Result = vkAcquireNextImageKHR(m_Device, pMWD->Swapchain, UINT64_MAX,
        imageAcquiredSemaphore, VK_NULL_HANDLE, &pMWD->FrameIndex);

    if (Result == VK_ERROR_OUT_OF_DATE_KHR ||
        Result == VK_SUBOPTIMAL_KHR ||
        Event::windowResized)
    {
        Event::windowResized = true;
        return false;
    }

    ImGui_ImplVulkanH_Frame* pFD = &pMWD->Frames[pMWD->FrameIndex];

    vkWaitForFences(m_Device, 1, &pFD->Fence, VK_TRUE, UINT64_MAX);
    vkResetFences(m_Device, 1, &pFD->Fence);

    vkResetCommandPool(m_Device, pFD->CommandPool, 0);
    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vkBeginCommandBuffer(pFD->CommandBuffer, &beginInfo);

    VkRenderPassBeginInfo renderPassBeginInfo = {};
    renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBeginInfo.renderPass = pMWD->RenderPass;
    renderPassBeginInfo.framebuffer = pFD->Framebuffer;
    renderPassBeginInfo.renderArea.extent.width = pMWD->Width;
    renderPassBeginInfo.renderArea.extent.height = pMWD->Height;
    renderPassBeginInfo.clearValueCount = 1;
    renderPassBeginInfo.pClearValues = &pMWD->ClearValue;
    vkCmdBeginRenderPass(pFD->CommandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

    ImGui_ImplVulkan_RenderDrawData(drawData, pFD->CommandBuffer);

    vkCmdEndRenderPass(pFD->CommandBuffer);

    vkEndCommandBuffer(pFD->CommandBuffer);

    VkCommandBuffer submitCommandBuffers[2] =
    {
        m_CommandBuffers[pMWD->FrameIndex],
        pFD->CommandBuffer
    };

    VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &imageAcquiredSemaphore;
    submitInfo.pWaitDstStageMask = &waitStage;
    submitInfo.commandBufferCount = 2;
    submitInfo.pCommandBuffers = submitCommandBuffers;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &renderCompleteSemaphore;

    UniformMVP mvp;

    auto swapChainExtent = m_VulkanInfo.swapChainExtent;
    float r = (float)swapChainExtent.width / swapChainExtent.height;

    if (Event::mainCamera.updateView(mvp.m_View))
    {
        mvp.m_Model = glm::mat4(1.0f);
        mvp.m_Project = glm::perspective(glm::radians(45.0f), r, 0.1f, 100.0f);

        void* uniformData;
        vkMapMemory
        (
            m_Device, m_UniformTransformMatBuffersMemory[pMWD->FrameIndex], 0,
            sizeof(UniformMVP), 0, &uniformData
        );

        memcpy(uniformData, &mvp, sizeof(UniformMVP));
        vkUnmapMemory(m_Device, m_UniformTransformMatBuffersMemory[pMWD->FrameIndex]);
    }

    vkQueueSubmit(m_GraphicsQueue, 1, &submitInfo, pFD->Fence);

    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &renderCompleteSemaphore;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &pMWD->Swapchain;
    presentInfo.pImageIndices = &pMWD->FrameIndex;
    vkQueuePresentKHR(m_GraphicsQueue, &presentInfo);

    pMWD->SemaphoreIndex = (pMWD->SemaphoreIndex + 1) % pMWD->ImageCount;

    return true;
}

bool VulkanManager::RenderUI()
{
    //Static
    static VulkanMisc::VulkanUIState uiState;

    //Layout
    const uint32_t ToolBoxWidth = 256;
    const uint32_t MenuBarHeight = 84;
    const uint32_t MainMenuHeight = 20;

    //Get States
    ImGuiIO IO = ImGui::GetIO();
    float displayWidth = (float)IO.DisplaySize.x;
    float displayHeight = (float)IO.DisplaySize.y;

    // Main Menu Bar
    {// Main Menu Bar Start
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("Help"))
            {
                if (ImGui::MenuItem("About"))
                {
                    uiState.showAbout = true;
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        if (uiState.showAbout)
        {
            if (ImGui::Begin("About reEngine", &uiState.showAbout, ImGuiWindowFlags_NoResize))
            {
                ImGui::Text("reEngine");
                ImGui::Separator();
                ImGui::Text("reEngine is a OpenSource Game Engine");
                ImGui::Text("Coded by GZhonghui mail@gzher.com");

                ImGui::End();
            }
        }
    }// Main Menu Bar End

    // Menu Bar
    {// Menu Bar Start
        ImGui::SetNextWindowPos(ImVec2(ToolBoxWidth, MainMenuHeight), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(displayWidth - ToolBoxWidth, MenuBarHeight), ImGuiCond_Always);

        if (ImGui::Begin("reEngine MenuBar", nullptr, ImGuiWindowFlags_NoResize))
        {
            if (ImGui::BeginTabBar("Tool Tab"))
            {
                if (ImGui::BeginTabItem("Actors"))
                {
                    if (ImGui::Button("Add Actor"))
                    {

                    }

                    ImGui::SameLine();
                    if (ImGui::Button("Delete Seleted"))
                    {

                    }

                    ImGui::SameLine();
                    if (ImGui::Button("Load Model"))
                    {

                    }

                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Build"))
                {
                    if (ImGui::Button("Generate"))
                    {

                    }

                    ImGui::EndTabItem();
                }

                ImGui::EndTabBar();
            }

            ImGui::End();
        }
    }// Menu Bar End

    // Tool Box
    {// Tool Box Start
        ImGui::SetNextWindowPos(ImVec2(0, MainMenuHeight), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(ToolBoxWidth, displayHeight - MainMenuHeight), ImGuiCond_Always);

        if (ImGui::Begin("reEngine ToolBox", nullptr, ImGuiWindowFlags_NoResize))
        {
            ImGui::Text("Now FPS: %d", m_FPS);
            ImGui::Separator();

            if (ImGui::CollapsingHeader("Scene Actors"))
            {

            }

            if (ImGui::CollapsingHeader("Actor Properities"))
            {

            }

            if (ImGui::CollapsingHeader("Game Info"))
            {
                ImGui::Text("Camera Move Speed");
                ImGui::SliderFloat("", &Event::cameraMoveSpeed, 1.0f, 10.0f, "Speed = %.3f");
                ImGui::Separator();

                ImGui::Text("Camera Position");
                ImGui::Text("X = %.3f", Event::mainCamera.m_Position.x());
                ImGui::Text("Y = %.3f", Event::mainCamera.m_Position.y());
                ImGui::Text("Z = %.3f", Event::mainCamera.m_Position.z());
                ImGui::Separator();

                ImGui::Text("reEngine");
            }

            ImGui::End();
        }
    }// Tool Box End

    return true;
}

bool VulkanManager::loadScene()
{
    //Load Default Scene
    m_Vertices.push_back(VulkanMisc::Vertex(Convert(Point(-5, 0, 5))));
    m_Vertices.push_back(VulkanMisc::Vertex(Convert(Point( 5, 0, 5))));
    m_Vertices.push_back(VulkanMisc::Vertex(Convert(Point( 5, 0,-5))));

    m_Vertices.push_back(VulkanMisc::Vertex(Convert(Point(-5, 0, 5))));
    m_Vertices.push_back(VulkanMisc::Vertex(Convert(Point( 5, 0,-5))));
    m_Vertices.push_back(VulkanMisc::Vertex(Convert(Point(-5, 0,-5))));

    return true;
}

bool VulkanManager::initGLFW()
{
    if (!glfwInit())
    {
        Out::Log(pType::ERROR, "GLFW Init Failed");

        return false;
    }

    //No OpenGL API
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    // Resize Window
    // glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    m_Window = glfwCreateWindow(m_wWidth, m_wHeight, "reEngine", nullptr, nullptr);

    if (!m_Window)
    {
        Out::Log(pType::ERROR, "GLFW cant create window");
        return false;
    }

    glfwSetWindowSizeLimits(m_Window, m_wWidth, m_wHeight, GLFW_DONT_CARE, GLFW_DONT_CARE);

    glfwSetKeyCallback(m_Window, Event::glfwKeyCallback);
    glfwSetCursorPosCallback(m_Window, Event::glfwCursorPositionCallback);
    glfwSetMouseButtonCallback(m_Window, Event::glfwMouseButtonCallback);
    glfwSetWindowSizeCallback(m_Window, Event::glfwWindowResizeCallback);

    glfwSetInputMode(m_Window, GLFW_STICKY_KEYS, GLFW_TRUE);
    glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    Out::Log(pType::MESSAGE, "Init GLFW Done");
    return true;
}

bool VulkanManager::initVulkan()
{
    createInstance();
    createSurface();

    createPhysicalDevice();
    createLogicalDevice();
    createQueue();

    createSwapChain();
    createImages();
    createImageViews();

    createShader();
    createRenderPass();
    createDescriptorSetLayout();

    createGraphicsPipeline();

    createFrameBuffers();
    createVertexBuffer();
    createUniformBuffers();

    createDescriptorPool();
    createDescriptorSets();

    createCommandPool();
    createCommandBuffers();

    createSemaphores();

    return true;
}

bool VulkanManager::initVulkanBeforeUI()
{
    createInstance();
    createSurface();

    createPhysicalDevice();
    createLogicalDevice();
    createQueue();

    return true;
}

bool VulkanManager::initVulkanAfterUI()
{
    createShader();
    createRenderPass();
    createDescriptorSetLayout();

    createGraphicsPipeline();

    createVertexBuffer();
    createUniformBuffers();

    createDescriptorPool();
    createDescriptorSets();

    createCommandPool();
    createCommandBuffers();

    return true;
}

bool VulkanManager::initVulkanAfterResize()
{
    int newWidth, newHeight;
    glfwGetFramebufferSize(m_Window, &newWidth, &newHeight);

    VulkanMisc::Tools::checkSwapchainSupport
    (
        m_PhysicalDevice, m_Surface,
        m_VulkanInfo.swapChainDetails
    );

    VulkanMisc::Tools::chooseSwapExtent
    (
        newWidth, newHeight,
        m_VulkanInfo.swapChainDetails.m_Capabilities,
        m_VulkanInfo.swapChainExtent
    );

    createRenderPass();
    
    createGraphicsPipeline();

    createCommandBuffers();

    return true;
}

bool VulkanManager::initUI()
{
    VkDescriptorPoolSize poolSizes[] =
    {
        { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
        { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
        { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
        { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
    };
    VkDescriptorPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    poolInfo.maxSets = 1000 * IM_ARRAYSIZE(poolSizes);
    poolInfo.poolSizeCount = (uint32_t)IM_ARRAYSIZE(poolSizes);
    poolInfo.pPoolSizes = poolSizes;
    vkCreateDescriptorPool(m_Device, &poolInfo, nullptr, &m_UIPool);

    m_MainWindowData.Surface = m_Surface;
    m_MainWindowData.SurfaceFormat = m_VulkanInfo.swapSurfaceFormat;
    m_MainWindowData.PresentMode = m_VulkanInfo.swapPresentMode;

    m_MainWindowData.ClearEnable = false;

    ImGui_ImplVulkanH_CreateOrResizeWindow
    (
        m_Instance, m_PhysicalDevice, m_Device, &m_MainWindowData,
        m_VulkanInfo.queueFamiliesIndex.m_GraphicsIndex, nullptr,
        m_wWidth, m_wHeight, MinSwapChainSize
    );

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsLight();

    //GLFW
    ImGui_ImplGlfw_InitForVulkan(m_Window, true);

    //Vulkan
    ImGui_ImplVulkan_InitInfo initInfo = {};
    initInfo.Instance = m_Instance;
    initInfo.PhysicalDevice = m_PhysicalDevice;
    initInfo.Device = m_Device;
    initInfo.QueueFamily = m_VulkanInfo.queueFamiliesIndex.m_GraphicsIndex;
    initInfo.Queue = m_GraphicsQueue;
    initInfo.PipelineCache = VK_NULL_HANDLE;
    initInfo.DescriptorPool = m_UIPool;
    initInfo.Allocator = nullptr;
    initInfo.MinImageCount = MinSwapChainSize;
    initInfo.ImageCount = m_MainWindowData.ImageCount;
    initInfo.CheckVkResultFn = nullptr;
    ImGui_ImplVulkan_Init(&initInfo, m_MainWindowData.RenderPass);

    auto pMWD = &m_MainWindowData;
    VkCommandPool commandPool = pMWD->Frames[pMWD->FrameIndex].CommandPool;
    VkCommandBuffer commandBuffer = pMWD->Frames[pMWD->FrameIndex].CommandBuffer;

    vkResetCommandPool(m_Device, commandPool, 0);

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);

    VkSubmitInfo endInfo = {};
    endInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    endInfo.commandBufferCount = 1;
    endInfo.pCommandBuffers = &commandBuffer;
    vkEndCommandBuffer(commandBuffer);

    vkQueueSubmit(m_GraphicsQueue, 1, &endInfo, VK_NULL_HANDLE);

    vkDeviceWaitIdle(m_Device);

    ImGui_ImplVulkan_DestroyFontUploadObjects();

    Out::Log(pType::MESSAGE, "Init UI Successfully");
    Out::Log(pType::MESSAGE, "Image Count %d", pMWD->ImageCount);

    return true;
}

bool VulkanManager::destroyGLFW()
{
    if (m_Window)
    {
        glfwDestroyWindow(m_Window);
    }
    glfwTerminate();

    Out::Log(pType::MESSAGE, "Cleaned GLFW");

    return true;
}

bool VulkanManager::destroyVulkan()
{
    uint32_t swapChainImagesCount = m_MainWindowData.ImageCount;

    vkFreeCommandBuffers
    (
        m_Device, m_CommandPool,
        static_cast<uint32_t>(m_CommandBuffers.size()), m_CommandBuffers.data()
    );

    vkDestroyPipeline(m_Device, m_GraphicsPipeline, nullptr);
    vkDestroyPipelineLayout(m_Device, m_PipelineLayout, nullptr);
    vkDestroyRenderPass(m_Device, m_RenderPass, nullptr);

    vkDestroySwapchainKHR(m_Device, m_SwapChain, nullptr);

    for (auto i = m_SwapChainFrameBuffers.begin(); i != m_SwapChainFrameBuffers.end(); ++i)
    {
        vkDestroyFramebuffer(m_Device, *i, nullptr);
    }

    for (auto i = m_SwapChainImageViews.begin(); i != m_SwapChainImageViews.end(); ++i)
    {
        vkDestroyImageView(m_Device, *i, nullptr);
    }

    for (size_t i = 0; i < swapChainImagesCount; i += 1)
    {
        vkDestroyBuffer(m_Device, m_UniformTransformMatBuffers[i], nullptr);
        vkFreeMemory(m_Device, m_UniformTransformMatBuffersMemory[i], nullptr);
    }

    vkDestroyDescriptorPool(m_Device, m_DescriptorPool, nullptr);
    vkDestroyDescriptorPool(m_Device, m_UIPool, nullptr);

    vkDestroyDescriptorSetLayout(m_Device, m_DescriptorSetLayout, nullptr);

    vkDestroyBuffer(m_Device, m_VertexBuffer, nullptr);
    vkFreeMemory(m_Device, m_VertexBufferMemory, nullptr);

    vkDestroyShaderModule(m_Device, m_ShaderFragment, nullptr);
    vkDestroyShaderModule(m_Device, m_ShaderVertex, nullptr);

    for (unsigned int i = 0; i < FramesInFlightLimit; i += 1)
    {
        vkDestroySemaphore(m_Device, m_ImageAvailableSemaphores[i], nullptr);
        vkDestroySemaphore(m_Device, m_RenderFinishedSemaphores[i], nullptr);

        vkDestroyFence(m_Device, m_InFlightFences[i], nullptr);
    }

    vkDestroyCommandPool(m_Device, m_CommandPool, nullptr);

    vkDestroyDevice(m_Device, nullptr);

    vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);

    vkDestroyInstance(m_Instance, nullptr);

    Out::Log(pType::MESSAGE, "Cleaned Vulkan");

    return true;
}

bool VulkanManager::destroyVulkanWithUI()
{
    vkFreeCommandBuffers
    (
        m_Device, m_CommandPool,
        static_cast<uint32_t>(m_CommandBuffers.size()), m_CommandBuffers.data()
    );

    for (size_t i = 0; i < m_UniformTransformMatBuffers.size(); i += 1)
    {
        vkDestroyBuffer(m_Device, m_UniformTransformMatBuffers[i], nullptr);
        vkFreeMemory(m_Device, m_UniformTransformMatBuffersMemory[i], nullptr);
    }

    vkDestroyBuffer(m_Device, m_VertexBuffer, nullptr);
    vkFreeMemory(m_Device, m_VertexBufferMemory, nullptr);

    vkDestroyCommandPool(m_Device, m_CommandPool, nullptr);

    vkDestroyPipeline(m_Device, m_GraphicsPipeline, nullptr);
    vkDestroyPipelineLayout(m_Device, m_PipelineLayout, nullptr);
    vkDestroyRenderPass(m_Device, m_RenderPass, nullptr);

    vkDestroyDescriptorPool(m_Device, m_DescriptorPool, nullptr);
    vkDestroyDescriptorPool(m_Device, m_UIPool, nullptr);

    vkDestroyDescriptorSetLayout(m_Device, m_DescriptorSetLayout, nullptr);

    vkDestroyShaderModule(m_Device, m_ShaderFragment, nullptr);
    vkDestroyShaderModule(m_Device, m_ShaderVertex, nullptr);

    vkDestroyDevice(m_Device, nullptr);

    // Do Not Destroy Surface Here

    vkDestroyInstance(m_Instance, nullptr);

    Out::Log(pType::MESSAGE, "Cleaned Vulkan");

    return true;
}

bool VulkanManager::destroyVulkanAfterResize()
{
    vkFreeCommandBuffers
    (
        m_Device, m_CommandPool,
        static_cast<uint32_t>(m_CommandBuffers.size()), m_CommandBuffers.data()
    );

    vkDestroyPipeline(m_Device, m_GraphicsPipeline, nullptr);

    vkDestroyRenderPass(m_Device, m_RenderPass, nullptr);

    return true;
}

bool VulkanManager::destroyUI()
{
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    ImGui_ImplVulkanH_DestroyWindow(m_Instance, m_Device, &m_MainWindowData, nullptr);
    
    Out::Log(pType::MESSAGE, "Cleaned UI");

    return true;
}

bool VulkanManager::Init()
{
    loadScene();

    initGLFW();
    
    initVulkanBeforeUI();
    initUI();
    initVulkanAfterUI();

    Out::Log(pType::MESSAGE, "Init Done");

    return true;
}

bool VulkanManager::Destroy()
{
    destroyUI();
    destroyVulkanWithUI();

    destroyGLFW();

    return true;
}

void VulkanManager::Loop()
{
    static time_t lastTime = time(nullptr);
    static unsigned int renderedFrame = 0;

    while (!glfwWindowShouldClose(m_Window) && !Event::shouldQuit)
    {
        glfwPollEvents();

        Event::Loop(m_Window);

        if (m_VerticesNeedUpdate)
        {
            uint32_t bufferSize = sizeof(VulkanMisc::Vertex) * m_Vertices.size();

            void* vertexData;
            vkMapMemory(m_Device, m_VertexBufferMemory, 0, bufferSize, 0, &vertexData);

            memcpy(vertexData, m_Vertices.data(), (size_t)bufferSize);
            vkUnmapMemory(m_Device, m_VertexBufferMemory);

            m_VerticesNeedUpdate = false;
        }

        if (Event::windowResized)
        {
            int newWidth, newHeight;
            glfwGetFramebufferSize(m_Window, &newWidth, &newHeight);

            if (newWidth > 0 && newHeight > 0)
            {
                vkDeviceWaitIdle(m_Device);

                ImGui_ImplVulkan_SetMinImageCount(2);
                ImGui_ImplVulkanH_CreateOrResizeWindow
                (
                    m_Instance, m_PhysicalDevice, m_Device,
                    &m_MainWindowData, m_VulkanInfo.queueFamiliesIndex.m_GraphicsIndex,
                    nullptr, newWidth, newHeight, MinSwapChainSize
                );
                m_MainWindowData.FrameIndex = 0;

                destroyVulkanAfterResize();
                initVulkanAfterResize();

                Out::Log(pType::MESSAGE, "Resize Window %d * %d", newWidth, newHeight);

                Event::windowResized = false;
            }
        }

        RenderWithUI();

        time_t nowTime = time(nullptr);

        if (lastTime != nowTime)
        {
            m_FPS = renderedFrame;
            renderedFrame = 0;
        }
        else
        {
            renderedFrame += 1;
        }

        lastTime = nowTime;
    }

    vkDeviceWaitIdle(m_Device);
}