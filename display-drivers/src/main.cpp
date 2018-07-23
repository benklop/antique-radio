#include "../driver/config.h"
#include "../driver/Noritake_VFD_GU7000.h"
#include "Config.h"

//libraries
#include "INIReader.h"
#include "../cimg/CImg.h"


#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>

Noritake_VFD_GU7000 vfd;

using namespace std;
using namespace cimg_library;

struct gu7000_image {
    uint8_t * data;
    int width;
    int height;
};

gu7000_image load_image(string filename) {
    cout << "loading image " << filename << " ... ";
    CImg<bool> src(filename.c_str());

    gu7000_image image;
    image.width = src.width();
    image.height = src.height();

    uint8_t data[image.width * image.height];
    for (int i=0; i < (image.width * image.height) / 8; i++) {
        for(int j=0; j < 8; j++) {
            int this_bit = i*8 + j;
            int col = this_bit + 1 % (image.width);
            int row = this_bit + 1 / (image.width);
            if(*src.data(row, col) == true) {
                data[i] |= (1<<j);
            }
        }
    }
    image.data = data;
    cout << "success!" << endl;
    return image;
}

int main() {
    vfd.GU7000_reset();
    vfd.GU7000_init();
   

    INIReader reader("config.ini");
    if (reader.ParseError() < 0) {
        cout << "Can't load 'config.ini'\n";
        return 1;
    }

    gu7000_image image = load_image(reader.Get("images", "initial", ""));
    vfd.GU7000_drawImage(image.width, image.height, image.data);
}
