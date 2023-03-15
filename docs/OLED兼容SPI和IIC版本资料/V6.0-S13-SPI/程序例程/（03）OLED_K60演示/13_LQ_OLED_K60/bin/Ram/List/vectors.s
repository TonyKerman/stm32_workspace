///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.10.1.52143/W32 for ARM    07/Jan/2012  00:36:20 /
// Copyright 1999-2010 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode     =  thumb                                                   /
//    Endian       =  little                                                  /
//    Source file  =  F:\Kinetis\IARK60X256\01_LQ_light\src\Sources\C\Frame_C /
//                    \vectors.c                                              /
//    Command line =  F:\Kinetis\IARK60X256\01_LQ_light\src\Sources\C\Frame_C /
//                    \vectors.c -D IAR -D TWR_K60N512 -lCN                   /
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
//    List file    =  F:\Kinetis\IARK60X256\01_LQ_light\bin\Ram\List\vectors. /
//                    s                                                       /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME vectors

        EXTERN __BOOT_STACK_ADDRESS
        EXTERN __startup

        PUBLIC __vector_table
        PUBLIC default_isr

// F:\Kinetis\IARK60X256\01_LQ_light\src\Sources\C\Frame_C\vectors.c
//    1 //-------------------------------------------------------------------------*
//    2 // 文件名: vectors.h                                                       *
//    3 // 说  明: 中断向量表                                                      *
//    4 //-------------------------------------------------------------------------*
//    5 
//    6 #include "vectors.h"
//    7 
//    8 //中断向量表
//    9 typedef void (*vector_entry)(void);
//   10 
//   11 #pragma location = ".intvec"

        SECTION `.intvec`:CONST:REORDER:NOROOT(2)
//   12 const vector_entry  __vector_table[] = //@ ".intvec" =
__vector_table:
        DATA
        DC32 __BOOT_STACK_ADDRESS, __startup, default_isr, default_isr
        DC32 default_isr, default_isr, default_isr, default_isr, default_isr
        DC32 default_isr, default_isr, default_isr, default_isr, default_isr
        DC32 default_isr, default_isr, default_isr, default_isr, default_isr
        DC32 default_isr, default_isr, default_isr, default_isr, default_isr
        DC32 default_isr, default_isr, default_isr, default_isr, default_isr
        DC32 default_isr, default_isr, default_isr, default_isr, default_isr
        DC32 default_isr, default_isr, default_isr, default_isr, default_isr
        DC32 default_isr, default_isr, default_isr, default_isr, default_isr
        DC32 default_isr, default_isr, default_isr, default_isr, default_isr
        DC32 default_isr, default_isr, default_isr, default_isr, default_isr
        DC32 default_isr, default_isr, default_isr, default_isr, default_isr
        DC32 default_isr, default_isr, default_isr, default_isr, default_isr
        DC32 default_isr, default_isr, default_isr, default_isr, default_isr
        DC32 default_isr, default_isr, default_isr, default_isr, default_isr
        DC32 default_isr, default_isr, default_isr, default_isr, default_isr
        DC32 default_isr, default_isr, default_isr, default_isr, default_isr
        DC32 default_isr, default_isr, default_isr, default_isr, default_isr
        DC32 default_isr, default_isr, default_isr, default_isr, default_isr
        DC32 default_isr, default_isr, default_isr, default_isr, default_isr
        DC32 default_isr, default_isr, default_isr, default_isr, default_isr
        DC32 default_isr, default_isr, default_isr, default_isr, default_isr
        DC32 default_isr, default_isr, default_isr, default_isr, default_isr
        DC32 default_isr, default_isr, default_isr, default_isr, default_isr
        DC32 default_isr, default_isr, default_isr, default_isr, default_isr
        DC32 default_isr, default_isr, default_isr, default_isr, default_isr
        DC32 default_isr, default_isr, default_isr, default_isr, default_isr
        DC32 default_isr, default_isr, default_isr, default_isr, default_isr
        DC32 default_isr, default_isr, default_isr, default_isr, default_isr
        DC32 default_isr, default_isr, default_isr, default_isr, default_isr
        DC32 default_isr, default_isr, default_isr, default_isr, default_isr
        DC32 default_isr, default_isr, default_isr, default_isr, default_isr
        DC32 default_isr, default_isr, default_isr, default_isr, default_isr
        DC32 default_isr, default_isr, default_isr, default_isr, default_isr
        DC32 default_isr, default_isr, default_isr, default_isr, default_isr
        DC32 default_isr, default_isr, default_isr, default_isr, default_isr
        DC32 default_isr, default_isr, default_isr, default_isr, default_isr
        DC32 default_isr, default_isr, default_isr, default_isr, default_isr
        DC32 default_isr, default_isr, default_isr, default_isr, default_isr
        DC32 default_isr, default_isr, default_isr, default_isr, default_isr
        DC32 default_isr, default_isr, default_isr, default_isr, default_isr
        DC32 default_isr, default_isr, default_isr, default_isr, default_isr
        DC32 default_isr, default_isr, default_isr, default_isr, default_isr
        DC32 default_isr, default_isr, default_isr, default_isr, default_isr
        DC32 default_isr, default_isr, default_isr, default_isr, default_isr
        DC32 default_isr, default_isr, default_isr, default_isr, default_isr
        DC32 default_isr, default_isr, default_isr, default_isr, default_isr
        DC32 default_isr, default_isr, default_isr, default_isr, default_isr
        DC32 default_isr, default_isr, default_isr, default_isr, default_isr
        DC32 default_isr, default_isr, default_isr, default_isr, default_isr
        DC32 default_isr, default_isr, default_isr, default_isr, default_isr
        DC32 default_isr, default_isr, 0FFFFFFFFH, 0FFFFFFFFH, 0FFFFFFFFH
        DC32 0FFFFFFFEH
//   13 {
//   14    VECTOR_000,           //初始化SP
//   15    VECTOR_001,           //初始化PC
//   16    VECTOR_002,
//   17    VECTOR_003,
//   18    VECTOR_004,
//   19    VECTOR_005,
//   20    VECTOR_006,
//   21    VECTOR_007,
//   22    VECTOR_008,
//   23    VECTOR_009,
//   24    VECTOR_010,
//   25    VECTOR_011,
//   26    VECTOR_012,
//   27    VECTOR_013,
//   28    VECTOR_014,
//   29    VECTOR_015,
//   30    VECTOR_016,
//   31    VECTOR_017,
//   32    VECTOR_018,
//   33    VECTOR_019,
//   34    VECTOR_020,
//   35    VECTOR_021,
//   36    VECTOR_022,
//   37    VECTOR_023,
//   38    VECTOR_024,
//   39    VECTOR_025,
//   40    VECTOR_026,
//   41    VECTOR_027,
//   42    VECTOR_028,
//   43    VECTOR_029,
//   44    VECTOR_030,
//   45    VECTOR_031,
//   46    VECTOR_032,
//   47    VECTOR_033,
//   48    VECTOR_034,
//   49    VECTOR_035,
//   50    VECTOR_036,
//   51    VECTOR_037,
//   52    VECTOR_038,
//   53    VECTOR_039,
//   54    VECTOR_040,
//   55    VECTOR_041,
//   56    VECTOR_042,
//   57    VECTOR_043,
//   58    VECTOR_044,
//   59    VECTOR_045,
//   60    VECTOR_046,
//   61    VECTOR_047,
//   62    VECTOR_048,
//   63    VECTOR_049,
//   64    VECTOR_050,
//   65    VECTOR_051,
//   66    VECTOR_052,
//   67    VECTOR_053,
//   68    VECTOR_054,
//   69    VECTOR_055,
//   70    VECTOR_056,
//   71    VECTOR_057,
//   72    VECTOR_058,
//   73    VECTOR_059,
//   74    VECTOR_060,
//   75    VECTOR_061,
//   76    VECTOR_062,
//   77    VECTOR_063,
//   78    VECTOR_064,
//   79    VECTOR_065,
//   80    VECTOR_066,
//   81    VECTOR_067,
//   82    VECTOR_068,
//   83    VECTOR_069,
//   84    VECTOR_070,
//   85    VECTOR_071,
//   86    VECTOR_072,
//   87    VECTOR_073,
//   88    VECTOR_074,
//   89    VECTOR_075,
//   90    VECTOR_076,
//   91    VECTOR_077,
//   92    VECTOR_078,
//   93    VECTOR_079,
//   94    VECTOR_080,
//   95    VECTOR_081,
//   96    VECTOR_082,
//   97    VECTOR_083,
//   98    VECTOR_084,
//   99    VECTOR_085,
//  100    VECTOR_086,
//  101    VECTOR_087,
//  102    VECTOR_088,
//  103    VECTOR_089,
//  104    VECTOR_090,
//  105    VECTOR_091,
//  106    VECTOR_092,
//  107    VECTOR_093,
//  108    VECTOR_094,
//  109    VECTOR_095,
//  110    VECTOR_096,
//  111    VECTOR_097,
//  112    VECTOR_098,
//  113    VECTOR_099,
//  114    VECTOR_100,
//  115    VECTOR_101,
//  116    VECTOR_102,
//  117    VECTOR_103,
//  118    VECTOR_104,
//  119    VECTOR_105,
//  120    VECTOR_106,
//  121    VECTOR_107,
//  122    VECTOR_108,
//  123    VECTOR_109,
//  124    VECTOR_110,
//  125    VECTOR_111,
//  126    VECTOR_112,
//  127    VECTOR_113,
//  128    VECTOR_114,
//  129    VECTOR_115,
//  130    VECTOR_116,
//  131    VECTOR_117,
//  132    VECTOR_118,
//  133    VECTOR_119,
//  134    VECTOR_120,
//  135    VECTOR_121,
//  136    VECTOR_122,
//  137    VECTOR_123,
//  138    VECTOR_124,
//  139    VECTOR_125,
//  140    VECTOR_126,
//  141    VECTOR_127,
//  142    VECTOR_128,
//  143    VECTOR_129,
//  144    VECTOR_130,
//  145    VECTOR_131,
//  146    VECTOR_132,
//  147    VECTOR_133,
//  148    VECTOR_134,
//  149    VECTOR_135,
//  150    VECTOR_136,
//  151    VECTOR_137,
//  152    VECTOR_138,
//  153    VECTOR_139,
//  154    VECTOR_140,
//  155    VECTOR_141,
//  156    VECTOR_142,
//  157    VECTOR_143,
//  158    VECTOR_144,
//  159    VECTOR_145,
//  160    VECTOR_146,
//  161    VECTOR_147,
//  162    VECTOR_148,
//  163    VECTOR_149,
//  164    VECTOR_150,
//  165    VECTOR_151,
//  166    VECTOR_152,
//  167    VECTOR_153,
//  168    VECTOR_154,
//  169    VECTOR_155,
//  170    VECTOR_156,
//  171    VECTOR_157,
//  172    VECTOR_158,
//  173    VECTOR_159,
//  174    VECTOR_160,
//  175    VECTOR_161,
//  176    VECTOR_162,
//  177    VECTOR_163,
//  178    VECTOR_164,
//  179    VECTOR_165,
//  180    VECTOR_166,
//  181    VECTOR_167,
//  182    VECTOR_168,
//  183    VECTOR_169,
//  184    VECTOR_170,
//  185    VECTOR_171,
//  186    VECTOR_172,
//  187    VECTOR_173,
//  188    VECTOR_174,
//  189    VECTOR_175,
//  190    VECTOR_176,
//  191    VECTOR_177,
//  192    VECTOR_178,
//  193    VECTOR_179,
//  194    VECTOR_180,
//  195    VECTOR_181,
//  196    VECTOR_182,
//  197    VECTOR_183,
//  198    VECTOR_184,
//  199    VECTOR_185,
//  200    VECTOR_186,
//  201    VECTOR_187,
//  202    VECTOR_188,
//  203    VECTOR_189,
//  204    VECTOR_190,
//  205    VECTOR_191,
//  206    VECTOR_192,
//  207    VECTOR_193,
//  208    VECTOR_194,
//  209    VECTOR_195,
//  210    VECTOR_196,
//  211    VECTOR_197,
//  212    VECTOR_198,
//  213    VECTOR_199,
//  214    VECTOR_200,
//  215    VECTOR_201,
//  216    VECTOR_202,
//  217    VECTOR_203,
//  218    VECTOR_204,
//  219    VECTOR_205,
//  220    VECTOR_206,
//  221    VECTOR_207,
//  222    VECTOR_208,
//  223    VECTOR_209,
//  224    VECTOR_210,
//  225    VECTOR_211,
//  226    VECTOR_212,
//  227    VECTOR_213,
//  228    VECTOR_214,
//  229    VECTOR_215,
//  230    VECTOR_216,
//  231    VECTOR_217,
//  232    VECTOR_218,
//  233    VECTOR_219,
//  234    VECTOR_220,
//  235    VECTOR_221,
//  236    VECTOR_222,
//  237    VECTOR_223,
//  238    VECTOR_224,
//  239    VECTOR_225,
//  240    VECTOR_226,
//  241    VECTOR_227,
//  242    VECTOR_228,
//  243    VECTOR_229,
//  244    VECTOR_230,
//  245    VECTOR_231,
//  246    VECTOR_232,
//  247    VECTOR_233,
//  248    VECTOR_234,
//  249    VECTOR_235,
//  250    VECTOR_236,
//  251    VECTOR_237,
//  252    VECTOR_238,
//  253    VECTOR_239,
//  254    VECTOR_240,
//  255    VECTOR_241,
//  256    VECTOR_242,
//  257    VECTOR_243,
//  258    VECTOR_244,
//  259    VECTOR_245,
//  260    VECTOR_246,
//  261    VECTOR_247,
//  262    VECTOR_248,
//  263    VECTOR_249,
//  264    VECTOR_250,
//  265    VECTOR_251,
//  266    VECTOR_252,
//  267    VECTOR_253,
//  268    VECTOR_254,
//  269    VECTOR_255,
//  270    CONFIG_1,
//  271    CONFIG_2,
//  272    CONFIG_3,
//  273    CONFIG_4,
//  274 };
//  275 //默认中断服务例程函数

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
//  276 void default_isr(void)
//  277 {
//  278    #define VECTORNUM                     (*(volatile uint8_t*)(0xE000ED04))
//  279    return;
default_isr:
        BX       LR               ;; return
//  280 }

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)

        END
// 
// 1 040 bytes in section .intvec
//     2 bytes in section .text
// 
//     2 bytes of CODE  memory
// 1 040 bytes of CONST memory
//
//Errors: none
//Warnings: none
