#ifndef __SCCB_H__
#define __SCCB_H__

#include "main.h"

#define OV2640_DEVICE_ADDRESS 0x60 // OV2640��ַ
#define OV5640_DEVICE_ADDRESS 0X78 // OV5640��ַ

/*----------------------------------------- IIIC �������ú� -----------------------------------------------*/

#define SCCB_SCL_CLK_ENABLE 	__HAL_RCC_GPIOB_CLK_ENABLE() // SCL ����ʱ��
#define SCCB_SCL_PORT 				GPIOB								 // SCL ���Ŷ˿�
#define SCCB_SCL_PIN 					GPIO_PIN_8							 // SCL ����

#define SCCB_SDA_CLK_ENABLE 	__HAL_RCC_GPIOB_CLK_ENABLE() // SDA ����ʱ��
#define SCCB_SDA_PORT 				GPIOB								 // SDA ���Ŷ˿�
#define SCCB_SDA_PIN 					GPIO_PIN_9							 // SDA ����

/*------------------------------------------ IIC��ض��� -------------------------------------------------*/

#define ACK_OK 1  // ��Ӧ����
#define ACK_ERR 0 // ��Ӧ����
/*-------------------------------------------- IO�ڲ��� ---------------------------------------------------*/

#define SCCB_SCL(x)                                                   \
	if (x)                                                            \
		HAL_GPIO_WritePin(SCCB_SCL_PORT, SCCB_SCL_PIN, GPIO_PIN_SET); \
	else                                                              \
		HAL_GPIO_WritePin(SCCB_SCL_PORT, SCCB_SCL_PIN, GPIO_PIN_RESET)

#define SCCB_SDA(x)                                                   \
	if (x)                                                            \
		HAL_GPIO_WritePin(SCCB_SDA_PORT, SCCB_SDA_PIN, GPIO_PIN_SET); \
	else                                                              \
		HAL_GPIO_WritePin(SCCB_SDA_PORT, SCCB_SDA_PIN, GPIO_PIN_RESET)

#define SCCB_READ_SDA HAL_GPIO_ReadPin(SCCB_SDA_PORT, SCCB_SDA_PIN)
/*--------------------------------------------- �������� --------------------------------------------------*/

void sccb_init(void);					  // IIC���ų�ʼ��
void sccb_delay(void);					  // IIC��ʱ����
void sccb_start(void);					  // ����IICͨ��
void sccb_stop(void);					  // IICֹͣ�ź�
void sccb_ack(void);					  // ������Ӧ�ź�
void sccb_nack(void);					  // ���ͷ�Ӧ���ź�
uint8_t sccb_wait_ack(void);			  // �ȴ�Ӧ���ź�
uint8_t sccb_send_byte(uint8_t data);	  // д�ֽں���
uint8_t sccb_read_byte(uint8_t ack_mode); // ���ֽں���

uint8_t sccb_write_reg(uint8_t addr, uint8_t value); // ��ָ���ļĴ���(8λ��ַ)дһ�ֽ����ݣ�OV2640�õ�
uint8_t sccb_read_reg(uint8_t addr);				 // ��ָ���ļĴ���(8λ��ַ)��һ�ֽ����ݣ�OV2640�õ�

uint8_t sccb_write_reg_16bit(uint16_t addr, uint8_t value);					// ��ָ���ļĴ���(16λ��ַ)дһ�ֽ����ݣ�OV5640�õ�
uint8_t sccb_read_reg_16bit(uint16_t addr);									// ��ָ���ļĴ���(16λ��ַ)��һ�ֽ����ݣ�OV5640�õ�
uint8_t sccb_write_buf_16bit(uint16_t addr, uint8_t *pData, uint32_t size); // ��ָ���ļĴ���(16λ��ַ)����д���ݣ�OV5640 д���Զ��Խ��̼�ʱ�õ�

#endif //__SCCB_H__
