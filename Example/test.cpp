#include <iostream>
#include <fstream>
#include "UltraImage.cpp"
using namespace std;


int main()
{
    UltraImage clena;
    clena.Load("pic.bmp");
    clena.Conv2Gray();

    clena.Save("pic_test.bmp");
    
    system("pause");
    return 0;
}
