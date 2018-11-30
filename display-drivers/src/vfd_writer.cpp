// Copyright 2018 Ben Klopfenstein

#include "vfd_writer.h"
VfdWriter::VfdWriter() {
    init();
}

void VfdWriter::init(int brightness) {
    vfd.GU7000_reset();
    vfd.GU7000_init();
    vfd.GU7000_home();

    vfd.GU7000_setScreenBrightness(brightness);
}

gu7000_image VfdWriter::load_image(string filename) {
    cout << "loading image " << filename << " ... ";
    cimg::CImg<bool> src(filename.c_str());
    gu7000_image image = convert_image(src);
    cout << "success!" << endl;
    return image;
}

gu7000_image VfdWriter::convert_image(const cimg::CImg<bool> &src) {
    gu7000_image image;
    image.width = src.width();
    image.height = src.height();
    vector<uint8_t> converted;

    int num_bytes = src.width() * (src.height()/8);

    for (int i = 0; i < src.width(); i++) {
        bitset<8> this_byte(0);
        for (int j = 0; j < src.height() / 8; j++) {
            for (int k = 0; k < 8; k++) {
                if (*src.data(i, j * 8 + k)) {
                    this_byte[k] = 0;
                } else {
                    this_byte[k] = 1;
                }
            }
            uint8_t b = (uint8_t)this_byte.to_ulong();
            b = ((b * 0x0802LU & 0x22110LU) | (b * 0x8020LU & 0x88440LU)) * 0x10101LU >> 16;
            converted.push_back(b);
        }
    }
    image.data = converted.data();
    return image;
}

void VfdWriter::draw_image(const gu7000_image &img) {
    vfd.GU7000_drawImage(img.width, img.height, img.data);
}
