#pragma once
#include "VkBuffer_Host.h"
#include <vulkan.h>

class CVkCommandBuffer //: public CVkBuffer
{
    public:
        CVkCommandBuffer();
        ~CVkCommandBuffer();
        CVkCommandBuffer(VkPhysicalDevice vkPhyDev, VkDevice vkDev, VkQueue vkQue, VkCommandPool vkCmdPool);

        VkCommandBuffer GetCommandBuffer();

    protected:
        VkCommandPool m_VkCommandPool;
        VkQueue m_VkQue;  // que for to submit the commands // 

        VkCommandBuffer m_VkCommandBuffer;

        bool CreateCommandPool();

        VkDevice    m_VkDevice;
        VkPhysicalDevice m_VkPhysicalDevice;

        


};