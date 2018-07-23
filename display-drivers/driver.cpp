#include "gu7000/config.h"
#include "gu7000/Noritake_VFD_GU7000.h"
Noritake_VFD_GU7000 vfd;
int main() {
    vfd.GU7000_reset();
    vfd.GU7000_init();
    vfd.GU7000_drawFROMImage(0x0, 16, 16, 16); // Display the image antenna 16X16
}
