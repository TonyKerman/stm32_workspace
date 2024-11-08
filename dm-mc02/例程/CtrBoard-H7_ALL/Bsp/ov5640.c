#include "ov5640.h"
#include "ov5640_cfg.h"
#include "usart.h"
#include "cmsis_os.h"
#include "stdio.h"
#include "dcmi.h"

volatile uint8_t OV5640_FrameState = 0; // DCMI״̬��־��������֡�������ʱ���ᱻ HAL_DCMI_FrameEventCallback() �жϻص������� 1
volatile uint8_t OV5640_FPS;			// ֡��
uint8_t dcmi_frame_cnt = 0;
/**
******************************************************************************************************************************
* @brief:      	ov5640_delay
* @param:      	ms - ��ʱʱ��
* @details:    	����ʱ���������Ǻܾ�ȷ
* @note:       	Ϊ����ֲ�ļ����,�˴����������ʱ��ʵ����Ŀ�п����滻��RTOS����ʱ����HAL�����ʱ
******************************************************************************************************************************
**/
void ov5640_delay(uint32_t ms)
{
    HAL_Delay(ms);
//	osDelay(ms); // ��ʹ��HAL�����ʱ
}

/**
******************************************************************************************************************************
* @brief:      	ov5640_init
* @details:    	��ʼSCCB��DCMI��DMA�Լ�����OV5640
* @retval:     	OV5640_Success - ��ʼ���ɹ�
* @retval:     	OV5640_Error - ��ʼ��ʧ��
******************************************************************************************************************************
**/
int8_t ov5640_init(void)
{
	uint16_t id; // ��������洢����ID

	sccb_init(); // SCCB���ų�ʼ��

	ov5640_reset();		  // ִ�������λ
	id = ov5640_read_id(); // ��ȡ����ID

	if (id == 0x5640) // ����ƥ��
	{
		printf("OV5640 OK,ID:0x%X\r\n", id); // ƥ��ͨ��

		ov5640_para_set();								   // ���ø������
		ov5640_set_framesize(OV5640_Width, OV5640_Height); //	����OV5640�����ͼ���С
														   //		ov5640_dcmi_crop( Display_Width, Display_Height, OV5640_Width, OV5640_Height );	// �����ͼ��ü�����Ӧ��Ļ�Ĵ�С��JPGģʽ����Ҫ�ü�

		return OV5640_Success; // ���سɹ���־
	}
	else
	{
		printf("OV5640 ERROR!!!!!  ID:%X\r\n", id); // ��ȡID����
		return OV5640_Error;						// ���ش����־
	}
}

/**
******************************************************************************************************************************
* @brief:      	ov5640_send_continuous
* @param:      	buf - DMA��Ҫ����ĵ�ַ�������ڴ洢����ͷ���ݵĴ洢����ַ
* @param:      	buf_size - ��������ݴ�С��32λ��
* @retval:     	void
* @details:    	����DMA���䣬����ģʽ
* @note:       	1. ��������ģʽ֮�󣬻�һֱ���д��䣬���ǹ������ֹͣDCMI
*               2. OV5640ʹ��RGB565ģʽʱ��1�����ص���Ҫ2���ֽ����洢
*               3. ��ΪDMA���ô�������Ϊ32λ������ buf_size ʱ����Ҫ����4�����磺
*                   Ҫ��ȡ 240*240�ֱ��� ��ͼ����Ҫ���� 240*240*2 = 115200 �ֽڵ����ݣ�
*                   �� buf_size = 115200 / 4 = 28800 ��
******************************************************************************************************************************
**/
void ov5640_send_continuous(uint32_t buf, uint32_t buf_size)
{
	hdma_dcmi.Init.Mode = DMA_CIRCULAR; // ѭ��ģʽ

	HAL_DMA_Init(&hdma_dcmi); // ����DMA

	// ʹ��DCMI�ɼ�����,�����ɼ�ģʽ
	HAL_DCMI_Start_DMA(&hdcmi, DCMI_MODE_CONTINUOUS, (uint32_t)buf, buf_size);
}

/**
******************************************************************************************************************************
* @brief:      	ov5640_send_snapshot
* @param:      	buf - DMA��Ҫ����ĵ�ַ�������ڴ洢����ͷ���ݵĴ洢����ַ
* @param:      	buf_size - ��������ݴ�С��32λ��
* @retval:     	void
* @details:    	����DMA���䣬����ģʽ������һ֡ͼ���ֹͣ
* @note:       	1. ����ģʽ��ֻ����һ֡������
*               2. OV5640ʹ��RGB565ģʽʱ��1�����ص���Ҫ2���ֽ����洢
*               3. ��ΪDMA���ô�������Ϊ32λ������ buf_size ʱ����Ҫ����4�����磺
*                   Ҫ��ȡ 240*240�ֱ��� ��ͼ����Ҫ���� 240*240*2 = 115200 �ֽڵ����ݣ�
*                   �� buf_size = 115200 / 4 = 28800 ��
*               4. ʹ�ø�ģʽ�������֮��DCMI�ᱻ�����ٴ����ô���֮ǰ����Ҫ���� ov5640_dcmi_resume() �ָ�DCMI
******************************************************************************************************************************
**/
void ov5640_send_snapshot(uint32_t buf, uint32_t buf_size)
{
	hdma_dcmi.Init.Mode = DMA_NORMAL; // ����ģʽ

	HAL_DMA_Init(&hdma_dcmi); // ����DMA

	HAL_DCMI_Start_DMA(&hdcmi, DCMI_MODE_SNAPSHOT, (uint32_t)buf, buf_size);
}
/**
******************************************************************************************************************************
* @brief:      	ov5640_dcmi_suspend
* @param:      	void
* @retval:     	void
* @details:    	����DCMI��ֹͣ��������
* @note:       	1. ��������ģʽ֮���ٵ��øú�������ֹͣ����DCMI������
*               2. ���Ե��� ov5640_dcmi_resume() �ָ�DCMI
*               3. ��Ҫע��ģ�����DCMI�ڼ䣬DMA��û��ֹͣ������
******************************************************************************************************************************
**/
void ov5640_dcmi_suspend(void)
{
	HAL_DCMI_Suspend(&hdcmi); // ����DCMI
}
/**
******************************************************************************************************************************
* @brief:      	ov5640_dcmi_resume
* @param:      	void
* @retval:     	void
* @details:    	�ָ�DCMI����ʼ��������
* @note:       	1. ��DCMI������ʱ�����Ե��øú����ָ�
*               2. ʹ�� ov5640_send_snapshot() ����ģʽ���������֮��DCMIҲ�ᱻ�����ٴ����ô���֮ǰ��
*                  ��Ҫ���ñ������ָ�DCMI����
******************************************************************************************************************************
**/
void ov5640_dcmi_resume(void)
{
	(&hdcmi)->State = HAL_DCMI_STATE_BUSY;	   // ���DCMI��־
	(&hdcmi)->Instance->CR |= DCMI_CR_CAPTURE; // ����DCMI����
}
/**
******************************************************************************************************************************
* @brief:      	ov5640_dcmi_stop
* @param:      	void
* @retval:     	void
* @details:    	��ֹDCMI��DMA����ֹͣDCMI���񣬽�ֹDCMI����
******************************************************************************************************************************
**/void ov5640_dcmi_stop(void)
{
	HAL_DCMI_Stop(&hdcmi);
}

/**
******************************************************************************************************************************
* @brief:      	ov5640_dcmi_crop
* @param:       dx - ��ʾ���Ŀ��
* @param:       dy - ��ʾ���ĸ߶�
* @param:       sx - ����ͷ���������ͼ��Ŀ��
* @param:       sy - ����ͷ���������ͼ��ĸ߶�
* @retval:     	OV5640_Success��ʾ�ɹ�
* @retval:     	OV5640_Error��ʾʧ��
* @details:    	ʹ��DCMI�Ĳü����ܣ��������������ͼ��ü�����Ӧ��Ļ�Ĵ�С
* @note:       	1. ��Ϊ����ͷ����Ļ��������һ��ƥ����ʾ����������Ҫ�ü�
*               2. ����ͷ�������������� ov5640_para_set()���ò������������ջ����С�� ov5640_set_framesize()����
*               3. DCMI��ˮƽ��Ч����Ҳ����Ҫ�ܱ�4������
*               4. ���������ˮƽ�ʹ�ֱƫ�ƣ������û�����вü�
******************************************************************************************************************************
**/
int8_t ov5640_dcmi_crop(uint16_t dx, uint16_t dy, uint16_t sx, uint16_t sy)
{
	uint16_t x_off, y_off; // ˮƽ�ʹ�ֱƫ�ƣ���ֱ�������������ˮƽ�����������ʱ������PCLK��������
	uint16_t dcmi_capcnt;				   // ˮƽ��Ч���أ������������ʱ������PCLK��������
	uint16_t dcmi_vline;				   // ��ֱ��Ч����

	if ((dx >= sx) || (dy >= sy))
	{
		//		printf("ʵ����ʾ�ĳߴ���ڻ��������ͷ����ĳߴ磬�˳�DCMI�ü�\r\n");
		return OV5640_Error; // ���ʵ����ʾ�ĳߴ���ڻ��������ͷ����ĳߴ磬���˳���ǰ�����������вü�
	}
	// ������ΪRGB565��ʽʱ��ˮƽƫ�ƣ�������������������ɫ�ʲ���ȷ��
	// ��Ϊһ����Ч������2���ֽڣ���Ҫ2��PCLK���ڣ����Ա��������λ��ʼ����Ȼ���ݻ���ң�
	// ��Ҫע����ǣ��Ĵ���ֵ�Ǵ�0��ʼ�����	��
	x_off = sx - dx; // ʵ�ʼ������Ϊ��sx - LCD_XSize��/2*2

	// ���㴹ֱƫ�ƣ������û�����вü�����ֵ�������������
	y_off = (sy - dy) / 2 - 1; // �Ĵ���ֵ�Ǵ�0��ʼ����ģ�����Ҫ-1

	// ��Ϊһ����Ч������2���ֽڣ���Ҫ2��PCLK���ڣ�����Ҫ��2
	// ���յõ��ļĴ���ֵ������Ҫ�ܱ�4������
	dcmi_capcnt = dx * 2 - 1; // �Ĵ���ֵ�Ǵ�0��ʼ����ģ�����Ҫ-1

	dcmi_vline = dy - 1; // ��ֱ��Ч����

	//	printf("%d  %d  %d  %d\r\n",x_off,y_off,dcmi_capcnt,dcmi_vline);

	HAL_DCMI_ConfigCrop(&hdcmi, x_off, y_off, dcmi_capcnt, dcmi_vline); // ���òü�����
	HAL_DCMI_EnableCrop(&hdcmi);														// ʹ�ܲü�

	return OV5640_Success;
}
/**
******************************************************************************************************************************
* @brief:      	ov5640_reset
* @param:      	void
* @retval:     	void
* @details:    	ִ�������λ
* @note:       	�ڼ��ж����ʱ����
******************************************************************************************************************************
**/
void ov5640_reset(void)
{
	OV5640_RST(0);

	ov5640_delay(100); // �ȴ�ģ���ϵ��ȶ�������5ms��Ȼ������PWDN

	OV5640_PWDN_OFF; // PWDN ��������͵�ƽ������������ģʽ������ͷ������������ʱ����ͷģ��İ�ɫLED�����

	ov5640_delay(100);
	OV5640_RST(1); /* ������λ */
				   // ��λ���֮��Ҫ>=20ms����ִ��SCCB����
	ov5640_delay(100);

	sccb_write_reg_16bit(0x3103, 0x11); // �����ֲ�Ľ��飬��λ֮ǰ��ֱ�ӽ�ʱ���������ŵ�ʱ����Ϊ��ʱ��
	sccb_write_reg_16bit(0x3008, 0x82); // ִ��һ����λ
	ov5640_delay(5);					// ��ʱ5ms
}

/**
******************************************************************************************************************************
* @brief:      	ov5640_read_id
* @param:      	void
* @retval:     	uint16_t - OV5640������ID
* @details:    	��ȡ OV5640 ������ID
******************************************************************************************************************************
**/
uint16_t ov5640_read_id(void)
{
	uint8_t id_h, id_l; // ID����

	id_h = sccb_read_reg_16bit(OV5640_ChipID_H); // ��ȡID���ֽ�
	id_l = sccb_read_reg_16bit(OV5640_ChipID_L); // ��ȡID���ֽ�

	return (id_h << 8) | id_l; // ��������������ID
}
/**
******************************************************************************************************************************
* @brief:      	ov5640_para_set
* @param:      	void
* @retval:     	void
* @details:    	���� OV5640 �����Ĵ�������
******************************************************************************************************************************
**/
void ov5640_para_set(void)
{
	uint32_t i; // ��������

	//	uint8_t	read_reg; // ��ȡ���ã����ڵ���

	for (i = 0; i < (sizeof(OV5640_INIT_Config) / 4); i++)
	{
		sccb_write_reg_16bit(OV5640_INIT_Config[i][0], OV5640_INIT_Config[i][1]); // д������
		ov5640_delay(1);
		//		read_reg = sccb_read_reg_16bit(OV5640_INIT_Config[i][0]);	// ��ȡ���ã����ڵ���

		//		if(OV5640_INIT_Config[i][1] != read_reg )	// ���ò��ɹ�
		//		{
		//			printf("����λ�ã�%d\r\n",i);	// ��ӡ����λ��
		//			printf("0x%x-0x%x-0x%x\r\n",OV5640_INIT_Config[i][0],OV5640_INIT_Config[i][1],read_reg);
		//		}
	}
}
/**
******************************************************************************************************************************
* @brief:      	ov5640_set_pixformat
* @param:      	pixformat - ���ظ�ʽ����ѡ�� Pixformat_RGB565��Pixformat_GRAY��Pixformat_JPEG
* @retval:     	void
* @details:    	������������ظ�ʽ
******************************************************************************************************************************
**/
void ov5640_set_pixformat(uint8_t pixformat)
{
	uint8_t reg; // �Ĵ�����ֵ

	if (pixformat == Pixformat_JPEG)
	{
		sccb_write_reg_16bit(OV5640_FORMAT_CONTROL, 0x30);	   //	�������ݽӿ�����ĸ�ʽ
		sccb_write_reg_16bit(OV5640_FORMAT_CONTROL_MUX, 0x00); // ����ISP�ĸ�ʽ

		sccb_write_reg_16bit(OV5640_JPEG_MODE_SELECT, 0x02); // JPEG ģʽ2

		sccb_write_reg_16bit(OV5640_JPEG_VFIFO_CTRL00, 0xA0); // JPEG �̶�����

		sccb_write_reg_16bit(OV5640_JPEG_VFIFO_HSIZE_H, OV5640_Width >> 8);		 // JPEG���ˮƽ�ߴ�,���ֽ�
		sccb_write_reg_16bit(OV5640_JPEG_VFIFO_HSIZE_L, (uint8_t)OV5640_Width);	 // JPEG���ˮƽ�ߴ�,���ֽ�
		sccb_write_reg_16bit(OV5640_JPEG_VFIFO_VSIZE_H, OV5640_Height >> 8);	 // JPEG�����ֱ�ߴ�,���ֽ�
		sccb_write_reg_16bit(OV5640_JPEG_VFIFO_VSIZE_L, (uint8_t)OV5640_Height); // JPEG�����ֱ�ߴ�,���ֽ�
	}
	else if (pixformat == Pixformat_GRAY)
	{
		sccb_write_reg_16bit(OV5640_FORMAT_CONTROL, 0x10);	   //	�������ݽӿ�����ĸ�ʽ
		sccb_write_reg_16bit(OV5640_FORMAT_CONTROL_MUX, 0x00); // ����ISP�ĸ�ʽ
	}
	else // RGB565
	{
		sccb_write_reg_16bit(OV5640_FORMAT_CONTROL, 0x60);	   // �˴�����ΪRGB565��ʽ������Ϊ G[2:0]B[4:0], R[4:0]G[5:3]
		sccb_write_reg_16bit(OV5640_FORMAT_CONTROL_MUX, 0x01); // ����ISP�ĸ�ʽ
	}

	reg = sccb_read_reg_16bit(0x3821); // ��ȡ�Ĵ���ֵ��Bit[5]�����Ƿ�ʹ��JPEGģʽ
	sccb_write_reg_16bit(0x3821, (reg & 0xDF) | ((pixformat == Pixformat_JPEG) ? 0x20 : 0x00));

	reg = sccb_read_reg_16bit(0x3002); // ��ȡ�Ĵ���ֵ��Bit[7]��Bit[4]��Bit[2]ʹ�� VFIFO��JFIFO��JPG
	sccb_write_reg_16bit(0x3002, (reg & 0xE3) | ((pixformat == Pixformat_JPEG) ? 0x00 : 0x1C));

	reg = sccb_read_reg_16bit(0x3006); // ��ȡ�Ĵ���ֵ��Bit[5]��Bit[3] �����Ƿ�ʹ��JPGʱ��
	sccb_write_reg_16bit(0x3006, (reg & 0xD7) | ((pixformat == Pixformat_JPEG) ? 0x28 : 0x00));
}
/**
******************************************************************************************************************************
* @brief:      	ov5640_jpeg_quantization_scale
* @param:      	scale - ѹ���ȼ���ȡֵ 0x01~0x3F
* @retval:     	void
* @details:    	��ֵԽ��ѹ����Խ�������õ���ͼƬռ�ÿռ��ԽС������Ӧ�Ļ��ʻ���ͻ������е���
******************************************************************************************************************************
**/
void ov5640_jpeg_quantization_scale(uint8_t scale)
{
	sccb_write_reg_16bit(0x4407, scale); // JPEG ѹ���ȼ�
}
/**
******************************************************************************************************************************
* @brief:      	ov5640_set_framesize
* @param:      	width - ʵ�����ͼ��ĳ���
* @param:      	height - ʵ�����ͼ��Ŀ��
* @retval:     	int8_t - OV5640_Success: �ɹ�, OV5640_Error: ʧ��
* @details:    	����ʵ�������ͼ���С�����ź�
******************************************************************************************************************************
**/
int8_t ov5640_set_framesize(uint16_t width, uint16_t height)
{
	// OV5640�ĺܶ��������Ҫ�������ֶ�Ӧ group ������
	sccb_write_reg_16bit(OV5640_GroupAccess, 0X03); // ��ʼ group 3 ������

	sccb_write_reg_16bit(OV5640_TIMING_DVPHO_H, width >> 8); // DVPHO���������ˮƽ�ߴ�
	sccb_write_reg_16bit(OV5640_TIMING_DVPHO_L, width & 0xff);
	sccb_write_reg_16bit(OV5640_TIMING_DVPVO_H, height >> 8); // DVPVO�����������ֱ�ߴ�
	sccb_write_reg_16bit(OV5640_TIMING_DVPVO_L, height & 0xff);

	sccb_write_reg_16bit(OV5640_GroupAccess, 0X13); // ��������
	sccb_write_reg_16bit(OV5640_GroupAccess, 0Xa3); // ��������

	return OV5640_Success;
}

/**
******************************************************************************************************************************
* @brief:      	ov5640_set_horizontal_mirror
* @param:      	state - ��1ʱ��ͼ���ˮƽ������0ʱ�ָ�����
* @retval:     	int8_t - OV5640_Success: �ɹ�, OV5640_Error: ʧ��
* @details:    	�������������ͼ���Ƿ����ˮƽ����
******************************************************************************************************************************
**/
int8_t ov5640_set_horizontal_mirror(int8_t state)
{
	uint8_t reg; // �Ĵ�����ֵ

	reg = sccb_read_reg_16bit(OV5640_TIMING_Mirror); // ��ȡ�Ĵ���ֵ

	// Bit[2:1]���������Ƿ�ˮƽ����
	if (state == OV5640_Enable) // ���ʹ�ܾ���
	{
		reg |= 0X06;
	}
	else // ȡ������
	{
		reg &= 0xF9;
	}
	return sccb_write_reg_16bit(OV5640_TIMING_Mirror, reg); // д��Ĵ���
}
/**
******************************************************************************************************************************
* @brief:      	ov5640_set_vertical_flip
* @param:      	state - ��1ʱ��ͼ��ᴹֱ��ת����0ʱ�ָ�����
* @retval:     	int8_t - OV5640_Success: �ɹ�, OV5640_Error: ʧ��
* @details:    	�������������ͼ���Ƿ���д�ֱ��ת
******************************************************************************************************************************
**/
int8_t ov5640_set_vertical_flip(int8_t state)
{
	uint8_t reg; // �Ĵ�����ֵ

	reg = sccb_read_reg_16bit(OV5640_TIMING_Flip); // ��ȡ�Ĵ���ֵ

	// Bit[2:1]���������Ƿ�ֱ��ת
	if (state == OV5640_Enable)
	{
		reg |= 0X06;
	}
	else // ȡ����ת
	{
		reg &= 0xF9;
	}
	return sccb_write_reg_16bit(OV5640_TIMING_Flip, reg); // д��Ĵ���
}
/**
******************************************************************************************************************************
* @brief:      	ov5640_set_brightness
* @param:      	brightness - ���ȣ�������Ϊ9���ȼ���4��3��2��1��0��-1��-2��-3��-4������Խ������Խ��
* @retval:     	void
* @details:    	����ͼ�����ȣ�ֱ��ʹ��OV5640�ֲ�����Ĵ���
******************************************************************************************************************************
**/
void ov5640_set_brightness(int8_t brightness)
{
	brightness = brightness + 4;
	sccb_write_reg_16bit(OV5640_GroupAccess, 0X03); // ��ʼ group 3 ������

	sccb_write_reg_16bit(0x5587, OV5640_brightness_Config[brightness][0]);
	sccb_write_reg_16bit(0x5588, OV5640_brightness_Config[brightness][1]);

	sccb_write_reg_16bit(OV5640_GroupAccess, 0X13); // ��������
	sccb_write_reg_16bit(OV5640_GroupAccess, 0Xa3); // ��������
}
/**
******************************************************************************************************************************
* @brief:      	ov5640_set_contrast
* @param:      	Contrast - �Աȶȣ�������Ϊ7���ȼ���3��2��1��0��-1��-2��-3
* @retval:     	void
* @details:    	����ͼ��Աȶȣ�ֱ��ʹ��OV5640�ֲ�����Ĵ���
******************************************************************************************************************************
**/
void ov5640_set_contrast(int8_t Contrast)
{
	Contrast = Contrast + 3;
	sccb_write_reg_16bit(OV5640_GroupAccess, 0X03); // ��ʼ group 3 ������

	sccb_write_reg_16bit(0x5586, OV5640_Contrast_Config[Contrast][0]);
	sccb_write_reg_16bit(0x5585, OV5640_Contrast_Config[Contrast][1]);

	sccb_write_reg_16bit(OV5640_GroupAccess, 0X13); // ��������
	sccb_write_reg_16bit(OV5640_GroupAccess, 0Xa3); // ��������
}
/**
******************************************************************************************************************************
* @brief:      	ov5640_set_effect
* @param:      	mode - ��Чģʽ����ѡ����� OV5640_Effect_Normal��OV5640_Effect_Negative��
*                          OV5640_Effect_BW��OV5640_Effect_Solarize
* @retval:     	void
* @details:    	��������OV5640����Ч����������Ƭ���ڰס�����Ƭ����ģʽ
*               ������о���4��ģʽ��������Чģʽ���Բο��ֲ��������
******************************************************************************************************************************
**/
void ov5640_set_effect(uint8_t mode)
{
	sccb_write_reg_16bit(OV5640_GroupAccess, 0X03); // ��ʼ group 3 ������

	sccb_write_reg_16bit(0x5580, OV5640_Effect_Config[mode][0]);
	sccb_write_reg_16bit(0x5583, OV5640_Effect_Config[mode][1]);
	sccb_write_reg_16bit(0x5584, OV5640_Effect_Config[mode][2]);
	sccb_write_reg_16bit(0x5003, OV5640_Effect_Config[mode][3]);

	sccb_write_reg_16bit(OV5640_GroupAccess, 0X13); // ��������
	sccb_write_reg_16bit(OV5640_GroupAccess, 0Xa3); // ��������
}
/**
******************************************************************************************************************************
* @brief:      	ov5640_af_download_firmware
* @param:      	void
* @retval:     	int8_t - OV5640_Success if firmware download successful, OV5640_Error otherwise
* @details:    	���Զ��Խ��̼�д��OV5640
*               ��ΪOV5640Ƭ��û��flash�����ܱ���̼������ÿ���ϵ綼Ҫд��һ��
******************************************************************************************************************************
**/
int8_t ov5640_af_download_firmware(void)
{
	uint8_t af_reg = 0;			   // �Խ�״̬
	uint16_t i = 0;					   // ��������
	uint16_t OV5640_MCU_Addr = 0x8000; // OV5640 MCU �洢������ʼ��ַΪ 0x8000����СΪ4KB

	sccb_write_reg_16bit(0x3000, 0x20); // Bit[5]����λMCU��д��̼�֮ǰ����Ҫִ�д˲���
										// ��ʼд��̼�������д�룬���д���ٶ�
	sccb_write_buf_16bit(OV5640_MCU_Addr, (uint8_t *)OV5640_AF_Firmware, sizeof(OV5640_AF_Firmware));
	sccb_write_reg_16bit(0x3000, 0x00); // Bit[5]��д����ϣ�д0ʹ��MCU

	// д��̼�֮�󣬻��и���ʼ���Ĺ��̣���˳��Զ�ȡ100��״̬������״̬�����ж�
	for (i = 0; i < 100; i++)
	{
		af_reg = sccb_read_reg_16bit(OV5640_AF_FW_STATUS); // ��ȡ״̬�Ĵ���
		if (af_reg == 0x7E)
		{
			printf("AF firmware initiating>>>\r\n");
		}
		if (af_reg == 0x70) // �ͷ�����ͷ�ص���ʼ���Խ�Ϊ����Զ����λ�ã���ζ�Ź̼�д��ɹ�
		{
			printf("AF firmware write OK��\r\n");
			return OV5640_Success;
		}
	}
	// ����100�ζ�ȡ֮�󣬻���û�ж���0x70״̬��˵���̼�ûд��ɹ�
	printf("�Զ��Խ��̼�д��ʧ�ܣ�����error����\r\n");
	return OV5640_Error;
}
/**
******************************************************************************************************************************
* @brief:      	ov5640_af_query_status
* @param:      	void
* @retval:     	int8_t - OV5640_AF_End if autofocus is completed, OV5640_AF_Focusing if autofocus is in progress
* @details:    	�Խ�״̬��ѯ
*               1. �Խ����̴�Ż����500��ms
*               2. �Խ�û���ʱ���ɼ����ĵ�ͼ���ڽ��㣬��ǳ�ģ��
******************************************************************************************************************************
**/
int8_t ov5640_af_query_status(void)
{
	uint8_t af_reg = 0; // �Խ�״̬

	af_reg = sccb_read_reg_16bit(OV5640_AF_FW_STATUS); // ��ȡ״̬�Ĵ���
	printf("af_reg:0x%x\r\n", af_reg);

	// ���ζԽ�ģʽ	�£����� 0x10�������Խ�ģʽ�£�����0x20
	if ((af_reg == 0x10) || (af_reg == 0x20))
		return OV5640_AF_End; // ���� �Խ����� ��־
	else
		return OV5640_AF_Focusing; // ���� ���ڶԽ� ��־
}
/**
******************************************************************************************************************************
* @brief:      	ov5640_af_trigger_constant
* @param:      	void
* @retval:     	void
* @details:    	���������Խ�����OV5640��⵽��ǰ���治�ڽ���ʱ����һֱ�����Խ��������û���Ԥ
*               1.���Ե��� ov5640_af_query_status() ������ѯ�Խ�״̬
*               2.���Ե��� ov5640_af_release() �˳������Խ�ģʽ
*               3.�Խ����̴�Ż����500��ms
*               4.��ʱ��������̫����OV5640�ᷴ���Ľ��жԽ����û��ɸ���ʵ������л������ζԽ�ģʽ
******************************************************************************************************************************
**/
void ov5640_af_trigger_constant(void)
{
	sccb_write_reg_16bit(0x3022, 0x04); //	�����Խ�
}
/**
******************************************************************************************************************************
* @brief:      	ov5640_af_trigger_single
* @param:      	void
* @retval:     	void
* @details:    	����һ���Զ��Խ�
*               �Խ����̴�Ż����500��ms���û����Ե��� ov5640_af_query_status() ������ѯ�Խ�״̬
******************************************************************************************************************************
**/
void ov5640_af_trigger_single(void)
{
	sccb_write_reg_16bit(OV5640_AF_CMD_MAIN, 0x03); // ����һ���Զ��Խ�
}
/**
******************************************************************************************************************************
* @brief:      	ov5640_af_trigger_single
* @param:      	void
* @retval:     	void
* @details:    	����һ���Զ��Խ�
*               �Խ����̴�Ż����500��ms���û����Ե��� ov5640_af_query_status() ������ѯ�Խ�״̬
******************************************************************************************************************************
**/
void ov5640_af_release(void)
{
	sccb_write_reg_16bit(OV5640_AF_CMD_MAIN, 0x08); // �Խ��ͷ�ָ��
}
/**
******************************************************************************************************************************
* @brief:      	HAL_DCMI_FrameEventCallback
* @param[in]  	hdcmi: ָ�� DCMI_HandleTypeDef �ṹ���ָ��
* @retval:     	void
* @details:    	֡�ص�������ÿ����һ֡���ݣ��������жϷ�����
******************************************************************************************************************************
**/
void HAL_DCMI_FrameEventCallback(DCMI_HandleTypeDef *hdcmi)
{
	static uint32_t dcmi_tick = 0;		 // ���ڱ��浱ǰ��ʱ�����ֵ

	if (HAL_GetTick() - dcmi_tick >= 1000) // ÿ�� 1s ����һ��֡��
	{
		dcmi_tick = HAL_GetTick(); // ���»�ȡ��ǰʱ�����ֵ

		OV5640_FPS = dcmi_frame_cnt; // ���fps

		dcmi_frame_cnt = 0; // ������0
	}
	dcmi_frame_cnt++; // ÿ����һ���жϣ�ÿ�δ�����һ֡���ݣ�������ֵ+1

	OV5640_FrameState = 1; // ������ɱ�־λ��1
}
/**
******************************************************************************************************************************
* @brief:      	HAL_DCMI_ErrorCallback
* @param[in]  	hdcmi: ָ�� DCMI_HandleTypeDef �ṹ���ָ��
* @retval:     	void
* @details:    	����ص�����
******************************************************************************************************************************
**/
void HAL_DCMI_ErrorCallback(DCMI_HandleTypeDef *hdcmi)
{
	// if( HAL_DCMI_GetError(hdcmi) == HAL_DCMI_ERROR_OVR)
	// {
	//    printf("FIFO������󣡣���\r\n");
	// }
	//   printf("error:0x%x!!!!\r\n",HAL_DCMI_GetError(hdcmi));
}

