#include "core.h"
#include "cwindow.h"
#include "vulkan/vulkan_core.h"
#include <stdint.h>


typedef struct FhDeviceManager_t{
    VkInstance instance;
    VkPhysicalDevice physicalDevice;
    VkDevice device;
    VkSurfaceKHR surface;
    VkQueue presentQueue;
} FhDeviceManager;

typedef struct FhPipelineConfig_t{
    VkRect2D scissor;
    VkViewport viewPort;
    VkPipelineViewportStateCreateInfo viewPortInfo;
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
    VkPipelineRasterizationStateCreateInfo rasterizationInfo;
    VkPipelineMultisampleStateCreateInfo multisampleInfo;
    VkPipelineColorBlendAttachmentState colorBlendAttachment;
    VkPipelineColorBlendStateCreateInfo colorBlendInfo;
    VkPipelineDepthStencilStateCreateInfo depthStencilInfo;

} FhPipelineConfig;

typedef struct FhPipeline_t {
    VkPipeline graphicPipeline;
    VkShaderModule vertShaders;
    VkShaderModule fragShaders;
} FhPipeline;



FhPipeline* createGraphicsPipeline(
        const FhDeviceManager *dmanager,
        VkPipelineLayout layout,
        VkRenderPass renderPass,
        uint32_t subpass,
        uint32_t height, 
        uint32_t width, 
        const char *vertexPath, 
        const char* fragPath);




void fhDestroyPipeline(FhPipeline *pipeline, FhDeviceManager *dmanager);

FhDeviceManager *createDeviceManager(GLFWwindow *window);
void fhDestroyDeviceManager(FhDeviceManager* device);

VkPipelineLayout fhCreatePipelineLayout(VkDevice device);