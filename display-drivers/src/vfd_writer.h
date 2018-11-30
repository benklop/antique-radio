// Copyright 2018 Ben Klopfenstein

#ifndef DISPLAY_DRIVERS_SRC_VFD_WRITER_H_
#define DISPLAY_DRIVERS_SRC_VFD_WRITER_H_
#include <fstream>
#include <string>
#include <vector>
#include <bitset>
#include <iostream>

#include "Config.h"
#include "./cimg/CImg.h"
#include "../driver/config.h"
#include "../driver/Noritake_VFD_GU7000.h"


using namespace std; //NOLINT
namespace cimg = ::cimg_library;

struct gu7000_image {
    uint8_t* data;
    int width;
    int height;
};

class VfdWriter {
 public:
    VfdWriter();

    static gu7000_image load_image(string filename);
    static gu7000_image convert_image(const cimg::CImg<bool> &src);

    void init(int brightness = 75);
    void draw_image(const gu7000_image &img);
    void flip();

 private:
    Noritake_VFD_GU7000 vfd;

    bool page = false;
};

#endif  // DISPLAY_DRIVERS_SRC_VFD_WRITER_H_
