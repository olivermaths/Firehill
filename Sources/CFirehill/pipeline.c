#include "include/pipeline.h"
#include "vulkan/vulkan_core.h"
#include <stdint.h>

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



FhPipelineConfig createPipelineConfig(VkExtent2D extent){
        FhPipelineConfig config = {
            .inputAssemblyInfo = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
                .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
                .primitiveRestartEnable = 0
            },
            .viewPort = {
                .x = 0.0f,
                .y = 0.0f,
                .width = (float)(extent.width),
                .height = (float)(extent.height),
                .minDepth = 0.0f,
                .maxDepth  = 1.0f
            },
            .scissor = {
                    .offset = {.x = 0, .y = 0},
                    .extent = extent
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
                    .srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,   // Optional
                    .dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,  // Optional
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
                .pAttachments = NULL,
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
                .pViewports = NULL,
                .scissorCount = 1,
                .pScissors = NULL
            },
        };
    return config;
}

VkShaderModule fhCreateShaderModule(VkDevice device, uint64_t codeSize, const uint32_t *data){
    VkShaderModule shader;


    VkShaderModuleCreateInfo createShaderInfo = {
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO
    };
    createShaderInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createShaderInfo.codeSize = codeSize;
    createShaderInfo.pCode = data;


    c_assert(vkCreateShaderModule(device, &createShaderInfo, NULL, &shader) == VK_SUCCESS)
    return shader;
}


VkPipeline fhCreateVkPipeline(
        VkDevice device,
        VkShaderModule vertexShader,         
        VkShaderModule fragShader,         
        VkPipelineLayout layout,
        VkRenderPass renderPass,
        VkVertexInputBindingDescription inputDescription,
        VkVertexInputAttributeDescription attributeDescription,
        uint32_t subpass,
        uint32_t width,
        uint32_t height 
    ){
    VkExtent2D extent = {
        .height = height,
        .width = width
    };
    FhPipelineConfig staticPassesConfig = createPipelineConfig(extent);
    staticPassesConfig.colorBlendInfo.pAttachments  = &(staticPassesConfig.colorBlendAttachment);
    staticPassesConfig.viewPortInfo.pViewports  =  &(staticPassesConfig.viewPort);
    staticPassesConfig.viewPortInfo.pScissors = &(staticPassesConfig.scissor);

    VkPipeline pipeline;

    VkPipelineShaderStageCreateInfo shaderStages[2] = { {},{} };
    shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    shaderStages[0].module = vertexShader;
    shaderStages[0].pName = "main";
    shaderStages[0].flags = 0;
    shaderStages[0].pNext = NULL;

    shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    shaderStages[1].module = fragShader;
    shaderStages[1].pName = "main";
    shaderStages[1].flags = 0;
    shaderStages[1].pNext = NULL;


    VkDynamicState dynamicStates[] = {  VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
    
    VkPipelineDynamicStateCreateInfo dynamicStateInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
        .dynamicStateCount = 2,
        .pDynamicStates = dynamicStates

    };

    VkVertexInputBindingDescription   bindings[] = {inputDescription};
    VkVertexInputAttributeDescription  attributes[] = {attributeDescription};
    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .vertexAttributeDescriptionCount = 1,
        .vertexBindingDescriptionCount = 1,
        .pVertexAttributeDescriptions = attributes,
        .pVertexBindingDescriptions = bindings,
        .pNext = NULL,
        .flags = 0,
    };


    VkGraphicsPipelineCreateInfo pipelineInfo= {
	    .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
	    .pNext = NULL,
        .stageCount = 2,
        .pStages = shaderStages,
        .pVertexInputState = &vertexInputInfo,
        .pInputAssemblyState = &(staticPassesConfig.inputAssemblyInfo),
        .pViewportState = &(staticPassesConfig.viewPortInfo),
        .pRasterizationState = &(staticPassesConfig.rasterizationInfo),
        .pMultisampleState = &(staticPassesConfig.multisampleInfo),
        .pColorBlendState = &(staticPassesConfig.colorBlendInfo),
        .layout = layout,
        .renderPass = renderPass,
        .subpass = subpass,
        .pDynamicState = &dynamicStateInfo,
        .basePipelineHandle = VK_NULL_HANDLE,
        .pTessellationState = VK_NULL_HANDLE, 
    };
    c_assert(vkCreateGraphicsPipelines(device, NULL, 1, &pipelineInfo, NULL, &pipeline) == VK_SUCCESS);
    
    return pipeline;
}