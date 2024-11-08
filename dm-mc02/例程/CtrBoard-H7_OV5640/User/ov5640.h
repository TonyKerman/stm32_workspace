#ifndef __OV5640_H__
#define __OV5640_H__

#include "main.h"
#include "sccb.h"
#include "usart.h"
#include "lcd.h"

/******************************************************************************************/
/* PWDN��RESET ���� ���� */

#define OV_PWDN_GPIO_PORT GPIOC
#define OV_PWDN_GPIO_PIN GPIO_PIN_5
#define OV_PWDN_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOC_CLK_ENABLE()

#define OV_RESET_GPIO_PORT GPIOB
#define OV_RESET_GPIO_PIN GPIO_PIN_12
#define OV_RESET_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOB_CLK_ENABLE()

/******************************************************************************************/


/*-------------------------------------------------------------- �������ú� ---------------------------------------------*/

#define OV5640_PWDN_PIN            			 GPIO_PIN_5        				 	// PWDN ����      
#define OV5640_PWDN_PORT           			 GPIOC                 			 	// PWDN GPIO�˿�     
#define GPIO_OV5640_PWDN_CLK_ENABLE    	__HAL_RCC_GPIOC_CLK_ENABLE() 		// PWDN GPIO�˿�ʱ��

// �͵�ƽ������������ģʽ������ͷ��������
#define	OV5640_PWDN_OFF	HAL_GPIO_WritePin(OV5640_PWDN_PORT, OV5640_PWDN_PIN, GPIO_PIN_RESET)	

// �ߵ�ƽ���������ģʽ������ͷֹͣ��������ʱ���Ľ������
#define 	OV5640_PWDN_ON		HAL_GPIO_WritePin(OV5640_PWDN_PORT, OV5640_PWDN_PIN, GPIO_PIN_SET)	

/* IO���ƺ��� */
#define OV5640_PWDN(x)                                                                                                                                     \
    do                                                                                                                                                     \
    {                                                                                                                                                      \
        x ? HAL_GPIO_WritePin(OV_PWDN_GPIO_PORT, OV_PWDN_GPIO_PIN, GPIO_PIN_SET) : HAL_GPIO_WritePin(OV_PWDN_GPIO_PORT, OV_PWDN_GPIO_PIN, GPIO_PIN_RESET); \
    } while (0) /* POWER DOWN�����ź� */

#define OV5640_RST(x)                                                                                                                                          \
    do                                                                                                                                                         \
    {                                                                                                                                                          \
        x ? HAL_GPIO_WritePin(OV_RESET_GPIO_PORT, OV_RESET_GPIO_PIN, GPIO_PIN_SET) : HAL_GPIO_WritePin(OV_RESET_GPIO_PORT, OV_RESET_GPIO_PIN, GPIO_PIN_RESET); \
    } while (0) /* ��λ�����ź� */

// DCMI״̬��־��������֡�������ʱ���ᱻ HAL_DCMI_FrameEventCallback() �жϻص������� 1
extern volatile uint8_t OV5640_FrameState; // �������������������ļ����е���
extern volatile uint8_t OV5640_FPS;        // ֡��

// ������������ĸ�ʽ���� ov5640_set_pixformat() ����
#define Pixformat_RGB565 0
#define Pixformat_JPEG 1
#define Pixformat_GRAY 2

#define OV5640_AF_Focusing 2 // ���ڴ����Զ��Խ���
#define OV5640_AF_End 1      // �Զ��Խ�����
#define OV5640_Success 0     // ͨѶ�ɹ���־
#define OV5640_Error -1      // ͨѶ����

#define OV5640_Enable 1
#define OV5640_Disable 0

// OV5640����Чģʽ���� ov5640_set_effect() ����
#define OV5640_Effect_Normal 0   // ����ģʽ
#define OV5640_Effect_Negative 1 // ��Ƭģʽ��Ҳ������ɫȫ��ȡ��
#define OV5640_Effect_BW 2       // �ڰ�ģʽ
#define OV5640_Effect_Solarize 3 // ����Ƭ����ģʽ

// 1. ����OV5640ʵ�������ͼ���С�����Ը���ʵ�ʵ�Ӧ�û�����ʾ�����е���
// 2. ��������������Ӱ��֡��
// 3. ��Ϊ���õ�OV5640��ISP���ڱ���Ϊ4:3(1280*960)���û����õ�����ߴ�ҲӦ�����������
// 4. �����Ҫ������������Ҫ�޸ĳ�ʼ��������Ĳ���
#define OV5640_Width LCD_W  // 440   // ͼ�񳤶�
#define OV5640_Height LCD_H // 330   // ͼ����

// 1. ����Ҫ��ʾ�Ļ����С����ֵһ��Ҫ�ܱ�4��������
// 2. RGB565��ʽ�£����ջ���DCMI��OV5640�����4:3ͼ��ü�Ϊ��Ӧ��Ļ�ı���
// 3. JPGģʽ�£���ֵһ��Ҫ�ܱ�8��������
#define Display_Width LCD_W  // ͼ�񳤶�
#define Display_Height LCD_H // ͼ����

#define Display_BufferSize Display_Width *Display_Height * 2 / 4 // DMA�������ݴ�С��32λ��

/*------------------------------------------------------------ ���üĴ��� ------------------------------------------------*/

#define OV5640_ChipID_H 0x300A // оƬID�Ĵ��� ���ֽ�
#define OV5640_ChipID_L 0x300B // оƬID�Ĵ��� ���ֽ�

#define OV5640_FORMAT_CONTROL 0x4300     // �������ݽӿ�����ĸ�ʽ
#define OV5640_FORMAT_CONTROL_MUX 0x501F // ����ISP�ĸ�ʽ

#define OV5640_JPEG_MODE_SELECT 0x4713   // JPEGģʽѡ����1~6ģʽ���û��������ֲ����˵��
#define OV5640_JPEG_VFIFO_CTRL00 0x4600  // ��������JPEGģʽ2�Ƿ�̶�������
#define OV5640_JPEG_VFIFO_HSIZE_H 0x4602 // JPEG���ˮƽ�ߴ�,���ֽ�
#define OV5640_JPEG_VFIFO_HSIZE_L 0x4603 // JPEG���ˮƽ�ߴ�,���ֽ�
#define OV5640_JPEG_VFIFO_VSIZE_H 0x4604 // JPEG�����ֱ�ߴ�,���ֽ�
#define OV5640_JPEG_VFIFO_VSIZE_L 0x4605 // JPEG�����ֱ�ߴ�,���ֽ�

#define OV5640_GroupAccess 0X3212    // �Ĵ��������
#define OV5640_TIMING_DVPHO_H 0x3808 // ���ˮƽ�ߴ�,���ֽ�
#define OV5640_TIMING_DVPHO_L 0x3809 // ���ˮƽ�ߴ�,���ֽ�
#define OV5640_TIMING_DVPVO_H 0x380A // �����ֱ�ߴ�,���ֽ�
#define OV5640_TIMING_DVPVO_L 0x380B // �����ֱ�ߴ�,���ֽ�
#define OV5640_TIMING_Flip 0x3820    // Bit[2:1]���������Ƿ�ֱ��ת
#define OV5640_TIMING_Mirror 0x3821  // Bit[2:1]���������Ƿ�ˮƽ����

#define OV5640_AF_CMD_MAIN 0x3022  // AF ������
#define OV5640_AF_CMD_ACK 0x3023   // AF ����ȷ��
#define OV5640_AF_FW_STATUS 0x3029 // �Խ�״̬�Ĵ���

/*------------------------------------------------------------ �������� ------------------------------------------------*/

int8_t ov5640_init(void); // ��ʼSCCB��DCMI��DMA�Լ�����OV5640

void ov5640_send_continuous(uint32_t buf, uint32_t buf_size);               // ����DMA���䣬����ģʽ
void ov5640_send_snapshot(uint32_t buf, uint32_t buf_size);                 //  ����DMA���䣬����ģʽ������һ֡ͼ���ֹͣ
void ov5640_dcmi_suspend(void);                                             // ����DCMI��ֹͣ��������
void ov5640_dcmi_resume(void);                                              // �ָ�DCMI����ʼ��������
void ov5640_dcmi_stop(void);                                                // ��ֹDCMI��DMA����ֹͣDCMI���񣬽�ֹDCMI����
int8_t ov5640_dcmi_crop(uint16_t x, uint16_t dy, uint16_t sx, uint16_t sy); // �ü�����

void ov5640_reset(void);       //	ִ�������λ
uint16_t ov5640_read_id(void); // ��ȡ����ID
void ov5640_para_set(void);    // ����OV5640�������

void ov5640_set_pixformat(uint8_t pixformat);                 // ����ͼ�������ʽ
void ov5640_jpeg_quantization_scale(uint8_t scale);           // ����JPEG��ʽ��ѹ���ȼ�,ȡֵ 0x01~0x3F
int8_t ov5640_set_framesize(uint16_t width, uint16_t height); // ����ʵ�������ͼ���С
int8_t ov5640_set_horizontal_mirror(int8_t state);            // �������������ͼ���Ƿ����ˮƽ����
int8_t ov5640_set_vertical_flip(int8_t state);                //	�������������ͼ���Ƿ���д�ֱ��ת
void ov5640_set_brightness(int8_t brightness);                // ��������
void ov5640_set_contrast(int8_t Contrast);                    // ���öԱȶ�
void ov5640_set_effect(uint8_t mode);                         // ����������Ч����������Ƭ��ģʽ

int8_t ov5640_af_download_firmware(void); //	���Զ��Խ��̼�д��OV5640
int8_t ov5640_af_query_status(void);      //	�Խ�״̬��ѯ
void ov5640_af_trigger_constant(void);    // �Զ��Խ� ������ ����
void ov5640_af_trigger_single(void);      // �Զ��Խ� ������ ����
void ov5640_af_release(void);             // �ͷ�����ͷ�ص���ʼ���Խ�Ϊ����Զ����λ��



#endif //__OV5640_H__
