//
// Created by tony on 23-10-24.
//

#include "mapping.h"
#include "stdlib.h"
#define MIN_OUT (-3.40E+38f)
#define MAX_OUT 3.40E+38f

void mapping_param_init(mapping_param_t *this,float in_a,float in_b,float in_off,float out_a,float out_b,float out_off)
{
    this->min_out = MIN_OUT;
    this->max_out = MAX_OUT;
    this->k_i2o = (float)(out_b - out_a) / (in_b - in_a);
    this->b_i20 = out_off - (float )(this->k_i2o * in_off);
    this->k_o2i = (float)(in_b - in_a) / (out_b - out_a);
    this->b_o2i = in_off - (float )(this->k_o2i * out_off);
}

void mapping_set_limit(mapping_param_t *this,float min_out,float max_out)
{
    this->min_out = min_out;
    this->max_out = max_out;
}
float trans_i2o(mapping_param_t *this,float target_pos)
{
    float target_out = this->k_i2o * target_pos + this->b_i20;
    if (this->max_out!=MAX_OUT&&target_out > this->max_out)
        target_out = this->max_out;
    else if (this->min_out!=MIN_OUT&&target_out < this->min_out)
        target_out = this->min_out;
    //printf("k_i2o:%f,b_i20:%d,target_out:%d\n", this->k_i2o, this->b_i20, target_out);
    //this->pos_out = target_out;
    return target_out;
}
float trans_o2i(mapping_param_t *this,float target_out)
{
    return this->k_o2i * target_out + this->b_o2i;
}