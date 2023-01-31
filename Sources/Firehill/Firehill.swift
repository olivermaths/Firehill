import WinSDK
import CFirehill

public class Firehill {
    var pPipeline : UnsafeMutablePointer<FhPipeline>
    var window : FhWindow
    let deviceManager : UnsafeMutablePointer<FhDeviceManager>
   // var swapChainManage : SwapChainManager
    let pipelineLayout : VkPipelineLayout
    var commandBuffers : Array<VkCommandBuffer>
    public init() {
        window = fhCreateWindow(600, 800, "Firehill")
        deviceManager = createDeviceManager(window.pInstance)
        assert(window.pInstance != nil)
       // swapChainManage = SwapChainManager()
        pipelineLayout = fhCreatePipelineLayout(deviceManager.pointee.device)
        pPipeline = createGraphicsPipeline(
            deviceManager,
            pipelineLayout,
            nil,
            0,
            window.height, 
            window.width, 
            "Shaders/vert.spv", 
            "Shaders/frag.spv"
         )
        commandBuffers = []
    }

    public func collectEvents(){
        fhCollectEvents()
    }

    public func drawFrame(){}
}

struct SwapChainManager{
    var swapChainImageFormat   : VkFormat
    var swapChainExtent        : VkExtent2D
    var swapChainFrameBuffers  : Array<VkFramebuffer>
    var swapChainImages        : Array<VkImage>
    var swapChainImageViews    : Array<VkImageView>
    
    var renderPass             : VkRenderPass
    
    var depthImages            : Array<VkImage>
    var depthImagesMemorys     : Array<VkDeviceMemory>
    var depthImageViews        : Array<VkImageView>

    var windowExtent           : VkExtent2D
    var swapChain              : VkSwapchainKHR
    var imageAvailableSemaphores : Array<VkSemaphore>
    var renderFinishedSemaphores : Array<VkSemaphore>
    var inFlightFences           : Array<VkFence>
    var imagesInFlight           : Array<VkFence>
    var currentFrame             : Int
    var subPass                  : UInt32

    func getNextImage(){

    }

}