#ifndef _VULKAN_TEXTURE_H_
#define _VULKAN_TEXTURE_H_

#include <vulkan.h>

#include <vector>
#include "vertex.h"


class CVTexture
{
    public:    
        VkSampler GetSampler();

        VkImageView GetImageView();
    /* data */
    private:
        int    m_iWidth;
        int    m_iHeight;
        VkImage     m_VkStaggingImage;
        VkImage     m_VkTextureImage;
        VkImageView m_VkImageView;
        VkSampler   m_VkSampler;
        VkDeviceMemory  m_VkDevMemStagging, m_VkDevMemTexture;

        
        VkPhysicalDevice    m_VkPhysicalDevice;
        VkDevice    m_VkDevice;
        VkCommandPool m_VkCommandPool;
        VkQueue         m_VkQueue;


        std::vector<char> m_vData;

        bool CreateSampler(VkDevice& rVkDevice);

        bool CreateImages();

        void TxDataToDevice();

        bool CreateTextureImageView();

        

        // helpers //

        void LayoutTransition(VkCommandBuffer vkCmdBuff, VkImage vkImage, VkPipelineStageFlags srcPipeLineStageFlag, VkPipelineStageFlags dstPipeLineStageFlag, VkAccessFlags srcAccessFlags, VkAccessFlags dstAccessFlags,  VkImageLayout newLayout, VkImageLayout oldLayout );// VkCommandBuffer vkCmdBuff,  VkImage vkImage, VkPipelineStageFlags srcFlag, VkPipelineStageFlags dstFlag, VkImageLayout newLayout, VkImageLayout oldLayout=VK_IMAGE_LAYOUT_PREINITIALIZED);

        bool CreateImage(VkImageTiling tiling, VkImageLayout initialLayout, VkImageUsageFlags usageFlags, VkImage& vkImage, VkMemoryPropertyFlags memPropertyFlags, VkDeviceMemory& rDevMem,  uint32_t uiWidth, uint32_t uiHeight);

        int  FindDeviceMemoryIndex(VkMemoryPropertyFlags memPropertyFlags, uint32_t memBits );

    
public:
   CVTexture(/*C args */);
    ~CVTexture();

    int CreateFromBMP(VkPhysicalDevice phyDev, VkDevice vkDev, VkQueue vkQue, VkCommandPool vkCmdPool, char* pszPath);
};




#endif

