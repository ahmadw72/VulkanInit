#include "VTexture.h"
#include "Bitmap.h"
#include <assert.h>
#include "VkSingleTimeCommandBuffer.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

CVTexture::CVTexture(/*C args */)
{
     m_VkStaggingImage = VK_NULL_HANDLE;
     m_VkImageView = VK_NULL_HANDLE;
     m_VkSampler = VK_NULL_HANDLE;
     m_VkDevMemStagging = VK_NULL_HANDLE;
     m_VkDevMemTexture = VK_NULL_HANDLE;
     m_VkPhysicalDevice = VK_NULL_HANDLE;
     m_VkDevice = VK_NULL_HANDLE;
     m_VkCommandPool = VK_NULL_HANDLE;

     m_iWidth = 0;
     m_iHeight = 0;


}
CVTexture::~CVTexture()
{
}

VkImageView CVTexture::GetImageView()
{
     assert(m_VkImageView != VK_NULL_HANDLE );
     return m_VkImageView;
}

VkSampler CVTexture::GetSampler()
{
     assert(m_VkSampler != VK_NULL_HANDLE);
     return m_VkSampler;
}

bool CVTexture::CreateSampler(VkDevice& rVkDevice)
{
     VkPhysicalDeviceProperties  phyDevProps = {};

      vkGetPhysicalDeviceProperties(m_VkPhysicalDevice, &phyDevProps);

     VkSamplerCreateInfo  samplerCreateInfo  {};
     samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;

     samplerCreateInfo.flags = 0;
     samplerCreateInfo.pNext = nullptr;
     samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
     samplerCreateInfo.minFilter = VK_FILTER_LINEAR;
     samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
     samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
     samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
     samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

    /* samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
     samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
     samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
*/

     samplerCreateInfo.minLod = 0.0f;
     samplerCreateInfo.maxLod = 10000.0f;
     samplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
     samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;
     samplerCreateInfo.anisotropyEnable = VK_TRUE;
     samplerCreateInfo.compareEnable = VK_FALSE;
     samplerCreateInfo.compareOp = VK_COMPARE_OP_ALWAYS;
     samplerCreateInfo.mipLodBias = 0;
     samplerCreateInfo.maxAnisotropy = phyDevProps.limits.maxSamplerAnisotropy;//1.0f;


     if (vkCreateSampler(rVkDevice, &samplerCreateInfo, NULL, &m_VkSampler) != VK_SUCCESS )
     {
          printf("vkCreateSampler(rVkDevice, &samplerCreateInfo, NULL, &m_VkSampler) failed \n");
          assert(false);
          return false;
     }

     return true;

}
/**
 * @brief step # 1 copy texture data to host visible memory
 *        step # 2 prep device visible memory 
 *        step # 2. copy texture data from host visible memory to device memory. 
 * 
 * @param rVkPhysicalDevice 
 * @param rVkDevice 
 * @param iMemIndex 
 * @return true 
 * @return false 
 */
bool CVTexture::CreateImages()
{
     VkDeviceMemory devMem = VK_NULL_HANDLE;
     CreateImage(VK_IMAGE_TILING_LINEAR, VK_IMAGE_LAYOUT_UNDEFINED,  VK_IMAGE_USAGE_TRANSFER_SRC_BIT, m_VkStaggingImage,VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_VkDevMemStagging,  m_iWidth, m_iHeight );
     // ..//

     VkImageSubresource  imageSubResource = {};
     imageSubResource.arrayLayer = 0;
     imageSubResource.mipLevel = 0;
     imageSubResource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

     VkSubresourceLayout subResourceLayout = {};
     vkGetImageSubresourceLayout(m_VkDevice, m_VkStaggingImage, &imageSubResource, &subResourceLayout );

     void* pMemGPU;
     if ( VK_SUCCESS != vkMapMemory(m_VkDevice, m_VkDevMemStagging, 0, VK_WHOLE_SIZE, 0, &pMemGPU) )
     {
          printf("vkMapMemory(rVkDevice, devMem, 0, VK_WHOLE_SIZE, 0, &pMemGPU)...failed\n");
          assert(false);

     }

     if (subResourceLayout.rowPitch == 3* m_iWidth)
     {
          memcpy(pMemGPU, &m_vData[0], m_vData.size()*sizeof(char) );
     }
   /*  else
     {
          for( UINT uiOffset =0; uiOffset < m_iHeight; uiOffset++)
          {
               unsigned char* pucMemGPU = (unsigned char*)pMemGPU;
               memcpy(&pucMemGPU[uiOffset*subResourceLayout.rowPitch], &m_vData[4*uiOffset*m_iWidth], 4*m_iWidth );
          }
     }
*/
     vkUnmapMemory(m_VkDevice, m_VkDevMemStagging);

     VkDeviceMemory devMem1 = VK_NULL_HANDLE;

     CreateImage(VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, m_VkTextureImage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_VkDevMemTexture, m_iWidth, m_iHeight);
     //step # 2 //
  
     return true;     

}
/**
 * @brief copies the staging buffer to texture image
 * 
 * @return true 
 * @return false 
 */

bool CVTexture::CreateTextureImageView()
{


     VkImageSubresourceRange imageSubRsrcRange;
     imageSubRsrcRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
     imageSubRsrcRange.baseMipLevel = 0;
     imageSubRsrcRange.levelCount = 1;
     imageSubRsrcRange.baseArrayLayer = 0;
     imageSubRsrcRange.layerCount = 1;

    // imageSubRsrcRange.



     VkImageViewCreateInfo imageViewCreateInfo;
     
   //  imageViewCreateInfo.
     imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;

     imageViewCreateInfo.pNext = nullptr;
     imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    // imageViewCreateInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
    imageViewCreateInfo.format = VK_FORMAT_R8G8B8_UNORM;
    // imageViewCreateInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
     
     imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_A;
     imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_B;
     imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_G;
     imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_R;
     imageViewCreateInfo.subresourceRange = imageSubRsrcRange;
     imageViewCreateInfo.image = m_VkTextureImage;


     if (vkCreateImageView(m_VkDevice, &imageViewCreateInfo, nullptr, &m_VkImageView ) != VK_SUCCESS )
     {
          printf("vkCreateImageView(rVkDevice, &imageViewCreateInfo, nullptr, &m_VkImageView )...failed\n");
          assert(false);
     }

     vkDestroyImage(m_VkDevice, m_VkStaggingImage, nullptr);

     return true;


}
 void CVTexture::TxDataToDevice()
 {
     CVkSingleTimeCommandBuffer singleTimeCmdBuffer(m_VkPhysicalDevice,m_VkDevice,m_VkQueue, m_VkCommandPool);

    // LayoutTransition(singleTimeCmdBuffer.GetCommandBuffer(), m_VkStaggingImage, VK_PIPELINE_STAGE_HOST_BIT, VK_PIPELINE_STAGE_HOST_BIT, VK_ACCESS_HOST_WRITE_BIT , 0/*VK_ACCESS_TRANSFER_READ_BIT*/ ,VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,VK_IMAGE_LAYOUT_PREINITIALIZED );
     //LayoutTransition(singleTimeCmdBuffer.GetCommandBuffer(), m_VkTextureImage, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,0 , VK_ACCESS_TRANSFER_WRITE_BIT , VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_PREINITIALIZED);
    // LayoutTransition(singleTimeCmdBuffer.GetCommandBuffer(), m_VkTextureImage, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,0 , VK_ACCESS_TRANSFER_WRITE_BIT , VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);

     // copy from stagging to textures //
     VkImageSubresourceLayers imgSubRscrLayers = {};
     imgSubRscrLayers.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
     imgSubRscrLayers.baseArrayLayer = 0;
     imgSubRscrLayers.layerCount = 1;
     imgSubRscrLayers.mipLevel = 0;

     VkOffset3D offSet3D = {};

     VkExtent3D extnt3D= {};
     extnt3D.depth = 1;
     extnt3D.height = m_iHeight;
     extnt3D.width = m_iWidth;

     VkImageCopy imgCopy = {};
     imgCopy.dstOffset = offSet3D;
     imgCopy.srcOffset = offSet3D;
     imgCopy.extent = extnt3D;
     imgCopy.dstSubresource = imgSubRscrLayers;
     imgCopy.srcSubresource = imgSubRscrLayers;

     vkCmdCopyImage(singleTimeCmdBuffer.GetCommandBuffer(), m_VkStaggingImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, m_VkTextureImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &imgCopy);

     LayoutTransition(singleTimeCmdBuffer.GetCommandBuffer(), m_VkTextureImage, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,0 , VK_ACCESS_SHADER_READ_BIT /*| VK_ACCESS_INPUT_ATTACHMENT_READ_BIT*/, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);





 }
/**
 * @brief copy the texture buffers to create texture image view //
 * 
 * @param pszPath 
 * @return int 
 */

int CVTexture::CreateFromBMP(VkPhysicalDevice phyDev, VkDevice vkDev, VkQueue vkQue, VkCommandPool vkCmdPool, char* pszPath)
{
    m_VkPhysicalDevice = phyDev;
    m_VkDevice = vkDev;
    m_VkQueue = vkQue;
    m_VkCommandPool = vkCmdPool;
    m_VkSampler = VK_NULL_HANDLE;
    
     CBitmap bitmap;
     assert( bitmap.Load(pszPath) );
     bitmap.GetDimension(m_iHeight, m_iWidth);
     std::vector<char> data;
     bitmap.GetData(data);
     m_vData = data; 
    
  /*   int iChannels;

     stbi_uc* pucData = stbi_load(pszPath, &m_iWidth, &m_iHeight, &iChannels ,STBI_rgb);

     m_vData.resize(m_iWidth*m_iHeight*3);

     memcpy((void*)m_vData.data(), (void*)pucData,  (size_t)m_iHeight*m_iWidth*3 );*/
     // 1. create sampler // 
     


     //2.  Create Images, creates stagging and actual texture image, copies m_vData into //
     CreateImages();
     //3. tansistion + moves the texture data from stagging buffer to device buffer //
     TxDataToDevice();
     //4. Create the image view //
     CreateTextureImageView();


    CreateSampler(m_VkDevice);


     return 0;
}

// helper //
void CVTexture::LayoutTransition(VkCommandBuffer vkCmdBuff, VkImage vkImage, VkPipelineStageFlags srcPipeLineStageFlag, VkPipelineStageFlags dstPipeLineStageFlag, VkAccessFlags srcAccessFlags, VkAccessFlags dstAccessFlags,  VkImageLayout newLayout, VkImageLayout oldLayout)
{
     VkImageSubresourceRange imageSubRsrcTexture = {};

     imageSubRsrcTexture.baseMipLevel =0;
     imageSubRsrcTexture.layerCount =1;
     imageSubRsrcTexture.baseArrayLayer = 0;
     imageSubRsrcTexture.levelCount=1;
     imageSubRsrcTexture.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
     
     VkImageMemoryBarrier imageMemBarrierTexture = {};
     imageMemBarrierTexture.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
     imageMemBarrierTexture.pNext = nullptr;
     imageMemBarrierTexture.newLayout = newLayout;
     imageMemBarrierTexture.oldLayout = oldLayout;
     imageMemBarrierTexture.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
     imageMemBarrierTexture.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
     imageMemBarrierTexture.image = vkImage;
     imageMemBarrierTexture.srcAccessMask = srcAccessFlags;
     imageMemBarrierTexture.dstAccessMask = dstAccessFlags;
     imageMemBarrierTexture.subresourceRange = imageSubRsrcTexture;

    /* if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
     {


     } else
      if( oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
      {

      } else
      {
          printf(" faile in LayoutTransition .. ");
          assert(false);
      }*/

     vkCmdPipelineBarrier(vkCmdBuff,srcPipeLineStageFlag, dstPipeLineStageFlag, 0,0, nullptr, 0, nullptr, 1, &imageMemBarrierTexture);
 
}
/**
 * @brief  helper to create image, allocates memory for the image
 * 
 * @param vkDevice 
 * @param usageFlags 
 * @param pVkImage 
 * @return true 
 * @return false 
 */
 bool  CVTexture::CreateImage(VkImageTiling tiling,  VkImageLayout initialLayout,  VkImageUsageFlags usageFlags, VkImage& vkImage,VkMemoryPropertyFlags memPropertyFlags, VkDeviceMemory& rDevMem,  uint32_t uiWidth, uint32_t uiHeight)
 {
     VkImageCreateInfo imageCreateInfo = {};

     imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
     imageCreateInfo.pNext = nullptr;
    // imageCreateInfo.tiling = VK_IMAGE_TILING_LINEAR;
    //  imageCreateInfo.tiling = VK_IMAGE_TILING_LINEAR;
     imageCreateInfo.tiling = tiling;
     imageCreateInfo.flags = 0;
   //  imageCreateInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
     imageCreateInfo.format = VK_FORMAT_R8G8B8_UNORM;
  //   imageCreateInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
   //  imageCreateInfo.format = VK_FORMAT_R32G32B32_SFLOAT;
     
     imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
     imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
     imageCreateInfo.usage = usageFlags;
     imageCreateInfo.mipLevels = 1;
     imageCreateInfo.arrayLayers = 1;
     imageCreateInfo.extent.width = uiWidth;
     imageCreateInfo.extent.height = uiHeight;
     imageCreateInfo.extent.depth = 1;
     imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
     //imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;//VK_IMAGE_LAYOUT_PREINITIALIZED;
     imageCreateInfo.initialLayout = initialLayout;// VK_IMAGE_LAYOUT_PREINITIALIZED;

     if ( vkCreateImage(m_VkDevice, &imageCreateInfo, nullptr, &vkImage) != VK_SUCCESS )
     {
          printf("vkCreateImage(rVkDevice, &imageCreateInfo, NULL, &m_VkStaggingImage) failed\n");
          assert(false);
          return false;
     }

     VkMemoryRequirements  memReq = {};

     vkGetImageMemoryRequirements(m_VkDevice, vkImage, &memReq) ; 

     VkMemoryAllocateInfo memAllocInfo = {};
     memAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
     memAllocInfo.allocationSize = memReq.size;
     memAllocInfo.memoryTypeIndex = FindDeviceMemoryIndex(memPropertyFlags, memReq.memoryTypeBits);


     //VkDeviceMemory  devMem = {};

     if ( VK_SUCCESS != vkAllocateMemory(m_VkDevice, &memAllocInfo, NULL, &rDevMem) )
     {
          printf("vkAllocateMemory(rVkDevice, &memAllocInfo, NULL, &devMem)...failed\n");
          assert(false);

     }

     if ( vkBindImageMemory(m_VkDevice, vkImage, rDevMem, 0) != VK_SUCCESS )
     {
          printf("vkBindImageMemory(rVkDevice, vkImage, devMem, 0)...failed\n");
          assert(false);

     }

     return true;
 }

 int  CVTexture::FindDeviceMemoryIndex(VkMemoryPropertyFlags memPropertyFlags, uint32_t memBits )
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