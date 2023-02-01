import CFirehill
import WinSDK

public class Firehill {
    var pipeline : Pipeline
    var window : FhWindow
    let deviceManager  : Device
    var swapChainManager : SwapChainManager
    let commandPool : OpaquePointer
    let commandBuffer : OpaquePointer

    
    public init() {
        window = fhCreateWindow(600, 800, "Firehill")
        deviceManager = Device()
        swapChainManager = SwapChainManager(physicalDevice: deviceManager.physicalDevice, device: deviceManager.device, instance: deviceManager.instance, window: window.pInstance)
        pipeline = Pipeline(vertexPath: "Shaders/blued.spv", fragmentPath: "Shaders/frag.spv", device: deviceManager.device, width: window.width, height: window.height, renderPass: swapChainManager.renderPass!, subpass: swapChainManager.subPass)
        commandPool = fhCreateCommandPool(deviceManager.physicalDevice, deviceManager.device)
        commandBuffer = fhCreateCommandBuffer(deviceManager.device, commandPool)

    }

    public func run(){
        let pointer = swapChainManager.swapChainFrameBuffers.withUnsafeMutableBufferPointer({$0.baseAddress})

        while glfwWindowShouldClose(window.pInstance) == 0 {
            collectEvents()
            drawFrame(fence: swapChainManager.inFlightFences[0], renderSemaphore: swapChainManager.renderFinishedSemaphores[0], imageSemaphore: swapChainManager.imageAvailableSemaphores[0], pointer: pointer)
        }
        vkDeviceWaitIdle(deviceManager.device);
    }


    public func collectEvents(){
        fhCollectEvents()
    }

    deinit{}

    public func drawFrame(fence: OpaquePointer, renderSemaphore: OpaquePointer, imageSemaphore: OpaquePointer, pointer: UnsafeMutablePointer<OpaquePointer?>?){
        //draw(pipeline.pointee.graphicPipeline, swapChainManager.swapChainExtent, swapChainManager.renderPass,  fence, renderSemaphore, imageSemaphore,deviceManager, swapChainManager.swapChain,  commandBuffers, pointer)

        draw( deviceManager.presentQueue,   pipeline.graphicPipeline, swapChainManager.swapChainExtent, swapChainManager.renderPass, fence, renderSemaphore, imageSemaphore, deviceManager.device, swapChainManager.swapChain,commandBuffer, pointer)
    }



}

