#pragma once
#include "core.h"




VkPhysicalDevice fhPickPhysicalDevice(VkInstance instance);
VkDevice fhCreateVkDevice(VkPhysicalDevice physicalDevice, uint32_t presentQueueIndex, uint32_t computeQueueIndex);
VkQueue fhGetVkQueue(VkDevice device, uint32_t index);
VkDeviceMemory fhGetDeviceMemory(VkDevice device, VkPhysicalDevice physicalDevice, VkBuffer buffer, uint32_t properties);
VkBuffer fhCreateVertexBuffer(VkDevice device, uint64_t size);