#include "sccb.h"

/**
************************************************************************
* @brief:      	sccb_init
* @param:       void
* @retval:     	void
* @details:    	��ʼ��IIC��GPIO�ڣ��������
* @note:       	����IICͨ���ٶȲ��ߣ������IO���ٶ�����Ϊ2M����
************************************************************************
**/
void sccb_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	SCCB_SCL_CLK_ENABLE; // ��ʼ��IO��ʱ��
	SCCB_SDA_CLK_ENABLE;

	GPIO_InitStruct.Pin = SCCB_SCL_PIN;			 // SCL����
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;	 // ��©���
	GPIO_InitStruct.Pull = GPIO_NOPULL;			 // ����������
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; // �ٶȵȼ�
	HAL_GPIO_Init(SCCB_SCL_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = SCCB_SDA_PIN; // SDA����
	HAL_GPIO_Init(SCCB_SDA_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // �������
	GPIO_InitStruct.Pull = GPIO_PULLUP;			// ����

	HAL_GPIO_WritePin(SCCB_SCL_PORT, SCCB_SCL_PIN, GPIO_PIN_SET); // SCL����ߵ�ƽ
	HAL_GPIO_WritePin(SCCB_SDA_PORT, SCCB_SDA_PIN, GPIO_PIN_SET); // SDA����ߵ�ƽ
}
/**
************************************************************************
* @brief:      	sccb_delay
* @param:       a - ��ʱʱ��
* @retval:     	void
* @details:    	����ʱ����
* @note:       	Ϊ����ֲ�ļ�����Ҷ���ʱ����Ҫ�󲻸ߣ����Բ���Ҫʹ�ö�ʱ������ʱ
************************************************************************
**/
void sccb_delay(void)
{
	uint32_t a = 15;
	volatile uint16_t i;
	while (a--)
		for (i = 0; i < 3; i++);
}
/**
************************************************************************
* @brief:      	sccb_start
* @param:       void
* @retval:     	void
* @details:    	IIC��ʼ�ź�
* @note:       	��SCL���ڸߵ�ƽ�ڼ䣬SDA�ɸߵ�������Ϊ��ʼ�ź�
************************************************************************
**/
void sccb_start(void)
{
	SCCB_SDA(1);
	SCCB_SCL(1);
	sccb_delay();
	SCCB_SDA(0);
	sccb_delay();
	SCCB_SCL(0);
	sccb_delay();
}
/**
************************************************************************
* @brief:      	sccb_stop
* @param:       ��
* @retval:     	��
* @details:    	IICֹͣ�ź�
* @note:       	��SCL���ڸߵ�ƽ�ڼ䣬SDA�ɵ͵�������Ϊ��ʼ�ź�
************************************************************************
**/
void sccb_stop(void)
{
	SCCB_SCL(0);
	sccb_delay();
	SCCB_SDA(0);
	sccb_delay();

	SCCB_SCL(1);
	sccb_delay();
	SCCB_SDA(1);
	sccb_delay();
}

/**
************************************************************************
* @brief:      	sccb_ack
* @param:       void
* @retval:     	void
* @details:    	IICӦ���ź�
* @note:       	��SCLΪ�ߵ�ƽ�ڼ䣬SDA�������Ϊ�͵�ƽ������Ӧ���ź�
************************************************************************
**/
void sccb_ack(void)
{
	SCCB_SCL(0);
	sccb_delay();
	SCCB_SDA(0);
	sccb_delay();
	SCCB_SCL(1);
	sccb_delay();
	SCCB_SCL(0); // SCL�����ʱ��SDAӦ�������ߣ��ͷ�����
	SCCB_SDA(1);
	sccb_delay();
}
/**
************************************************************************
* @brief:      	sccb_nack
* @param:       void
* @retval:     	void
* @details:    	IIC��Ӧ���ź�
* @note:       	��SCLΪ�ߵ�ƽ�ڼ䣬��SDA����Ϊ�ߵ�ƽ��������Ӧ���ź�
************************************************************************
**/
void sccb_nack(void)
{
	SCCB_SCL(0);
	sccb_delay();
	SCCB_SDA(1);
	sccb_delay();
	SCCB_SCL(1);
	sccb_delay();
	SCCB_SCL(0);
	sccb_delay();
}
/**
************************************************************************
* @brief:      	sccb_wait_ack
* @param:       void
* @retval:     	void
* @details:    	�ȴ������豸����Ӧ���ź�
* @note:       	��SCLΪ�ߵ�ƽ�ڼ䣬����⵽SDA����Ϊ�͵�ƽ��������豸��Ӧ����
************************************************************************
**/
uint8_t sccb_wait_ack(void)
{
	SCCB_SDA(1);
	sccb_delay();
	SCCB_SCL(1);
	sccb_delay();

	if (SCCB_READ_SDA != 0) // �ж��豸�Ƿ���������Ӧ
	{
		SCCB_SCL(0);
		sccb_delay();
		return ACK_ERR; // ��Ӧ��
	}
	else
	{
		SCCB_SCL(0);
		sccb_delay();
		return ACK_OK; // Ӧ������
	}
}
/**
************************************************************************
* @brief:      	sccb_send_byte
* @param:      	data - Ҫд���8λ����
* @retval:     	ACK_OK  - �豸��Ӧ����
*              	ACK_ERR - �豸��Ӧ����
* @details:    	дһ�ֽ�����
* @note:       	��λ��ǰ
************************************************************************
**/
uint8_t sccb_send_byte(uint8_t data)
{
	uint8_t i;

	for (i = 0; i < 8; i++)
	{
		SCCB_SDA(data & 0x80);

		sccb_delay();
		SCCB_SCL(1);
		sccb_delay();
		SCCB_SCL(0);
		if (i == 7)
		{
			SCCB_SDA(1);
		}
		data <<= 1;
	}

	return sccb_wait_ack(); // �ȴ��豸��Ӧ
}
/**
************************************************************************
* @brief:      	sccb_read_byte
* @param:      	ack_mode - ��Ӧģʽ������1�򷢳�Ӧ���źţ�����0������Ӧ���ź�
* @retval:     	ACK_OK  - �豸��Ӧ����
*              	ACK_ERR - �豸��Ӧ����
* @details:    	��һ�ֽ�����
* @note:       	1.��λ��ǰ
*              	2.Ӧ�������������һ�ֽ�����ʱ���ͷ�Ӧ���ź�
************************************************************************
**/
uint8_t sccb_read_byte(uint8_t ack_mode)
{
	uint8_t data = 0;
	uint8_t i = 0;

	for (i = 0; i < 8; i++)
	{
		data <<= 1;

		SCCB_SCL(1);
		sccb_delay();
		data |= (SCCB_READ_SDA & 0x01);
		SCCB_SCL(0);
		sccb_delay();
	}

	if (ack_mode == 1) //	Ӧ���ź�
		sccb_ack();
	else
		sccb_nack(); // ��Ӧ���ź�

	return data;
}
/**
******************************************************************************************************************************
* @brief:      	sccb_write_handle
* @param:      	addr - Ҫ���в����ļĴ���(8λ��ַ)
* @retval:     	SUCCESS - �����ɹ���ERROR	  - ����ʧ��
* @details:    	��ָ���ļĴ���(8λ��ַ)ִ��д������OV2640�õ�
******************************************************************************************************************************
**/
uint8_t sccb_write_handle(uint8_t addr)
{
	uint8_t status; // ״̬��־λ

	sccb_start();										 // ����IICͨ��
	if (sccb_send_byte(OV2640_DEVICE_ADDRESS) == ACK_OK) // д����ָ��
	{
		if (sccb_send_byte((uint8_t)(addr)) != ACK_OK)
		{
			status = ERROR; // ����ʧ��
		}
	}
	status = SUCCESS; // �����ɹ�
	return status;
}
/**
******************************************************************************************************************************
* @brief:      	sccb_write_reg
* @param:      	addr - Ҫд��ļĴ���(8λ��ַ)��
* @param:      	value - Ҫд�������
* @retval:     	SUCCESS - �����ɹ��� ERROR	  - ����ʧ��
* @details:    	��ָ���ļĴ���(8λ��ַ)дһ�ֽ����ݣ�OV2640�õ�
******************************************************************************************************************************
**/
uint8_t sccb_write_reg(uint8_t addr, uint8_t value)
{
	uint8_t status;

	sccb_start(); // ����IICͨѶ

	if (sccb_write_handle(addr) == SUCCESS) // д��Ҫ�����ļĴ���
	{
		if (sccb_send_byte(value) != ACK_OK) // д����
		{
			status = ERROR;
		}
	}
	sccb_stop(); // ֹͣͨѶ

	status = SUCCESS; // д��ɹ�
	return status;
}
/**
******************************************************************************************************************************
* @brief:      	sccb_read_reg
* @param:      	addr - Ҫ��ȡ�ļĴ���(8λ��ַ)
* @retval:     	����������
* @details:    	��ָ���ļĴ���(8λ��ַ)��ȡһ�ֽ����ݣ�OV2640�õ�
******************************************************************************************************************************
**/
uint8_t sccb_read_reg(uint8_t addr)
{
	uint8_t value = 0;

	sccb_start(); // ����IICͨ��

	if (sccb_write_handle(addr) == SUCCESS) // д��Ҫ�����ļĴ���
	{
		sccb_stop();  // ֹͣIICͨ��
		sccb_start(); // ��������IICͨѶ

		if (sccb_send_byte(OV2640_DEVICE_ADDRESS | 0X01) == ACK_OK) // ���Ͷ�����
		{
			value = sccb_read_byte(0); // �������һ������ʱ���� ��Ӧ���ź�
		}
		sccb_stop(); // ֹͣIICͨ��
	}

	return value;
}
/**
******************************************************************************************************************************
* @brief:      	sccb_write_handle_16Bit
* @param:      	addr - Ҫ���в����ļĴ���(16λ��ַ)
* @retval:     	SUCCESS - �����ɹ�
* @retval:			ERROR - ����ʧ��
* @details:    	��ָ���ļĴ���(16λ��ַ)ִ��д������OV5640�õ�
******************************************************************************************************************************
**/
uint8_t sccb_write_handle_16Bit(uint16_t addr)
{
	uint8_t status; // ״̬��־λ

	sccb_start();										 // ����IICͨ��
	if (sccb_send_byte(OV5640_DEVICE_ADDRESS) == ACK_OK) // д����ָ��
	{
		if (sccb_send_byte((uint8_t)(addr >> 8)) == ACK_OK) // д��16λ��ַ
		{
			if (sccb_send_byte((uint8_t)(addr)) != ACK_OK)
			{
				status = ERROR; // ����ʧ��
			}
		}
	}
	status = SUCCESS; // �����ɹ�
	return status;
}
/**
******************************************************************************************************************************
* @brief:      	sccb_write_reg_16bit
* @param:      	addr - Ҫд��ļĴ���(16λ��ַ)  
* @param:      	value - Ҫд�������
* @retval:     	SUCCESS - �����ɹ�
* @retval:     	ERROR	  - ����ʧ��
* @details:    	��ָ���ļĴ���(16λ��ַ)дһ�ֽ����ݣ�OV5640�õ�
******************************************************************************************************************************
**/
uint8_t sccb_write_reg_16bit(uint16_t addr, uint8_t value)
{
	uint8_t status;

	sccb_start(); // ����IICͨѶ

	if (sccb_write_handle_16Bit(addr) == SUCCESS) // д��Ҫ�����ļĴ���
	{
		if (sccb_send_byte(value) != ACK_OK) // д����
		{
			status = ERROR;
		}
	}
	sccb_stop(); // ֹͣͨѶ

	status = SUCCESS; // д��ɹ�
	return status;
}
/**
******************************************************************************************************************************
* @brief:      	sccb_read_reg_16bit
* @param:      	addr - Ҫ��ȡ�ļĴ���(16λ��ַ)
* @retval:     	����������
* @details:    	��ָ���ļĴ���(16λ��ַ)��ȡһ�ֽ����ݣ�OV5640�õ�
******************************************************************************************************************************
**/
uint8_t sccb_read_reg_16bit(uint16_t addr)
{
	uint8_t value = 0;

	sccb_start(); // ����IICͨ��

	if (sccb_write_handle_16Bit(addr) == SUCCESS) // д��Ҫ�����ļĴ���
	{
		sccb_stop();  // ֹͣIICͨ��
		sccb_start(); // ��������IICͨѶ

		if (sccb_send_byte(OV5640_DEVICE_ADDRESS | 0X01) == ACK_OK) // ���Ͷ�����
		{
			value = sccb_read_byte(0); // �������һ������ʱ���� ��Ӧ���ź�
		}
		sccb_stop(); // ֹͣIICͨ��
	}

	return value;
}
/**
******************************************************************************************************************************
* @brief:      	sccb_write_buf_16bit
* @param:      	addr - Ҫд��ļĴ���(16λ��ַ)  
* @param:      	*pData - ������  
* @param:      	size - Ҫ�������ݵĴ�С
* @retval:     	SUCCESS - �����ɹ�
* @retval:     	ERROR	  - ����ʧ��
* @details:    	��ָ���ļĴ���(16λ��ַ)����д���ݣ�OV5640 д���Զ��Խ��̼�ʱ�õ�
******************************************************************************************************************************
**/
uint8_t sccb_write_buf_16bit(uint16_t addr, uint8_t *pData, uint32_t size)
{
	uint8_t status;
	uint32_t i;

	sccb_start(); // ����IICͨѶ

	if (sccb_write_handle_16Bit(addr) == SUCCESS) // д��Ҫ�����ļĴ���
	{
		for (i = 0; i < size; i++)
		{
			sccb_send_byte(*pData); // д����
			pData++;
		}
	}
	sccb_stop(); // ֹͣͨѶ

	status = SUCCESS; // д��ɹ�
	return status;
}
