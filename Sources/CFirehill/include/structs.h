#pragma once
#include "core.h"
typedef struct FhQueueFamilyIndices_t{
    uint32_t computeQueueIndex;
    uint32_t presentQueueIndex;
}FhQueueFamilyIndices;

typedef struct FhSwapChainSupportDetails_t{
    VkSurfaceCapabilitiesKHR capabilities;
    uint32_t formtCount;
    VkSurfaceFormatKHR *formats;
    uint32_t presentModeCount;
    VkPresentModeKHR *presentModes;
}FhSwapChainSupportDetails;
