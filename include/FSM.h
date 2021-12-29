#ifndef FSM_H
#define FSM_H

/*
    STATE_STOP ---button--> STATE_RUN
       /|\                       |
        |                        |
        |                      button
        |                        |
        |                       \|/
        |---button----------STATE_OUTPUT
*/
enum{
    STATE_STOP,
    STATE_RUN,
    STATE_OUTPUT
};

class FSM{
    private:
        float velocity[40];
    public:
        FSM();
        int current_state;
        void change_state();
        void push(float);
        void pop();
};


#endif