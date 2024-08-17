#ifndef _WIN32_WINDOWCREATE_
#define _WIN32_WINDOWCREATE_

#include <Windows.h>
#include <string>

class CWindowWin32
{
    public:
        CWindowWin32();
        ~CWindowWin32();
        int Create(int iWidth, int iLength, const char* pszAppName);

        HINSTANCE GetInstance();
        HWND GetHandle(); 
        
        void StartMsgLoop();

        void SetRenderer(void* pvRenderer);
    private:
        HINSTANCE m_hInstance;
        HWND      m_hWnd;

        std::wstring m_strAppName;

        void*    m_pvRenderer;

        bool m_bLoop;


};


#endif