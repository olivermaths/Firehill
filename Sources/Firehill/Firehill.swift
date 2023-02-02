import CFirehill
import Foundation
import WinSDK


public class Firehill {
    var pipeline : Pipeline
    var window : Window
    let deviceManager  : Device
    var swapChainManager : SwapChainManager
    let commandPool : OpaquePointer
    var commandBuffers : Array<OpaquePointer?>
    let maxFramesInFlight = 3
    let model : Model
    
    public init() {
        
        let coordX = Model.Vertex(position: Vector2D<Float>(x: 0.0, y: 0.5))
        let coordY = Model.Vertex(position: Vector2D<Float>(x: 0.5, y: -0.5))
        let coordZ = Model.Vertex(position: Vector2D<Float>(x: -0.5, y: -0.5))
 

        
        window = Window(title: "Firehill", height: 600, width: 800)
        deviceManager = Device()
        model = Model(vertices: [coordX, coordY, coordZ], device: deviceManager.device, physicalDevice: deviceManager.physicalDevice)      
       
        swapChainManager = SwapChainManager(physicalDevice: deviceManager.physicalDevice, device: deviceManager.device, instance: deviceManager.instance, window: window.pWindow)
        pipeline = Pipeline(vertexPath: "Shaders/inshader.spv", fragmentPath: "Shaders/infrag.spv", device: deviceManager.device, width: window.width, height: window.height, renderPass: swapChainManager.renderPass!, subpass: swapChainManager.subPass)
        commandPool = fhCreateCommandPool(deviceManager.physicalDevice, deviceManager.device, deviceManager.indices.presentQueueIndex!)
        commandBuffers = Array(repeating: nil, count: 3)
        commandBuffers.withUnsafeMutableBufferPointer({
             fhCreateCommandBuffer(deviceManager.device, commandPool, $0.baseAddress, 3)
        })
        
        
    }

    public func run(){
        let pointer = swapChainManager.swapChainFrameBuffers.withUnsafeMutableBufferPointer({$0.baseAddress})
        var frequency = LARGE_INTEGER(QuadPart: 0)
        QueryPerformanceFrequency(&frequency)

        var frameCounter = 0
        var frameTimeSum = 0.0

     
        while glfwWindowShouldClose(window.pWindow) == 0 {
            window.collectEvents()
            var start = LARGE_INTEGER(QuadPart: 0)
            QueryPerformanceCounter(&start)
            
            drawFrame(renderSemaphore: swapChainManager.renderFinishedSemaphores[swapChainManager.currentFrame], imageSemaphore: swapChainManager.imageAvailableSemaphores[swapChainManager.currentFrame], frameBuffers: pointer)
            


            var end = LARGE_INTEGER(QuadPart: 0)
            QueryPerformanceCounter(&end)
            let difference = end.QuadPart - start.QuadPart
            let frameTime = Double(difference) * 1000000000 / Double(frequency.QuadPart)
            frameTimeSum += frameTime
            frameCounter += 1
            if frameCounter == 60 {
                let averageFrameTime = frameTimeSum / Double(frameCounter)
                let frameRate = 1.0 / (averageFrameTime / 1000000000.0)
                print("Frame rate: \(frameRate) fps")
                frameCounter = 0
                frameTimeSum = 0.0
            }
     
        }
        vkDeviceWaitIdle(deviceManager.device);
    }


    deinit{
        swapChainManager.delete(device: deviceManager.device, instance: deviceManager.instance)
    }

    public func drawFrame(renderSemaphore: OpaquePointer, imageSemaphore: OpaquePointer, frameBuffers: UnsafeMutablePointer<OpaquePointer?>?){
        vkWaitForFences(deviceManager.device, 1, &(swapChainManager.inFlightFences[swapChainManager.currentFrame]), 0, UInt64.max);
        var imageIndex : UInt32 = 0
         vkAcquireNextImageKHR(deviceManager.device, swapChainManager.swapChain, UInt64.max, imageSemaphore, nil, &imageIndex);
        
        vkResetFences(deviceManager.device, 1,&(swapChainManager.inFlightFences[swapChainManager.currentFrame]))
        vkResetCommandBuffer(commandBuffers[swapChainManager.currentFrame], 0);
        fhRecordCommandBuffer(
                    pipeline.graphicPipeline,
                    commandBuffers[swapChainManager.currentFrame], 
                    swapChainManager.renderPass, 
                    swapChainManager.swapChainFrameBuffers[Int(imageIndex)],
                    swapChainManager.swapChainExtent
        );
        model.bind(commandBuffer: commandBuffers[swapChainManager.currentFrame]!)
        model.draw(commandBuffer: commandBuffers[swapChainManager.currentFrame]!)
        fhEndRecord(commandBuffers[swapChainManager.currentFrame])
        let result = draw( deviceManager.presentQueue,   pipeline.graphicPipeline, swapChainManager.swapChainExtent, swapChainManager.renderPass, swapChainManager.inFlightFences[swapChainManager.currentFrame], renderSemaphore, imageSemaphore, deviceManager.device, swapChainManager.swapChain,commandBuffers[swapChainManager.currentFrame], frameBuffers, imageIndex)
        
    
        if result == false {
                var height  : UInt32 = 0
                var width  : UInt32 = 0
                glfwGetWindowSize(window.pWindow, &width, &height)
                swapChainManager.swapChainExtent = VkExtent2D(width: width, height:height)
                swapChainManager.recreateSwapChain(physicalDevice: deviceManager.physicalDevice, device: deviceManager.device, window: window.pWindow)
        }
        
        
        
        swapChainManager.currentFrame = (swapChainManager.currentFrame + 1) % maxFramesInFlight
    }
}


protocol Draw{

}

extension Firehill : Draw {

}
