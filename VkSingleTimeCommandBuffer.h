#pragma once
#include "VkCommandBuffer.h"
class CVkSingleTimeCommandBuffer : public CVkCommandBuffer
{
private:
    /* data */
public:
    CVkSingleTimeCommandBuffer(/* args */);
    ~CVkSingleTimeCommandBuffer();
    CVkSingleTimeCommandBuffer(VkPhysicalDevice vkPhyDev, VkDevice vkDev, VkQueue vkQue, VkCommandPool vkCmdPool);//,  VkQueue m_VkQue;  // que for to)

   void vkCmdCpyBuffer(VkBuffer srcBuff, VkBuffer destBuff, VkDeviceSize vkDevSize);
};


