#include "../driver/config.h"
#include "../driver/Noritake_VFD_GU7000.h"
#include "Config.h"

#include "boost/dynamic_bitset.hpp"

//included libraries
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
    uint8_t* data;
    int width;
    int height;
};

gu7000_image load_image(string filename) {
    cout << "loading image " << filename << " ... ";
    CImg<bool> src(filename.c_str());

    gu7000_image image;
    image.width = src.width();
    image.height = src.height();
    vector<uint8_t> converted;

    int num_bytes = src.width() * (src.height()/8);

    for (int i=0; i < src.width(); i++) {
        bitset<8> this_byte(0);
        for(int j=0; j < src.height() / 8; j++) {
            for(int k=0; k < 8; k++) {
                if(*src.data(i, j * 8 + k)) {
                    this_byte[k] = 0;
                }
                else {
                    this_byte[k] = 1;
                }
            }
            uint8_t b = (uint8_t)this_byte.to_ulong();
            b = ((b * 0x0802LU & 0x22110LU) | (b * 0x8020LU & 0x88440LU)) * 0x10101LU >> 16;
            converted.push_back(b);
        }
    }
    image.data = converted.data();
    cout << "success!" << endl;
    return image;
}

int main() {
    vfd.GU7000_reset();
    vfd.GU7000_init();
    vfd.GU7000_home();



    INIReader reader("config.ini");
    if (reader.ParseError() < 0) {
        cout << "Can't load 'config.ini'\n";
        return 1;
    }

    vfd.GU7000_setScreenBrightness(reader.GetInteger("screen", "brightness", 75));

    gu7000_image image = load_image(reader.Get("screen", "splash", ""));
    vfd.GU7000_drawImage(image.width, image.height, image.data);
}
