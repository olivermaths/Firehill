#include "include/core.h"
#include "include/cfirehill.h"


VkInstance fhCreateVkInstance(const char *applicationName){
        VkInstance instance;


        VkApplicationInfo appinfo = {
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .apiVersion = VK_API_VERSION_1_3,
            .pApplicationName = applicationName,
            .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
            .pEngineName = "Firehill",
            .engineVersion = VK_MAKE_VERSION(1, 0, 0)

        };
        /*! FIXME: Implement check for validation layer support here. */
        /*
        uint32_t layerCount = 0;
        c_assert(vkEnumerateInstanceLayerProperties(&layerCount, NULL) == VK_SUCCESS);

        struct VkLayerProperties layers[layerCount];
        c_assert(vkEnumerateInstanceLayerProperties(&layerCount, layers) == VK_SUCCESS);
        */

        const char *requiredValidationLayers[] = {"VK_LAYER_KHRONOS_validation"};


        uint32_t count;
        const char **extensions = glfwGetRequiredInstanceExtensions(&count);

        VkInstanceCreateInfo instanceInfo = {
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pApplicationInfo = &appinfo,
            .enabledExtensionCount = count,
            .ppEnabledExtensionNames = extensions,
            .enabledLayerCount = 1,
            .ppEnabledLayerNames = requiredValidationLayers,
        };

        c_assert(vkCreateInstance(&instanceInfo, 0, &instance) ==VK_SUCCESS);

        c_assert(instance != NULL);
        return instance;
}


