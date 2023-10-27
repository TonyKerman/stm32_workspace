//
// Created by tony on 23-10-5.
//

#ifndef MAPPING_HPP
#define MAPPING_HPP
#ifdef __cplusplus
extern "C" {
#endif
#include "main.h"
#ifdef __cplusplus
}
class Mapping {
private:

    float k_i2o;
    int16_t b_i20;
    float k_o2i;
    int16_t b_o2i;
    int16_t min_out;
    int16_t max_out;

public:
    //int16_t pos_out;
    Mapping(int16_t min_out,int16_t max_out){
        this->min_out = min_out;
        this->max_out = max_out;
        this->k_i2o = 1;
        this->b_i20 = 0;
        this->k_o2i = 1;
        this->b_o2i = 0;

    }
    void  set_trans_in2out(float in_a,float in_b,float in_off,int16_t out_a,int16_t out_b,int16_t out_off)
    {
        this->k_i2o = (float)(out_b - out_a) / (in_b - in_a);
        this->b_i20 = out_off - (int16_t)(this->k_i2o * in_off);
        this->k_o2i = (float)(in_b - in_a) / (out_b - out_a);
        this->b_o2i = in_off - (int16_t)(this->k_o2i * out_off);

    }


    int16_t trans_i2o(float target_pos) {

        int16_t target_out = (int16_t)(this->k_i2o * target_pos + this->b_i20);
        if (target_out > this->max_out)
            target_out = this->max_out;
        else if (target_out < this->min_out)
            target_out = this->min_out;
        //printf("k_i2o:%f,b_i20:%d,target_out:%d\n", this->k_i2o, this->b_i20, target_out);
        //this->pos_out = target_out;
        return target_out;
    }
    float trans_o2i(int16_t target_out) {
        return this->k_o2i * target_out + this->b_o2i;
    }
};
#endif


#endif //SERVO_H
