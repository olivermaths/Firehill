#include "include/cfirehill.h"
#include "glfw/glfw3.h"
#include "vulkan/vulkan_core.h"
#include <stdint.h>



VkFramebufferCreateInfo fhCreateFrameBufferInfo(VkImageView imageView, VkRenderPass renderPass, VkExtent2D extent){
    VkFramebufferCreateInfo framebufferInfo = {
        .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
        .renderPass = renderPass,
        .attachmentCount = 1,
        .pAttachments = &imageView,
        .width = extent.width,
        .height = extent.height,
        .layers = 1
    };
    return framebufferInfo;
}

VkCommandPool  fhCreateCommandPool(VkPhysicalDevice physicalDevice, VkDevice device){
    VkCommandPool  commandPool;
    FhQueueFamilyIndices indices = findQueueFamilies(physicalDevice);
    VkCommandPoolCreateInfo poolInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = indices.presentQueueIndex,
    };

    c_assert(vkCreateCommandPool(device, &poolInfo,NULL, &commandPool) == VK_SUCCESS);
    return commandPool;
}


VkRenderPass fhCreateRenderPass(VkDevice device, VkFormat format){
    VkAttachmentDescription colorAttachment = {
        .format = format,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
    };
    VkAttachmentReference colorAttachmentRef = {
        .attachment = 0,
        .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
    };
    VkSubpassDependency dependency = {
        .srcSubpass = VK_SUBPASS_EXTERNAL,
        .dstSubpass = 0,
        .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        .srcAccessMask = 0,
        .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT
    };
    VkSubpassDescription subpass = {
        .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
        .colorAttachmentCount = 1,
        .pColorAttachments = &colorAttachmentRef
    };
    VkRenderPassCreateInfo renderPassInfo = {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .attachmentCount = 1,
        .pAttachments = &colorAttachment,
        .subpassCount = 1,
        .pSubpasses = &subpass,
        .dependencyCount = 1,
        .pDependencies = &dependency
    };
    VkRenderPass renderPass;

    c_assert(vkCreateRenderPass(device, &renderPassInfo, NULL, &renderPass) == VK_SUCCESS);
    return renderPass;
};


VkCommandBuffer fhCreateCommandBuffer(VkDevice device, VkCommandPool commandPool){
    VkCommandBuffer commandBuffer;
    VkCommandBufferAllocateInfo allocInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool = commandPool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1
    };

    c_assert(vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer) == VK_SUCCESS);
    return commandBuffer;
}

void fhRecordCommandBuffer(
    VkPipeline graphicPipeline,
    VkCommandBuffer commandBuffer, 
    VkRenderPass renderPass, 
    VkFramebuffer frameBuffer,
    VkExtent2D extent
){
    VkCommandBufferBeginInfo beginInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = 0,
        .pInheritanceInfo = NULL
    };

    vkBeginCommandBuffer(commandBuffer, &beginInfo);
    
    VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    VkRenderPassBeginInfo renderPassInfo = {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .framebuffer = frameBuffer,
        .renderPass = renderPass,
        .renderArea = {
            .extent = extent,
            .offset = {
                .x = 0,
                .y = 0
            }
        },
        .clearValueCount = 1,
        .pClearValues = &clearColor
    };
    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicPipeline);


    VkViewport viewport = {
        .x = 0.0f,
        .y = 0.0f,
        .width = extent.width,
        .height = extent.height,
        .minDepth = 0.0f,
        .maxDepth = 1.0f,
    };
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    VkRect2D scissor= {
        .extent = extent,
        .offset = {
            .x = 0,
            .y = 0
        }
    };
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
    vkCmdDraw(commandBuffer, 3, 1, 0, 0);
    vkCmdEndRenderPass(commandBuffer);
    vkEndCommandBuffer(commandBuffer);
}


void draw(VkQueue presentQueue,  VkPipeline pipeline, VkExtent2D extent, VkRenderPass renderPass, VkFence fence, VkSemaphore renderSemaphore, VkSemaphore imageSemaphore, VkDevice device, VkSwapchainKHR swapChain, VkCommandBuffer commandBuffer, VkFramebuffer *frameBuffers){

        vkWaitForFences(device, 1, &fence, 0, UINT64_MAX);
        
        uint32_t imageIndex  = 0;
        vkAcquireNextImageKHR(device,swapChain, UINT64_MAX, imageSemaphore, NULL, &imageIndex);

        vkResetCommandBuffer(commandBuffer, 0);
        
        VkFramebuffer frameBuffer = frameBuffers[imageIndex];
        fhRecordCommandBuffer(
                    pipeline,
                    commandBuffer, 
                    renderPass, 
                    frameBuffer,
                    extent
        );
        
        c_assert(vkResetFences(device, 1, &fence) == VK_SUCCESS);
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        VkSemaphore imageSemaphores[] = {imageSemaphore};
        VkSemaphore renderSemaphores[] = {renderSemaphore};

        VkSubmitInfo submitInfo = {
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = imageSemaphores,
            .pWaitDstStageMask = waitStages,
            .commandBufferCount = 1,
            .pCommandBuffers = &commandBuffer,
            .signalSemaphoreCount = 1,
            .pSignalSemaphores = renderSemaphores
        };
        c_assert(vkQueueSubmit(presentQueue, 1, &submitInfo, fence) == VK_SUCCESS);
        
        VkSwapchainKHR swaps[] = {swapChain};
        uint32_t indices[] = {imageIndex};
        
        VkPresentInfoKHR presentInfo = {
            .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = renderSemaphores,
            .swapchainCount = 1,
            .pSwapchains = swaps,
            .pImageIndices = indices,
            .pResults = 0
        };
        vkQueuePresentKHR(presentQueue, &presentInfo);
}


VkSemaphore fhCreateSemaphore(VkDevice device){
     VkSemaphoreCreateInfo semaphoreInfo = {
            .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
            .flags = 0,
     };
     VkSemaphore semaphore;
     vkCreateSemaphore(device, &semaphoreInfo, NULL, &semaphore);
     return semaphore;
}


VkFence fhCreateFence(VkDevice device){
     VkFenceCreateInfo  info = {
            .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
            .flags = VK_FENCE_CREATE_SIGNALED_BIT 
     };
     VkFence fence;
     vkCreateFence(device, &info, NULL, &fence);
     return fence;
}




VkPresentInfoKHR fhCreatePresentInfoKHR(VkSwapchainKHR swapChain, VkSemaphore renderSemaphore, uint32_t imageIndex){
    c_assert(swapChain != NULL);
    VkSemaphore renders[] = {renderSemaphore};
    VkSwapchainKHR swaps[] = {swapChain};
    uint32_t indices[] = {imageIndex};
    VkPresentInfoKHR presentInfo = {
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = renders,
        .swapchainCount = 1,
        .pSwapchains = swaps,
        .pImageIndices = indices,
        .pResults = 0
    };
    return presentInfo;
}



void fhDestroypipelineLayout(VkDevice device, VkPipelineLayout layout){
    vkDestroyPipelineLayout(device, layout, NULL);
}




void fhDestroyRenderPass(VkDevice device, VkRenderPass renderPass){
    vkDestroyRenderPass(device, renderPass, NULL);
}
void fhDestroyCommandPool(VkDevice device, VkCommandPool commandPool){
    vkDestroyCommandPool(device,commandPool,NULL);
}

void fhDestroySemaphore(VkDevice device, VkSemaphore semaphore){
    vkDestroySemaphore(device, semaphore, NULL);
};
void fhDestroyFence(VkDevice device, VkFence fence){
     vkDestroyFence(device, fence, NULL);
};