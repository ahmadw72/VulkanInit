#pragma once


#include "VkBuffer_Host.h"
#include "VkSingleTimeCommandBuffer.h"
#include <stdio.h>
#include <assert.h>
#include <vulkan.h>
#include "vertex.h"
class CVkBuffer_Device
{

    public:
        CVkBuffer_Device();
        ~CVkBuffer_Device();
        virtual bool MoveDataToDevice()=0; // moves the data to gpu //
       /* void SetPhysicalDevice(VkPhysicalDevice phyDev);
        void SetDevice(VkDevice dev);
        void SetCommandPool(VkCommandPool   cmdPool);
        void SetQueue(VkQueue queue);*/

        void SetVulkanData(VkPhysicalDevice phyDev, VkDevice dev, VkCommandPool cmdPool, VkQueue queue);
       // bool CopyMem(const char* pcData, size_t uiSize);
       VkBuffer     GetVkBuffer();
    private:
    protected:
        VkBuffer    m_VkBuffer;
        VkDeviceMemory  m_VkDeviceMemory;
        VkDeviceSize    m_VkDeviceSize;

        VkDevice m_VkDevice;
        VkPhysicalDevice m_VkPhysicalDevice;
        VkCommandPool   m_VkCommandPool;
        VkQueue     m_VkQueue;

    protected:
        int FindDeviceMemoryIndex(VkMemoryPropertyFlags memPropertyFlags, uint32_t memBits );
        bool         TxDataToDevice(VkBufferUsageFlags usage, const char* pcData, size_t uiSize);
        
        bool Create(/*VkPhysicalDevice rVkPhysicalDevice, VkDevice rVkDevice, */VkDeviceSize size, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memPropFlags, VkSharingMode sharingMode=VK_SHARING_MODE_EXCLUSIVE);

};