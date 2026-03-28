#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <map>
#include <optional>

#include "logging.h"


// struct for storing queue families

struct QueueFamilyIndices {
    
    std::optional<uint32_t> graphicsFamily;


    // mandatory things are added here so they are validated
    bool isComplete() {
        return graphicsFamily.has_value();
    }
};

class VulkanTutApplication 
{

private:
    GLFWwindow* window;
    VkInstance instance; 
    VkDevice device;

    // VkDebugUtilsMessengerEXT debugMessenger; - when implement validation layers

    // stores the gpu we will end up using
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE; // implicitly destoroyed when VkInstance is destroyed

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
    void pickPhysicalDevice();
    void createLogicalDevice();



    bool isDeviceSuitable(VkPhysicalDevice device);

    // FIND QUEUE FAMILIES FOR PHYSICAL DEVICE (GPU) AND ADD TO QueueFamilyIndicies STRUCT
    // check which queue families are supported by the device and which one of these supports the commands that we want to use

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

    






    
};




