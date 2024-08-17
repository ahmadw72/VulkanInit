#ifndef _VULKANINIT_H
#define _VULKANINIT_H

#include "C:\\VulkanSDK\\1.3.224.0\\Include\\vulkan\\vulkan.h"
#include <vector>

#include <Windows.h>   // should be befoe the inclusion of vulkan_win32.h //
#include "C:\\VulkanSDK\\1.3.224.0\\Include\\vulkan\\vulkan_win32.h"

#include "VkVertexBuffer.h"
#include "UniforBuffer_MVP.h"

#include "VTexture.h"

#include <windows.h>

class CVulkanInit
{
    struct
    {
        /* data */
        std::vector<VkPhysicalDevice>	m_VkPhysicalDevices;   // all the physical devices // 
	    std::vector<VkPhysicalDeviceProperties> m_VkPhysicalDeviceProperties; // properties of each physical device //											//physical devices, say device at index x has n properties, we will intialize it m_PhysicalDeviceProperties[x][n] //
	    std::vector< std::vector < VkQueueFamilyProperties> > m_VkQueuefamilyProperties ;// properties of que //
	    std::vector <  std::vector<VkBool32>   > m_bQueueSupportsPresent; //
        std::vector < std::vector<VkSurfaceFormatKHR> > m_VkSurfaceFormats; // formats //
        std::vector < VkSurfaceCapabilitiesKHR> m_VkSurfaceCapabilities; 

       

	
    } m_Devices;
    std::vector<VkExtensionProperties> m_InstExtProps;
    public:
     CVulkanInit();  
        VkBool32 CreateSurfaceWin32_02(HWND hWnd, HINSTANCE hInstance, int iHeight, int iWidth);
        int CreateInstance_01();
        int GetPhysicalDevice_03();
        int CreateLogicalDevice_04();

        int CreateSwapChain_05();
        int CreateCommandBuffers_06();

        int CreateRenderPass_07();

        int CreatePipeline_13();

        int CreateShader(const char* pszShaderFileName, VkShaderModule& rVkShaderModule);

        VkFormat GetCurrnetSurfaceFormat();

        int CreateFrameBuffer_08();

        int RecordCommandBuffer(unsigned int uiImageIndex);

        int Draw();

        int FindMemoryIndexByFlagandType(VkMemoryPropertyFlagBits memPropFlagBits, uint32_t uiMemTypeBits);

        VkPhysicalDevice GetSelectedPhysicalDevice();

        bool CreateSynchObjects_14();

        bool Init(HWND hwnd, HINSTANCE hinst, int iWndHeight, int iWndWidth);

        void CreateVertexBuffer_09();

        void CreateDescriptorPool();

        void CreateDescriptorSetLayouts();

        void CreateDescriptorSets();

        void CreateDescriptorSets_12();

        void CreateUniformBuffer_10();

        void AvailableLayers();
   private:

        VkPipeline m_VKGraphicsPipeline;
        VkInstance m_VkInstance;
        VkSurfaceKHR m_VkSurfaceKHR;
        VkSwapchainKHR m_VkSwapChain;

        std::vector < VkImage  >  m_VkSwapChainImages;
        
        std::vector <VkCommandBuffer> m_VkCommandBuffers;

        std::vector < VkFramebuffer> m_VkFrameBuffers;
        
        std::vector< VkImageView> m_VkImageViews;
        
        VkDevice m_VkDevice;
        VkRenderPass m_VkRenderPass;
        int m_iDeviceIndex;
        int m_iQIndex;

        VkCommandPool m_VkCommandPool;

        
        int m_iWindowWidth;
        int m_iWindowHeight;

        VkShaderModule m_VKVertexShaderModule, m_VKFragmentShaderModule;


        VkQueue m_VKPresentQue; // que that supports present //

        //synchronization objects //
/*          

        std::vector<VkSemaphore> m_vSemaphores_Wait;   // signaled when an image is availabe, used in vkGetNextImage, vkQueueSubimt waits for this semaphore to set //
        std::vector<VkSemaphore> m_vSemaphores_RenderFinsh; //  signaled when a command buffer finished rendering, vkQueueSubmit sets this semaphoe for next function in synchronization pipeline 

        std::vector<VkFence>    m_vFences_Frame; // synchronizes each frame //

        // end synchronization objects //
    at this point, I dont understand the need of having array of synch objects, so I will look into this later
*/

    VkSemaphore m_VkSemaphore_Wait, m_VkSemaphore_RenderFinish;

    VkFence    m_VkFence_Frame; // synchronizes each frame //

    CVkVertexBuffer m_VertexBuffer;

    CUniformBuffer_MVP  m_UniformBuffer;

    // descriptor set //

    VkDescriptorPool    m_VkDescriptorPool;
    std::vector<VkDescriptorSet> m_vVkDescriptorSets;
    VkDescriptorSetLayout   m_VkDescriptorSetLayout;

    //VkPipelineLayoutCreateInfo m_VkPipeLineLayoutCreateInfo;

    VkPipelineLayout m_VkPipelineLayout;

    // texture //
    CVTexture m_Texture;

    std::vector<char*>  m_vExtensions;
    std::vector<char*>  m_vLayers;

    protected:
};
#endif