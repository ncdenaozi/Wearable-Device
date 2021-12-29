#ifndef GYROSCOPE_H
#define GYROSCOPE_H
//I3G4250D gyroscope

// Documents
// Manual for dev board: https://www.st.com/resource/en/user_manual/um1670-discovery-kit-with-stm32f429zi-mcu-stmicroelectronics.pdf
// gyroscope datasheet: https://www.mouser.com/datasheet/2/389/dm00168691-1798633.pdf

#define WHOAMI 0x0F

#define CONTROL_REG1 0x20

#define OUT_X_L 0x28
#define OUT_X_H 0x29
#define OUT_Y_L 0x2A
#define OUT_Y_H 0x2B
#define OUT_Z_L 0x2C
#define OUT_Z_H 0x2D

#include <mbed.h>
#include <vector>

class GYROSCOPE{
    public:
        short ang_x;
        short ang_y;
        short ang_z;

        SPI device;
        DigitalOut cs;

        GYROSCOPE();
        ~GYROSCOPE();

        void whoami();
        void setup();
        void readreg(uint8_t addr);
        void getxyz();
        void unsigned_to_signed();
        vector<short> dump();
};


#endif