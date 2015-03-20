#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include "UltraImage.h"
using namespace std;

UltraImage::UltraImage(){
    width = 0;
    height = 0;
    factor = 1;
    width_d = 0;
    height_d = 0;
    rgb_raw_data_offset = 0;
    file_size = 0;
    biSizeImage = 0;
}

void UltraImage::Load(const char *fileName){
    ifstream file(fileName, ios::in|ios::binary );
    if(!file)
        cout << " Read image error!!" << endl;

    // move offset to 3 to find bit data size
    file.seekg(2,ios::beg);
    file.read((char*)&file_size,sizeof(int));
    // move offset to 34 to find bit data size
    file.seekg(34,ios::beg);
    file.read((char*)&biSizeImage,sizeof(int));      
    // move offset to 10 to find rgb raw data offset
    file.seekg(10,ios::beg);
    file.read((char*)&rgb_raw_data_offset,sizeof(int));        
    // move offset to 18 to get width & height;
    file.seekg(18,ios::beg);
    file.read((char*)&width,sizeof(int));
    file.read((char*)&height,sizeof(int));
    // move offset to rgb_raw_data_offset to get RGB raw data
    file.seekg(rgb_raw_data_offset,ios::beg);
    
    ImgValue_s = new unsigned char [width * height * 3];
    ImgValue_t = new unsigned char [width * height * 3];
    file.read((char*)ImgValue_s,sizeof(char)*width * height * 3);

    file.close();
}

void UltraImage::Conv2Gray(){
     unsigned char R,G,B;
     unsigned char gray;
    
     for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {

            R = ImgValue_s[3 * (width*y+x) + 2];
            G = ImgValue_s[3 * (width*y+x) + 1];
            B = ImgValue_s[3 * (width*y+x) + 0];

            gray = ( R*11 + G*16 + B*5 )/32; // define gray color
            //gray = 0.299*R + 0.587*G + 0.114*B;
            ImgValue_t[3 * (width*y+x) + 2] = gray;
            ImgValue_t[3 * (width*y+x) + 1] = gray;
            ImgValue_t[3 * (width*y+x) + 0] = gray;
       }
    }
}

void UltraImage::Save(const char *fileName){

     char header[54] = {
      0x42,        // identity : B
      0x4d,        // identity : M
      0, 0, 0, 0,  // file size
      0, 0,        // reserved1
      0, 0,        // reserved2
      54, 0, 0, 0, // RGB data offset
      40, 0, 0, 0, // struct BITMAPINFOHEADER size
      0, 0, 0, 0,  // bmp width
      0, 0, 0, 0,  // bmp height
      1, 0,        // planes
      24, 0,       // bit per pixel
      0, 0, 0, 0,  // compression
      0, 0, 0, 0,  // data size
      0, 0, 0, 0,  // h resolution
      0, 0, 0, 0,  // v resolution 
      0, 0, 0, 0,  // used colors
      0, 0, 0, 0   // important colors
    };
   
    ofstream file(fileName, ios::out|ios::binary );
     
    if(!file)
        cout << " Write image error!!" << endl;

    // file size = width * height * 3 + rgb_raw_data_offset  ;
    header[2] = (unsigned char)(file_size & 0x000000ff);
    header[3] = (file_size >> 8)  & 0x000000ff;
    header[4] = (file_size >> 16) & 0x000000ff;
    header[5] = (file_size >> 24) & 0x000000ff;
     
    // width
    header[18] = width & 0x000000ff;
    header[19] = (width >> 8)  & 0x000000ff;
    header[20] = (width >> 16) & 0x000000ff;
    header[21] = (width >> 24) & 0x000000ff;

    // height
    header[22] = height &0x000000ff;
    header[23] = (height >> 8)  & 0x000000ff;
    header[24] = (height >> 16) & 0x000000ff;
    header[25] = (height >> 24) & 0x000000ff;  
    
    // bitSizeimage
    header[34] = biSizeImage &0x000000ff;
    header[35] = (biSizeImage >> 8)  & 0x000000ff;
    header[36] = (biSizeImage >> 16) & 0x000000ff;
    header[37] = (biSizeImage >> 24) & 0x000000ff; 
 
    // write header
    file.write(header,54);

    // write image
    file.write((char*)ImgValue_t,sizeof(char)*width * height * 3);    
}

void UltraImage::DownSample(int fac){
    factor = fac;
    width_d = width / factor;
    height_d = height / factor;
    
    for(int y=0; y < height_d; ++y){
        for(int x=0; x < width_d; ++x){
            unsigned char R = ImgValue_s[3 * (width*(y*factor)+(x*factor) ) + 2];
            unsigned char G = ImgValue_s[3 * (width*(y*factor)+(x*factor) ) + 1];
            unsigned char B = ImgValue_s[3 * (width*(y*factor)+(x*factor) ) + 0];
            
            ImgValue_t[3 * (width_d*y+x) + 2] = R;
            ImgValue_t[3 * (width_d*y+x) + 1] = G;
            ImgValue_t[3 * (width_d*y+x) + 0] = B;
            
            ImgValue_s[3 * (width_d*y+x) + 2] = R;
            ImgValue_s[3 * (width_d*y+x) + 1] = G;
            ImgValue_s[3 * (width_d*y+x) + 0] = B;       
        }
    }

    ImgValue_s = ImgValue_t;
    height = height_d;
    width = width_d;
}

