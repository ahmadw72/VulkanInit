#include "VkSingleTimeCommandBuffer.h"
#include <assert.h>
#include <stdio.h>

CVkSingleTimeCommandBuffer::CVkSingleTimeCommandBuffer(/* args */)
{
    assert(false); // call the constructor with commandpool
}

CVkSingleTimeCommandBuffer::~CVkSingleTimeCommandBuffer()
{
    if ( vkEndCommandBuffer(m_VkCommandBuffer) != VK_SUCCESS )
    {
        printf("vkEndCommandBuffer(m_VkCommandBuffer) failed \n");
        assert(false);
    }

    VkSubmitInfo submitInfo = {};

    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pCommandBuffers = &m_VkCommandBuffer;
    submitInfo.commandBufferCount = 1;

    if ( vkQueueSubmit(m_VkQue, 1, &submitInfo, VK_NULL_HANDLE ) != VK_SUCCESS )
    {
        printf("vkQueueSubmit(m_VkQue, 1, &submitInfo, VK_NULL_HANDLE ) failed \n");
        assert(false);
    }

    if ( vkQueueWaitIdle(m_VkQue) != VK_SUCCESS )
    {
        printf("vkQueueWaitIdle(m_VkQue) .. failed \n");
        assert(false);
    }

    vkFreeCommandBuffers(m_VkDevice, m_VkCommandPool, 1, &m_VkCommandBuffer);

}


void CVkSingleTimeCommandBuffer::vkCmdCpyBuffer(VkBuffer srcBuff, VkBuffer dstBuffer, VkDeviceSize vkDevSize)
{
    VkBufferCopy buffCpy ={};
    buffCpy.size = vkDevSize;

    vkCmdCopyBuffer(m_VkCommandBuffer, srcBuff, dstBuffer,1, &buffCpy);



}

CVkSingleTimeCommandBuffer::CVkSingleTimeCommandBuffer(VkPhysicalDevice vkPhyDev, VkDevice vkDev, VkQueue vkQue, VkCommandPool vkCmdPool):CVkCommandBuffer::CVkCommandBuffer(vkPhyDev, vkDev, vkQue, vkCmdPool)

{
    //CVkCommandBuffer::CVkCommandBuffer(vkPhyDev, vkDev, vkQue, vkCmdPool);
    VkCommandBufferAllocateInfo cmdBuffAllocInfo = {};
    cmdBuffAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cmdBuffAllocInfo.commandBufferCount = 1;
    cmdBuffAllocInfo.commandPool = m_VkCommandPool;
    cmdBuffAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO; 
    cmdBuffAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;


    if ( vkAllocateCommandBuffers(m_VkDevice, &cmdBuffAllocInfo, &m_VkCommandBuffer) != VK_SUCCESS )
    {
        printf("vkAllocateCommandBuffers(m_VkDevice, &cmdBuffAllocInfo, &m_VkCommandBuffer) failed ... \n");
        assert(false);

    } 


    VkCommandBufferBeginInfo cmdBuffBeginInfo ={};

    ///cmdBuffAllocInfo.
    cmdBuffBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmdBuffBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    if ( vkBeginCommandBuffer(m_VkCommandBuffer, &cmdBuffBeginInfo) != VK_SUCCESS )
    {
        printf(" vkBeginCommandBuffer(m_VkCommandBuffer, &cmdBuffBeginInfo)...failed\n");
        assert(false);
    }



}