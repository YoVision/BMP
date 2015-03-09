#include <iostream>
#include <fstream>
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
}

void UltraImage::Load(const char *fileName){
    ifstream file(fileName, ios::in|ios::binary );
    if(!file)
        cout << " Read image error!!" << endl;
        
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
