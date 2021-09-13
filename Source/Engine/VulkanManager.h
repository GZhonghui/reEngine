#pragma once

// Vulkan Renderable for now
#include"VulkanMisc.h"
#include"Event.h"

class VulkanManager
{
protected:
    //FPS
    uint32_t m_FPS;

protected:
    //For ImGUI Swapchain
    const uint32_t MinSwapChainSize = 2;

protected:
    //Window Size
    uint32_t m_wWidth;
    uint32_t m_wHeight;

protected:
    //Extensions
    std::vector<const char*> m_NeedExtensionsChar;

protected:
    //Vertex
    std::vector<VulkanMisc::Vertex> m_Vertices;
    bool m_VerticesNeedUpdate;

protected:
    //Infos
    VulkanMisc::VulkanInfo m_VulkanInfo;

protected:
    //Init GLFW
    GLFWwindow* m_Window;

protected:
    //Creating Instance
    VkInstance m_Instance;

    //Creating Surface
    VkSurfaceKHR m_Surface;

    //Creating Device
    VkPhysicalDevice m_PhysicalDevice;
    VkDevice m_Device;

    //Creating Queue
    VkQueue m_GraphicsQueue;
    VkQueue m_PresentQueue;

    //Creating Swapchain
    VkSwapchainKHR m_SwapChain;

    //Creating Images & Views
    std::vector<VkImage> m_SwapChainImages;
    std::vector<VkImageView> m_SwapChainImageViews;

    //Ready to create Pipeline
    //Shader
    VkShaderModule m_ShaderFragment;
    VkShaderModule m_ShaderVertex;
    //Render Pass
    VkRenderPass m_RenderPass;
    //DescriptorSet Layout
    VkDescriptorSetLayout m_DescriptorSetLayout;

    //Creating Pipeline
    VkPipelineLayout m_PipelineLayout;
    VkPipeline m_GraphicsPipeline;

    //Creating Buffer
    //Frame Buffers
    std::vector<VkFramebuffer> m_SwapChainFrameBuffers;
    //Vertex Buffer
    VkBuffer m_VertexBuffer;
    VkDeviceMemory m_VertexBufferMemory;
    //Uniform Buffers
    std::vector<VkBuffer> m_UniformTransformMatBuffers;
    std::vector<VkDeviceMemory> m_UniformTransformMatBuffersMemory;

    //Creating Description
    //Descriptor Pool
    VkDescriptorPool m_DescriptorPool;
    //Descriptor Sets
    std::vector<VkDescriptorSet> m_DescriptorSets;

    //Creating Command
    //Command Pool
    VkCommandPool m_CommandPool;
    //Command Buffers
    std::vector<VkCommandBuffer> m_CommandBuffers;

    //Creating Semaphores & Fences
    std::vector<VkSemaphore> m_ImageAvailableSemaphores;
    std::vector<VkSemaphore> m_RenderFinishedSemaphores;
    std::vector<VkFence> m_InFlightFences;
    std::vector<VkFence> m_ImagesInFlight;

protected:
    //Use for Semaphores & Fences
    const uint32_t FramesInFlightLimit = 2;
    uint32_t m_CurrentFrame;

    //UI
protected:
    ImGui_ImplVulkanH_Window m_MainWindowData;
    VkDescriptorPool m_UIPool;

public:
    VulkanManager(uint32_t width = 1024, uint32_t height = 768) :
        m_wWidth(width), m_wHeight(height)
    {
        m_NeedExtensionsChar.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

        Event::initEventState();

        m_CurrentFrame = 0;

        m_VerticesNeedUpdate = false;

        m_FPS = 0;
    }

public:
    ~VulkanManager() = default;

    //Steps
protected:
    bool createInstance();
    bool createSurface();

    bool createPhysicalDevice();
    bool createLogicalDevice();
    bool createQueue();

    bool createSwapChain();
    bool createImages();
    bool createImageViews();

    bool createShader();
    bool createRenderPass();
    bool createDescriptorSetLayout();

    bool createGraphicsPipeline();

    bool createFrameBuffers();
    bool createVertexBuffer();
    bool createUniformBuffers();

    bool createDescriptorPool();
    bool createDescriptorSets();

    bool createCommandPool();
    bool createCommandBuffers();

    bool createSemaphores();

protected:
    // Do NOT Use This Function
    bool Render();
    // Use THIS Function
    bool RenderWithUI();
    bool RenderUI();

protected:
    bool loadScene();

protected:
    bool initGLFW();
    // Do NOT Use This Function
    bool initVulkan();
    // Use THESE Functions
    bool initVulkanBeforeUI();
    bool initVulkanAfterUI();
    bool initVulkanAfterResize();
    bool initUI();

protected:
    bool destroyGLFW();
    // Do NOT Use This Function
    bool destroyVulkan();
    // Use THIS Function
    bool destroyVulkanWithUI();
    bool destroyVulkanAfterResize();
    bool destroyUI();

public:
    bool Init();
    bool Destroy();

public:
    void Loop();
};