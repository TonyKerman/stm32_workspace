///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.10.1.52143/W32 for ARM    07/Jan/2012  00:36:17 /
// Copyright 1999-2010 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode     =  thumb                                                   /
//    Endian       =  little                                                  /
//    Source file  =  F:\Kinetis\IARK60X256\01_LQ_light\src\Sources\C\Frame_C /
//                    \common.c                                               /
//    Command line =  F:\Kinetis\IARK60X256\01_LQ_light\src\Sources\C\Frame_C /
//                    \common.c -D IAR -D TWR_K60N512 -lCN                    /
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
//    List file    =  F:\Kinetis\IARK60X256\01_LQ_light\bin\Ram\List\common.s /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME common

        PUBLIC disable_irq
        PUBLIC enable_irq
        PUBLIC set_irq_priority
        PUBLIC stop
        PUBLIC wait
        PUBLIC write_vtor

// F:\Kinetis\IARK60X256\01_LQ_light\src\Sources\C\Frame_C\common.c
//    1 //-------------------------------------------------------------------------*
//    2 // 文件名: common.h (通用头文件)                                           *
//    3 // 说  明:                                                                 *
//    4 //-------------------------------------------------------------------------*
//    5 
//    6 #include "common.h"
//    7 
//    8 //-------------------------------------------------------------------------*
//    9 //函数名: stop                                                             *
//   10 //功  能: 设置CPU为STOP模式                                                * 
//   11 //参  数: 无								   *	
//   12 //返  回: 无                                                               *
//   13 //说  明: 无                                                               *
//   14 //-------------------------------------------------------------------------*

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
//   15 void stop (void)
//   16 {
//   17     //置位SLEEPDEEP来使能STOP模式
//   18     SCB_SCR |= SCB_SCR_SLEEPDEEP_MASK;	
stop:
        LDR.N    R0,??DataTable5  ;; 0xe000ed10
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x4
        LDR.N    R1,??DataTable5  ;; 0xe000ed10
        STR      R0,[R1, #+0]
//   19     //进入STOP模式
//   20     asm("WFI");
        WFI              
//   21 }
        BX       LR               ;; return
//   22 
//   23 //-------------------------------------------------------------------------*
//   24 //函数名: wait                                                             *
//   25 //功  能: 设置CPU为WAIT模式                                                * 
//   26 //参  数: 无								   *	
//   27 //返  回: 无                                                               *
//   28 //说  明: 无                                                               *
//   29 //-------------------------------------------------------------------------*

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
//   30 void wait (void)
//   31 {
//   32     //清SLEEPDEEP位来确定进入WAIT模式
//   33     SCB_SCR &= ~SCB_SCR_SLEEPDEEP_MASK;	
wait:
        LDR.N    R0,??DataTable5  ;; 0xe000ed10
        LDR      R0,[R0, #+0]
        BICS     R0,R0,#0x4
        LDR.N    R1,??DataTable5  ;; 0xe000ed10
        STR      R0,[R1, #+0]
//   34     //进入WAIT模式
//   35     asm("WFI");
        WFI              
//   36 }
        BX       LR               ;; return
//   37 
//   38 //-------------------------------------------------------------------------*
//   39 //函数名: write_vtor                                                       *
//   40 //功  能: 更改中断向量表偏移寄存器的值                                     * 
//   41 //参  数: 要更改的值    						   *	
//   42 //返  回: 无                                                               *
//   43 //说  明: 无                                                               *
//   44 //-------------------------------------------------------------------------*

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
//   45 void write_vtor (int vtor)
//   46 {
//   47     //写新值
//   48     SCB_VTOR = vtor;	
write_vtor:
        LDR.N    R1,??DataTable5_1  ;; 0xe000ed08
        STR      R0,[R1, #+0]
//   49 }
        BX       LR               ;; return
//   50 
//   51 //-------------------------------------------------------------------------*
//   52 //函数名: enable_irq                                                       *
//   53 //功  能: 使能irq中断                                                      * 
//   54 //参  数: irq:irq号       						   *	
//   55 //返  回: 无                                                               *
//   56 //说  明: irq号不是中断向量号                                              *
//   57 //-------------------------------------------------------------------------*

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
//   58 void enable_irq (int irq)
//   59 {
//   60     int div;
//   61 
//   62     //确定irq号为有效的irq号
//   63     if (irq > 91)	irq=91;
enable_irq:
        CMP      R0,#+92
        BLT.N    ??enable_irq_0
        MOVS     R0,#+91
//   64     
//   65     //确定对应的NVICISER
//   66     div = irq/32;
??enable_irq_0:
        MOVS     R1,#+32
        SDIV     R1,R0,R1
//   67     
//   68     switch (div)
        CMP      R1,#+0
        BEQ.N    ??enable_irq_1
        CMP      R1,#+2
        BEQ.N    ??enable_irq_2
        BCC.N    ??enable_irq_3
        B.N      ??enable_irq_4
//   69     {
//   70     	case 0x0:
//   71               NVICICPR0 = 1 << (irq%32);
??enable_irq_1:
        MOVS     R1,#+1
        MOVS     R2,#+32
        SDIV     R3,R0,R2
        MLS      R3,R3,R2,R0
        LSLS     R1,R1,R3
        LDR.N    R2,??DataTable5_2  ;; 0xe000e280
        STR      R1,[R2, #+0]
//   72               NVICISER0 = 1 << (irq%32);
        MOVS     R1,#+1
        MOVS     R2,#+32
        SDIV     R3,R0,R2
        MLS      R3,R3,R2,R0
        LSLS     R0,R1,R3
        LDR.N    R1,??DataTable5_3  ;; 0xe000e100
        STR      R0,[R1, #+0]
//   73               break;
        B.N      ??enable_irq_4
//   74     	case 0x1:
//   75               NVICICPR1 = 1 << (irq%32);
??enable_irq_3:
        MOVS     R1,#+1
        MOVS     R2,#+32
        SDIV     R3,R0,R2
        MLS      R3,R3,R2,R0
        LSLS     R1,R1,R3
        LDR.N    R2,??DataTable5_4  ;; 0xe000e284
        STR      R1,[R2, #+0]
//   76               NVICISER1 = 1 << (irq%32);
        MOVS     R1,#+1
        MOVS     R2,#+32
        SDIV     R3,R0,R2
        MLS      R3,R3,R2,R0
        LSLS     R0,R1,R3
        LDR.N    R1,??DataTable5_5  ;; 0xe000e104
        STR      R0,[R1, #+0]
//   77               break;
        B.N      ??enable_irq_4
//   78     	case 0x2:
//   79               NVICICPR2 = 1 << (irq%32);
??enable_irq_2:
        MOVS     R1,#+1
        MOVS     R2,#+32
        SDIV     R3,R0,R2
        MLS      R3,R3,R2,R0
        LSLS     R1,R1,R3
        LDR.N    R2,??DataTable5_6  ;; 0xe000e288
        STR      R1,[R2, #+0]
//   80               NVICISER2 = 1 << (irq%32);
        MOVS     R1,#+1
        MOVS     R2,#+32
        SDIV     R3,R0,R2
        MLS      R3,R3,R2,R0
        LSLS     R0,R1,R3
        LDR.N    R1,??DataTable5_7  ;; 0xe000e108
        STR      R0,[R1, #+0]
//   81               break;
//   82     }              
//   83 }
??enable_irq_4:
        BX       LR               ;; return
//   84 
//   85 //-------------------------------------------------------------------------*
//   86 //函数名: disable_irq                                                      *
//   87 //功  能: 禁止irq中断                                                      * 
//   88 //参  数: irq:irq号       						   *	
//   89 //返  回: 无                                                               *
//   90 //说  明: irq号不是中断向量号                                              *
//   91 //-------------------------------------------------------------------------*

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
//   92 void disable_irq (int irq)
//   93 {
//   94     int div;
//   95     
//   96     //确定irq号为有效的irq号
//   97     if (irq > 91)	irq=91;
disable_irq:
        CMP      R0,#+92
        BLT.N    ??disable_irq_0
        MOVS     R0,#+91
//   98     
//   99     //确定对应的NVICISER
//  100     div = irq/32;
??disable_irq_0:
        MOVS     R1,#+32
        SDIV     R1,R0,R1
//  101     
//  102     switch (div)
        CMP      R1,#+0
        BEQ.N    ??disable_irq_1
        CMP      R1,#+2
        BEQ.N    ??disable_irq_2
        BCC.N    ??disable_irq_3
        B.N      ??disable_irq_4
//  103     {
//  104     	case 0x0:
//  105                NVICICER0 = 1 << (irq%32);
??disable_irq_1:
        MOVS     R1,#+1
        MOVS     R2,#+32
        SDIV     R3,R0,R2
        MLS      R3,R3,R2,R0
        LSLS     R0,R1,R3
        LDR.N    R1,??DataTable5_8  ;; 0xe000e180
        STR      R0,[R1, #+0]
//  106               break;
        B.N      ??disable_irq_4
//  107     	case 0x1:
//  108               NVICICER1 = 1 << (irq%32);
??disable_irq_3:
        MOVS     R1,#+1
        MOVS     R2,#+32
        SDIV     R3,R0,R2
        MLS      R3,R3,R2,R0
        LSLS     R0,R1,R3
        LDR.N    R1,??DataTable5_9  ;; 0xe000e184
        STR      R0,[R1, #+0]
//  109               break;
        B.N      ??disable_irq_4
//  110     	case 0x2:
//  111               NVICICER2 = 1 << (irq%32);
??disable_irq_2:
        MOVS     R1,#+1
        MOVS     R2,#+32
        SDIV     R3,R0,R2
        MLS      R3,R3,R2,R0
        LSLS     R0,R1,R3
        LDR.N    R1,??DataTable5_10  ;; 0xe000e188
        STR      R0,[R1, #+0]
//  112               break;
//  113     }              
//  114 }
??disable_irq_4:
        BX       LR               ;; return
//  115  
//  116 //-------------------------------------------------------------------------*
//  117 //函数名: set_irq_priority                                                 *
//  118 //功  能: 设置irq中断和优先级                                              * 
//  119 //参  数: irq:irq号         						   *	
//  120 //        prio:优先级						           *	
//  121 //返  回: 无                                                               *
//  122 //说  明: irq号不是中断向量号                                              *
//  123 //-------------------------------------------------------------------------*

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
//  124 void set_irq_priority (int irq, int prio)
//  125 {
//  126     uint8 *prio_reg;
//  127 
//  128     //确定irq号和优先级有效
//  129     if (irq > 91)	irq=91;
set_irq_priority:
        CMP      R0,#+92
        BLT.N    ??set_irq_priority_0
        MOVS     R0,#+91
//  130     if (prio > 15)	prio=15;
??set_irq_priority_0:
        CMP      R1,#+16
        BLT.N    ??set_irq_priority_1
        MOVS     R1,#+15
//  131 
//  132     //确定对应的NVICISER
//  133     prio_reg = (uint8 *)(((uint32)&NVICIP0) + irq);
??set_irq_priority_1:
        LDR.N    R2,??DataTable5_11  ;; 0xe000e400
        ADDS     R0,R2,R0
//  134     //设置优先级
//  135     *prio_reg = ( (prio&0xF) << (8 - ARM_INTERRUPT_LEVEL_BITS) );             
        LSLS     R1,R1,#+4
        STRB     R1,[R0, #+0]
//  136 }
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable5:
        DC32     0xe000ed10

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable5_1:
        DC32     0xe000ed08

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable5_2:
        DC32     0xe000e280

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable5_3:
        DC32     0xe000e100

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable5_4:
        DC32     0xe000e284

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable5_5:
        DC32     0xe000e104

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable5_6:
        DC32     0xe000e288

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable5_7:
        DC32     0xe000e108

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable5_8:
        DC32     0xe000e180

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable5_9:
        DC32     0xe000e184

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable5_10:
        DC32     0xe000e188

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable5_11:
        DC32     0xe000e400

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)

        END
//  137 
// 
// 342 bytes in section .text
// 
// 342 bytes of CODE memory
//
//Errors: none
//Warnings: none
