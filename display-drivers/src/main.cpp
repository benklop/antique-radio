#include "vfd_writer.h"
#include "INIReader.h"

// Name of program mainreturn.cpp
#include <iostream>
using namespace std;
 
int main(int argc, char** argv)
{
    INIReader reader("config.ini");
    if (reader.ParseError() < 0) {
        cout << "Can't load 'config.ini'\n";
        return 1;
    }
    VfdWriter vfd = VfdWriter();

    vfd.init(reader.GetInteger("screen", "brightness", 75));

    if(argc > 1) {
        vector<gu7000_image> images;
        cout << "Loading files:" << endl;
        for (int i = 1; i < argc; ++i) {
            cout << argv[i] << "\n";
            images.push_back(VfdWriter::load_image(argv[i]));
        }
        
        for (int i = 0; i < images.size(); i++) {
            vfd.draw_image(images.at(i));
            nanosleep((const struct timespec[]){{0, 16666667L}}, null);
        }
    }
    else {
        gu7000_image image = VfdWriter::load_image(reader.Get("screen", "splash", ""));
        vfd.draw_image(image);
    }
    return 0;
}
