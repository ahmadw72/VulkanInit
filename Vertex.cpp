#include "Vertex.h"
#include<stdio.h>
#include <assert.h>


CVertex::CVertex()
{
    assert(false);
    
}
CVertex::~CVertex()
{

}
float CVertex::GetX()
{
   return m_fX;

}

float CVertex::GetY()
{
   return m_fY;
   
}

float CVertex::GetZ()
{
   return m_fZ;
}

float CVertex::GetU()
{
   return m_fU;
   
}

float CVertex::GetV()
{
   return m_fV;
}

float CVertex::GetA()
{
   return m_fA;
   
}
float CVertex::GetR()
{
   return m_fR;   
}
float CVertex::GetG()
{
   return m_fG;   
}
float CVertex::GetB()
{
   return m_fB;
   
}

std::vector<float> CVertex::GetData()
{
   std::vector<float> vData;
   vData.push_back(m_fX);
   vData.push_back(m_fY);
   vData.push_back(m_fZ);

   vData.push_back(m_fU);
   vData.push_back(m_fV);
   
   
   vData.push_back(m_fR);
   vData.push_back(m_fG);
   vData.push_back(m_fB);
   vData.push_back(m_fA);

   return vData;



}
/**
 * @brief Construct a new CVertex::CVertex object
 * 
 * @param fX 
 * @param fY 
 * @param fZ 
 * @param fU 
 * @param fV 
 * @param fR 
 * @param fG 
 * @param fB 
 * @param fA 
 */
CVertex::CVertex(float fX, float fY, float fZ, float fU, float fV, float fR, float fG, float fB, float fA)
:m_fX(fX),m_fY(fY),m_fZ(fZ),m_fU(fU), m_fV(fV),m_fR(fR),m_fG(fG), m_fB(fB), m_fA(fA)
{



}
 VkVertexInputBindingDescription CVertex::GetVertexInputBindingDescription_PCT()
 {

    if ( m_fU > 1.0f || m_fV > 1.0f || m_fA > 1.0f || m_fB > 1.0 || m_fG > 1.0f || m_fR > 1.0f ||
         m_fU < 0.0f || m_fV < 0.0f || m_fA < 0.0f || m_fB < 0.0 || m_fG < 0.0f || m_fR < 0.0f )
         {
            printf("GetVertexInputBindingDescription_PCT.. vertex type is different\n ");
            assert(false);
         }
    VkVertexInputBindingDescription inputBindingDesc{};
    inputBindingDesc.binding =0;
    inputBindingDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    inputBindingDesc.stride = sizeof(float)*2 + sizeof(float)*3 + sizeof(float)*2;
    return inputBindingDesc;
 }
/**
 * @brief returns the vkvertexInputAttributeDescription of Point-Color-Texture vertex
 * 
 * @return std::vector<VkVertexInputAttributeDescription> 
 */

std::array<VkVertexInputAttributeDescription,3> CVertex::GetVertexInputAttributeDescription_PCT()
{
    if ( m_fU > 1.0f || m_fV > 1.0f || m_fA > 1.0f || m_fB > 1.0 || m_fG > 1.0f || m_fR > 1.0f ||
         m_fU < 0.0f || m_fV < 0.0f || m_fA < 0.0f || m_fB < 0.0 || m_fG < 0.0f || m_fR < 0.0f )
         {
            printf("std::array<VkVertexInputAttributeDescription,3> ).. . vertex type is different\n ");
            assert(false);
         }
    std::array<VkVertexInputAttributeDescription,3>  attribDescrip;

    attribDescrip[0].binding =0; 
    attribDescrip[0].location = 0;
    attribDescrip[0].offset = 0;
    attribDescrip[0].format = VK_FORMAT_R32G32_SFLOAT; // position 2-d

    attribDescrip[1].binding =0;
    attribDescrip[1].location = 1;
    attribDescrip[1].offset = attribDescrip[0].offset + sizeof(float) * 2 ;
    attribDescrip[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;////VK_FORMAT_R32G32B32_SFLOAT;  // color 3  

    attribDescrip[2].binding =0;
    attribDescrip[2].location = 2;
    attribDescrip[2].offset = attribDescrip[1].offset + sizeof(float)*4;
    attribDescrip[2].format = VK_FORMAT_R32G32_SFLOAT; // texture uv


    return attribDescrip;

}


VkVertexInputBindingDescription CVertex::GetVertexInputBindingDescription()
 {

    if ( m_fU > 1.0f || m_fV > 1.0f || m_fA > 1.0f || m_fB > 1.0 || m_fG > 1.0f || m_fR > 1.0f ||
         m_fU < 0.0f || m_fV < 0.0f || m_fA < 0.0f || m_fB < 0.0 || m_fG < 0.0f || m_fR < 0.0f )
         {
            printf("GetVertexInputBindingDescription_PCT.. vertex type is different\n ");
            assert(false);
         }
    VkVertexInputBindingDescription inputBindingDesc{};
    inputBindingDesc.binding =0;
    inputBindingDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    inputBindingDesc.stride = sizeof(float)*3 + sizeof(float)*4 + sizeof(float)*2;
    return inputBindingDesc;
 }
/**
 * @brief returns the vkvertexInputAttributeDescription of Point-Color-Texture vertex
 * 
 * @return std::vector<VkVertexInputAttributeDescription> 
 */

std::vector<VkVertexInputAttributeDescription> CVertex::GetVertexInputAttributeDescription()
{
    if ( m_fU > 1.0f || m_fV > 1.0f || m_fA > 1.0f || m_fB > 1.0 || m_fG > 1.0f || m_fR > 1.0f ||
         m_fU < 0.0f || m_fV < 0.0f || m_fA < 0.0f || m_fB < 0.0 || m_fG < 0.0f || m_fR < 0.0f )
         {
            printf("std::array<VkVertexInputAttributeDescription,3> ).. . vertex type is different\n ");
            assert(false);
         }
    std::vector<VkVertexInputAttributeDescription>  attribDescrip;
    attribDescrip.resize(1);
   //if `(m)
    attribDescrip[0].binding =0; 
    attribDescrip[0].location = 0;
    attribDescrip[0].offset = 0;
    attribDescrip[0].format = VK_FORMAT_R32G32B32_SFLOAT; // position 2-d

   if (m_fR >= 0.0 && m_fR <= 1.0f && m_fG >= 0.0f && m_fG <= 1.0f && m_fB >= 0.0f && m_fB <= 1.0f && m_fA >= 0.0f && m_fA <= 1.0f)
   {
      attribDescrip.resize(2);
      attribDescrip[1].binding =0;
      attribDescrip[1].location = 1;
      attribDescrip[1].offset = attribDescrip[0].offset + sizeof(float) * 3 ;
      attribDescrip[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;////VK_FORMAT_R32G32B32_SFLOAT;  // color 3  
   }
   if ( m_fU >= 0.0 && m_fU <= 1.0f && m_fV >= 0.0f && m_fV <= 1.0f ) // texture coordinates //
   {
      attribDescrip.resize(3);
      attribDescrip[2].binding =0;
      attribDescrip[2].location = 2;
      attribDescrip[2].offset = attribDescrip[1].offset + sizeof(float)*4;  // rgba float //
      attribDescrip[2].format = VK_FORMAT_R32G32_SFLOAT; // texture uv
   }

    return attribDescrip;

}