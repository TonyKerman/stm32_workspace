# 串口
## 发送debug（C++）
1. in cubemx: 打开usart
2. in code：

        #include<string>
        string text;
        ...
        text ="***";
        HAL_UART_Transmit(&huart1,(uint8_t *)text.data(),text.size(),100);

## 中断接收
1. in cubemx：打开串口gloabl中断
2. 直接 

## 接收DMA IDLE中断
<https://blog.csdn.net/qq_31562655/article/details/121014887>

1. in cubemx :打开usart ，dma ，global exti

2. in init:
    HAL_UARTEx_ReceiveToIdle_DMA(&huart7, rxBufArr, BUFARR_SIZE);//打开idle中断
	__HAL_DMA_DISABLE_IT(&hdma_uart7_rx,DMA_IT_HT);//关闭dma本来的中断

3. 回调

    int pos = 0;
    void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
    {
    	if (huart == &huart7)
    	{
    		for(int j = 0;j<=MSG_LEN;j++)  //在缓存数组里循环接收报文字节
    		{
    			rxbuf = rxBufArr[pos];
    			//开启解析流程
    		}
    		//重新开启接收
            HAL_UARTEx_ReceiveToIdle_DMA(&huart7, rxBufArr, BUFARR_SIZE);//打开idle中断
	       __HAL_DMA_DISABLE_IT(&hdma_uart7_rx,DMA_IT_HT);//关闭dma本来的中断
    		pos++;
    		if(pos>=BUFARR_SIZE)
    		{
    			pos = 0;
    		}
    	}	
    }

在这里，pos为数组的下标，表明当前读到缓存数组的某个字节位置上，当下次报文发送过来时则在原来位置上继续往下读MSG_LEN个字节。当超过DMA缓存长度时，则将pos置零，从头开始。