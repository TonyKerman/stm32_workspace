# CAN
<https://blog.csdn.net/thisway_diy/article/details/117702782?>

## CAN协议
* CAN通讯并不是以时钟信号来进行同步的，它是一种异步半双工通讯。（半双工即接受与发送不能同时进行）

* CAN总线使用CAN_H和CAN_L的电位差来表示数据电平。电位差分为显性电平和隐性电平，分别表示逻辑0和1。

* CAN总线有两个ISO国际标准：ISO11519 和ISO11898

    1. ISO11519定义了通信速率为10～125Kbps的低速CAN通信标准，属于开环总线
    2. ISO11898定义了通信速率为125Kbps～1 Mbps的高速CAN通信标准，属于闭环总线
    
* CAN总线以“帧”（Frame）的形式进行通信。CAN 总线协议规定了5种帧，分别是数据帧、远程帧、错误帧、超载帧以及帧间隔，其中数据帧最常用
### 通讯过程
<https://blog.csdn.net/fengge2018/article/details/107592487>
1. 空闲状态，所谓的空闲状态就是指没有节点正在传输数据的时候，在CAN协议中，当总线上的上出现连续的11位隐性电平，那么总线就处于空闲状态。也就是说对于任意一个节点而言，只要它监听到总线上连续出现了11位隐性电平，那么该节点就会认为总线当前处于空闲状态。(只要有一个节点输出显性电平，总线上即为显性电平。只有所有的单元都输出隐性电平，总线上才为隐性电平)
2. 每次发送数据前，节点都会监听总线的状态，如果总线状态为空闲时，它就会立即向总线上发送自己的数据，这个数据里不仅有数据，还有本身的ID信息或者其他的控制指令，应称为数据包(数据帧)，也叫做报文。当报文被传输到其它节点时，只要这些节点按格式去解读，就能还原出原始数据
#### 数据帧
* 帧起始
包含一个显性电平，用于通知各节点进行硬同步。
* 仲裁段
包含报文ID，ID有11位的标准格式和29位的扩展格式两种，通过IDE位进行标识。
IDE为显性位则ID为为11位，IDE位隐形位则ID为29位，由此可以看到标准格式的优先级大于扩展格式
RTR位为显性是表示数据帧，隐形位表示遥控帧，由此可以看到数据帧的优先级大于遥控帧。
SRR位仅仅用于扩展格式中代替RTR而存在的，是一个隐形电平。
* 控制段
包含6个数据位，其中R0和R1是显性保留位，DLR用于表示数据段个长度，数值范围为0-8，对饮数据段0-8个字节。
数据段
包含待发数据，MSB先行。
* CRC段
包含15位的校验码，检验出错时将会根据最大重发数来向发送节点反馈一个错误帧来请求重新发送。
CRC部分一般由CAN控制器硬件来完成，出错时的处理则由软件来控制。
CRC界定符用于间隔CRC段和ACK段，是一个隐形位，同ACK界定符。
* ACK段
包含一个用于应答的ACK槽位和用于间隔的隐性的ACK界定符。
帧结束
包含7个隐性位，用于表示一帧数据的结束


## 接线

CAN通信只由两根线完成的，一条称为CAN_H（CAN High），一条称为CAN_L（CAN Low），共同构成一组差分信号线。整个过程是以差分信号的形式进行通讯的，即信号的逻辑 0 和逻辑 1 由两根差分信号线的电压差来表示。

## 代码
<https://blog.csdn.net/lxzdsg/article/details/113218517>
 stm32f4xx_hal_can.c
### 过滤器
四种模式：

* 16位列表模式
* 16位掩码模式
* 32位列表模式
* 32位掩码模式

<https://blog.csdn.net/flydream0/article/details/52317532>

    typedef struct
    {
      uint32_t FilterIdHigh;          /*!< Specifies the filter identification number (MSBs for a 32-bit
                                           configuration, first one for a 16-bit configuration).
                                           This parameter must be a number between Min_Data = 0x0000 and Max_Data = 0xFFFF. */
    
      uint32_t FilterIdLow;           /*!< Specifies the filter identification number (LSBs for a 32-bit
                                           configuration, second one for a 16-bit configuration).
                                           This parameter must be a number between Min_Data = 0x0000 and Max_Data = 0xFFFF. */
    
      uint32_t FilterMaskIdHigh;      /*!< Specifies the filter mask number or identification number,
                                           according to the mode (MSBs for a 32-bit configuration,
                                           first one for a 16-bit configuration).
                                           This parameter must be a number between Min_Data = 0x0000 and Max_Data = 0xFFFF. */
    
      uint32_t FilterMaskIdLow;       /*!< Specifies the filter mask number or identification number,
                                           according to the mode (LSBs for a 32-bit configuration,
                                           second one for a 16-bit configuration).
                                           This parameter must be a number between Min_Data = 0x0000 and Max_Data = 0xFFFF. */
    
      uint32_t FilterFIFOAssignment;  /*!< Specifies the FIFO (0 or 1U) which will be assigned to the filter.
                                           This parameter can be a value of @ref CAN_filter_FIFO */
    
      uint32_t FilterBank;            /*!< Specifies the filter bank which will be initialized.
                                           For single CAN instance(14 dedicated filter banks),
                                           this parameter must be a number between Min_Data = 0 and Max_Data = 13.
                                           For dual CAN instances(28 filter banks shared),
                                           this parameter must be a number between Min_Data = 0 and Max_Data = 27. */
    
      uint32_t FilterMode;            /*!< Specifies the filter mode to be initialized.
                                           This parameter can be a value of @ref CAN_filter_mode */
    
      uint32_t FilterScale;           /*!< Specifies the filter scale.
                                           This parameter can be a value of @ref CAN_filter_scale */
    
      uint32_t FilterActivation;      /*!< Enable or disable the filter.
                                           This parameter can be a value of @ref CAN_filter_activation */
    
      uint32_t SlaveStartFilterBank;  /*!< Select the start filter bank for the slave CAN instance.
                                           For single CAN instances, this parameter is meaningless.
                                           For dual CAN instances, all filter banks with lower index are assigned to master
                                           CAN instance, whereas all filter banks with greater index are assigned to slave
                                           CAN instance.
                                           This parameter must be a number between Min_Data = 0 and Max_Data = 27. */
    
    } CAN_FilterTypeDef;
#### 过滤器要自己配置
eg：32列表模式

    static void CANFilterConfig_Scale32_IdList(void)
    {
      CAN_FilterConfTypeDef  sFilterConfig;
      uint32_t StdId =0x321;				//这里写入两个CAN ID，一个位标准CAN ID
      uint32_t ExtId =0x1800f001;			//一个位扩展CAN ID
      
      sFilterConfig.FilterNumber = 0;				//使用过滤器0
      sFilterConfig.FilterMode = CAN_FILTERMODE_IDLIST;		//设为列表模式
      sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;	//配置为32位宽
      sFilterConfig.FilterIdHigh = StdId<<5;			//基本ID放入到STID中
      sFilterConfig.FilterIdLow = 0|CAN_ID_STD;			//设置IDE位为0
      sFilterConfig.FilterMaskIdHigh = ((ExtId<<3)>>16)&0xffff;
      sFilterConfig.FilterMaskIdLow = (ExtId<<3)&0xffff|CAN_ID_EXT;	//设置IDE位为1
      sFilterConfig.FilterFIFOAssignment = 0;			//接收到的报文放入到FIFO0中
      sFilterConfig.FilterActivation = ENABLE;
      sFilterConfig.BankNumber = 14;
      
      if(HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK)
      {
        Error_Handler();
      }
    }
（在Init时执行一遍）
### 相关函数
    HAL_CAN_Start	//开启CAN通讯
    HAL_CAN_Stop	//关闭CAN通讯
    HAL_CAN_RequestSleep	//尝试进入休眠模式
    HAL_CAN_WakeUp	//从休眠模式中唤醒
    HAL_CAN_IsSleepActive	//检查是否成功进入休眠模式
    HAL_CAN_AddTxMessage	//向 Tx 邮箱中增加一个消息,并且激活对应的传输请求
    HAL_CAN_AbortTxRequest	//请求中断传输
    HAL_CAN_IsTxMessagePending	//检查是否有传输请求在指定的 Tx 邮箱上等待
    HAL_CAN_GetRxMessage	//从Rx FIFO 收取一个 CAN 帧
 
回调函数
    CAN_TxHeaderTypeDef TXHeader;
    CAN_RxHeaderTypeDef RXHeader;
    void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)//接受邮箱0挂起中断回调函数
    {
    	if(hcan->Instance==CAN1)
    	{
    		HAL_CAN_GetRxMessage(hcan,CAN_FILTER_FIFO0,&RXHeader,RXmessage);//获取数据
        }
    	
    }
发送函数
    void CAN_senddata(CAN_HandleTypeDef *hcan)
    {
         TXHeader.StdId=0x00000000;
    	 TXHeader.ExtId=0x12345000;
    	 TXHeader.DLC=8;
    	 TXHeader.IDE=CAN_ID_EXT;
    	 TXHeader.RTR=CAN_RTR_DATA;
    	 TXHeader.TransmitGlobalTime = DISABLE;
    	 HAL_CAN_AddTxMessage(hcan,&TXHeader,TXmessage,&pTxMailbox);
    }
    

