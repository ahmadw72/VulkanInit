//
#include "Bitmap.h"
/**
	Description: A helper to read the bitmap files
	Author:	W.Ahmad
	Lisence: MIT
	Note:
	
*/

//basic constructor
CBitmap::CBitmap()
{
    Reset();
}

//constructor loads the CBitmap when it is created
CBitmap::CBitmap(const char *pszFile)
{
    Reset();
    Load(pszFile);
}

//destructor
CBitmap::~CBitmap()
{
   
	if(m_vcImageData.empty() )
		m_vcImageData.clear();
	
}


bool CBitmap::Load ( const char *pszFile) 
{
	FILE* pBMPFile;
 
    int iNumColours;            //total available colours

	m_bLoaded = false;
    pBMPFile=fopen(pszFile,"rb");

   
    if( pBMPFile==NULL ) 
	{
        m_strError="File not found";
        fclose(pBMPFile);
        return false;
    }

    //read in the entire BITMAPFILEHEADER
    fread(&m_BitmapFileHeader,sizeof(BITMAPFILEHEADER),1,pBMPFile);
	
    if(m_BitmapFileHeader.bfType!=BITMAP_MAGIC_NUMBER) 
	{
        m_strError="File is not in DIB format";
        fclose(pBMPFile);
        return false;
    }

    //read in the entire BITMAPINFOHEADER
    fread(&m_BitmapInfoHeader,sizeof(BITMAPINFOHEADER),1,pBMPFile);
	

    //save the width, height and bits per pixel for external use
    m_iWidth=m_BitmapInfoHeader.biWidth;
    m_iHeight=m_BitmapInfoHeader.biHeight;
    m_sBitPerPixel=m_BitmapInfoHeader.biBitCount;

	//calculate the witdh of the final image in bytes
    m_iWidthInBytes=m_iPadWidthInBytes=(int)((float)m_iWidth*(float)m_sBitPerPixel/8.0);

    //adjust the width for padding as necessary
    while( m_iPadWidthInBytes % 4 != 0 ) 
	{
        m_iPadWidthInBytes++;
    }
	
    //calculate the size of the image data with padding
   // m_uiDataSize=( m_iWidth*m_iHeight*(unsigned int)(m_BitmapInfoHeader.biBitCount/8.0));
	//m_uiDataSize = m_BitmapInfoHeader.biSizeImage;
	m_uiDataSize = m_iPadWidthInBytes * m_iHeight;

    //calculate the number of available colours
    iNumColours=1<<m_BitmapInfoHeader.biBitCount;

    //if the bitmap is not 8 bits per pixel or more
    //return in error
    if(m_sBitPerPixel < 8) 
	{
        m_strError="File is not 8 or 24 bits per pixel";
        fclose(pBMPFile);
        return false;
    }

    //load the palette for 8 bits per pixel
    if( m_sBitPerPixel == 8 ) 
	{
    	m_vRGBQuads.assign(iNumColours, RGBQUAD());
    	fread(&m_vRGBQuads[0],sizeof(RGBQUAD),iNumColours,pBMPFile);
    }

 
    //read in the entire image
	std::vector<char> vImageData;
	vImageData.assign(m_uiDataSize, 0);

    fread(&vImageData[0],sizeof(char),m_uiDataSize,pBMPFile);

    //close the file now that we have all the info
    fclose(pBMPFile);

    

    //change format from GBR to RGB
    if(m_sBitPerPixel == 8) 
	{
    	m_bLoaded=Convert8(vImageData);
   	}
    else 
		if(m_sBitPerPixel == 24) 
		{
    		m_bLoaded = Convert24(vImageData);
		}

      //bitmap is now loaded
    m_strError="Bitmap loaded";

    //return success
    return m_bLoaded;
}

//function to set the inital values
void CBitmap::Reset(void) 
{
    m_bLoaded=false;
   
	m_vcImageData.clear();
	m_vRGBQuads.clear();
    m_strError="";
}
/**
	helper to read if 24 bits / pixel
*/
bool CBitmap::Convert24(std::vector<char>& rvcImageData) 
{
	//int offset,iDiff;

	int iOffset, iDiff;

	iDiff =  m_iWidth*m_iHeight*RGB_BYTE_SIZE;
    //allocate the buffer for the final image data
  //  data = new char[iDiff];
//	m_vcImageData.assign(iDiff, 0);

 
	m_vcImageData.clear();
    if(m_iHeight > 0) 
	{
        iOffset = m_iPadWidthInBytes-m_iWidthInBytes;
        //count backwards so you start at the front of the image
      
		for(unsigned int uiCount = 0; uiCount < m_uiDataSize; uiCount += 3 ) 
		{
            //jump over the padding at the start of a new line
			if(  uiCount  % m_iWidthInBytes  == 0) 
			{
                uiCount += iOffset;//+1;
            }
			if ( uiCount >= m_iWidthInBytes )
			{
				int iStop =0;
			}
			/**/
            
			m_vcImageData.push_back( rvcImageData[uiCount + 2] );
			m_vcImageData.push_back( rvcImageData[uiCount + 1]);
			m_vcImageData.push_back( rvcImageData[uiCount] );

			//jump over the padding at the start of a new line
           /* if( ( uiCount+1+iOffset ) % m_iPadWidthInBytes==0) 
			{
                uiCount += iOffset;
            }
			*/
        }
    }

    //image parser for a forward image
    else 
	{
        iOffset = m_iPadWidthInBytes-m_iWidthInBytes;
       
		int iEnd = m_uiDataSize-3;
        //count backwards so you start at the front of the image
		//here you can start from the back of the file or the front,
		//after the header  The only problem is that some programs
		//will pad not only the data, but also the file size to
		//be divisible by 4 bytes.
      
		 for( unsigned int uiCount = 0; uiCount < m_uiDataSize; uiCount += 3) 
		 {
            //jump over the padding at the start of a new line
            if( (uiCount+1) % m_iPadWidthInBytes == 0 ) 
			{
                uiCount += iOffset;
            }
           	m_vcImageData[iEnd + 2] = rvcImageData[uiCount];
			m_vcImageData[iEnd + 1] = rvcImageData[uiCount + 1];
			m_vcImageData[iEnd] = rvcImageData[uiCount + 2];
			iEnd -= 3;
        }
    }

    return true;
}
/**
	helper to read if 8 bits / pixel
*/
bool CBitmap::Convert8( std::vector<char>& rvcImageData ) 
{
	int iOffset,iDiff;

	iDiff = m_iWidth * m_iHeight * RGB_BYTE_SIZE;
   
	m_vcImageData.assign( iDiff, 0);

    

    if(m_iHeight > 0) 
	{
        iOffset = m_iPadWidthInBytes - m_iWidthInBytes;
        int j=0;
		int iEnd = 0;
        //count backwards so you start at the front of the image
        for(unsigned int uiCount = 0; uiCount < m_uiDataSize*RGB_BYTE_SIZE; uiCount += 3) 
		{
            //jump over the padding at the start of a new line
        
			if(( uiCount + 1 )% m_iPadWidthInBytes==0) 
			{
                uiCount += iOffset;
            }
            
			m_vcImageData[uiCount] = m_vRGBQuads[ rvcImageData[ iEnd ] ].rgbRed;
			m_vcImageData[uiCount + 1] = m_vRGBQuads[ rvcImageData[ iEnd ] ].rgbGreen;
			m_vcImageData[uiCount + 2] = m_vRGBQuads[ rvcImageData[ iEnd ] ].rgbBlue;
			iEnd++;
        }
    }

    //image parser for a forward image
    else 
	{
        iOffset = m_iPadWidthInBytes - m_iWidthInBytes;
        int iEnd = m_uiDataSize - 1;
        //count backwards so you start at the front of the image
  
		for(unsigned int uiCount = 0; uiCount < m_uiDataSize*RGB_BYTE_SIZE; uiCount += 3) 
		{
            //jump over the padding at the start of a new line
            if( ( uiCount+1 ) % m_iPadWidthInBytes==0) 
			{
                uiCount += iOffset;
            }
         
			m_vcImageData[uiCount] = m_vRGBQuads[ rvcImageData[ iEnd ] ].rgbRed;
			m_vcImageData[uiCount + 1] = m_vRGBQuads[ rvcImageData[ iEnd ] ].rgbRed;
			m_vcImageData[uiCount + 2] = m_vRGBQuads[ rvcImageData[ iEnd ] ].rgbRed;
			iEnd--;
        }
    }

    return true;
}
/**
	returns the height and width of the image
*/
void CBitmap::GetDimension(int& riHeight, int& riWidth)
{
	riHeight = m_iHeight;
	riWidth =  m_iWidth;

}
/**
	returns the data
*/
void CBitmap::GetData(std::vector<char>&	rvcImageData)
{
	rvcImageData = m_vcImageData;
}
/**
	returns  pixel at specified row and colument
*/

unsigned int CBitmap::GetPixel(int iRow, int iColumn)
{
	

	if (m_sBitPerPixel == 8 )
	{
		int iIndex = iRow * m_iWidth + iColumn;
		return m_vcImageData.at(iIndex) / 255.0f;
		
	}

	if ( m_sBitPerPixel == 24 )
	{
		int iIndex = 	iRow * (m_iWidth + 3) + iColumn*3;

		unsigned char cR = m_vcImageData.at(iIndex);
		unsigned char cG = m_vcImageData.at(iIndex+1);
		unsigned char cB = m_vcImageData.at(iIndex+2);    

		unsigned int uiPixel = 0xFF << 24 |  cR << 16 | cG << 8 | cB ;
		return uiPixel;
	}



}

float CBitmap::GetPixelLuminance(int iRow, int iColumn)
{
	//0.3f*getRed() + 0.59f*getGreen() + 0.11f*getBlue();

	if ( m_sBitPerPixel == 24 )
	{
		int iIndex = 	iRow * (m_iWidth * 3) + iColumn*3;
		unsigned char cR = m_vcImageData.at(iIndex);
		unsigned char cG = m_vcImageData.at(iIndex+1);
		unsigned char cB = m_vcImageData.at(iIndex+2);  

		return  0.3f * cR / 255.0f + 0.59 * cG / 255.0f +  0.11f * cB / 255.0f;
	}


}