#include "VkBuffer_Device.h"



CVkBuffer_Device::CVkBuffer_Device()
{
    m_VkPhysicalDevice = VK_NULL_HANDLE;
    m_VkDevice = VK_NULL_HANDLE;
}

CVkBuffer_Device::~CVkBuffer_Device()
{

}
/*
void CVkBuffer_Device::SetPhysicalDevice(VkPhysicalDevice phyDev)
{
    m_VkPhysicalDevice = phyDev;
}

void CVkBuffer_Device::SetDevice(VkDevice dev)
{
    m_VkDevice = dev;
}
void CVkBuffer_Device::SetQueue(VkQueue queue)
{ 
    m_VkQueue = queue;
}
void CVkBuffer_Device::SetCommandPool(VkCommandPool cmdPool)
{
    m_VkCommandPool = cmdPool;
}

*/
void CVkBuffer_Device::SetVulkanData(VkPhysicalDevice phyDev, VkDevice dev, VkCommandPool cmdPool, VkQueue queue)
{
     m_VkPhysicalDevice = phyDev;
     m_VkDevice = dev;
     m_VkQueue = queue;
    m_VkCommandPool = cmdPool;
}
/**
 * @brief  usage -> vertex buffer, index buffer, etc
 * 
 * @param usage 
 * @return true 
 * @return false 
 */
bool CVkBuffer_Device::TxDataToDevice(VkBufferUsageFlags usage, const char* pcData, size_t uiSize)
{
    CVkBuffer_Host vkBuffStaging;
    VkDeviceSize size = uiSize;
    if ( vkBuffStaging.Create(m_VkPhysicalDevice, m_VkDevice, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT  ) != true )
    {
        printf("vkBuffStaging.Create(m_VkPhysicalDevice, m_VkDevice, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT  ) ...\n");
        assert(false);
    }

    vkBuffStaging.CopyMem(pcData, uiSize);


  // create device buffer //
    if ( Create(/*m_VkPhysicalDevice, m_VkDevice, */size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT ) != true )
    {
        printf("vkVertexBuffer.Create(m_VkPhysicalDevice, m_VkDevice, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT  ) ...\n");
        assert(false);
    }

    CVkSingleTimeCommandBuffer  singleTimeCmdBuff(m_VkPhysicalDevice, m_VkDevice, m_VkQueue, m_VkCommandPool);
    singleTimeCmdBuff.vkCmdCpyBuffer(vkBuffStaging.GetVkBuffer(), m_VkBuffer, size) ;
    return true;
  
}
/**
 * @brief this function is replica to CVkBuffer_Host::Create(); Despite this common functionality, host side and device side buffers 
 * have to be different classes as the DEVICE SIDE OBJECT CONTAINS AN OBJECT OF HOST SIDE FOR STAGGING PURPOSES; LOOK IN MoveDataToDevice function
 * 
 * @param rVkPhysicalDevice 
 * @param rVkDevice 
 * @param size 
 * @param usageFlags 
 * @param memPropFlags 
 * @param sharingMode 
 * @return true 
 * @return false 
 */
bool CVkBuffer_Device::Create(/*VkPhysicalDevice rVkPhysicalDevice, VkDevice rVkDevice, */VkDeviceSize size, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memPropFlags, VkSharingMode sharingMode)
{
      VkBufferCreateInfo buffCreateInfo ={};

    buffCreateInfo.sharingMode = sharingMode;
    buffCreateInfo.size = size;
    buffCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    //buffCreateInfo.usage
    buffCreateInfo.usage = usageFlags;

    //m_VkBufferUsageFlags = usageFlags;

    if(vkCreateBuffer(m_VkDevice, &buffCreateInfo, nullptr, &m_VkBuffer)  != VK_SUCCESS )
    {
        printf("vkCreateBuffer(rVkDevice, &buffCreateInfo, nullptr, &m_VkBuffer) failed \n");
        assert(0);
        return false;

    }

    VkMemoryRequirements memReq = {};
    vkGetBufferMemoryRequirements(m_VkDevice, m_VkBuffer, &memReq);



    VkMemoryAllocateInfo memAllocInfo ={};
    memAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memAllocInfo.allocationSize = memReq.size;
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
/**
 * @brief  Again, this function is replice of CVkBuffer_Host::FindDeviceMemoryIndex; for reason of this bad design, read the 
 * description of Create
 * 
 * @param memPropertyFlags 
 * @param memBits 
 * @return int 
 */

int  CVkBuffer_Device::FindDeviceMemoryIndex(VkMemoryPropertyFlags memPropertyFlags, uint32_t memBits )
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

/**
 * @brief returns the VkBuffer
 * 
 * @return VkBuffer 
 */
VkBuffer CVkBuffer_Device::GetVkBuffer()
{
    return m_VkBuffer;
}