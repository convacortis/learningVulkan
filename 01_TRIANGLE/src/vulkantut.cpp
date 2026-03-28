#include "vulkantut.hpp"

int main() 
{
    VulkanTutApplication app;

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}


void VulkanTutApplication::initWindow()
{
    
    // initialise glfw
    glfwInit();

    int platform = glfwGetPlatform();

    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_WAYLAND);

    // do not create opengl context
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    
    // disable window resizing
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    // shows the window
    glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);

    // create window
    window = glfwCreateWindow(800, 600, "Vulkan", nullptr, nullptr);

    glfwShowWindow(window);  // explicitly show it
    glfwFocusWindow(window); // and focus it

    LOG_INFO("Successfully initialised window");


}

// create a vulkan instance and configure preferences
void VulkanTutApplication::createInstance()
{
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "no engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_LOADER_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;

    createInfo.enabledLayerCount = 0;
    
    if ( vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS )
    {
        throw std::runtime_error("failed to create instance!");
    }
}

void VulkanTutApplication::initVulkan() 
{
    createInstance();
    pickPhysicalDevice();
    createLogicalDevice();
}


void VulkanTutApplication::mainLoop() 
{
    while (!glfwWindowShouldClose(window)) 
    {
        glfwPollEvents();

    }
}
void VulkanTutApplication::cleanup() 
{
    vkDestroyInstance(instance, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();
}










void VulkanTutApplication::pickPhysicalDevice()
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0) 
    {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    for (const auto& device : devices) 
    {
        if (isDeviceSuitable(device)) 
        {
            physicalDevice = device;
            break;
        }
    }
    
    if (physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("failed to find a suitable GPU!");

    }


}




void VulkanTutApplication::createLogicalDevice() 
{

    QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
    queueCreateInfo.queueCount = 1;


}






// only accept dedicated graphics card

bool VulkanTutApplication::isDeviceSuitable(VkPhysicalDevice device) 
{ 
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && deviceFeatures.geometryShader;
}

/* for future
int VulkanTutApplication::rateDeviceSuitability(VkPhysicalDevice device)
{

    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    
    int score = 0;

    // Discrete GPUs have a significant performance advantage
    if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        score += 1000;
    }

    // Maximum possible size of textures affects graphics quality
    score += deviceProperties.limits.maxImageDimension2D;

    // Application can't function without geometry shaders
    if (!deviceFeatures.geometryShader) {
        return 0;
    }

    return score;

}
*/

/*

// guide uses

bool isDeviceSuitable(VkPhysicalDevice device) {
    return true;
}


*/
QueueFamilyIndices VulkanTutApplication::findQueueFamilies(VkPhysicalDevice device)
{
    QueueFamilyIndices indices;


    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
    
    int i = 0;

    for (const auto& queueFamily : queueFamilies)
    {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) 
            {
                indices.graphicsFamily = i;
            }

        if (indices.isComplete()) {
            break;
        }
    }

    return indices;
}






