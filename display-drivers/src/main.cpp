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
    vector<uint8_t> data;
    int width;
    int height;
};

gu7000_image load_image(string filename) {
    cout << "loading image " << filename << " ... ";
    CImg<bool> src(filename.c_str());

    gu7000_image image;
    image.width = src.width();
    image.height = src.height();

    int current_bit = 0;
    for (int i=0; i < image.height; i++) {

        bitset<8> this_byte(0);
        bitset<8> other_byte(0);
        for(int j=0; j < image.width; j++) {
            int swapped_bit = (current_bit % 8);
            if(*src.data(i,j)) {
                this_byte[swapped_bit] = 0;
            }
            else {
                this_byte[swapped_bit] = 1;
            }
            current_bit++;

            if(current_bit % 16 == 0) {
                image.data.push_back((uint8_t)this_byte.to_ulong());
                image.data.push_back((uint8_t)other_byte.to_ulong());
            }
            if(current_bit % 8 == 0) {
                other_byte = this_byte;
            }
        }
    }

  /*  int half = image.data.size() / 2;

    for(int i = 0; i < half; i++) {
        image.data.push_back(image.data.front());
        image.data.erase(image.data.begin());
    }
*/
    cout << "success!" << endl;
    return image;
}

void render_image(gu7000_image image) {
    int bpr = image.width / 8;
    cout << "image uses " << image.data.size() << " 8 bit bytes" << endl;
    for(int i = 0; i < image.data.size(); i++) {
        bitset<8> data(image.data[i]);
        for(int j = 0; j < 8; j++) {
            cout << (data[j] ? " " : "■");
        }
        if(i % bpr == 1)
            cout << endl;
    }
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

    gu7000_image image = load_image(reader.Get("images", "initial", ""));
    render_image(image);
    vfd.GU7000_drawImage(image.width, image.height, image.data.data());
}
