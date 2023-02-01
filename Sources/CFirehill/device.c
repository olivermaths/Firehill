#include "include/core.h"
#include "include/structs.h"
#include "include/helpers.h"
#include "vulkan/vulkan_core.h"
#include <stdint.h>


VkPhysicalDevice fhPickPhysicalDevice(VkInstance instance){
        VkPhysicalDevice physicalDevice;

        uint32_t deviceCount = 0;
        c_assert(
            vkEnumeratePhysicalDevices(instance, &deviceCount, NULL) == VK_SUCCESS
        );
        
        
        VkPhysicalDevice devices[deviceCount];
        
        c_assert(vkEnumeratePhysicalDevices(instance, &deviceCount,devices) == VK_SUCCESS);

        for (size_t i = 0; i < deviceCount; i++) {
            VkPhysicalDeviceProperties props;
            vkGetPhysicalDeviceProperties(devices[i], &props);
            if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
                physicalDevice = devices[i];
                break;
            } else {
                physicalDevice = devices[i];
            }
        }

        c_assert(physicalDevice != VK_NULL_HANDLE);
        return physicalDevice;
}




VkDevice fhCreateVkDevice(VkPhysicalDevice physicalDevice, uint32_t presentQueueIndex, uint32_t computeQueueIndex){
        VkDevice device;
        const char *requiredDeviceExtensions[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
        
        float queuePriorities[] = {1.0f};
       

        VkDeviceQueueCreateInfo queuesInfos[2] = {{}, {}};
        queuesInfos[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queuesInfos[0].queueFamilyIndex = presentQueueIndex;
        queuesInfos[0].queueCount = 1;
        queuesInfos[0].pQueuePriorities = queuePriorities;

        queuesInfos[1].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queuesInfos[1].queueFamilyIndex = computeQueueIndex;
        queuesInfos[1].queueCount = 1;
        queuesInfos[1].pQueuePriorities = queuePriorities;


        VkDeviceCreateInfo deviceCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .queueCreateInfoCount = 2,
            .pQueueCreateInfos = queuesInfos,
            .enabledExtensionCount = 1,
            .ppEnabledExtensionNames = requiredDeviceExtensions
        };


        c_assert(vkCreateDevice(physicalDevice, &deviceCreateInfo, NULL, &device) == VK_SUCCESS);
        return device;
}

VkQueue fhGetVkQueue(VkDevice device, uint32_t index){
    VkQueue queue;
    vkGetDeviceQueue(device,index, 0, &queue);
    c_assert(queue != VK_NULL_HANDLE);
    return queue;
}