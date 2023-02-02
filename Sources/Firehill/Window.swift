import GLFW

private var isInitialized : Bool = false
struct Window{
    let pWindow : OpaquePointer
    let title : String
    var height, width: UInt32

    init(title: String, height: UInt32, width: UInt32){
        self.title = title
        self.height = height
        self.width = width
        if !isInitialized {
            glfwInit()
            isInitialized = true
        }
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        self.pWindow = glfwCreateWindow( Int32(self.width),  Int32(self.height), self.title, nil, nil)
    }

    func collectEvents(){
        glfwPollEvents()
    }
}