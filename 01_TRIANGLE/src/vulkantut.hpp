#pragma once

// // platform specific stuff - useful for future

// #ifdef _WIN32
//     #define VK_USE_PLATFORM_WIN32_KHR
// #elif defined(__linux__)
//     #define VK_USE_PLATFORM_WAYLAND_KHR
// #endif



#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// // platform specific stuff - useful for future

// #ifdef _WIN32
//     #define GLFW_EXPOSE_NATIVE_WIN32
// #elif defined(__linux__)
//     #define GLFW_EXPOSE_NATIVE_WAYLAND
// #endif


// #include <GLFW/glfw3native.h>


#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <limits>
#include <optional>
#include <set>

#include "logging.h"





// validation layers

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};


// if not debug build don't include validation layers

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif





VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    
    if (func != nullptr) 
    {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } 
    else 
    {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) 
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    
    if (func != nullptr) 
    {
        func(instance, debugMessenger, pAllocator);
    }
}






// struct for storing queue families

struct QueueFamilyIndices 
{
    
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;


    // mandatory things are added here so they are validated
    bool isComplete() {
        return graphicsFamily.has_value();
    }
};

struct SwapChainSupportDetails 
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};




class VulkanTutApplication 
{

private:
    const uint32_t WIDTH = 2000;
    const uint32_t HEIGHT = 1200;


    GLFWwindow* window;

    VkInstance instance; 
    VkDebugUtilsMessengerEXT debugMessenger;
    VkSurfaceKHR surface;

    // stores the gpu we will end up using
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE; // implicitly destoroyed when VkInstance is destroyed

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
    VkPipelineLayout pipelineLayout;    

    VkPipeline graphicsPipeline;

    VkCommandPool commandPool;
    VkCommandBuffer commandBuffer;

    VkSemaphore imageAvailableSemaphore;
    VkSemaphore renderFinishedSemaphore;
    VkFence inFlightFence;


    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) 
    {
        std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

        return VK_FALSE;

    }



public:
    void run() 
    {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

private:

    // create window and vulkan instance
    void initWindow();
    void createInstance();
    void initVulkan();
    void mainLoop();

    // destroy program
    void cleanup();

    // set up graphics card
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    void setupDebugMessenger();


    void createSurface();

    void pickPhysicalDevice();
    void createLogicalDevice();
    void createSwapChain();
    void createImageViews();
    void createRenderPass();

    void createGraphicsPipeline();

    void createFramebuffers();
    void createCommandPool();
    void createCommandBuffer();
    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

    void createSyncObjects();
    void drawFrame();



    VkShaderModule createShaderModule(const std::vector<char>& code);


    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
    
    bool isDeviceSuitable(VkPhysicalDevice device);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    
    
    std::vector<const char*> getRequiredExtensions();
    bool checkValidationLayerSupport();
    

    // readfile will take the file and return a buffer for the size of that file
    static std::vector<char> readFile(const std::string& filename) 
    {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);

        if (!file.is_open()) 
        {
            throw std::runtime_error("failed to open file!");
        }

        size_t fileSize = (size_t) file.tellg();
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();

        return buffer;
    }


    // FIND QUEUE FAMILIES FOR PHYSICAL DEVICE (GPU) AND ADD TO QueueFamilyIndicies STRUCT
    // check which queue families are supported by the device and which one of these supports the commands that we want to use

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

    

    
};




