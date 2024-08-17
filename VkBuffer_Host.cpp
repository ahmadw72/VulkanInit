#include "VkBuffer_Host.h"
#include <stdio.h>
#include <cstring>
#include <assert.h>
/***
 * Version #1
 * 1. creates buffer, 
 * 2. use Create method to create the buffer 
 * 3. at the moment the derived class do nothing, but it is better to use the drive class for future funcitonality.
 * 4. host side base class.  gpu side buffers in drived classes.
 * 5. should be used int CBufferFactory
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
*/

CVkBuffer_Host::CVkBuffer_Host()
{
    m_VkBuffer = VK_NULL_HANDLE;
    m_VkDeviceMemory = VK_NULL_HANDLE;
    m_VkDevice = VK_NULL_HANDLE;
    m_VkPhysicalDevice = VK_NULL_HANDLE;

}
/**
 * @brief copies rcvData into buffer memory. should only be used on host side
 * 
 * @param rcvData 
 * @return true 
 * @return false 
 */
bool CVkBuffer_Host::CopyMem(const char* pcData, size_t uiSize)
{
    //assert(m_VkBufferUsageFlags != vk_bu)

    void* pvData;

    if ( vkMapMemory(m_VkDevice, m_VkDeviceMemory, 0, uiSize, 0, &pvData) != VK_SUCCESS )
    {
        printf("vkMapMemory(m_VkDevice, m_VkDeviceMemory, 0, rcvData.size(), 0, &pvData)..\n");
        assert(false);
        return false;
    }
    memcpy(pvData, pcData, uiSize);
    vkUnmapMemory(m_VkDevice, m_VkDeviceMemory);

    return true;
}
bool CVkBuffer_Host::Create(VkPhysicalDevice rVkPhysicalDevice, VkDevice rVkDevice, VkDeviceSize size, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memPropFlags, VkSharingMode sharingMode)
{
    m_VkDevice = rVkDevice; m_VkPhysicalDevice = rVkPhysicalDevice;
    VkBufferCreateInfo buffCreateInfo ={};

    buffCreateInfo.sharingMode = sharingMode;
    buffCreateInfo.size = size;
    buffCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    //buffCreateInfo.usage
    buffCreateInfo.usage = usageFlags;
    

    m_VkBufferUsageFlags = usageFlags;

    if(vkCreateBuffer(rVkDevice, &buffCreateInfo, nullptr, &m_VkBuffer)  != VK_SUCCESS )
    {
        printf("vkCreateBuffer(rVkDevice, &buffCreateInfo, nullptr, &m_VkBuffer) failed \n");
        assert(0);
        return false;

    }

    VkMemoryRequirements memReq ;//= {};
    vkGetBufferMemoryRequirements(m_VkDevice, m_VkBuffer, &memReq);



    VkMemoryAllocateInfo memAllocInfo ={};
    memAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memAllocInfo.allocationSize = memReq.size;
    //memReq.
    memAllocInfo.memoryTypeIndex = FindDeviceMemoryIndex(memPropFlags, memReq.memoryTypeBits);

    if( vkAllocateMemory(m_VkDevice,&memAllocInfo, nullptr, &m_VkDeviceMemory) != VK_SUCCESS )
    {
        printf("vkAllocateMemory(m_VkDevice,&memAllocInfo, nullptr, &m_VkDeviceMemory) ..failed\n");
        assert(false);
        return false;

    }

    if (vkBindBufferMemory(m_VkDevice, m_VkBuffer, m_VkDeviceMemory, 0) != VK_SUCCESS )
    {
        printf("vkBindBufferMemory(m_VkDevice, m_VkBuffer, m_VkDeviceMemory, 0)");
        assert(false);
        return false;
    }
    
    return true;
}

CVkBuffer_Host::~CVkBuffer_Host()
{

}
VkBuffer CVkBuffer_Host::GetVkBuffer()
{
    return m_VkBuffer;
}

VkDeviceMemory CVkBuffer_Host::GetVkDeviceMemory()
{
    return m_VkDeviceMemory;
}

int  CVkBuffer_Host::FindDeviceMemoryIndex(VkMemoryPropertyFlags memPropertyFlags, uint32_t memBits )
{
    VkPhysicalDeviceMemoryProperties physicalDevMemProp = {};
    vkGetPhysicalDeviceMemoryProperties(m_VkPhysicalDevice, &physicalDevMemProp);

    for(int iCount=0; iCount<physicalDevMemProp.memoryTypeCount; iCount++)
    {
        if ( (memBits & (1 << iCount)) && (physicalDevMemProp.memoryTypes[iCount].propertyFlags & memPropertyFlags ) == memPropertyFlags )
                 return iCount;
    }

    printf("failed in FindDeviceMemoryIndex(VkMemoryPropertyFlags memPropertyFlags, uint32_t memBits )");
    assert(0);
    return -1;


}