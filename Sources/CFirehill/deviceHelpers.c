#include "include/cfirehill.h"
#include "vulkan/vulkan_core.h"
#include <stdint.h>
#include <stdlib.h>


FhQueueFamilyIndices findQueueFamilies(VkPhysicalDevice device){
    FhQueueFamilyIndices indices = {
        .presentQueueIndex = UINT32_MAX,
        .computeQueueIndex = UINT32_MAX
    };

       uint32_t queueFamilyCount  = 0;
        
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount,  NULL);
        
        VkQueueFamilyProperties queueFamilies[queueFamilyCount];

        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies);

        for (size_t it = 0; it < queueFamilyCount; it++) {
            if (queueFamilies[it].queueFlags &  VK_QUEUE_GRAPHICS_BIT){
                indices.presentQueueIndex = it;
            }
            if(queueFamilies[it].queueCount > 0 &&( queueFamilies[it].queueFlags & VK_QUEUE_COMPUTE_BIT) && !(queueFamilies[it].queueFlags & VK_QUEUE_GRAPHICS_BIT)){
                indices.computeQueueIndex = it;
            }
            if(indices.presentQueueIndex != UINT32_MAX && indices.computeQueueIndex != UINT32_MAX) break;
        }
    return indices;
}


_Bool checkDeviceExtensionSupport(VkPhysicalDevice device, const char *requiredExtension){
    uint32_t extensionCount = 0;
    vkEnumerateDeviceExtensionProperties(device, NULL, &extensionCount, NULL);
    VkExtensionProperties availableExtensions[extensionCount];
    vkEnumerateDeviceExtensionProperties(device, NULL, &extensionCount, availableExtensions);
    _Bool result = false;
    for(size_t it = 0; it < extensionCount; it++){
       if(strcmp(requiredExtension, availableExtensions[it].extensionName)) result = true;
    }
    return result;
}

bool querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface, FhSwapChainSupportDetails* supportDetails){
    bool result = false;
    
    
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &supportDetails->capabilities);

    // quering formats
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &supportDetails->formtCount, NULL);
    
    supportDetails->formats = malloc(sizeof(VkSurfaceFormatKHR) * supportDetails->formtCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &supportDetails->formtCount, supportDetails->formats);


    for (size_t it = 0; it < supportDetails->formtCount ; it++) {
        printf("formats: %i %i\n", supportDetails->formats[it].format, supportDetails->formats[it].colorSpace);
    }

 
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &supportDetails->presentModeCount, NULL);

    supportDetails->presentModes = malloc(sizeof(VkPresentModeKHR) * supportDetails->presentModeCount);


    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &supportDetails->presentModeCount, supportDetails->presentModes);

    for (size_t it = 0; it < supportDetails->presentModeCount ; it++) {
        printf("presentModes: %i\n",supportDetails->presentModes[it]);
    }

    
    printf("minImage: %i, maxImage: %i, minHeight: %i minWidth: %i maxHeight: %i maxWidth %i\n", 
    supportDetails->capabilities.minImageCount, 
    supportDetails->capabilities.maxImageCount, 
    supportDetails->capabilities.minImageExtent.height, 
    supportDetails->capabilities.minImageExtent.width, 
    supportDetails->capabilities.maxImageExtent.height, 
    supportDetails->capabilities.maxImageExtent.width);
    if(supportDetails->presentModeCount != 0 && supportDetails->formtCount != 0) result = true;
    return result;

}

void deleteSwapChainSupportDeletes(FhSwapChainSupportDetails* swainChainSupportDetails){
    free(swainChainSupportDetails->formats);
    swainChainSupportDetails->formats = NULL;
    free(swainChainSupportDetails->presentModes);
    swainChainSupportDetails->presentModes = NULL;
}


_Bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface){
    bool result = checkDeviceExtensionSupport(device, VK_KHR_SWAPCHAIN_EXTENSION_NAME);
    FhQueueFamilyIndices indices = findQueueFamilies(device);
    if(indices.presentQueueIndex == UINT32_MAX || indices.computeQueueIndex == UINT32_MAX){
        result = false;
    };
    FhSwapChainSupportDetails supportDetails = {};
    result = querySwapChainSupport(device, surface, &supportDetails);
    deleteSwapChainSupportDeletes(&supportDetails);
    return result;
}


