//
// Created by tony on 23-10-5.
//

#ifndef SERVO_HPP
#define SERVO_HPP
#ifdef __cplusplus
extern "C" {
#endif
#include "main.h"
#ifdef __cplusplus
}
class Genetic_Servo {
private:

    float k;
    int16_t b;
    int16_t min_out;
    int16_t max_out;

public:
    int16_t pos_out;
    Genetic_Servo(int16_t min_out,int16_t max_out){
        this->min_out = min_out;
        this->max_out = max_out;
        this->k = 1;
        this->b = 0;
        this->pos_out = 0;
    }
    void  set_trans_in2out(float in_a,float in_b,float in_off,int16_t out_a,int16_t out_b,int16_t out_off)
    {
        this->k = (float)(out_b-out_a)/(in_b-in_a);
        this->b = out_off-(int16_t)(this->k*in_off);
    }
    void set_pos(int16_t pos) { this->pos_out = pos_out; }


    int16_t run(float target_pos) {

        int16_t target_out = (int16_t)(this->k * target_pos + this->b);
        if (target_out > this->max_out)
            target_out = this->max_out;
        else if (target_out < this->min_out)
            target_out = this->min_out;
        //printf("k:%f,b:%d,target_out:%d\n", this->k, this->b, target_out);
        this->pos_out = target_out;
        return target_out;
    }
};
#endif


#endif //SERVO_H
