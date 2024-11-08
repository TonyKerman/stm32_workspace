#ifndef __KEY_DRIVER_H__
#define __KEY_DRIVER_H__

#include "main.h"

/* ������������ֵ���� */
#define key_alway_shield 0xFFFFFFFF

/* ����ʱ������ */
#define click_one_wait_for_double_time 500       // ˫������֮��������ʱ�䣬��λms
#define long_press_time 1600                     // ʶ��Ϊ������ʱ�䣬��λms
#define long_press_effective_interval_time 10000 // ����ʱÿ�δ����ļ��ʱ�䣬��λms

typedef void (*key_callback_t)(void);

/* �������� */
typedef enum {
	key_sw1,
	key_sw2,
	key_sw3,
	key_sw4,
	key_sw5,
	key_sw6,
	key_num,
} key_num_e;

/* ״̬�� */
typedef enum 
{
	key_release,                       // �����ͷ�״̬
	key_short_pressing,                // �����̰�״̬
	key_double_pressing,               // ����˫��״̬
	key_click_one_wait_for_double,     // ���������ȴ�˫��״̬
	key_long_pressing,                 // ��������״̬
	key_state_machine_num,             // ״̬�����������ڸ�������
} key_state_machine_e;

/* ��ֵ */
typedef enum {
	key_none = key_state_machine_num,
	key_click_one,
	key_click_double,
	key_long_press,
	key_value_num
} key_value_e;

/* �������� */
typedef struct {
	uint32_t hold_time;             // ��������ʱ��
	uint64_t time_point;            // ʱ���
	uint32_t trigger_variable;      // ��������
	uint8_t last_variable_state;    // ��һ�α���״̬
	uint8_t result;                 // ���
} hold_filter_t;

/* ���������� */
typedef struct {
	hold_filter_t bsp_hold_filter;    	// Ӳ����İ����Ƿ����жϣ��˲���
	uint32_t shield;                    // ����ʱ�䣺0����ȫ���Σ�1������ʹ�ܣ�����1������ʱ����
	uint32_t press_time;                // ��ס��ʼʱ��
	uint32_t release_time;              // �ͷſ�ʼʱ��
	key_state_machine_e state_machine;  // ������ǰ״̬��״̬����
	key_value_e key_value;              // ��ֵ
	key_callback_t key_callback[key_value_num];
} key_driver_t;

void key_driver_init(void);
void key_check(void);
void key_array_init(void);
void key_attach(key_num_e num, key_value_e value, key_callback_t callback);
void key_process(void);
void key_callback(key_driver_t *key_p);
uint8_t hold_filter(hold_filter_t *filter_p, uint32_t variable);

#endif /* __KEY_DRIVER_H__ */


