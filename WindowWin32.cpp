#include "WindowWin32.h"
#include "VulkanInit.h"
void CWindowWin32::SetRenderer(void* pRenderer)
{
    m_pvRenderer = pRenderer;
}

// constructor //

CWindowWin32::CWindowWin32()
{
    m_hInstance = GetModuleHandle(NULL);

    m_hWnd = 0;
    

}

CWindowWin32::~CWindowWin32()
{

}
//
// window proc //

LRESULT WindowProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{


    return DefWindowProc(hWnd, uiMsg, wParam, lParam);
}

// creates a window //
int CWindowWin32::Create(int iWidth, int iLength, const char* pszAppName)
{

    WNDCLASSEX wndClassEx = {0};
    wndClassEx.cbSize = sizeof(WNDCLASSEX);

    wndClassEx.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    wndClassEx.lpszClassName = (LPCTSTR)L"BlueThunder";
    wndClassEx.lpfnWndProc = (WNDPROC)&WindowProc;
    
    if( ! RegisterClassEx(&wndClassEx ))
    {
        printf("failed to register window class struct\n");
        return 0;
    }

    m_hWnd = CreateWindowEx(0, wndClassEx.lpszClassName, (LPCTSTR)m_strAppName.c_str(), 0,100,100, iWidth, iLength, NULL, NULL, m_hInstance, NULL);

    if ( m_hWnd == NULL )
    {
        printf("failed to create window\n");
        return 0;
    }

    ShowWindow(m_hWnd, SW_SHOW);
    UpdateWindow(m_hWnd);

   // StartMsgLoop();
    // msg pump //






    return 1;
}

HWND CWindowWin32::GetHandle()
{
    return m_hWnd;
}

HINSTANCE CWindowWin32::GetInstance()
{
    return m_hInstance;
}




/**
 * @brief 
 * 
 * private: message loop
 * 
 */

void CWindowWin32::StartMsgLoop()
{
    MSG Msg;
    m_bLoop = true;
    while (m_bLoop)
    {
        if ( PeekMessage(&Msg, m_hWnd,0,0, PM_REMOVE) )
            {
                ::TranslateMessage(&Msg);
                ::DispatchMessageA(&Msg);

            }
        else
            ((CVulkanInit*)m_pvRenderer)->Draw();
            //printf("..\n");

    }





}