#ifndef _VERTEX_H_
#define _VERTEX_H_

#include <vulkan.h>

#include <array>
#include <vector>
class CVertex
{
    private:
    float m_fX, m_fY, m_fZ; // x, y and z position //
    float m_fU, m_fV; // u, v texture coords //
    float m_fR, m_fG, m_fB, m_fA; // r,g,b,a  color coords //
    public:
        CVertex();
        ~CVertex();
         CVertex(float fX, float fY, float fZ, float fU, float fV, float fR, float fG, float fB, float fA);
         std::vector<float> GetData();
        
         float GetX();
         float GetY();
         float GetZ();



         float GetU();
         float GetV();


         float  GetR();
         float  GetG();
         float  GetB();
         float  GetA();
    
    std::array<VkVertexInputAttributeDescription,3> GetVertexInputAttributeDescription_PCT();  
    VkVertexInputBindingDescription GetVertexInputBindingDescription_PCT();


    std::vector<VkVertexInputAttributeDescription> GetVertexInputAttributeDescription();  
    VkVertexInputBindingDescription GetVertexInputBindingDescription();


    size_t  GetSize();


};

#endif