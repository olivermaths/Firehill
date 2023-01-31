#pragma once
#include "core.h"
typedef struct FhWindow_t{
    GLFWwindow *pInstance;
    uint32_t height, width;
}FhWindow;


FhWindow fhCreateWindow(uint32_t height, uint32_t width, const char* title);

void fhCollectEvents();