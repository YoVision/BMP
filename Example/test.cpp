#include <iostream>
#include "UltraImage.h"
using namespace std;


int main()
{
    UltraImage pic;
    pic.Load("pic.bmp");
    pic.Conv2Gray();

    pic.Save("pic_test.bmp");
    
    system("pause");
    return 0;
}
