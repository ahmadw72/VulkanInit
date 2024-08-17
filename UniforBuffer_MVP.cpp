#include "UniforBuffer_MVP.h"




CUniformBuffer_MVP::CUniformBuffer_MVP(){}
CUniformBuffer_MVP::~CUniformBuffer_MVP(){}



CUniformBuffer_MVP::CUniformBuffer_MVP(float pfMatModel[],float pfMatView[],float pfMatProjection[] )
{
    memcpy(m_pfMatView, pfMatView, 16*sizeof(float));
    memcpy(m_pfMatModel, pfMatModel, 16*sizeof(float));
    memcpy(m_pfMatView, pfMatProjection, 16*sizeof(float));

    
}


 bool CUniformBuffer_MVP::Create(VkPhysicalDevice vkPhyDev, VkDevice vkDev, glm::mat4 m, glm::mat4 v, glm::mat4 p)
 {
    m_stMVP.matModel = m;
    m_stMVP.matView = v;
    m_stMVP.matProjection = p;

    m_VkPhysicalDevice = vkPhyDev;
    m_VkDevice = vkDev;

    VkDeviceSize devSize = sizeof(stMVP);

    if (CVkBuffer_Host::Create(m_VkPhysicalDevice, m_VkDevice, devSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) != true )
    {
        printf("Create(m_VkPhysicalDevice, m_VkDevice, devSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)...\n");
        assert(false);
        return false;
    }

    if ( CopyMem((char*)&m_stMVP, sizeof(stMVP)) != true)
    {
        printf("CopyMem((char*)&m_stMVP, sizeof(stMVP))..\n");
        assert(false);
        return false;
    }

    return true;
 }