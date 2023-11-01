//
// Created by tony on 23-10-24.
//

#ifndef MAPPING_H
#define MAPPING_H
#ifdef __cplusplus
extern "C" {
#endif
#include "main.h"
typedef struct{
    double k_i2o;
    float b_i20;
    float k_o2i;
    float b_o2i;
    float min_out;
    float max_out;
}mapping_param_t;

void mapping_param_init(mapping_param_t *this,float in_a,float in_b,float in_off,float out_a,float out_b,float out_off);
void mapping_set_limit(mapping_param_t *this,float min_out,float max_out);
float trans_i2o(mapping_param_t *this,float target_pos);
float trans_o2i(mapping_param_t *this,float target_out);

#ifdef __cplusplus
}
#endif
#endif //MAPPING_H
