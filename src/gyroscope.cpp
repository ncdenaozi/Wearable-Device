#include <gyroscope.h>

GYROSCOPE::GYROSCOPE():
device(PF_9, PF_8, PF_7),cs(PC_1)
{
    this->ang_x=0;
    this->ang_y=0;
    this->ang_z=0;

    this->cs=1;
    this->device.format(8,3);
    this->device.frequency(1000000);
}

GYROSCOPE::~GYROSCOPE(){
    this->cs=0;
    this->device.write(CONTROL_REG1);
    this->device.write(0x07);
    this->cs=1;
}

void GYROSCOPE::whoami(){
    this->cs=0;
    this->device.write(1<<7|WHOAMI);
    int whoami = this->device.write(0x00);
    printf("WHOAMI register = 0x%X\n", whoami);
    
    this->cs=1;
}

void GYROSCOPE::readreg(uint8_t addr){
    this->cs=0;
    this->device.write(1<<7|addr);
    int temp = this->device.write(0x00);
    printf("register = 0x%X\n", temp);
    this->cs=1;
}

void GYROSCOPE::setup(){
    this->cs=0;
    this->device.write(CONTROL_REG1);
    this->device.write(0x0F);
    this->cs=1;
}

vector<short> GYROSCOPE::dump(){
    getxyz();
    vector<short> result;
    //printf("X = %d, Y = %d, Z = %d\n", this->ang_x,this->ang_y,this->ang_z);
    result.push_back(ang_x);
    result.push_back(ang_y);
    result.push_back(ang_z);

    return result;
}

void GYROSCOPE::getxyz(){
    uint8_t temp=0;

    this->cs=0;
    this->device.write(1<<7|OUT_X_L);
    temp= this->device.write(0x00);
    this->ang_x=temp;
    this->cs=1;

    this->cs=0;
    this->device.write(1<<7|OUT_X_H);
    temp= this->device.write(0x00);
    this->ang_x=(temp<<8)|this->ang_x;
    this->cs=1;

    this->cs=0;
    this->device.write(1<<7|OUT_Y_L);
    temp= this->device.write(0x00);
    this->ang_y=temp;
    this->cs=1;

    this->cs=0;
    this->device.write(1<<7|OUT_Y_H);
    temp= this->device.write(0x00);
    this->ang_y=(temp<<8)|this->ang_y;
    this->cs=1;

    this->cs=0;
    this->device.write(1<<7|OUT_Z_L);
    temp= this->device.write(0x00);
    this->ang_z=temp;
    this->cs=1;

    this->cs=0;
    this->device.write(1<<7|OUT_Z_H);
    temp= this->device.write(0x00);
    this->ang_z=(temp<<8)|this->ang_z;
    this->cs=1;

    this->cs=1;

    unsigned_to_signed();
}

void GYROSCOPE::unsigned_to_signed(){
    if(this->ang_x>>15==1){
        this->ang_x=!this->ang_x+1;
        this->ang_x=0-this->ang_x;
    }
    if(this->ang_y>>15==1){
        this->ang_y=!this->ang_y+1;
        this->ang_y=0-this->ang_y;
    }
    if(this->ang_z>>15==1){
        this->ang_z=!this->ang_z+1;
        this->ang_z=0-this->ang_z;
    }
}