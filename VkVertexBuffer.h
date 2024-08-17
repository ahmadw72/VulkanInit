#ifndef _CVKVERTEX_BUFFER_H_
#define _CVKVERTEX_BUFFER_H_



#include "VkBuffer_Device.h"
#include "Vertex.h"
#include <vector>


class CVkVertexBuffer : public CVkBuffer_Device
{
    public:
        CVkVertexBuffer();
        ~CVkVertexBuffer();
        int AddVertex(CVertex rVertex);
        virtual bool MoveDataToDevice();

        std::vector<VkVertexInputAttributeDescription> GetVertexInputAttributeDescription();  
        VkVertexInputBindingDescription GetVertexInputBindingDescription();
        
    private:
        std::vector<CVertex> m_Vertices;
        std::vector<float> m_vfData;

};


#endif