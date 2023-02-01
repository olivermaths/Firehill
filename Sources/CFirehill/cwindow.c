#include "include/cwindow.h"
#include "glfw/glfw3.h"




static uint8_t isInitialized = 0;



FhWindow fhCreateWindow(uint32_t height, uint32_t width, const char* title){
    printf("nor here %s is not here\n", title);
    if(!isInitialized){
        c_assert(glfwInit() == GLFW_TRUE);
        isInitialized  = 1;
    }
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    FhWindow instance = {
        .pInstance = glfwCreateWindow( width,  height, title, NULL, NULL),
        .height = height,
        .width = width
    };
    return instance;
}
void fhCollectEvents(){
    glfwPollEvents();
}