#include "can_bsp.h"
/**
************************************************************************
* @brief:      	can_bsp_init(void)
* @param:       void
* @retval:     	void
* @details:    	CAN ʹ��
************************************************************************
**/
void can_bsp_init(void)
{
	can_filter_init();
	HAL_FDCAN_Start(&hfdcan1);                               //����FDCAN
//	HAL_FDCAN_Start(&hfdcan2);
//	HAL_FDCAN_Start(&hfdcan3);
	HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_WATERMARK, 0);
	HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO1_WATERMARK, 0);
	HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_BUFFER_NEW_MESSAGE, 0);
}
/**
************************************************************************
* @brief:      	can_filter_init(void)
* @param:       void
* @retval:     	void
* @details:    	CAN�˲�����ʼ��
************************************************************************
**/
void can_filter_init(void)
{
	FDCAN_FilterTypeDef fdcan_filter;
	
	fdcan_filter.IdType = FDCAN_STANDARD_ID;                       //��׼ID
	fdcan_filter.FilterIndex = 0;                                  //�˲�������                   
	fdcan_filter.FilterType = FDCAN_FILTER_DUAL;                   
	fdcan_filter.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;           //������0������FIFO0  
	fdcan_filter.FilterID1 = 0x01;                               
	fdcan_filter.FilterID2 = 0x01;

	HAL_FDCAN_ConfigFilter(&hfdcan1,&fdcan_filter); 		 				  //����ID2
	//�ܾ�����ƥ�䲻�ɹ��ı�׼ID����չID,������Զ��֡
	HAL_FDCAN_ConfigGlobalFilter(&hfdcan1,FDCAN_REJECT,FDCAN_REJECT,FDCAN_REJECT_REMOTE,FDCAN_REJECT_REMOTE);
	HAL_FDCAN_ConfigFifoWatermark(&hfdcan1, FDCAN_CFG_RX_FIFO0, 1);
	HAL_FDCAN_ConfigFifoWatermark(&hfdcan1, FDCAN_CFG_RX_FIFO1, 1);
	HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_TX_COMPLETE, FDCAN_TX_BUFFER0);
}
/**
************************************************************************
* @brief:      	fdcanx_send_data(FDCAN_HandleTypeDef *hfdcan, uint16_t id, uint8_t *data, uint32_t len)
* @param:       hfdcan��FDCAN���
* @param:       id��CAN�豸ID
* @param:       data�����͵�����
* @param:       len�����͵����ݳ���
* @retval:     	void
* @details:    	��������
************************************************************************
**/
uint8_t fdcanx_send_data(FDCAN_HandleTypeDef *hfdcan, uint16_t id, uint8_t *data, uint32_t len)
{	
    FDCAN_TxHeaderTypeDef pTxHeader;
    pTxHeader.Identifier=id;
    pTxHeader.IdType=FDCAN_EXTENDED_ID;
    pTxHeader.TxFrameType=FDCAN_DATA_FRAME;
	
	if(len<=8)
		pTxHeader.DataLength = len;
	if(len==12)
		pTxHeader.DataLength = FDCAN_DLC_BYTES_12;
	if(len==16)
		pTxHeader.DataLength = FDCAN_DLC_BYTES_16;
	if(len==20)
		pTxHeader.DataLength = FDCAN_DLC_BYTES_20;
	if(len==24)
		pTxHeader.DataLength = FDCAN_DLC_BYTES_24;
	if(len==32)
		pTxHeader.DataLength = FDCAN_DLC_BYTES_32;
	if(len==48)
		pTxHeader.DataLength = FDCAN_DLC_BYTES_48;
	if(len==64)
		pTxHeader.DataLength = FDCAN_DLC_BYTES_64;
	
    pTxHeader.ErrorStateIndicator=FDCAN_ESI_ACTIVE;
    pTxHeader.BitRateSwitch=FDCAN_BRS_ON;
    pTxHeader.FDFormat=FDCAN_FD_CAN;
    pTxHeader.TxEventFifoControl=FDCAN_NO_TX_EVENTS;
    pTxHeader.MessageMarker=0;
 
	if(HAL_FDCAN_AddMessageToTxFifoQ(hfdcan, &pTxHeader, data)!=HAL_OK) 
		return 1;//����
	return 0;	
}
/**
************************************************************************
* @brief:      	fdcanx_receive(FDCAN_HandleTypeDef *hfdcan, uint8_t *buf)
* @param:       hfdcan��FDCAN���
* @param:       buf���������ݻ���
* @retval:     	���յ����ݳ���
* @details:    	��������
************************************************************************
**/
uint8_t fdcanx_receive(FDCAN_HandleTypeDef *hfdcan, uint16_t *rec_id, uint8_t *buf)
{	
	FDCAN_RxHeaderTypeDef pRxHeader;
	uint8_t len;
	
	if(HAL_FDCAN_GetRxMessage(hfdcan,FDCAN_RX_FIFO0, &pRxHeader, buf)==HAL_OK)
	{
		*rec_id = pRxHeader.Identifier;
		if(pRxHeader.DataLength<=FDCAN_DLC_BYTES_8)
			len = pRxHeader.DataLength;
		if(pRxHeader.DataLength<=FDCAN_DLC_BYTES_12)
			len = 12;
		if(pRxHeader.DataLength<=FDCAN_DLC_BYTES_16)
			len = 16;
		if(pRxHeader.DataLength<=FDCAN_DLC_BYTES_20)
			len = 20;
		if(pRxHeader.DataLength<=FDCAN_DLC_BYTES_24)
			len = 24;
		if(pRxHeader.DataLength<=FDCAN_DLC_BYTES_32)
			len = 32;
		if(pRxHeader.DataLength<=FDCAN_DLC_BYTES_48)
			len = 48;
		if(pRxHeader.DataLength<=FDCAN_DLC_BYTES_64)
			len = 64;
		
		return len;//��������
	}
	return 0;	
}
/**
************************************************************************
* @brief:      	HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
* @param:       hfdcan��FDCAN���
* @param:       RxFifo0ITs���жϱ�־λ
* @retval:     	void
* @details:    	HAL���FDCAN�жϻص�����
************************************************************************
**/

uint8_t rx_data1[64] = {0};
uint16_t reid;
void fdcan1_rx_callback(void)
{
	fdcanx_receive(&hfdcan1, &reid, rx_data1);
}

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
	FDCAN_RxHeaderTypeDef pRxHeader;
	
    if (hfdcan == &hfdcan1)
    {
		fdcan1_rx_callback();
    }
}











