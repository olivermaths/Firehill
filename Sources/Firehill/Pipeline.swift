import Foundation
import CFirehill
class Pipeline{
    let vertexShader : OpaquePointer?
    let fragShader : OpaquePointer?
    let pipelineLayout : OpaquePointer?
    let graphicPipeline : OpaquePointer?

    init(vertexPath: String, fragmentPath: String, device: OpaquePointer, width: UInt32, height: UInt32, renderPass: OpaquePointer, subpass: UInt32){
        let vertexData = try? Data(contentsOf: URL(fileURLWithPath: vertexPath))
        let fragData  = try? Data(contentsOf: URL(fileURLWithPath: fragmentPath))     
        let pVertexData = vertexData?.withUnsafeBytes({
            return $0.baseAddress?.assumingMemoryBound(to: UInt32.self)
        })
        let pFragData = fragData?.withUnsafeBytes({
            return $0.baseAddress?.assumingMemoryBound(to: UInt32.self)
        })

        vertexShader =  fhCreateShaderModule(device, UInt64((vertexData?.count)!),  pVertexData)
        fragShader =  fhCreateShaderModule(device, UInt64((fragData?.count)!),  pFragData)
        pipelineLayout = fhCreatePipelineLayout(device)
        var bindingsDescriptions = Model.Vertex<Float>.getBindingDescription()
        var attributeDescriptions = Model.Vertex<Float>.getAttributeDescription()
        
        graphicPipeline = fhCreateVkPipeline( device, vertexShader,   fragShader, pipelineLayout, renderPass, bindingsDescriptions[0], attributeDescriptions[0], subpass, width, height )
    }
}