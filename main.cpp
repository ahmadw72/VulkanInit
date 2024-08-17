#include "WindowWin32.h"
#include "VulkanInit.h"
#include "glm\glm.hpp"


struct vertex
{
    glm::vec3 pos;
    glm::vec4 clr;
    glm::vec2 uv;


};// stVert;


int main()
{

    std::vector<vertex> vertices = {{{0.0, 0.0, 0.0},{0.0, 0.0, 0.0, 1.0},{0.0, 0.0}}, {{0.0, 0.0, 0.0},{0.0, 0.0, 0.0, 1.0},{0.0, 0.0}}, {{0.0, 0.0, 0.0},{0.0, 0.0, 0.0, 1.0},{0.0, 0.0}}};
    std::vector<float> fvertices = {0.0, 0.0, 0.0,0.0, 0.0, 0.0, 1.0,0.0, 0.0,0.0, 0.0, 0.0,0.0, 0.0, 0.0, 1.0,0.0, 0.0,0.0, 0.0, 0.0,0.0, 0.0, 0.0, 1.0,0.0, 0.0};
    char* pData1 = (char*)vertices.data();
    char* pData2 = (char*)fvertices.data();



    CVulkanInit vulkan;
    
    CWindowWin32 wndw;
    printf("main program");

    int iWndHeight = 700;
    int iWndWidth = 800;
    wndw.Create(iWndWidth, iWndHeight, "HelloWorld");

    HWND hwnd = wndw.GetHandle();
    HINSTANCE hinst = wndw.GetInstance();
    vulkan.Init(hwnd, hinst, iWndHeight, iWndWidth);
    //vulkan.CreateSurfaceWin32(hwnd, hinst, iWndHeight, iWndWidth);

   // vulkan.Init();
    

    
    wndw.SetRenderer(&vulkan);
    wndw.StartMsgLoop()   ;

    printf("after create");



}