#ifndef  _VULKAN_BUFFER_H_
#define  _VULKAN_BUFFER_H_
#include <windows.h>
#include <vector>

#include   <vulkan.h>
class CVkBuffer_Host   // host side //
{
    private:
    VkBuffer    m_VkBuffer;
    VkDeviceMemory  m_VkDeviceMemory;
    VkDeviceSize    m_VkDeviceSize;

    // above three may be deleted //
    protected:
    VkDevice m_VkDevice;
    VkPhysicalDevice m_VkPhysicalDevice;


    VkDeviceMemory GetVkDeviceMemory();

    VkBufferUsageFlags m_VkBufferUsageFlags;
    
   
    

    public:
        CVkBuffer_Host();
        ~CVkBuffer_Host();
    
        VkBuffer GetVkBuffer();


        bool Create(VkPhysicalDevice rVkPhysicalDevice, VkDevice rVkDevice, VkDeviceSize size, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memPropFlags, VkSharingMode sharingMode=VK_SHARING_MODE_EXCLUSIVE);
       
       
        bool CopyMem(const char* pcData, size_t uiSize);
    protected:
        int FindDeviceMemoryIndex(VkMemoryPropertyFlags memPropertyFlags, uint32_t memBits );
};

#endif