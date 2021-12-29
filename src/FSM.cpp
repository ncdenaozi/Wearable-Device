#include <FSM.h>
#include <mbed.h>
#include <string>

FSM::FSM(){
    for(int i=0;i<40;i++){
        velocity[i]=0;
    }
    current_state=STATE_STOP;
}

void FSM::change_state(){
    switch (current_state)
    {
    case STATE_STOP:
        current_state=STATE_RUN;
        break;
    case STATE_RUN:
        current_state=STATE_OUTPUT;
        break;
    case STATE_OUTPUT:
        current_state=STATE_STOP;
        break;    
    default:
        break;
    }
}

void FSM::push(float v){
    for(int i=39;i>0;i--){
        velocity[i]=velocity[i-1];
    }
    velocity[0]=v;
}

void FSM::pop(){
    float total_length=0;
    printf("last 20 second linear forward velocity is: ");
    for(int i=0;i<40;i++){
        printf("%f, ",velocity[i]); 
        total_length+=0.5*velocity[i];
    }
    printf("\n");
    printf("last 20 second total length is: %f meters \n",total_length);
}
