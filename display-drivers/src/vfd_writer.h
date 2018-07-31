#include <fstream>
#include <vector>
#include <bitset>
#include <iostream>

#include "Config.h"
#include "./cimg/CImg.h"
#include "../driver/config.h"
#include "../driver/Noritake_VFD_GU7000.h"



using namespace std;
using namespace cimg_library;

struct gu7000_image {
    uint8_t* data;
    int width;
    int height;
};

class VfdWriter {
    public:
    VfdWriter();

    static gu7000_image load_image(string filename);
    static gu7000_image convert_image(CImg<bool> &src);
    
    void init(int brightness = 75);
    void draw_image(gu7000_image &img);

    private:
    Noritake_VFD_GU7000 vfd;
};