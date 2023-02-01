#pragma once
#include "core.h"



VkPipelineLayout fhCreatePipelineLayout(VkDevice device);
VkShaderModule fhCreateShaderModule(VkDevice device, uint64_t codeSize, const uint32_t *data);

VkPipeline fhCreateVkPipeline(
        VkDevice device,
        VkShaderModule vertexShader,         
        VkShaderModule fragShader,         
        VkPipelineLayout layout,
        VkRenderPass renderPass,
        uint32_t subpass,
        uint32_t width,
        uint32_t height 
    );