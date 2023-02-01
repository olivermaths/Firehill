import CFirehill
import Foundation
class Device{
    let instance : OpaquePointer
    let device   : OpaquePointer
    let physicalDevice : OpaquePointer
    let presentQueue   : OpaquePointer
    let computeQueue   : OpaquePointer
    init(){
        instance = fhCreateVkInstance("Triangle")
        physicalDevice = fhPickPhysicalDevice(instance)
        let indices = findQueueFamilies(physicalDevice)
        device = fhCreateVkDevice(physicalDevice, indices.presentQueueIndex, indices.computeQueueIndex)
        presentQueue = fhGetVkQueue(device, indices.presentQueueIndex)
        computeQueue = fhGetVkQueue(device, indices.computeQueueIndex)
    }

}

protocol DeviceManager {}

extension Device : DeviceManager{

}