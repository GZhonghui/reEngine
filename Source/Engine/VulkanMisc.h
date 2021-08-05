#pragma once

#include"MathAIO.h"
#include"ToolAIO.h"

class Vertex
{
public:
    glm::vec3 m_Position;

public:
    Vertex() = default;
    Vertex(const glm::vec3& position) :m_Position(position) {}

public:
    ~Vertex() = default;

public:
    static VkVertexInputBindingDescription getBindingDescription()
    {
        VkVertexInputBindingDescription bindingDescription = {};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::vector<VkVertexInputAttributeDescription> getAttributeDescription()
    {
        std::vector<VkVertexInputAttributeDescription> attributeDescription;

        attributeDescription.resize(1);

        attributeDescription[0].binding = 0;
        attributeDescription[0].location = 0;
        attributeDescription[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescription[0].offset = offsetof(Vertex, m_Position);

        return attributeDescription;
    }

public:
    friend class VulkanManager;
};

class QueueFamiliesIndices
{
public:
    int m_GraphicsIndex;
    int m_PresentIndex;

public:
    QueueFamiliesIndices()
    {
        m_GraphicsIndex = -1;
        m_PresentIndex = -1;
    }

public:
    ~QueueFamiliesIndices() = default;

public:
    bool Complete() const
    {
        if (m_GraphicsIndex < 0 || m_PresentIndex < 0)
        {
            return false;
        }

        return true;
    }

    void Clear()
    {
        m_GraphicsIndex = -1;
        m_PresentIndex = -1;
    }
};

class SwapChainDetails
{
public:
    VkSurfaceCapabilitiesKHR m_Capabilities;
    std::vector<VkSurfaceFormatKHR> m_Formats;
    std::vector<VkPresentModeKHR> m_PresentModes;

public:
    void Clear()
    {
        m_Formats.clear();
        m_PresentModes.clear();
    }
};

class VulkanInfo
{
public:
    VulkanInfo() = default;
    ~VulkanInfo() = default;

public:
    QueueFamiliesIndices queueFamiliesIndex;
    SwapChainDetails swapChainDetails;

    VkSurfaceFormatKHR swapSurfaceFormat;
    VkPresentModeKHR swapPresentMode;
    VkExtent2D swapChainExtent;
};

class VulkanMisc
{
public:
    VulkanMisc() = default;
    ~VulkanMisc() = default;

    //Checking Device
public:
    static bool checkQueueFamilies
    (
        const VkPhysicalDevice& device,
        const VkSurfaceKHR& surface,
        QueueFamiliesIndices& queueFamiliesIndex
    )
    {
        uint32_t queueFamiliesCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties
        (
            device, &queueFamiliesCount, nullptr
        );

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamiliesCount);
        vkGetPhysicalDeviceQueueFamilyProperties
        (
            device, &queueFamiliesCount, queueFamilies.data()
        );

        queueFamiliesIndex.Clear();

        int i = 0;
        auto pQF = &queueFamilies;

        for (auto qIndex = pQF->begin(); qIndex != pQF->end(); ++qIndex)
        {
            if (qIndex->queueCount > 0 && 
                qIndex->queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                queueFamiliesIndex.m_GraphicsIndex = i;
            }

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR
            (
                device, i, surface, &presentSupport
            );

            if (qIndex->queueCount > 0 && presentSupport)
            {
                queueFamiliesIndex.m_PresentIndex = i;
            }

            if (queueFamiliesIndex.Complete())
            {
                break;
            }

            i += 1;
        }

        if (!queueFamiliesIndex.Complete())
        {
            return false;
        }

        return true;
    }

    static bool checkSupportExtensions
    (
        const VkPhysicalDevice& device,
        const std::vector<const char*>& needExtensionsChar
    )
    {
        uint32_t deviceExtensionsCount;
        vkEnumerateDeviceExtensionProperties
        (
            device, nullptr, &deviceExtensionsCount, nullptr
        );

        std::vector<VkExtensionProperties> availableExtensions;
        availableExtensions.resize(deviceExtensionsCount);

        vkEnumerateDeviceExtensionProperties
        (
            device, nullptr, &deviceExtensionsCount, availableExtensions.data()
        );

        std::unordered_set<std::string> needExtensions;

        auto pNEC = &needExtensionsChar;
        for (auto extension = pNEC->begin(); extension != pNEC->end(); ++extension)
        {
            needExtensions.insert(std::string(*extension));
        }

        auto pAE = &availableExtensions;
        for (auto extension = pAE->begin(); extension != pAE->end(); ++extension)
        {
            if (needExtensions.count(extension->extensionName))
            {
                needExtensions.erase(extension->extensionName);
            }
        }

        if (!needExtensions.empty())
        {
            return false;
        }

        return true;
    }

    static bool checkSwapchainSupport
    (
        const VkPhysicalDevice& device,
        const VkSurfaceKHR& surface,
        SwapChainDetails& swapChainDetails
    )
    {
        swapChainDetails.Clear();

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR
        (
            device, surface, &swapChainDetails.m_Capabilities
        );

        //Format
        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR
        (
            device, surface, &formatCount, nullptr
        );

        if (formatCount)
        {
            swapChainDetails.m_Formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR
            (
                device, surface, &formatCount, swapChainDetails.m_Formats.data()
            );
        }

        //PresentMode
        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR
        (
            device, surface, &presentModeCount, nullptr
        );

        if (presentModeCount)
        {
            swapChainDetails.m_PresentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR
            (
                device, surface, &presentModeCount,
                swapChainDetails.m_PresentModes.data()
            );
        }

        return true;
    }

    static bool checkDevice
    (
        const VkPhysicalDevice& device,
        const VkSurfaceKHR& surface,
        const std::vector<const char*>& needExtensionsChar
    )
    {
        VkPhysicalDeviceProperties deviceProperties;
        VkPhysicalDeviceFeatures deviceFeatures;

        vkGetPhysicalDeviceProperties(device, &deviceProperties);
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

        QueueFamiliesIndices queueFamiliesIndex;
        checkQueueFamilies(device, surface, queueFamiliesIndex);

        if (!queueFamiliesIndex.Complete())
        {
            return false;
        }

        if (!checkSupportExtensions(device, needExtensionsChar))
        {
            return false;
        }

        SwapChainDetails swapChainDetails;
        checkSwapchainSupport(device, surface, swapChainDetails);

        if (swapChainDetails.m_Formats.empty() ||
            swapChainDetails.m_PresentModes.empty())
        {
            return false;
        }

        return true;
    }

    //Creating Swapchain
public:
    static bool chooseSwapSurfaceFormat
    (
        const std::vector<VkSurfaceFormatKHR>& formats, VkSurfaceFormatKHR& choice
    )
    {
        if (formats.empty())
        {
            return false;
        }

        VkFormat expectFormat = VK_FORMAT_B8G8R8A8_UNORM;
        VkColorSpaceKHR expectColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;

        if (formats.size() == 1 && formats.begin()->format == VK_FORMAT_UNDEFINED)
        {
            choice.format = expectFormat;
            choice.colorSpace = expectColorSpace;
            return true;
        }

        for (auto format = formats.begin(); format != formats.end(); ++format)
        {
            if (format->format == expectFormat &&
                format->colorSpace == expectColorSpace)
            {
                choice = *format;
                return true;
            }
        }

        Out::Log(pType::WARNING, "No expect Format, Use the default");
        choice = *formats.begin();

        return true;
    }

    static bool chooseSwapPresentMode
    (
        const std::vector<VkPresentModeKHR>& modes, VkPresentModeKHR& choice
    )
    {
        std::vector<VkPresentModeKHR> expectModes;

        expectModes.push_back(VK_PRESENT_MODE_FIFO_KHR);
        expectModes.push_back(VK_PRESENT_MODE_MAILBOX_KHR);
        expectModes.push_back(VK_PRESENT_MODE_IMMEDIATE_KHR);
        expectModes.push_back(VK_PRESENT_MODE_FIFO_RELAXED_KHR);

        std::unordered_set<VkPresentModeKHR> supportModes;
        for (auto mode = modes.begin(); mode != modes.end(); ++mode)
        {
            supportModes.insert(*mode);
        }

        for (auto mode = expectModes.begin(); mode != expectModes.end(); ++mode)
        {
            if (supportModes.count(*mode))
            {
                choice = *mode;
                return true;
            }
        }

        Out::Log(pType::ERROR, "No expect PresentMode");
        return false;
    }

    static bool chooseSwapExtent
    (
        uint32_t windowWidth, uint32_t windowHeight,
        const VkSurfaceCapabilitiesKHR& capabilities, VkExtent2D& choice
    )
    {
        if (capabilities.currentExtent.width != 
            std::numeric_limits<uint32_t>::max())
        {
            choice = capabilities.currentExtent;
            return true;
        }

        choice.width = windowWidth;
        choice.height = windowHeight;

        using std::min;
        using std::max;

        choice.width = min(choice.width, capabilities.maxImageExtent.width);
        choice.width = max(choice.width, capabilities.minImageExtent.width);

        choice.height = min(choice.height, capabilities.maxImageExtent.height);
        choice.height = max(choice.height, capabilities.minImageExtent.height);

        return true;
    }

    //Create Buffers
public:
    static bool findMemoryType
    (
        uint32_t& result, uint32_t filter,
        const VkMemoryPropertyFlags& properties,
        const VkPhysicalDevice& physicalDevice
    )
    {
        VkPhysicalDeviceMemoryProperties memoryProperties;
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);

        for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i += 1)
        {
            auto U = memoryProperties.memoryTypes[i].propertyFlags & properties;
            if ((filter & (1 << i)) && U == properties)
            {
                result = i;
                return true;
            }
        }

        Out::Log(pType::ERROR, "Cant find suit Memory Type");
        return false;
    }

public:
    friend class VulkanManager;
};

class VulkanUIState
{
public:
    VulkanUIState()
    {
        showAbout = false;

        Out::Log(pType::MESSAGE, "Init UI");
    }

public:
    ~VulkanUIState() = default;

protected:
    bool showAbout;

public:
    friend class VulkanManager;
};