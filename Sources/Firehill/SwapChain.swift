import CFirehill
struct SwapChainManager {
    let surface                : OpaquePointer
    // SwapChain Specifics
    var swapChain              : VkSwapchainKHR?
    var swapChainImageFormat   : VkFormat
    var swapChainExtent        : VkExtent2D
    var swapChainFrameBuffers  : Array<VkFramebuffer?>
    var swapChainImages        : Array<VkImage?>
    var swapChainImageViews    : Array<VkImageView?>
    
    
    var renderPass             : VkRenderPass?
    
    var depthImages            : Array<VkImage>?
    var depthImagesMemorys     : Array<VkDeviceMemory>?
    var depthImageViews        : Array<VkImageView>?

    var windowExtent           : VkExtent2D?

    var imageAvailableSemaphores : Array<VkSemaphore>
    var renderFinishedSemaphores : Array<VkSemaphore>
    var inFlightFences           : Array<VkFence?>
    
    var imagesInFlight           : Array<VkFence>?

    var currentFrame             : Int
    
    var subPass                  : UInt32

    init(physicalDevice: VkPhysicalDevice, device: OpaquePointer, instance: OpaquePointer, window: OpaquePointer){
        surface = fhCreateVkSurface(instance, window)
        swapChainImageFormat = VkFormat(0)
        swapChainExtent = VkExtent2D()
        swapChainImages = []
        swapChainImageViews = []
        swapChainFrameBuffers = []
        inFlightFences = Array(repeating: fhCreateFence(device), count: 3)
        imageAvailableSemaphores = Array(repeating: fhCreateSemaphore(device), count: 3)
        renderFinishedSemaphores = Array(repeating: fhCreateSemaphore(device), count: 3)
        subPass = 0
        currentFrame = 0


        withUnsafeMutablePointer(to: &swapChainImageFormat){formatPointer in
           
            swapChain =  fhCreateSwapChain(
                physicalDevice, 
                device,  
                surface, 
                formatPointer, 
                &swapChainExtent, 
                window
            );
            return;
        }
        renderPass = fhCreateRenderPass(device, swapChainImageFormat)

        var count : UInt32 = 0
           
        vkGetSwapchainImagesKHR(device, swapChain, &count, nil);
           
        swapChainImages = Array(repeating: nil, count: Int(count))
        swapChainImageViews = Array(repeating: nil, count: Int(count))
        swapChainFrameBuffers = Array(repeating: nil, count: Int(count))

        swapChainImages.withUnsafeMutableBufferPointer({pointer in
                vkGetSwapchainImagesKHR(device, swapChain, &count, pointer.baseAddress);
        })


        for (index, image) in swapChainImages.enumerated(){
            var createInfo : VkImageViewCreateInfo = fhCreateImageViewInfo(image, swapChainImageFormat)
            
            vkCreateImageView(device, &createInfo, nil, UnsafeMutablePointer(&swapChainImageViews[index]))
            
            var framebufferInfo =  fhCreateFrameBufferInfo(swapChainImageViews[index], renderPass, swapChainExtent)
            
            vkCreateFramebuffer(device, &framebufferInfo, nil, UnsafeMutablePointer(&swapChainFrameBuffers[index]))
        }



    }
    func delete(device: OpaquePointer, instance: OpaquePointer){
        fhDeleteSwapChainKHR(device, swapChain)

        for (index, imageView) in swapChainImageViews.enumerated(){
            vkDestroyImageView(device, imageView, nil)
            vkDestroyFramebuffer(device, swapChainFrameBuffers[index], nil)
        }
        
        inFlightFences.map({
            fhDestroyFence(device, $0)
        })

        imageAvailableSemaphores.map({
            fhDestroySemaphore(device, $0)
        })

        renderFinishedSemaphores.map({
            fhDestroySemaphore(device, $0)
        })
        fhDestroyRenderPass(device, renderPass)
        vkDestroySurfaceKHR(instance, surface, nil)
    }

 
}

protocol SwapChainHelpers{
    mutating func recreateSwapChain(physicalDevice: VkPhysicalDevice, device: OpaquePointer, window: OpaquePointer)
    mutating func cleanUpSwapChain(device: OpaquePointer)

}


extension SwapChainManager : SwapChainHelpers {
    mutating func recreateSwapChain(physicalDevice: VkPhysicalDevice, device: OpaquePointer, window: OpaquePointer){
        vkDeviceWaitIdle(device)
        cleanUpSwapChain(device: device)
        
        withUnsafeMutablePointer(to: &swapChainImageFormat){formatPointer in
            swapChain = nil
            swapChain =  fhCreateSwapChain(
                physicalDevice, 
                device,  
                surface, 
                formatPointer, 
                &swapChainExtent, 
                window
            );
            return;
        }
        
        var count : UInt32 = 0
        vkGetSwapchainImagesKHR(device, swapChain, &count, nil);
        
        swapChainImages.withUnsafeMutableBufferPointer({pointer in
                vkGetSwapchainImagesKHR(device, swapChain, &count, pointer.baseAddress);
        })
        print(count)


        for (index, image) in swapChainImages.enumerated(){
            print("here  \(index)")
            var createInfo : VkImageViewCreateInfo = fhCreateImageViewInfo(image, swapChainImageFormat)
            
            vkCreateImageView(device, &createInfo, nil, UnsafeMutablePointer(&swapChainImageViews[index]))
            
            var framebufferInfo =  fhCreateFrameBufferInfo(swapChainImageViews[index], renderPass, swapChainExtent)
            
            vkCreateFramebuffer(device, &framebufferInfo, nil, UnsafeMutablePointer(&swapChainFrameBuffers[index]))
        }


    }

    mutating func cleanUpSwapChain(device: OpaquePointer){
        for frameBuffer in self.swapChainFrameBuffers {
            vkDestroyFramebuffer(device, frameBuffer, nil)
        }
        for imageView in self.swapChainImageViews{
            vkDestroyImageView(device, imageView, nil)
        }
        vkDestroySwapchainKHR(device, self.swapChain, nil)
    }
}