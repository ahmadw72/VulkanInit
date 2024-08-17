#include "VkBuffer_Host.h"

#include "glm\glm.hpp"

struct stMVP{
    alignas(16) glm::mat4 matModel;
    alignas(16) glm::mat4 matView;
    alignas(16) glm::mat4 matProjection;
   
};


class CUniformBuffer_MVP : public CVkBuffer_Host
{
    private:
    float m_pfMatModel[16], m_pfMatView[16], m_pfMatProjection[16];
    stMVP m_stMVP;
    public :
        CUniformBuffer_MVP();
        ~CUniformBuffer_MVP();
        CUniformBuffer_MVP(float pfMatModel[],float pfMatViewl[],float pfMatProjection[] );
        bool Create(VkPhysicalDevice vkPhyDev, VkDevice vkDev, glm::mat4 m, glm::mat4 v, glm::mat4 p);



};