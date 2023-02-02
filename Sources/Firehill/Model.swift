import CFirehill

struct Model{
    struct Vertex<T: FloatingPoint>{
            var position: Vector2D<T>
            init(position: Vector2D<T>){
                self.position = position
            }

    }
    //var vertex : Vertex
    var vertexBuffer : OpaquePointer
    var vertexBufferMemory : OpaquePointer 
    var vertexCount : UInt32

    init<T : FloatingPoint>(vertices: Array<Vertex<T>>, device: OpaquePointer,physicalDevice: OpaquePointer){
        //vertex = vertices[0]
        vertexCount = UInt32(vertices.count)
        assert(vertexCount >= 3, "Vertex count must be at least 3")
        let bufferSize : UInt64 = UInt64(MemoryLayout.size(ofValue: vertices[0])) * UInt64(vertexCount)
        vertexBuffer = fhCreateVertexBuffer(device, bufferSize)
        vertexBufferMemory =  fhGetDeviceMemory( device,  physicalDevice,  vertexBuffer, UInt32(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT.rawValue | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT.rawValue))
    
        var data : UnsafeMutableRawPointer?
        vkMapMemory(device, vertexBufferMemory, 0, bufferSize, 0, UnsafeMutablePointer<UnsafeMutableRawPointer?>(&data))
        vertices.withUnsafeBufferPointer({
            memcpy(data, $0.baseAddress, Int(bufferSize))
        })
        vkUnmapMemory(device, vertexBufferMemory)

    }



    func cleanModel(device: OpaquePointer){
        vkDestroyBuffer(device, vertexBuffer, nil)
        vkFreeMemory(device, vertexBufferMemory, nil)
    }

}

extension Model.Vertex{
        static func getBindingDescription() -> Array<VkVertexInputBindingDescription>{
            var bindingsDescriptions = [VkVertexInputBindingDescription()]
            bindingsDescriptions[0].binding = 0 
            bindingsDescriptions[0].stride = UInt32(8)
            bindingsDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX 
            return bindingsDescriptions
        }
        static func getAttributeDescription() -> Array<VkVertexInputAttributeDescription>{
            var attributeDescriptions = [VkVertexInputAttributeDescription()]
            attributeDescriptions[0].binding = 0 
            attributeDescriptions[0].location = 0
            attributeDescriptions[0].format  = VK_FORMAT_R32G32_SFLOAT 
            attributeDescriptions[0].offset  = 0
            return attributeDescriptions
        }
}

extension Model{
    func bind(commandBuffer: OpaquePointer){
        var buffers = [ (OpaquePointer?)(vertexBuffer)]
        var offsets: [UInt64] = [0]
        let pbuffers = buffers.withUnsafeBufferPointer({ $0.baseAddress});
        let pOffesets = offsets.withUnsafeBufferPointer({$0.baseAddress});
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, pbuffers, pOffesets)
    }
    
    func draw(commandBuffer: OpaquePointer){
        vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0)
    }


}