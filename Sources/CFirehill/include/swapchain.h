#pragma once
#include "core.h"
#include <glfw/glfw3.h>

VkSurfaceKHR fhCreateVkSurface(VkInstance instance, GLFWwindow *window);

VkSwapchainKHR fhRecreateSwapChain(
    VkPhysicalDevice device, 
    VkDevice logicalDevice, 
    VkSurfaceKHR surface,
    VkFormat *format,
    VkExtent2D *extent,
    GLFWwindow *window
    );
void fhDeleteSwapChainKHR(VkDevice device, VkSwapchainKHR swapchain);