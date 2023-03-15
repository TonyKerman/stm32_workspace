///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.10.1.52143/W32 for ARM    07/Jan/2012  00:36:19 /
// Copyright 1999-2010 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode     =  thumb                                                   /
//    Endian       =  little                                                  /
//    Source file  =  F:\Kinetis\IARK60X256\01_LQ_light\src\Sources\C\Compone /
//                    nt_C\uart.c                                             /
//    Command line =  F:\Kinetis\IARK60X256\01_LQ_light\src\Sources\C\Compone /
//                    nt_C\uart.c -D IAR -D TWR_K60N512 -lCN                  /
//                    F:\Kinetis\IARK60X256\01_LQ_light\bin\Ram\List\ -lB     /
//                    F:\Kinetis\IARK60X256\01_LQ_light\bin\Ram\List\ -o      /
//                    F:\Kinetis\IARK60X256\01_LQ_light\bin\Ram\Obj\          /
//                    --no_cse --no_unroll --no_inline --no_code_motion       /
//                    --no_tbaa --no_clustering --no_scheduling --debug       /
//                    --endian=little --cpu=Cortex-M4 -e --fpu=None           /
//                    --dlib_config "C:\Program Files\IAR Systems\Embedded    /
//                    Workbench 6.0\arm\INC\c\DLib_Config_Normal.h" -I        /
//                    F:\Kinetis\IARK60X256\01_LQ_light\src\Sources\H\ -I     /
//                    F:\Kinetis\IARK60X256\01_LQ_light\src\Sources\H\Compone /
//                    nt_H\ -I F:\Kinetis\IARK60X256\01_LQ_light\src\Sources\ /
//                    H\Frame_H\ -Ol --use_c++_inline                         /
//    List file    =  F:\Kinetis\IARK60X256\01_LQ_light\bin\Ram\List\uart.s   /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME uart

        EXTERN disable_irq
        EXTERN enable_irq

        PUBLIC disableuartreint
        PUBLIC enableuartreint
        PUBLIC uart_init
        PUBLIC uart_re1
        PUBLIC uart_reN
        PUBLIC uart_send1
        PUBLIC uart_sendN

// F:\Kinetis\IARK60X256\01_LQ_light\src\Sources\C\Component_C\uart.c
//    1 //-------------------------------------------------------------------------*
//    2 // 文件名: uart.c                                                          *
//    3 // 说  明: uart构件源文件                                                  *
//    4 //-------------------------------------------------------------------------*
//    5 
//    6 #include "uart.h"
//    7 
//    8 //-------------------------------------------------------------------------*
//    9 //函数名: uart_init                                                        *
//   10 //功  能: 初始化uartx模块。                                                *
//   11 //参  数: uartch:串口号                                                    *
//   12 //        sysclk:系统总线时钟，以MHz为单位                                 *
//   13 //        baud:波特率，如9600，38400等，一般来说，速度越慢，通信越稳       *
//   14 //返  回: 无                                                               *
//   15 //说  明:                                                                  *
//   16 //-------------------------------------------------------------------------*

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
//   17 void uart_init (UART_MemMapPtr uartch, uint32 sysclk, uint32 baud)
//   18 {
uart_init:
        PUSH     {R4,R5}
//   19 	register uint16 sbr, brfa;
//   20 	uint8 temp;
//   21 
//   22 	//使能引脚
//   23 	if (uartch == UART0_BASE_PTR)
        LDR.N    R3,??DataTable0  ;; 0x4006a000
        CMP      R0,R3
        BNE.N    ??uart_init_0
//   24 	{
//   25 		//在PTD6上使能UART0_TXD功能
//   26 		PORTD_PCR6 = PORT_PCR_MUX(0x3);
        LDR.N    R3,??DataTable0_1  ;; 0x4004c018
        MOV      R4,#+768
        STR      R4,[R3, #+0]
//   27 		//在PTD7上使能UART0_RXD
//   28 		PORTD_PCR7 = PORT_PCR_MUX(0x3); 
        LDR.N    R3,??DataTable0_2  ;; 0x4004c01c
        MOV      R4,#+768
        STR      R4,[R3, #+0]
        B.N      ??uart_init_1
//   29 	}else if (uartch == UART1_BASE_PTR)
??uart_init_0:
        LDR.N    R3,??DataTable0_3  ;; 0x4006b000
        CMP      R0,R3
        BNE.N    ??uart_init_2
//   30 	{
//   31 		//在PTC4上使能UART1_TXD功能
//   32 		PORTC_PCR4 = PORT_PCR_MUX(0x3); 
        LDR.N    R3,??DataTable0_4  ;; 0x4004b010
        MOV      R4,#+768
        STR      R4,[R3, #+0]
//   33 		
//   34 		//在PTC3上使能UART1_RXD
//   35 		PORTC_PCR3 = PORT_PCR_MUX(0x3); 
        LDR.N    R3,??DataTable0_5  ;; 0x4004b00c
        MOV      R4,#+768
        STR      R4,[R3, #+0]
        B.N      ??uart_init_1
//   36 	}else if (uartch == UART2_BASE_PTR)
??uart_init_2:
        LDR.N    R3,??DataTable0_6  ;; 0x4006c000
        CMP      R0,R3
        BNE.N    ??uart_init_3
//   37 	{
//   38 		//在PTD3上使能UART2_TXD功能
//   39 		PORTD_PCR3 = PORT_PCR_MUX(0x3); 
        LDR.N    R3,??DataTable0_7  ;; 0x4004c00c
        MOV      R4,#+768
        STR      R4,[R3, #+0]
//   40 		//在PTD2上使能UART2_RXD
//   41 		PORTD_PCR2 = PORT_PCR_MUX(0x3); 
        LDR.N    R3,??DataTable0_8  ;; 0x4004c008
        MOV      R4,#+768
        STR      R4,[R3, #+0]
        B.N      ??uart_init_1
//   42 	}else if (uartch == UART3_BASE_PTR)
??uart_init_3:
        LDR.N    R3,??DataTable0_9  ;; 0x4006d000
        CMP      R0,R3
        BNE.N    ??uart_init_4
//   43 	{
//   44 		//在PTC17上使能UART3_TXD功能
//   45 		PORTC_PCR17 = PORT_PCR_MUX(0x3); 
        LDR.N    R3,??DataTable0_10  ;; 0x4004b044
        MOV      R4,#+768
        STR      R4,[R3, #+0]
//   46 		//在PTC16上使能UART3_RXD
//   47 		PORTC_PCR16 = PORT_PCR_MUX(0x3); 
        LDR.N    R3,??DataTable0_11  ;; 0x4004b040
        MOV      R4,#+768
        STR      R4,[R3, #+0]
        B.N      ??uart_init_1
//   48 	}else if (uartch == UART4_BASE_PTR)
??uart_init_4:
        LDR.N    R3,??DataTable0_12  ;; 0x400ea000
        CMP      R0,R3
        BNE.N    ??uart_init_5
//   49 	{
//   50 		//在PTE24上使能UART4_TXD功能
//   51 		PORTE_PCR24 = PORT_PCR_MUX(0x3); 
        LDR.N    R3,??DataTable0_13  ;; 0x4004d060
        MOV      R4,#+768
        STR      R4,[R3, #+0]
//   52 		//在PTE25上使能UART4_RXD
//   53 		PORTE_PCR25 = PORT_PCR_MUX(0x3); 
        LDR.N    R3,??DataTable0_14  ;; 0x4004d064
        MOV      R4,#+768
        STR      R4,[R3, #+0]
        B.N      ??uart_init_1
//   54 	}else if (uartch == UART5_BASE_PTR)
??uart_init_5:
        LDR.N    R3,??DataTable0_15  ;; 0x400eb000
        CMP      R0,R3
        BNE.N    ??uart_init_1
//   55 	{
//   56 		//在PTE8上使能UART5_TXD功能
//   57 		PORTE_PCR8 = PORT_PCR_MUX(0x3); 
        LDR.N    R3,??DataTable0_16  ;; 0x4004d020
        MOV      R4,#+768
        STR      R4,[R3, #+0]
//   58 		//在PTE9上使能UART5_RXD
//   59 		PORTE_PCR9 = PORT_PCR_MUX(0x3); 
        LDR.N    R3,??DataTable0_17  ;; 0x4004d024
        MOV      R4,#+768
        STR      R4,[R3, #+0]
//   60 	}
//   61 	 
//   62 	//使能串口时钟    
//   63 	if(uartch == UART0_BASE_PTR)
??uart_init_1:
        LDR.N    R3,??DataTable0  ;; 0x4006a000
        CMP      R0,R3
        BNE.N    ??uart_init_6
//   64 		SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;
        LDR.N    R3,??DataTable0_18  ;; 0x40048034
        LDR      R3,[R3, #+0]
        ORRS     R3,R3,#0x400
        LDR.N    R4,??DataTable0_18  ;; 0x40048034
        STR      R3,[R4, #+0]
        B.N      ??uart_init_7
//   65 	else
//   66 		if (uartch == UART1_BASE_PTR)
??uart_init_6:
        LDR.N    R3,??DataTable0_3  ;; 0x4006b000
        CMP      R0,R3
        BNE.N    ??uart_init_8
//   67 			SIM_SCGC4 |= SIM_SCGC4_UART1_MASK;
        LDR.N    R3,??DataTable0_18  ;; 0x40048034
        LDR      R3,[R3, #+0]
        ORRS     R3,R3,#0x800
        LDR.N    R4,??DataTable0_18  ;; 0x40048034
        STR      R3,[R4, #+0]
        B.N      ??uart_init_7
//   68 		else
//   69 			if (uartch == UART2_BASE_PTR)
??uart_init_8:
        LDR.N    R3,??DataTable0_6  ;; 0x4006c000
        CMP      R0,R3
        BNE.N    ??uart_init_9
//   70 				SIM_SCGC4 |= SIM_SCGC4_UART2_MASK;
        LDR.N    R3,??DataTable0_18  ;; 0x40048034
        LDR      R3,[R3, #+0]
        ORRS     R3,R3,#0x1000
        LDR.N    R4,??DataTable0_18  ;; 0x40048034
        STR      R3,[R4, #+0]
        B.N      ??uart_init_7
//   71 			else
//   72 				if(uartch == UART3_BASE_PTR)
??uart_init_9:
        LDR.N    R3,??DataTable0_9  ;; 0x4006d000
        CMP      R0,R3
        BNE.N    ??uart_init_10
//   73 					SIM_SCGC4 |= SIM_SCGC4_UART3_MASK;
        LDR.N    R3,??DataTable0_18  ;; 0x40048034
        LDR      R3,[R3, #+0]
        ORRS     R3,R3,#0x2000
        LDR.N    R4,??DataTable0_18  ;; 0x40048034
        STR      R3,[R4, #+0]
        B.N      ??uart_init_7
//   74 				else
//   75 					if(uartch == UART4_BASE_PTR)
??uart_init_10:
        LDR.N    R3,??DataTable0_12  ;; 0x400ea000
        CMP      R0,R3
        BNE.N    ??uart_init_11
//   76 						SIM_SCGC1 |= SIM_SCGC1_UART4_MASK;
        LDR.N    R3,??DataTable0_19  ;; 0x40048028
        LDR      R3,[R3, #+0]
        ORRS     R3,R3,#0x400
        LDR.N    R4,??DataTable0_19  ;; 0x40048028
        STR      R3,[R4, #+0]
        B.N      ??uart_init_7
//   77 					else
//   78 						SIM_SCGC1 |= SIM_SCGC1_UART5_MASK;
??uart_init_11:
        LDR.N    R3,??DataTable0_19  ;; 0x40048028
        LDR      R3,[R3, #+0]
        ORRS     R3,R3,#0x800
        LDR.N    R4,??DataTable0_19  ;; 0x40048028
        STR      R3,[R4, #+0]
//   79 								
//   80 	//禁止发送接受
//   81 	UART_C2_REG(uartch) &= ~(UART_C2_TE_MASK
//   82 				| UART_C2_RE_MASK );
??uart_init_7:
        LDRB     R3,[R0, #+3]
        ANDS     R3,R3,#0xF3
        STRB     R3,[R0, #+3]
//   83 	
//   84 	//配置成8位无校验模式
//   85 	UART_C1_REG(uartch) = 0;
        MOVS     R3,#+0
        STRB     R3,[R0, #+2]
//   86 	
//   87 	//计算波特率，串口0、1使用内核时钟，其它串口使用外设时钟，系统时钟为
//   88 	//外设时钟的2倍
//   89 	if ((uartch == UART0_BASE_PTR) | (uartch == UART1_BASE_PTR))//
        LDR.N    R3,??DataTable0  ;; 0x4006a000
        CMP      R0,R3
        BEQ.N    ??uart_init_12
        MOVS     R3,#+0
        LDR.N    R4,??DataTable0_3  ;; 0x4006b000
        CMP      R0,R4
        BEQ.N    ??uart_init_12
        ORRS     R3,R3,#0x0
        UXTB     R3,R3            ;; ZeroExt  R3,R3,#+24,#+24
        CMP      R3,#+0
        BEQ.N    ??uart_init_13
//   90 		sysclk+=sysclk;
??uart_init_12:
        ADDS     R1,R1,R1
//   91 	
//   92 	sbr = (uint16)((sysclk*1000)/(baud * 16));
??uart_init_13:
        MOV      R3,#+1000
        MUL      R3,R3,R1
        LSLS     R4,R2,#+4
        UDIV     R3,R3,R4
//   93 	temp = UART_BDH_REG(uartch) & ~(UART_BDH_SBR(0x1F));
        LDRB     R4,[R0, #+0]
        ANDS     R4,R4,#0xE0
//   94 	UART_BDH_REG(uartch) = temp |  UART_BDH_SBR(((sbr & 0x1F00) >> 8));
        UXTH     R3,R3            ;; ZeroExt  R3,R3,#+16,#+16
        LSRS     R5,R3,#+8
        ANDS     R5,R5,#0x1F
        ORRS     R4,R5,R4
        STRB     R4,[R0, #+0]
//   95 	UART_BDL_REG(uartch) = (uint8)(sbr & UART_BDL_SBR_MASK);
        STRB     R3,[R0, #+1]
//   96 	brfa = (((sysclk*32000)/(baud * 16)) - (sbr * 32));
        MOV      R4,#+32000
        MULS     R1,R4,R1
        LSLS     R2,R2,#+4
        UDIV     R1,R1,R2
        UXTH     R3,R3            ;; ZeroExt  R3,R3,#+16,#+16
        SUBS     R1,R1,R3, LSL #+5
//   97 	temp = UART_C4_REG(uartch) & ~(UART_C4_BRFA(0x1F));
        LDRB     R2,[R0, #+10]
        ANDS     R4,R2,#0xE0
//   98 	UART_C4_REG(uartch) = temp |  UART_C4_BRFA(brfa);    
        ANDS     R1,R1,#0x1F
        ORRS     R1,R1,R4
        STRB     R1,[R0, #+10]
//   99 	
//  100 	//使能发送接受
//  101 	UART_C2_REG(uartch) |= (UART_C2_TE_MASK
//  102 				| UART_C2_RE_MASK );
        LDRB     R1,[R0, #+3]
        ORRS     R1,R1,#0xC
        STRB     R1,[R0, #+3]
//  103 }
        POP      {R4,R5}
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable0:
        DC32     0x4006a000

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable0_1:
        DC32     0x4004c018

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable0_2:
        DC32     0x4004c01c

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable0_3:
        DC32     0x4006b000

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable0_4:
        DC32     0x4004b010

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable0_5:
        DC32     0x4004b00c

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable0_6:
        DC32     0x4006c000

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable0_7:
        DC32     0x4004c00c

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable0_8:
        DC32     0x4004c008

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable0_9:
        DC32     0x4006d000

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable0_10:
        DC32     0x4004b044

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable0_11:
        DC32     0x4004b040

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable0_12:
        DC32     0x400ea000

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable0_13:
        DC32     0x4004d060

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable0_14:
        DC32     0x4004d064

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable0_15:
        DC32     0x400eb000

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable0_16:
        DC32     0x4004d020

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable0_17:
        DC32     0x4004d024

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable0_18:
        DC32     0x40048034

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable0_19:
        DC32     0x40048028
//  104 
//  105 //-------------------------------------------------------------------------*
//  106 //函数名: uart_re1                                                         *
//  107 //功  能: 串行接受1个字节                                                  *
//  108 //参  数: uartch: 串口号                                                   *
//  109 //         ch:    接收到的字节                                             *
//  110 //返  回: 成功:1;失败:0                                                    *
//  111 //说  明:                                                                  *
//  112 //-------------------------------------------------------------------------*

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
//  113 uint8 uart_re1 (UART_MemMapPtr uartch,uint8 *ch)
//  114 {
//  115     uint32 k;
//  116     
//  117     for (k = 0; k < 0xfbbb; k++)//有时间限制
uart_re1:
        MOVS     R2,#+0
        B.N      ??uart_re1_0
??uart_re1_1:
        ADDS     R2,R2,#+1
??uart_re1_0:
        MOVW     R3,#+64443
        CMP      R2,R3
        BCS.N    ??uart_re1_2
//  118 		if((UART_S1_REG(uartch) & UART_S1_RDRF_MASK)!= 0)//判断接收缓冲区是否满
        LDRB     R3,[R0, #+4]
        LSLS     R3,R3,#+26
        BPL.N    ??uart_re1_1
//  119 		{
//  120 			*ch = UART_D_REG(uartch);
        LDRB     R0,[R0, #+7]
        STRB     R0,[R1, #+0]
//  121 			return 1; 			//接受成功
        MOVS     R0,#+1
        B.N      ??uart_re1_3
//  122 		} 
//  123 	if(k>=0xfbbb) 
??uart_re1_2:
        MOVW     R0,#+64443
        CMP      R2,R0
        BCC.N    ??uart_re1_4
//  124 	{
//  125 		return 0;			//接受失败
        MOVS     R0,#+0
        B.N      ??uart_re1_3
//  126 	} 
//  127     return 0;
??uart_re1_4:
        MOVS     R0,#+0
??uart_re1_3:
        BX       LR               ;; return
//  128 }
//  129 
//  130 //-------------------------------------------------------------------------*
//  131 //函数名: uart_send1                                                       *
//  132 //功  能: 串行发送1个字节                                                  *
//  133 //参  数: uartch: 串口号                                                   *
//  134 //         ch:    要发送的字节                                             *
//  135 //返  回: 无                                                               *
//  136 //说  明:                                                                  *
//  137 //-------------------------------------------------------------------------*

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
//  138 void uart_send1 (UART_MemMapPtr uartch, uint8 ch)
//  139 {
//  140     //等待发送缓冲区空
//  141     while(!(UART_S1_REG(uartch) & UART_S1_TDRE_MASK));
uart_send1:
??uart_send1_0:
        LDRB     R2,[R0, #+4]
        LSLS     R2,R2,#+24
        BPL.N    ??uart_send1_0
//  142     //发送数据
//  143     UART_D_REG(uartch) = (uint8)ch;
        STRB     R1,[R0, #+7]
//  144  }
        BX       LR               ;; return
//  145 
//  146 //-------------------------------------------------------------------------*
//  147 //函数名: uart_reN                                                         *
//  148 //功  能: 串行 接收n个字节                                                 *
//  149 //参  数: uartch: 串口号                                                   *
//  150 //        buff: 接收缓冲区                                                 *
//  151 //		  len:接收长度                                             *
//  152 //返  回: 1:成功;0:失败                                                    *
//  153 //说  明:                                                                  *
//  154 //-------------------------------------------------------------------------*

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
//  155 uint8 uart_reN (UART_MemMapPtr uartch ,uint8* buff,uint16 len)
//  156 {
uart_reN:
        PUSH     {R3-R7,LR}
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//  157     uint16 m=0; 
        MOVS     R7,#+0
        B.N      ??uart_reN_0
//  158     while (m < len)
//  159     { 	          
//  160   	    if(0==uart_re1(uartch,&buff[m]))
//  161   	    	return 0;  //接收失败
//  162   	    else m++;
??uart_reN_1:
        ADDS     R7,R7,#+1
??uart_reN_0:
        UXTH     R7,R7            ;; ZeroExt  R7,R7,#+16,#+16
        UXTH     R6,R6            ;; ZeroExt  R6,R6,#+16,#+16
        CMP      R7,R6
        BCS.N    ??uart_reN_2
        UXTH     R7,R7            ;; ZeroExt  R7,R7,#+16,#+16
        ADDS     R1,R7,R5
        MOVS     R0,R4
        BL       uart_re1
        CMP      R0,#+0
        BNE.N    ??uart_reN_1
        MOVS     R0,#+0
        B.N      ??uart_reN_3
//  163     } 
//  164     
//  165     return 1;          //接收成功
??uart_reN_2:
        MOVS     R0,#+1
??uart_reN_3:
        POP      {R1,R4-R7,PC}    ;; return
//  166     
//  167 }
//  168 
//  169 //-------------------------------------------------------------------------*
//  170 //函数名: uart_sendN                                                       *
//  171 //功  能: 串行 接收n个字节                                                 *
//  172 //参  数: uartch: 串口号                                                   *
//  173 //        buff: 发送缓冲区                                                 *
//  174 //		  len:发送长度                                             *
//  175 //返  回: 无                                                               *
//  176 //说  明:                                                                  *
//  177 //-------------------------------------------------------------------------*

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
//  178 void uart_sendN (UART_MemMapPtr uartch ,uint8* buff,uint16 len)
//  179 {
uart_sendN:
        PUSH     {R3-R7,LR}
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//  180     int i;
//  181 	for(i=0;i<len;i++)
        MOVS     R7,#+0
        B.N      ??uart_sendN_0
//  182     {
//  183 		uart_send1(uartch,buff[i]);
??uart_sendN_1:
        LDRB     R1,[R7, R5]
        MOVS     R0,R4
        BL       uart_send1
//  184     }
        ADDS     R7,R7,#+1
??uart_sendN_0:
        UXTH     R6,R6            ;; ZeroExt  R6,R6,#+16,#+16
        CMP      R7,R6
        BLT.N    ??uart_sendN_1
//  185 }
        POP      {R0,R4-R7,PC}    ;; return
//  186 
//  187 //-------------------------------------------------------------------------*
//  188 //函数名: enableuartreint                                                  *
//  189 //功  能: 开串口接收中断                                                   *
//  190 //参  数: uartch: 串口号                                                   *
//  191 //        irqno: 对应irq号                                                 *
//  192 //返  回: 无                                                               *
//  193 //说  明:                                                                  *
//  194 //-------------------------------------------------------------------------*

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
//  195 void enableuartreint(UART_MemMapPtr uartch,uint8 irqno)
//  196 {
enableuartreint:
        PUSH     {R7,LR}
//  197 	UART_C2_REG(uartch)|=UART_C2_RIE_MASK;   //开放UART接收中断
        LDRB     R2,[R0, #+3]
        ORRS     R2,R2,#0x20
        STRB     R2,[R0, #+3]
//  198 	enable_irq(irqno);			 //开接收引脚的IRQ中断
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R1
        BL       enable_irq
//  199 }
        POP      {R0,PC}          ;; return
//  200 
//  201 //-------------------------------------------------------------------------*
//  202 //函数名: disableuartreint                                                 *
//  203 //功  能: 关串口接收中断                                                   *
//  204 //参  数: uartch: 串口号                                                   *
//  205 //        irqno: 对应irq号                                                 *
//  206 //返  回: 无                                                               *
//  207 //说  明:                                                                  *
//  208 //-------------------------------------------------------------------------*

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
//  209 void disableuartreint(UART_MemMapPtr uartch,uint8 irqno)
//  210 {
disableuartreint:
        PUSH     {R7,LR}
//  211 	UART_C2_REG(uartch)&=~UART_C2_RIE_MASK;   //禁止UART接收中断
        LDRB     R2,[R0, #+3]
        ANDS     R2,R2,#0xDF
        STRB     R2,[R0, #+3]
//  212 	disable_irq(irqno);			  //关接收引脚的IRQ中断
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R1
        BL       disable_irq
//  213 }
        POP      {R0,PC}          ;; return

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)

        END
//  214 
//  215 
//  216 
//  217 
//  218 
//  219 
//  220 
//  221 
//  222 
//  223 
//  224 
//  225 
//  226 
//  227 
//  228 
// 
// 620 bytes in section .text
// 
// 620 bytes of CODE memory
//
//Errors: none
//Warnings: none
