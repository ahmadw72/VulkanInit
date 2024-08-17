#include "VkCommandBuffer.h";
#include<assert.h>

CVkCommandBuffer::CVkCommandBuffer()
{
    m_VkCommandPool = VK_NULL_HANDLE;
    m_VkQue = VK_NULL_HANDLE;
    m_VkCommandBuffer = VK_NULL_HANDLE;
    
    assert(false);  // should never be called // 
    

}


CVkCommandBuffer::CVkCommandBuffer(VkPhysicalDevice vkPhyDev, VkDevice vkDev, VkQueue vkQue, VkCommandPool vkCmdPool) : m_VkPhysicalDevice(vkPhyDev), m_VkDevice(vkDev), m_VkQue(vkQue), m_VkCommandPool(vkCmdPool)
{
    


}


CVkCommandBuffer::~CVkCommandBuffer()
{
    
}

VkCommandBuffer CVkCommandBuffer::GetCommandBuffer()
{
    return m_VkCommandBuffer;
}