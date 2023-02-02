import CFirehill
import Foundation

class Device{
    struct QueueFamilyIndices{
        let presentQueueIndex : Optional<UInt32>
        let computeQueueIndex : Optional<UInt32>
    }
    let instance : OpaquePointer
    let device   : OpaquePointer
    let physicalDevice : OpaquePointer
    let presentQueue   : OpaquePointer
    let computeQueue   : OpaquePointer
    let indices : QueueFamilyIndices
    
    init(){
        instance = fhCreateVkInstance("Triangle")
        physicalDevice = fhPickPhysicalDevice(instance)
        indices    =     Device.findQueueFamilies(physicalDevice: physicalDevice)
        device =         fhCreateVkDevice(physicalDevice, indices.presentQueueIndex!, indices.computeQueueIndex!)
        presentQueue =   fhGetVkQueue(device, indices.presentQueueIndex!)
        computeQueue =   fhGetVkQueue(device, indices.computeQueueIndex!)
    }
    deinit{
        vkDestroyInstance(instance, nil)
        vkDestroyDevice(device, nil)
    }

}

private protocol DeviceHelpers {

    static func findQueueFamilies(physicalDevice: OpaquePointer) -> Device.QueueFamilyIndices


}

extension Device : DeviceHelpers{

    func createInstance(){
    }

    static func findQueueFamilies(physicalDevice: OpaquePointer) -> QueueFamilyIndices {
        var queueFamilyCount : UInt32 = 0
        var presentQueueIndex : UInt32? = nil
        var computeQueueIndex : UInt32? = nil
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount,  nil)

        var queueFamilies : Array<VkQueueFamilyProperties> = Array(repeating: VkQueueFamilyProperties(), count: Int(queueFamilyCount))
        queueFamilies.withUnsafeMutableBufferPointer({
            vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount,  $0.baseAddress)
        })

        for (idx, family) in queueFamilies.enumerated(){
            if (family.queueFlags & UInt32(VK_QUEUE_GRAPHICS_BIT.rawValue)) != 0 {
                presentQueueIndex = UInt32(idx)
            }
            if family.queueCount > 1 && ((family.queueFlags & UInt32(VK_QUEUE_COMPUTE_BIT.rawValue)) != 0) {
                computeQueueIndex = UInt32(idx)
            }
        }


        return QueueFamilyIndices(presentQueueIndex: presentQueueIndex, computeQueueIndex: computeQueueIndex)

    }


       


}