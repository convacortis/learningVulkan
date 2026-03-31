#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <map>
#include <optional>

#include "logging.h"





// validation layers

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

// if not debug
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


    // mandatory things are added here so they are validated
    bool isComplete() {
        return graphicsFamily.has_value();
    }
};


class VulkanTutApplication 
{

private:
    const uint32_t WIDTH = 2000;
    const uint32_t HEIGHT = 1200;


    GLFWwindow* window;

    VkInstance instance; 
    VkDebugUtilsMessengerEXT debugMessenger;
    VkDevice device;

    // VkDebugUtilsMessengerEXT debugMessenger; - when implement validation layers

    // stores the gpu we will end up using
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE; // implicitly destoroyed when VkInstance is destroyed

    VkQueue graphicsQueue;


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


    void pickPhysicalDevice();
    void createLogicalDevice();
    
    
    
    bool isDeviceSuitable(VkPhysicalDevice device);
    
    
    std::vector<const char*> getRequiredExtensions();
    bool checkValidationLayerSupport();
    
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) 
    {
        std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

        return VK_FALSE;

    }


    // FIND QUEUE FAMILIES FOR PHYSICAL DEVICE (GPU) AND ADD TO QueueFamilyIndicies STRUCT
    // check which queue families are supported by the device and which one of these supports the commands that we want to use

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);


    
};




