#pragma once
#include "core.h"
#include "structs.h"

bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface);
bool checkDeviceExtensionSupport(VkPhysicalDevice device, const char *requiredExtension);
FhQueueFamilyIndices fhFindQueueFamilies(VkPhysicalDevice device);


// swapChain
bool querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface, FhSwapChainSupportDetails *);
VkSurfaceFormatKHR chooseSwapSurfaceFormat(VkSurfaceFormatKHR *availableFormats, uint32_t formatCount);