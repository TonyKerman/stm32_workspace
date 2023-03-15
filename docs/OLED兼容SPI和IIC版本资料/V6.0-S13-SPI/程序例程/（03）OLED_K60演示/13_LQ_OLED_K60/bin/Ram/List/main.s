///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.10.1.52143/W32 for ARM    24/Feb/2012  22:08:39 /
// Copyright 1999-2010 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode     =  thumb                                                   /
//    Endian       =  little                                                  /
//    Source file  =  E:\kinetis_IAR\13_LQ_OLED_K60\src\Sources\C\main.c      /
//    Command line =  E:\kinetis_IAR\13_LQ_OLED_K60\src\Sources\C\main.c -D   /
//                    IAR -D TWR_K60N512 -lCN E:\kinetis_IAR\13_LQ_OLED_K60\b /
//                    in\Ram\List\ -lB E:\kinetis_IAR\13_LQ_OLED_K60\bin\Ram\ /
//                    List\ -o E:\kinetis_IAR\13_LQ_OLED_K60\bin\Ram\Obj\     /
//                    --no_cse --no_unroll --no_inline --no_code_motion       /
//                    --no_tbaa --no_clustering --no_scheduling --debug       /
//                    --endian=little --cpu=Cortex-M4 -e --fpu=None           /
//                    --dlib_config "C:\Program Files (x86)\IAR               /
//                    Systems\Embedded Workbench                              /
//                    6.0\arm\INC\c\DLib_Config_Normal.h" -I                  /
//                    E:\kinetis_IAR\13_LQ_OLED_K60\src\Sources\H\ -I         /
//                    E:\kinetis_IAR\13_LQ_OLED_K60\src\Sources\H\Component_H /
//                    \ -I E:\kinetis_IAR\13_LQ_OLED_K60\src\Sources\H\Frame_ /
//                    H\ -Ol --use_c++_inline                                 /
//    List file    =  E:\kinetis_IAR\13_LQ_OLED_K60\bin\Ram\List\main.s       /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME main

        EXTERN Draw_BMP
        EXTERN LCD_CLS
        EXTERN LCD_Fill
        EXTERN LCD_Init
        EXTERN LCD_P6x8Str
        EXTERN LCD_P8x16Str
        EXTERN LCD_Print
        EXTERN light_init
        EXTERN longqiu96x64

        PUBLIC Dly_ms
        PUBLIC main

// E:\kinetis_IAR\13_LQ_OLED_K60\src\Sources\C\main.c
//    1 /********************************************************   
//    2 【平    台】龙丘K10/60DN/DX多功能开发板
//    3 【编    写】龙丘
//    4 【Designed】by Chiu Sir
//    5 【E-mail  】chiusir@yahoo.cn
//    6 【软件版本】V1.0
//    7 【最后更新】2012年2月23日
//    8 【相关信息参考下列地址】
//    9 【网    站】http://www.lqist.cn
//   10 【淘宝店铺】http://shop36265907.taobao.com
//   11 ------------------------------------------------
//   12 【dev.env.】CodeWarrior 10.1/IAR
//   13 【Target  】K60DX/DN
//   14 【Crystal 】50.000Mhz
//   15 【busclock】???.000MHz
//   16 【pllclock】100.000MHz    
//   17 ***************************
//   18 ------------------------------------
//   19   使用说明： 
//   20 OLED电源使用3.3V。   
//   21 ----------------
//   22 G    电源地
//   23 3.3V 接3.3V电源
//   24 D0   PORTA_PA14  
//   25 D1   PORTA_PA15
//   26 RST  PORTA_PA16 
//   27 DC   PORTA_PA17
//   28 CS   已接地，不用接
//   29   
//   30  *********************************************************/
//   31 
//   32 //头文件
//   33 #include "includes.h"
//   34 #include "LQ12864.h"
//   35 
//   36 //全局变量声明

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
//   37 void Dly_ms(int ms)
//   38 {
//   39    int ii,jj;
//   40    if (ms<1) ms=1;
Dly_ms:
        CMP      R0,#+1
        BGE.N    ??Dly_ms_0
        MOVS     R0,#+1
//   41    for(ii=0;ii<ms;ii++)     
??Dly_ms_0:
        MOVS     R1,#+0
        B.N      ??Dly_ms_1
//   42      for(jj=0;jj<5341;jj++);     
??Dly_ms_2:
        ADDS     R2,R2,#+1
??Dly_ms_3:
        MOVW     R3,#+5341
        CMP      R2,R3
        BLT.N    ??Dly_ms_2
        ADDS     R1,R1,#+1
??Dly_ms_1:
        CMP      R1,R0
        BGE.N    ??Dly_ms_4
        MOVS     R2,#+0
        B.N      ??Dly_ms_3
//   43 }
??Dly_ms_4:
        BX       LR               ;; return
//   44 //主函数

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
//   45 void main(void)
//   46 {
main:
        PUSH     {R4,LR}
        SUB      SP,SP,#+8
//   47     //1 主程序使用的变量定义
//   48     uint32 i;	//运行计数器
//   49     
//   50     //2 关中断
//   51     DisableInterrupts;     //禁止总中断
        CPSID i         
//   52     
//   53     //3 模块初始化
//   54     light_init(Light_Run_PORT,Light_Run1,Light_OFF); //指示灯初始化
        MOVS     R2,#+1
        MOVS     R1,#+17
        LDR.N    R0,??DataTable5  ;; 0x400ff000
        BL       light_init
//   55     light_init(Light_Run_PORT,Light_Run2,Light_ON); //指示灯初始化
        MOVS     R2,#+0
        MOVS     R1,#+14
        LDR.N    R0,??DataTable5  ;; 0x400ff000
        BL       light_init
//   56     LCD_Init();  
        BL       LCD_Init
        B.N      ??main_0
//   57     //4 开中断
//   58 
//   59     //主循环
//   60     while(1)
//   61     {
//   62       LCD_Fill(0xff);//黑屏 
//   63       Dly_ms(1000); 
//   64       LCD_Fill(0x00);//亮屏
//   65       Dly_ms(1000);      
//   66       LCD_Print(8,0,"北京龙丘智能科技");  //汉字字符串显示
//   67       LCD_Print(15,2,"智能车首选液晶");   //汉字字符串显示
//   68       LCD_P8x16Str(48,4,"OLED");          //英文字符串显示
//   69       LCD_P6x8Str(16,6,"chiusir@yahoo.cn");//英文字符串显示 
//   70       LCD_P6x8Str(34,7,"2011-09-03");     //英文字符串显示
//   71       Dly_ms(2000);       
//   72       LCD_CLS();                          //清屏
//   73       Draw_BMP(16,0,112,7,longqiu96x64);  //图片显示
//   74       Dly_ms(2000);
//   75         
//   76      
//   77      for(i=0;i<32;i+=2)
//   78      {
//   79         LCD_CLS();
//   80         Draw_BMP(i,0,i+96,7,longqiu96x64);
//   81         Dly_ms(300);        
//   82         
//   83      }
//   84      for(i=32;i>1;i-=2)
//   85      {
//   86         LCD_CLS();
??main_1:
        BL       LCD_CLS
//   87         Draw_BMP(i,0,i+96,7,longqiu96x64);
        LDR.N    R0,??DataTable5_1
        STR      R0,[SP, #+0]
        MOVS     R3,#+7
        ADDS     R2,R4,#+96
        UXTB     R2,R2            ;; ZeroExt  R2,R2,#+24,#+24
        MOVS     R1,#+0
        MOVS     R0,R4
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        BL       Draw_BMP
//   88         Dly_ms(300);
        MOV      R0,#+300
        BL       Dly_ms
//   89         
//   90      }
        SUBS     R4,R4,#+2
??main_2:
        CMP      R4,#+2
        BCS.N    ??main_1
//   91     
//   92      Dly_ms(500);
        MOV      R0,#+500
        BL       Dly_ms
??main_0:
        MOVS     R0,#+255
        BL       LCD_Fill
        MOV      R0,#+1000
        BL       Dly_ms
        MOVS     R0,#+0
        BL       LCD_Fill
        MOV      R0,#+1000
        BL       Dly_ms
        ADR.W    R2,`?<Constant "\\261\\261\\276\\251\\301\\372\\307\\360\\3`
        MOVS     R1,#+0
        MOVS     R0,#+8
        BL       LCD_Print
        ADR.W    R2,`?<Constant "\\326\\307\\304\\334\\263\\265\\312\\327\\3`
        MOVS     R1,#+2
        MOVS     R0,#+15
        BL       LCD_Print
        ADR.W    R2,`?<Constant "OLED">`
        MOVS     R1,#+4
        MOVS     R0,#+48
        BL       LCD_P8x16Str
        ADR.W    R2,`?<Constant "chiusir@yahoo.cn">`
        MOVS     R1,#+6
        MOVS     R0,#+16
        BL       LCD_P6x8Str
        ADR.W    R2,`?<Constant "2011-09-03">`
        MOVS     R1,#+7
        MOVS     R0,#+34
        BL       LCD_P6x8Str
        MOV      R0,#+2000
        BL       Dly_ms
        BL       LCD_CLS
        LDR.N    R0,??DataTable5_1
        STR      R0,[SP, #+0]
        MOVS     R3,#+7
        MOVS     R2,#+112
        MOVS     R1,#+0
        MOVS     R0,#+16
        BL       Draw_BMP
        MOV      R0,#+2000
        BL       Dly_ms
        MOVS     R4,#+0
        B.N      ??main_3
??main_4:
        BL       LCD_CLS
        LDR.N    R0,??DataTable5_1
        STR      R0,[SP, #+0]
        MOVS     R3,#+7
        ADDS     R2,R4,#+96
        UXTB     R2,R2            ;; ZeroExt  R2,R2,#+24,#+24
        MOVS     R1,#+0
        MOVS     R0,R4
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        BL       Draw_BMP
        MOV      R0,#+300
        BL       Dly_ms
        ADDS     R4,R4,#+2
??main_3:
        CMP      R4,#+32
        BCC.N    ??main_4
        MOVS     R4,#+32
        B.N      ??main_2
//   93     }
//   94 }

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable5:
        DC32     0x400ff000

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable5_1:
        DC32     longqiu96x64

        SECTION `.text`:CODE:NOROOT(2)
        DATA
`?<Constant "\\261\\261\\276\\251\\301\\372\\307\\360\\3`:
        ; Initializer data, 20 bytes
        DC8 177, 177, 190, 169, 193, 250, 199, 240, 214, 199
        DC8 196, 220, 191, 198, 188, 188, 0, 0, 0, 0

        SECTION `.text`:CODE:NOROOT(2)
        DATA
`?<Constant "\\326\\307\\304\\334\\263\\265\\312\\327\\3`:
        ; Initializer data, 16 bytes
        DC8 214, 199, 196, 220, 179, 181, 202, 215, 209, 161
        DC8 210, 186, 190, 167, 0, 0

        SECTION `.text`:CODE:NOROOT(2)
        DATA
`?<Constant "OLED">`:
        ; Initializer data, 8 bytes
        DC8 79, 76, 69, 68, 0, 0, 0, 0

        SECTION `.text`:CODE:NOROOT(2)
        DATA
`?<Constant "chiusir@yahoo.cn">`:
        ; Initializer data, 20 bytes
        DC8 99, 104, 105, 117, 115, 105, 114, 64, 121, 97
        DC8 104, 111, 111, 46, 99, 110, 0, 0, 0, 0

        SECTION `.text`:CODE:NOROOT(2)
        DATA
`?<Constant "2011-09-03">`:
        ; Initializer data, 12 bytes
        DC8 50, 48, 49, 49, 45, 48, 57, 45, 48, 51
        DC8 0, 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)

        END
//   95 
//   96 
//   97 
//   98 
// 
// 368 bytes in section .text
// 
// 368 bytes of CODE memory
//
//Errors: none
//Warnings: none
