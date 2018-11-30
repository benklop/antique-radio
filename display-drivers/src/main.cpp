// Copyright 2018 Ben Klopfenstein

#include "vfd_writer.h"
#include "INIReader.h"

int main() {
    INIReader reader("/etc/antique-radio/config.ini");
    if (reader.ParseError() < 0) {
        cout << "Can't load 'config.ini'\n";
        return 1;
    }

    gu7000_image image = VfdWriter::load_image(reader.Get("screen", "splash", ""));
    VfdWriter vfd = VfdWriter();
    vfd.init(reader.GetInteger("screen", "brightness", 75));
    vfd.draw_image(image);
    return 0;
}
