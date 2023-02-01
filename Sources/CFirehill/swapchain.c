#include "include/cfirehill.h"
#include "vulkan/vulkan_core.h"



VkSurfaceKHR fhCreateVkSurface(VkInstance instance, GLFWwindow *window){
    VkSurfaceKHR surface;
    c_assert(glfwCreateWindowSurface(instance, window, NULL, &surface) == VK_SUCCESS);
    return surface;
}



VkSurfaceFormatKHR chooseSwapSurfaceFormat(VkSurfaceFormatKHR *availableFormats, uint32_t formatCount){
    for (size_t it = 0; it < formatCount; it++) {
        if(availableFormats[it].format == VK_FORMAT_B8G8R8A8_SRGB && availableFormats[it].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR){
            return availableFormats[it];
        }
    }
    return availableFormats[formatCount - 1];
}


VkPresentModeKHR  chooseSwapSurfacePresentMode(VkPresentModeKHR  *available, uint32_t count){
    for (size_t it = 0; it < count; it++) {
        if(available[it] == VK_PRESENT_MODE_MAILBOX_KHR){
            return available[it];
        }
    }
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D chooseSwapExtent(VkSurfaceCapabilitiesKHR capabilities, GLFWwindow *window){
    if(capabilities.currentExtent.width != UINT32_MAX){
        return capabilities.currentExtent;
    }
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    VkExtent2D extent  = {
        .height = height,
        .width = width
    };
    return extent;
}


VkSwapchainKHR fhCreateSwapChain(
    VkPhysicalDevice device, 
    VkDevice logicalDevice, 
    VkSurfaceKHR surface,
    VkFormat *format,
    VkExtent2D *extent,
    GLFWwindow *window
    ){
    FhSwapChainSupportDetails details = {};
    querySwapChainSupport(device,surface, &details);
    
    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(details.formats, details.formtCount);
    
    VkPresentModeKHR  presetMode = chooseSwapSurfacePresentMode(details.presentModes, details.presentModeCount);

    *extent = chooseSwapExtent(details.capabilities, window);
    
    uint32_t imageCount = details.capabilities.minImageCount;
    
    if(details.capabilities.maxImageCount > 3){
        imageCount = 3;
    }
    printf("image count %i\n", imageCount);


    VkSwapchainCreateInfoKHR infos = {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface = surface,
        .minImageCount = imageCount,
        .imageFormat = surfaceFormat.format,
        .imageColorSpace = surfaceFormat.colorSpace,
        .imageExtent = (*extent),
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .preTransform = details.capabilities.currentTransform,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = presetMode,
        .clipped = VK_TRUE,
        .oldSwapchain = VK_NULL_HANDLE
    };
    FhQueueFamilyIndices indices = findQueueFamilies(device);
    uint32_t queueuIndices[] = {indices.computeQueueIndex, indices.presentQueueIndex};
    // @Fixme
    if(indices.computeQueueIndex != indices.presentQueueIndex){
        infos.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        infos.queueFamilyIndexCount = 2;
        infos.pQueueFamilyIndices = queueuIndices;
    }else{
            infos.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            infos.queueFamilyIndexCount = 0; // Optional
            infos.pQueueFamilyIndices = NULL; // Optional
    }
    VkSwapchainKHR swapChain;


    c_assert(vkCreateSwapchainKHR(logicalDevice, &infos,NULL, &swapChain) == VK_SUCCESS);
    *format = surfaceFormat.format;
    return swapChain;
}

void fhGetSwapChainImages(VkDevice device, VkSwapchainKHR swapChain, void(*callback)(VkImage)){
    VkImageViewCreateInfo info = {};
}


VkImageViewCreateInfo fhCreateImageViewInfo(VkImage image, VkFormat format){
    VkImageViewCreateInfo createInfo = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .image = image,
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = format,
        .components = {
            .r = VK_COMPONENT_SWIZZLE_IDENTITY,
            .g = VK_COMPONENT_SWIZZLE_IDENTITY,
            .b = VK_COMPONENT_SWIZZLE_IDENTITY,
            .a = VK_COMPONENT_SWIZZLE_IDENTITY
        },
        .subresourceRange = {
            .aspectMask  = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1
        }
    };
    return createInfo;
}


void fhDeleteSwapChainKHR(VkDevice device, VkSwapchainKHR swapchain){
    vkDestroySwapchainKHR(device, swapchain, NULL);
}