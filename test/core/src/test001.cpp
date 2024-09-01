#include <print>

#include "resources/image.h"

int main(int argc, char* argv[]) {

    CG::Image image("c:/Users/yaojie/Desktop/c5366469b00940f5b8077c20c2c2d089.jpg");

    image.Resize(image.GetWidth() / 10, image.GetHeight() / 10);

    image.SaveAsPNG();
    image.SaveAsBMP();
    image.SaveAsTGA();
    image.SaveAsJPG();

    return 0;
}
