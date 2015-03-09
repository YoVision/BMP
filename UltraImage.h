#ifndef UltraImage_H
#define UltraImage_H
#include <iostream>
using namespace std;

class UltraImage{
public:
       UltraImage();
       void Load(const char*);
       //void Save(const char*);
       //void Conv2Gray();
       //void DownSample(int);
       //void Mirror_y();
       //void Mirror_x();
       //void Rotate_CW();
       //void Rotate_CCW();
       //void Rotate_2(double);

private:
       int width,height;        // image width, image height
       char factor;             // factor of downsample
       int  width_d, height_d;  // downsample_image width, height
       int rgb_raw_data_offset; // RGB raw data offset
       int file_size;           //file_size = width_d * height_d * 3  + rgb_raw_data_offset;
       unsigned char *ImgValue_s;          //Img RGB Info recently
       unsigned char *ImgValue_t;          //Img RGB Info changed
};

#endif
