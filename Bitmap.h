#pragma once

#include <windows.h>
#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
//#include "types.h"
using namespace std;

const short BITMAP_MAGIC_NUMBER=19778;
const int RGB_BYTE_SIZE=3;

class   CBitmap 
{
public:
  
    CBitmap(void);
    CBitmap(const char *pszFile);
    ~CBitmap();
    bool Load(const char *pszFile);
	void GetDimension(int& riHeight, int& riWidth);
	void GetData(std::vector<char>&	rvcImageData);
	unsigned int GetPixel(int iRow, int iColumn);

	float GetPixelLuminance(int iRow, int iColumn);

private:
    //variables
	std::vector<RGBQUAD>	m_vRGBQuads;
	std::vector<char>	m_vcImageData;
	unsigned short m_sBitPerPixel;
	std::string m_strError;
	bool m_bLoaded;
    //BitmapFileHeader bmfh;
	BITMAPFILEHEADER  m_BitmapFileHeader;

  //  BitmapInfoHeader bmih;

	BITMAPINFOHEADER	m_BitmapInfoHeader;
    //int byteWidth;            //the width in bytes of the image

	int m_iWidthInBytes;    // 

    //int padWidth;             //the width in bytes of the added image

	int m_iPadWidthInBytes;


  //  unsigned int dataSize;                //size of the data in the file

	unsigned int m_uiDataSize;

	//int width,height;
	int m_iWidth,m_iHeight;

    //methods
    void Reset(void);
    bool Convert24(std::vector<char>& rvcImageData);		//convert to 24bit RGB bottom up data
    bool Convert8(std::vector<char>& rvcImageData);		//convert to 24bit RGB bottom up data
};
