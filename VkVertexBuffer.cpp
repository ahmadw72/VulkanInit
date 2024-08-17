#include "VkVertexBuffer.h"
#include "VkBuffer_Host.h"
#include <assert.h>
#include "glm/glm.hpp"

struct vertex
{
    glm::vec3 pos;
    glm::vec4 clr;
    glm::vec2 uv;


};

std::vector<vertex> vertices = {{{0.0, -0.5, 0.0}, {0.0,0.0,0.0,1.0f}, {0.0f, 0.0}}, 
                                {{0.5, 0.5, 0.0}, {0.0,0.0,0.0,1.0f}, {0.0f, 0.0}}, 
                                {{-0.5, -0.5, 0.0}, {0.0,0.0,0.0,1.0f}, {0.0f, 0.0}} };

CVkVertexBuffer::CVkVertexBuffer()
{
    
}

CVkVertexBuffer::~CVkVertexBuffer()
{

}


int CVkVertexBuffer::AddVertex(CVertex rVertex)
{
    m_Vertices.push_back(rVertex);

    m_vfData.push_back(rVertex.GetX());
    m_vfData.push_back(rVertex.GetY());
    m_vfData.push_back(rVertex.GetZ());

    m_vfData.push_back(rVertex.GetU());
    m_vfData.push_back(rVertex.GetV());
    
    m_vfData.push_back(rVertex.GetR());
    m_vfData.push_back(rVertex.GetG());
    m_vfData.push_back(rVertex.GetB());
    m_vfData.push_back(rVertex.GetA());



    return m_Vertices.size();
}
/**
 * @brief moves data to device (gpu). This method must be called before attaching the vertex buffer to pipeline
 * 
 * @return true 
 * @return false 
 */

bool CVkVertexBuffer::MoveDataToDevice()
{

    assert ( m_VkPhysicalDevice != VK_NULL_HANDLE && m_VkDevice != VK_NULL_HANDLE );  // call SetPhysicalDevice and SetDevice //
    size_t uiSize = sizeof(m_Vertices.at(0))*m_Vertices.size( );
   // MoveDataToDevice
   // return TxDataToDevice(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, (char*)m_Vertices.data(),  uiSize); 
    char* pData = (char*) vertices.data();
    uiSize = vertices.size() * sizeof(vertices[0]);
   //  return TxDataToDevice(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, pData,  uiSize );
   return TxDataToDevice(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, (char*)m_vfData.data(),  m_vfData.size()*sizeof(float) ); 
}

std::vector<VkVertexInputAttributeDescription> CVkVertexBuffer::GetVertexInputAttributeDescription()
{

    int o1 = offsetof(vertex, pos);
    int o2 = offsetof(vertex, clr);
    int o3 = offsetof(vertex, uv);

    return m_Vertices[0].GetVertexInputAttributeDescription();
 
 
}  
VkVertexInputBindingDescription CVkVertexBuffer::GetVertexInputBindingDescription()
{

    return m_Vertices[0].GetVertexInputBindingDescription();


}