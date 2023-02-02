

#include "pipeline.h"
#include "instance.h"
#include "device.h"
#include "swapchain.h"

#include "structs.h"






//! @brief SwapChain Functions
VkSwapchainKHR fhCreateSwapChain(
    VkPhysicalDevice device, 
    VkDevice logicalDevice, 
    VkSurfaceKHR surface,
    VkFormat *format,
    VkExtent2D *extent,
    GLFWwindow *window
);


bool draw(VkQueue presentQueue,  VkPipeline pipeline, VkExtent2D extent, VkRenderPass renderPass, VkFence fence, VkSemaphore renderSemaphore, VkSemaphore imageSemaphore, VkDevice device, VkSwapchainKHR swapChain, VkCommandBuffer commandBuffer, VkFramebuffer *frameBuffers, uint32_t imageIndex);
VkImageViewCreateInfo fhCreateImageViewInfo(VkImage image, VkFormat format);

VkRenderPass fhCreateRenderPass(VkDevice device, VkFormat format);
void fhEndRecord(VkCommandBuffer commandBuffer);
VkFramebufferCreateInfo fhCreateFrameBufferInfo(VkImageView imageView, VkRenderPass renderPass, VkExtent2D extent);



VkCommandPool  fhCreateCommandPool(VkPhysicalDevice physicalDevice, VkDevice device,  uint32_t presentQueueIndex);

void fhCreateCommandBuffer(VkDevice device, VkCommandPool commandPool, VkCommandBuffer *commnadBuffers, uint32_t count);

void fhRecordCommandBuffer(
    VkPipeline graphicPipeline,
    VkCommandBuffer commandBuffer, 
    VkRenderPass renderPass, 
    VkFramebuffer frameBuffer,
    VkExtent2D extent
);
void submitCommand( VkCommandBuffer commandbuffer, VkQueue presentQueue, VkFence fence, VkSemaphore imageSemaphore,VkSemaphore renderSemaphore);


VkFence fhCreateFence(VkDevice device);
VkSemaphore fhCreateSemaphore(VkDevice device);
VkPresentInfoKHR fhCreatePresentInfoKHR(VkSwapchainKHR swapChain, VkSemaphore renderSemaphore, uint32_t imageIndex);


void fhDeleteSwapChainKHR(VkDevice device, VkSwapchainKHR swapchain);
void fhDestroypipelineLayout(VkDevice device, VkPipelineLayout layout);
void fhDestroyRenderPass(VkDevice device, VkRenderPass renderPass);
void fhDestroyCommandPool(VkDevice device, VkCommandPool commandPool);
void fhDestroySemaphore(VkDevice device, VkSemaphore semaphore);
void fhDestroyFence(VkDevice device, VkFence fence);