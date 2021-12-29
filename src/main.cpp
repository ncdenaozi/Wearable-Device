#define M_PI    3.14159265358979323846

#include <mbed.h>
#include <gyroscope.h>
#include <FSM.h>

float angular_to_velocity(const vector<vector<short>> &buffer);

DigitalOut led(LED1);

InterruptIn buttonInterrupt(USER_BUTTON);
volatile uint8_t flag=0;

void set_flag(){
  flag=1;
}

int main() {
  buttonInterrupt.rise(&set_flag);

  //debug only 
  float length=0;
  //debug only int counter=0;
  GYROSCOPE gyro;
  FSM fsm1;
  gyro.setup();
  //gyro.readreg(CONTROL_REG1);
  while(1) {
    switch (fsm1.current_state)
    {
    case STATE_STOP:  //do nothing    
      break;
    case STATE_RUN:{
      
      vector<vector<short>> buffer;
      for(int n=0;n<5;n++){
        buffer.push_back(gyro.dump());
        ThisThread::sleep_for(100);
      }
      float velocity=angular_to_velocity(buffer);
      //debug only 
      //length+=velocity*0.5f;printf("velocity is: %f\n",velocity);printf("length is: %f\n",length);
      fsm1.push(velocity);
      
      //for debug and data plotting
      /*
      vector<short> temp=gyro.dump();
      printf("X(%d)= %d; Y(%d) = %d; Z(%d)=%d;\n", counter,temp[0],counter,temp[1],counter,temp[2]);
      counter++;
      ThisThread::sleep_for(100);
      }
      */
      }
      break;
    case STATE_OUTPUT: {
      length=0;
      fsm1.pop();
      ThisThread::sleep_for(1s);
      }
      break;
    default:
      break;
    }
    if(flag==1){
      fsm1.change_state();
      flag=0;
    }
  }  
}

float angular_to_velocity(const vector<vector<short>> &buffer){
    short ang_pivot_x=-10;  //-10
    short ang_pivot_y=-14;  //-13.85
    short ang_pivot_z=7;    //6.88
    float delta_x=0;
    float delta_y=0;
    float delta_z=0;

    short Rth=500; 
    for(int i=0;i<5;i++){
      if(abs(buffer[i][0]-ang_pivot_x) > Rth){
        float temp_x = (buffer[i][0] - ang_pivot_x)*0.00875;
        delta_x= delta_x + 0.1f*temp_x;
      }
      if(abs(buffer[i][1]-ang_pivot_y) > Rth){
        if(abs(buffer[i][1])<=15000){
          float temp_y = (buffer[i][1] - ang_pivot_y)*0.00875;
          delta_y= delta_y + 0.1f*temp_y;
        }
      }
      if(abs(buffer[i][2]-ang_pivot_z) > Rth){
        float temp_z = (buffer[i][2] - ang_pivot_z)*0.00875;
        delta_z= delta_z + 0.1f*temp_z;
      }
    }
    float lx=0.00f;
    float ly=0.80f;
    float lz=0.80f;
    float vx=(delta_x/0.5f)*lx*M_PI/180;
    float vy=(delta_y/0.5f)*ly*M_PI/180;
    float vz=(delta_z/0.5f)*lz*M_PI/180;
    float vtotal=0.76*sqrt(vx*vx+vy*vy+vz*vz);

    return vtotal;
}
