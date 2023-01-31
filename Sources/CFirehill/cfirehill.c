#include "include/cfirehill.h"
#include "glfw/glfw3.h"
#include "vulkan/vulkan_core.h"
#include <stdbool.h>
#include <string.h>
// Helpers Functions

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

void querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface){
    VkSurfaceCapabilitiesKHR capabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &capabilities);
    printf("minImage: %i, maxImage: %i, minHeight: %i minWidth: %i maxHeight: %i maxWidth %i\n", capabilities.minImageCount, capabilities.maxImageCount, capabilities.minImageExtent.height, capabilities.minImageExtent.width, capabilities.maxImageExtent.height, capabilities.maxImageExtent.width);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, NULL);
    VkSurfaceFormatKHR formats[formatCount];
   
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, formats);
    for (size_t it = 0; it < formatCount ; it++) {
        printf("formats: %x %i\n", formats[it].format, formats[it].colorSpace);
    }

}


//




static int32_t getPresentModeSupported(FhDeviceManager *dm){
    uint32_t presentCount = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(dm->physicalDevice,dm->surface, &presentCount, NULL);

    VkPresentModeKHR presentModes[presentCount];

    vkGetPhysicalDeviceSurfacePresentModesKHR(dm->physicalDevice,dm->surface, &presentCount, presentModes);

    for (size_t it = 0; it < presentCount; it++) {
        if((VK_PRESENT_MODE_MAILBOX_KHR == presentModes[it])) return VK_PRESENT_MODE_MAILBOX_KHR;
    }
    return VK_PRESENT_MODE_FIFO_KHR;
}

FhDeviceManager *createDeviceManager(GLFWwindow *window) {
        FhDeviceManager *deviceManager = malloc(sizeof(FhDeviceManager));

        VkApplicationInfo appinfo = {
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .apiVersion = VK_API_VERSION_1_3,
            .pApplicationName = "Triangles",
            .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
            .pEngineName = "Firehill",
            .engineVersion = VK_MAKE_VERSION(1, 0, 0)

        };

        uint32_t layerCount = 0;

        c_assert(vkEnumerateInstanceLayerProperties(&layerCount, NULL) == VK_SUCCESS);

        struct VkLayerProperties layers[layerCount];
        const char *requiredValidationLayers[] = {"VK_LAYER_KHRONOS_validation"};

        c_assert(vkEnumerateInstanceLayerProperties(&layerCount, layers) ==
                VK_SUCCESS);

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

        c_assert(vkCreateInstance(&instanceInfo, 0, &(deviceManager->instance)) ==
                VK_SUCCESS);

        uint32_t deviceCount = 0;
        c_assert(vkEnumeratePhysicalDevices(deviceManager->instance, &deviceCount,
                                            NULL) == VK_SUCCESS);
        VkPhysicalDevice devices[deviceCount];
        c_assert(vkEnumeratePhysicalDevices(deviceManager->instance, &deviceCount,
                                            devices) == VK_SUCCESS);

        for (size_t i = 0; i < deviceCount; i++) {
            VkPhysicalDeviceProperties props;
            vkGetPhysicalDeviceProperties(devices[i], &props);
            if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
                deviceManager->physicalDevice = devices[i];
                break;
            } else {
                deviceManager->physicalDevice = devices[i];
            }
        }

        c_assert(deviceManager->physicalDevice != VK_NULL_HANDLE);


        const char *requiredDeviceExtensions[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
        bool result = checkDeviceExtensionSupport(deviceManager->physicalDevice, requiredDeviceExtensions[0]);
        c_assert(result);

        uint32_t queueFamilyCount;
        vkGetPhysicalDeviceQueueFamilyProperties(deviceManager->physicalDevice, &queueFamilyCount, NULL);
        VkQueueFamilyProperties queueFamilyProperties[queueFamilyCount];
        vkGetPhysicalDeviceQueueFamilyProperties(deviceManager->physicalDevice, &queueFamilyCount, queueFamilyProperties);


        uint32_t presentQueueIndex = UINT32_MAX;
        
        for (size_t i = 0; i < queueFamilyCount; i++) {
            if(queueFamilyProperties[i].queueCount > 0 && queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT){
                presentQueueIndex = i;
                break;
            }
        }

        float queuePriorities[] = {1.0f};
       
        VkDeviceQueueCreateInfo deviceQueueCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex = presentQueueIndex,
            .queueCount = 1,
            .pQueuePriorities = queuePriorities
        };


        VkDeviceCreateInfo deviceCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .queueCreateInfoCount = 1,
            .pQueueCreateInfos = &deviceQueueCreateInfo,
            .enabledExtensionCount = 1,
            .ppEnabledExtensionNames = requiredDeviceExtensions
        };

   

        c_assert(vkCreateDevice(deviceManager->physicalDevice, &deviceCreateInfo, NULL, &(deviceManager->device)) == VK_SUCCESS);

        vkGetDeviceQueue(deviceManager->device, presentQueueIndex, 0, &(deviceManager->presentQueue));
        
        c_assert(deviceManager->presentQueue != VK_NULL_HANDLE);
        c_assert(deviceManager->instance != NULL);
        c_assert(window != NULL);
        c_assert(glfwCreateWindowSurface(deviceManager->instance, window, NULL, &(deviceManager->surface)) == VK_SUCCESS);
        querySwapChainSupport(deviceManager->physicalDevice, deviceManager->surface);
        return deviceManager;
};








FhPipelineConfig* createPipelineConfig(uint32_t width, uint32_t height){
        FhPipelineConfig *config = malloc(sizeof(FhPipelineConfig));
        
        *config = (FhPipelineConfig){
            .inputAssemblyInfo = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
                .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
                .primitiveRestartEnable = 0
            },
            .viewPort = {
                .x = 0.0f,
                .y = 0.0f,
                .width = (float)(width),
                .height = (float)(height),
                .minDepth = 0.0f,
                .maxDepth  = 1.0f
            },
            .scissor = {
                    .offset = {.x = 0, .y = 0},
                    .extent = {.width = width, .height = height}
            },
            .rasterizationInfo = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
                .depthClampEnable = 0,
                .rasterizerDiscardEnable = 0,
                .polygonMode = VK_POLYGON_MODE_FILL,
                .cullMode = 0,
                .frontFace = VK_FRONT_FACE_CLOCKWISE,
                .depthBiasEnable = 0,
                .depthBiasConstantFactor = (float)0.0f,
                .depthBiasClamp = (float)0.0f,
                .depthBiasSlopeFactor = (float)0.0f,
                .lineWidth = (float)1.0f
            },
            .multisampleInfo = {
                    .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
                    .sampleShadingEnable = 0,
                    .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
                    .minSampleShading = (float)1.0f,
                    .pSampleMask = NULL,
                    .alphaToCoverageEnable = 0,
                    .alphaToOneEnable = 0
            },
            .colorBlendAttachment = {
                    .colorWriteMask = (VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT),
                    .blendEnable = 0,
                    .srcColorBlendFactor = VK_BLEND_FACTOR_ONE,   // Optional
                    .dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,  // Optional
                    .colorBlendOp = VK_BLEND_OP_ADD,              // Optional
                    .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,   // Optional
                    .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,  // Optional
                    .alphaBlendOp = VK_BLEND_OP_ADD,              // Optional
            },
            .colorBlendInfo = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
                .logicOpEnable = 0,
                .logicOp = VK_LOGIC_OP_COPY,  // Optional
                .attachmentCount = 1,
                .pAttachments = &(config->colorBlendAttachment),
                .blendConstants = {0.0f, 0.0f, 0.0f, 0.0f}  // Optional
            },
            .depthStencilInfo = {
                    .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
                    .depthTestEnable = VK_TRUE,
                    .depthWriteEnable = VK_TRUE,
                    .depthCompareOp = VK_COMPARE_OP_LESS,
                    .depthBoundsTestEnable = VK_FALSE,
                    .minDepthBounds = (float)0.0f,  // Optional
                    .maxDepthBounds = (float)1.0f,  // Optional
                    .stencilTestEnable = VK_FALSE,
                    .front = {},  // Optional
                    .back = {}   // Optional
            },
            .viewPortInfo = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
                .viewportCount = 1,
                .pViewports = &(config->viewPort),
                .scissorCount = 1,
                .pScissors = &(config->scissor)

            },
        };

    
    return config;
}

static void initializeShaderModule(FhDeviceManager *dm, const char* path, VkShaderModule *instance){

    FILE* file = fopen(path, "rb");
    fseek(file, 0L, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0L, SEEK_SET);

    const char *buffer[size];
    size_t dataRead = fread(buffer, 1, size, file);
    c_assert(dataRead == size);

    VkShaderModuleCreateInfo createShaderInfo = {};
    createShaderInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createShaderInfo.codeSize = size;
    createShaderInfo.pCode = (const uint32_t *)buffer;
    c_assert(vkCreateShaderModule(dm->device, &createShaderInfo, NULL, &(*instance)) == VK_SUCCESS);
}







FhPipeline* createGraphicsPipeline(
        const FhDeviceManager *dmanager,         
        VkPipelineLayout layout,
        VkRenderPass renderPass,
        uint32_t subpass,
        uint32_t height, 
        uint32_t width, 
        const char *vertexPath, 
        const char* fragPath){


    FhPipeline *pipeline = malloc(sizeof(FhPipeline));
    pipeline->fragShaders = NULL;
    pipeline->vertShaders = NULL;
    
    initializeShaderModule(dmanager, vertexPath, &(pipeline->vertShaders));
    
    initializeShaderModule(dmanager, fragPath, &(pipeline->fragShaders));
    
    c_assert(pipeline->vertShaders != NULL);
    
    c_assert(pipeline->fragShaders != NULL);

    VkPipelineShaderStageCreateInfo shaderStages[2] = { {},{} };
    shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    shaderStages[0].module = pipeline->vertShaders;
    shaderStages[0].pName = "main";
    shaderStages[0].flags = 0;
    shaderStages[0].pNext = NULL;

    shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    shaderStages[1].module = pipeline->fragShaders;
    shaderStages[1].pName = "main";
    shaderStages[1].flags = 0;
    shaderStages[1].pNext = NULL;


    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .vertexAttributeDescriptionCount = 0,
        .vertexBindingDescriptionCount = 0,
        .pVertexAttributeDescriptions = NULL,
        .pVertexBindingDescriptions = NULL,
        .pNext = NULL,
        .flags = 0,
    };
    FhPipelineConfig* pipelineConfigInfo = createPipelineConfig(width, height);

    VkGraphicsPipelineCreateInfo pipelineInfo= {
	    .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
	    .pNext = NULL,
        .stageCount = (const uint32_t)2,
        .pStages = shaderStages,
        .pVertexInputState = &vertexInputInfo,
        .pInputAssemblyState = &(pipelineConfigInfo->inputAssemblyInfo),
        .pViewportState = &(pipelineConfigInfo->viewPortInfo),
        .pRasterizationState = &(pipelineConfigInfo->rasterizationInfo),
        .pMultisampleState = &(pipelineConfigInfo->multisampleInfo),
        .pColorBlendState = &(pipelineConfigInfo->colorBlendInfo),
        .layout = layout,
        .renderPass = renderPass,
        .subpass = subpass,
        .pDynamicState = NULL,
        .basePipelineHandle = VK_NULL_HANDLE,
        .pTessellationState = NULL, 
    };
    vkCreateGraphicsPipelines(dmanager->device, NULL, 1, &pipelineInfo, NULL, &pipeline->graphicPipeline);
    
    free(pipelineConfigInfo);
    return pipeline;
}


VkPipelineLayout fhCreatePipelineLayout(VkDevice device){
    VkPipelineLayout layout = NULL;
    VkPipelineLayoutCreateInfo infos = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .pSetLayouts = NULL,
        .setLayoutCount = 0,
        .pushConstantRangeCount = 0,
        .pPushConstantRanges = NULL
    };
    c_assert(vkCreatePipelineLayout(device,&infos, NULL, &layout) == VK_SUCCESS);
    return layout;
}


void fhCreateRenderPass(VkPhysicalDevice phyDevice){
    VkAttachmentDescription depthAttachment = {};
}






void fhDestroyDeviceManager(FhDeviceManager* device){
    vkDestroyDevice(device->device, NULL);
    vkDestroySurfaceKHR(device->instance, device->surface, NULL);
    vkDestroyInstance(device->instance,NULL);
    free(device);
    device = NULL;
}

void fhDestroyPipeline(FhPipeline *pipeline, FhDeviceManager *dmanager){
    vkDestroyShaderModule(dmanager->device, pipeline->vertShaders, NULL);
    vkDestroyShaderModule(dmanager->device, pipeline->fragShaders, NULL);
    vkDestroyPipeline(dmanager->device, pipeline->graphicPipeline, NULL);
    free(pipeline);
}



void fhDestroyPipelineConfig(FhPipelineConfig* config){
    free(config);
    config = NULL;
}

