///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.10.1.52143/W32 for ARM    16/Apr/2011  16:44:06 /
// Copyright 1999-2010 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode     =  thumb                                                   /
//    Endian       =  little                                                  /
//    Source file  =  E:\Project\15_K60\03_Software\02_My                     /
//                    program\K60_IAR\01_light\src\drivers\mcg\mcg.c          /
//    Command line =  "E:\Project\15_K60\03_Software\02_My                    /
//                    program\K60_IAR\01_light\src\drivers\mcg\mcg.c" -D IAR  /
//                    -D TWR_K60N512 -lCN "E:\Project\15_K60\03_Software\02_M /
//                    y program\K60_IAR\01_light\bin\Flash\List\" -lB         /
//                    "E:\Project\15_K60\03_Software\02_My                    /
//                    program\K60_IAR\01_light\bin\Flash\List\" -o            /
//                    "E:\Project\15_K60\03_Software\02_My                    /
//                    program\K60_IAR\01_light\bin\Flash\Obj\" --no_cse       /
//                    --no_unroll --no_inline --no_code_motion --no_tbaa      /
//                    --no_clustering --no_scheduling --debug                 /
//                    --endian=little --cpu=Cortex-M4 -e --fpu=None           /
//                    --dlib_config D:\iar\arm\INC\c\DLib_Config_Normal.h -I  /
//                    "E:\Project\15_K60\03_Software\02_My                    /
//                    program\K60_IAR\01_light\src\common\" -I                /
//                    "E:\Project\15_K60\03_Software\02_My                    /
//                    program\K60_IAR\01_light\src\cpu\" -I                   /
//                    "E:\Project\15_K60\03_Software\02_My                    /
//                    program\K60_IAR\01_light\src\cpu\headers\" -I           /
//                    "E:\Project\15_K60\03_Software\02_My                    /
//                    program\K60_IAR\01_light\src\drivers\" -I               /
//                    "E:\Project\15_K60\03_Software\02_My                    /
//                    program\K60_IAR\01_light\src\drivers\uart\" -I          /
//                    "E:\Project\15_K60\03_Software\02_My                    /
//                    program\K60_IAR\01_light\src\drivers\gpio\" -I          /
//                    "E:\Project\15_K60\03_Software\02_My                    /
//                    program\K60_IAR\01_light\src\drivers\mcg\" -I           /
//                    "E:\Project\15_K60\03_Software\02_My                    /
//                    program\K60_IAR\01_light\src\drivers\wdog\" -I          /
//                    "E:\Project\15_K60\03_Software\02_My                    /
//                    program\K60_IAR\01_light\src\projects\" -I              /
//                    "E:\Project\15_K60\03_Software\02_My                    /
//                    program\K60_IAR\01_light\src\" -Ol --use_c++_inline     /
//    List file    =  E:\Project\15_K60\03_Software\02_My                     /
//                    program\K60_IAR\01_light\bin\Flash\List\mcg.s           /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME mcg

        #define SHT_PROGBITS 0x1
        #define SHF_WRITE 0x1
        #define SHF_EXECINSTR 0x4

        EXTERN core_clk_khz
        EXTERN core_clk_mhz
        EXTERN periph_clk_khz

        PUBLIC mcg_blpi_2_pee
        PUBLIC mcg_pbe_2_pee
        PUBLIC mcg_pee_2_blpi
        PUBLIC pll_init
        PUBLIC set_sys_dividers

// E:\Project\15_K60\03_Software\02_My program\K60_IAR\01_light\src\drivers\mcg\mcg.c
//    1 /*
//    2  * File:    mcg.c
//    3  * Purpose: Driver for enabling the PLL in 1 of 4 options
//    4  *
//    5  * Notes:
//    6  * Assumes the MCG mode is in the default FEI mode out of reset
//    7  * One of 4 clocking oprions can be selected.
//    8  * One of 16 crystal values can be used
//    9  */
//   10 
//   11 #include "common.h"
//   12 #include "mcg.h"
//   13 
//   14 extern int core_clk_khz;
//   15 extern int core_clk_mhz;
//   16 extern int periph_clk_khz;
//   17 

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
//   18 unsigned char pll_init(unsigned char clk_option, unsigned char crystal_val)
//   19 {
pll_init:
        PUSH     {R7,LR}
        MOVS     R2,R1
//   20   unsigned char pll_freq;
//   21 
//   22   if (clk_option > 3) {return 0;} //return 0 if one of the available options is not selected
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        CMP      R0,#+4
        BCC.N    ??pll_init_0
        MOVS     R0,#+0
        B.N      ??pll_init_1
//   23   if (crystal_val > 15) {return 1;} // return 1 if one of the available crystal options is not available
??pll_init_0:
        UXTB     R2,R2            ;; ZeroExt  R2,R2,#+24,#+24
        CMP      R2,#+16
        BCC.N    ??pll_init_2
        MOVS     R0,#+1
        B.N      ??pll_init_1
//   24 //This assumes that the MCG is in default FEI mode out of reset.
//   25 
//   26 // First move to FBE mode
//   27 #if (defined(K60_CLK) || defined(ASB817))
//   28      MCG_C2 = 0;
??pll_init_2:
        LDR.N    R2,??DataTable3  ;; 0x40064001
        MOVS     R3,#+0
        STRB     R3,[R2, #+0]
//   29 #else
//   30 // Enable external oscillator, RANGE=2, HGO=1, EREFS=1, LP=0, IRCS=0
//   31     MCG_C2 = MCG_C2_RANGE(2) | MCG_C2_HGO_MASK | MCG_C2_EREFS_MASK;
//   32 #endif
//   33 
//   34 // after initialization of oscillator release latched state of oscillator and GPIO
//   35     SIM_SCGC4 |= SIM_SCGC4_LLWU_MASK;
        LDR.N    R2,??DataTable3_1  ;; 0x40048034
        LDR      R2,[R2, #+0]
        ORRS     R2,R2,#0x10000000
        LDR.N    R3,??DataTable3_1  ;; 0x40048034
        STR      R2,[R3, #+0]
//   36     LLWU_CS |= LLWU_CS_ACKISO_MASK;
        LDR.N    R2,??DataTable3_2  ;; 0x4007c008
        LDRB     R2,[R2, #+0]
        ORRS     R2,R2,#0x80
        LDR.N    R3,??DataTable3_2  ;; 0x4007c008
        STRB     R2,[R3, #+0]
//   37   
//   38 // Select external oscilator and Reference Divider and clear IREFS to start ext osc
//   39 // CLKS=2, FRDIV=3, IREFS=0, IRCLKEN=0, IREFSTEN=0
//   40   MCG_C1 = MCG_C1_CLKS(2) | MCG_C1_FRDIV(3);
        LDR.N    R2,??DataTable3_3  ;; 0x40064000
        MOVS     R3,#+152
        STRB     R3,[R2, #+0]
//   41 
//   42   /* if we aren't using an osc input we don't need to wait for the osc to init */
//   43 #if (!defined(K60_CLK) && !defined(ASB817))
//   44     while (!(MCG_S & MCG_S_OSCINIT_MASK)){};  // wait for oscillator to initialize
//   45 #endif
//   46 
//   47   while (MCG_S & MCG_S_IREFST_MASK){}; // wait for Reference clock Status bit to clear
??pll_init_3:
        LDR.N    R2,??DataTable3_4  ;; 0x40064006
        LDRB     R2,[R2, #+0]
        LSLS     R2,R2,#+27
        BMI.N    ??pll_init_3
//   48 
//   49   while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x2){}; // Wait for clock status bits to show clock source is ext ref clk
??pll_init_4:
        LDR.N    R2,??DataTable3_4  ;; 0x40064006
        LDRB     R2,[R2, #+0]
        UBFX     R2,R2,#+2,#+2
        UXTB     R2,R2            ;; ZeroExt  R2,R2,#+24,#+24
        CMP      R2,#+2
        BNE.N    ??pll_init_4
//   50 
//   51 // Now in FBE
//   52 
//   53 #if (defined(K60_CLK))
//   54    MCG_C5 = MCG_C5_PRDIV(0x18);
        LDR.N    R2,??DataTable3_5  ;; 0x40064004
        MOVS     R3,#+24
        STRB     R3,[R2, #+0]
//   55 #else
//   56 // Configure PLL Ref Divider, PLLCLKEN=0, PLLSTEN=0, PRDIV=5
//   57 // The crystal frequency is used to select the PRDIV value. Only even frequency crystals are supported
//   58 // that will produce a 2MHz reference clock to the PLL.
//   59   MCG_C5 = MCG_C5_PRDIV(crystal_val); // Set PLL ref divider to match the crystal used
//   60 #endif
//   61 
//   62   // Ensure MCG_C6 is at the reset default of 0. LOLIE disabled, PLL disabled, clk monitor disabled, PLL VCO divider is clear
//   63   MCG_C6 = 0x0;
        LDR.N    R2,??DataTable3_6  ;; 0x40064005
        MOVS     R3,#+0
        STRB     R3,[R2, #+0]
//   64 // Select the PLL VCO divider and system clock dividers depending on clocking option
//   65   switch (clk_option) {
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        CMP      R0,#+0
        BEQ.N    ??pll_init_5
        CMP      R0,#+2
        BEQ.N    ??pll_init_6
        BCC.N    ??pll_init_7
        CMP      R0,#+3
        BEQ.N    ??pll_init_8
        B.N      ??pll_init_9
//   66     case 0:
//   67       // Set system options dividers
//   68       //MCG=PLL, core = MCG, bus = MCG, FlexBus = MCG, Flash clock= MCG/2
//   69       set_sys_dividers(0,0,0,1);
??pll_init_5:
        MOVS     R3,#+1
        MOVS     R2,#+0
        MOVS     R1,#+0
        MOVS     R0,#+0
        BL       set_sys_dividers
//   70       // Set the VCO divider and enable the PLL for 50MHz, LOLIE=0, PLLS=1, CME=0, VDIV=1
//   71       MCG_C6 = MCG_C6_PLLS_MASK | MCG_C6_VDIV(1); //VDIV = 1 (x25)
        LDR.N    R0,??DataTable3_6  ;; 0x40064005
        MOVS     R1,#+65
        STRB     R1,[R0, #+0]
//   72       pll_freq = 50;
        MOVS     R1,#+50
//   73       break;
        B.N      ??pll_init_9
//   74    case 1:
//   75       // Set system options dividers
//   76       //MCG=PLL, core = MCG, bus = MCG/2, FlexBus = MCG/2, Flash clock= MCG/4
//   77      set_sys_dividers(0,1,1,3);
??pll_init_7:
        MOVS     R3,#+3
        MOVS     R2,#+1
        MOVS     R1,#+1
        MOVS     R0,#+0
        BL       set_sys_dividers
//   78       // Set the VCO divider and enable the PLL for 100MHz, LOLIE=0, PLLS=1, CME=0, VDIV=26
//   79       MCG_C6 = MCG_C6_PLLS_MASK | MCG_C6_VDIV(26); //VDIV = 26 (x50)
        LDR.N    R0,??DataTable3_6  ;; 0x40064005
        MOVS     R1,#+90
        STRB     R1,[R0, #+0]
//   80       pll_freq = 100;
        MOVS     R1,#+100
//   81       break;
        B.N      ??pll_init_9
//   82     case 2:
//   83       // Set system options dividers
//   84       //MCG=PLL, core = MCG, bus = MCG/2, FlexBus = MCG/2, Flash clock= MCG/4
//   85       set_sys_dividers(0,1,1,3);
??pll_init_6:
        MOVS     R3,#+3
        MOVS     R2,#+1
        MOVS     R1,#+1
        MOVS     R0,#+0
        BL       set_sys_dividers
//   86       // Set the VCO divider and enable the PLL for 96MHz, LOLIE=0, PLLS=1, CME=0, VDIV=24
//   87       MCG_C6 = MCG_C6_PLLS_MASK | MCG_C6_VDIV(24); //VDIV = 24 (x48)
        LDR.N    R0,??DataTable3_6  ;; 0x40064005
        MOVS     R1,#+88
        STRB     R1,[R0, #+0]
//   88       pll_freq = 96;
        MOVS     R1,#+96
//   89       break;
        B.N      ??pll_init_9
//   90    case 3:
//   91       // Set system options dividers
//   92       //MCG=PLL, core = MCG, bus = MCG, FlexBus = MCG, Flash clock= MCG/2
//   93       set_sys_dividers(0,0,0,1);
??pll_init_8:
        MOVS     R3,#+1
        MOVS     R2,#+0
        MOVS     R1,#+0
        MOVS     R0,#+0
        BL       set_sys_dividers
//   94       // Set the VCO divider and enable the PLL for 48MHz, LOLIE=0, PLLS=1, CME=0, VDIV=0
//   95       MCG_C6 = MCG_C6_PLLS_MASK; //VDIV = 0 (x24)
        LDR.N    R0,??DataTable3_6  ;; 0x40064005
        MOVS     R1,#+64
        STRB     R1,[R0, #+0]
//   96       pll_freq = 48;
        MOVS     R1,#+48
//   97       break;
//   98   }
//   99   while (!(MCG_S & MCG_S_PLLST_MASK)){}; // wait for PLL status bit to set
??pll_init_9:
        LDR.N    R0,??DataTable3_4  ;; 0x40064006
        LDRB     R0,[R0, #+0]
        LSLS     R0,R0,#+26
        BPL.N    ??pll_init_9
//  100 
//  101   while (!(MCG_S & MCG_S_LOCK_MASK)){}; // Wait for LOCK bit to set
??pll_init_10:
        LDR.N    R0,??DataTable3_4  ;; 0x40064006
        LDRB     R0,[R0, #+0]
        LSLS     R0,R0,#+25
        BPL.N    ??pll_init_10
//  102 
//  103 // Now running PBE Mode
//  104 
//  105 // Transition into PEE by setting CLKS to 0
//  106 // CLKS=0, FRDIV=3, IREFS=0, IRCLKEN=0, IREFSTEN=0
//  107   MCG_C1 &= ~MCG_C1_CLKS_MASK;
        LDR.N    R0,??DataTable3_3  ;; 0x40064000
        LDRB     R0,[R0, #+0]
        ANDS     R0,R0,#0x3F
        LDR.N    R2,??DataTable3_3  ;; 0x40064000
        STRB     R0,[R2, #+0]
//  108 
//  109 // Wait for clock status bits to update
//  110   while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x3){};
??pll_init_11:
        LDR.N    R0,??DataTable3_4  ;; 0x40064006
        LDRB     R0,[R0, #+0]
        UBFX     R0,R0,#+2,#+2
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        CMP      R0,#+3
        BNE.N    ??pll_init_11
//  111 
//  112 // Now running PEE Mode
//  113 
//  114 return pll_freq;
        MOVS     R0,R1
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
??pll_init_1:
        POP      {R1,PC}          ;; return
//  115 } //pll_init
//  116 

        SECTION `.textrw`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, SHF_WRITE | SHF_EXECINSTR
        THUMB
//  117 __ramfunc void set_sys_dividers(uint32 outdiv1, uint32 outdiv2, uint32 outdiv3, uint32 outdiv4)
//  118 {
set_sys_dividers:
        PUSH     {R4-R6}
//  119  /*
//  120   * This routine must be placed in RAM. It is a workaround for errata e2448.
//  121   * Flash prefetch must be disabled when the flash clock divider is changed.
//  122   * This cannot be performed while executing out of flash.
//  123   * There must be a short delay after the clock dividers are changed before prefetch
//  124   * can be re-enabled.
//  125   */
//  126   uint32 temp_reg;
//  127   uint8 i;
//  128   
//  129   temp_reg = FMC_PFAPR; // store present value of FMC_PFAPR
        LDR.N    R4,??set_sys_dividers_0  ;; 0x4001f000
        LDR      R4,[R4, #+0]
//  130   
//  131   // set M0PFD through M7PFD to 1 to disable prefetch
//  132   FMC_PFAPR |= FMC_PFAPR_M7PFD_MASK | FMC_PFAPR_M6PFD_MASK | FMC_PFAPR_M5PFD_MASK
//  133              | FMC_PFAPR_M4PFD_MASK | FMC_PFAPR_M3PFD_MASK | FMC_PFAPR_M2PFD_MASK
//  134              | FMC_PFAPR_M1PFD_MASK | FMC_PFAPR_M0PFD_MASK;
        LDR.N    R5,??set_sys_dividers_0  ;; 0x4001f000
        LDR      R5,[R5, #+0]
        ORRS     R5,R5,#0xFF0000
        LDR.N    R6,??set_sys_dividers_0  ;; 0x4001f000
        STR      R5,[R6, #+0]
//  135   
//  136   // set clock dividers to desired value  
//  137   SIM_CLKDIV1 = SIM_CLKDIV1_OUTDIV1(outdiv1) | SIM_CLKDIV1_OUTDIV2(outdiv2) 
//  138               | SIM_CLKDIV1_OUTDIV3(outdiv3) | SIM_CLKDIV1_OUTDIV4(outdiv4);
        LSLS     R1,R1,#+24
        ANDS     R1,R1,#0xF000000
        ORRS     R0,R1,R0, LSL #+28
        LSLS     R1,R2,#+20
        ANDS     R1,R1,#0xF00000
        ORRS     R0,R1,R0
        LSLS     R1,R3,#+16
        ANDS     R1,R1,#0xF0000
        ORRS     R0,R1,R0
        LDR.N    R1,??set_sys_dividers_0+0x4  ;; 0x40048044
        STR      R0,[R1, #+0]
//  139 
//  140   // wait for dividers to change
//  141   for (i = 0 ; i < outdiv4 ; i++)
        MOVS     R0,#+0
        B.N      ??set_sys_dividers_1
??set_sys_dividers_2:
        ADDS     R0,R0,#+1
??set_sys_dividers_1:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        CMP      R0,R3
        BCC.N    ??set_sys_dividers_2
//  142   {}
//  143   
//  144   FMC_PFAPR = temp_reg; // re-store original value of FMC_PFAPR
        LDR.N    R0,??set_sys_dividers_0  ;; 0x4001f000
        STR      R4,[R0, #+0]
//  145   
//  146   return;
        POP      {R4-R6}
        BX       LR               ;; return
        DATA
??set_sys_dividers_0:
        DC32     0x4001f000
        DC32     0x40048044
//  147 } // set_sys_dividers
//  148 
//  149 
//  150 /********************************************************************/

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
//  151 void mcg_pee_2_blpi(void)
//  152 {
//  153     uint8 temp_reg;
//  154     // Transition from PEE to BLPI: PEE -> PBE -> FBE -> FBI -> BLPI
//  155   
//  156     // Step 1: PEE -> PBE
//  157     MCG_C1 |= MCG_C1_CLKS(2);  // System clock from external reference OSC, not PLL.
mcg_pee_2_blpi:
        LDR.N    R0,??DataTable3_3  ;; 0x40064000
        LDRB     R0,[R0, #+0]
        ORRS     R0,R0,#0x80
        LDR.N    R1,??DataTable3_3  ;; 0x40064000
        STRB     R0,[R1, #+0]
//  158     while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x2){};  // Wait for clock status to update.
??mcg_pee_2_blpi_0:
        LDR.N    R0,??DataTable3_4  ;; 0x40064006
        LDRB     R0,[R0, #+0]
        UBFX     R0,R0,#+2,#+2
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        CMP      R0,#+2
        BNE.N    ??mcg_pee_2_blpi_0
//  159     
//  160     // Step 2: PBE -> FBE
//  161     MCG_C6 &= ~MCG_C6_PLLS_MASK;  // Clear PLLS to select FLL, still running system from ext OSC.
        LDR.N    R0,??DataTable3_6  ;; 0x40064005
        LDRB     R0,[R0, #+0]
        ANDS     R0,R0,#0xBF
        LDR.N    R1,??DataTable3_6  ;; 0x40064005
        STRB     R0,[R1, #+0]
//  162     while (MCG_S & MCG_S_PLLST_MASK){};  // Wait for PLL status flag to reflect FLL selected.
??mcg_pee_2_blpi_1:
        LDR.N    R0,??DataTable3_4  ;; 0x40064006
        LDRB     R0,[R0, #+0]
        LSLS     R0,R0,#+26
        BMI.N    ??mcg_pee_2_blpi_1
//  163     
//  164     // Step 3: FBE -> FBI
//  165     MCG_C2 &= ~MCG_C2_LP_MASK;  // FLL remains active in bypassed modes.
        LDR.N    R0,??DataTable3  ;; 0x40064001
        LDRB     R0,[R0, #+0]
        ANDS     R0,R0,#0xFD
        LDR.N    R1,??DataTable3  ;; 0x40064001
        STRB     R0,[R1, #+0]
//  166     MCG_C2 |= MCG_C2_IRCS_MASK;  // Select fast (1MHz) internal reference
        LDR.N    R0,??DataTable3  ;; 0x40064001
        LDRB     R0,[R0, #+0]
        ORRS     R0,R0,#0x1
        LDR.N    R1,??DataTable3  ;; 0x40064001
        STRB     R0,[R1, #+0]
//  167     temp_reg = MCG_C1;
        LDR.N    R0,??DataTable3_3  ;; 0x40064000
        LDRB     R0,[R0, #+0]
//  168     temp_reg &= ~(MCG_C1_CLKS_MASK | MCG_C1_IREFS_MASK);
        ANDS     R0,R0,#0x3B
//  169     temp_reg |= (MCG_C1_CLKS(1) | MCG_C1_IREFS_MASK);  // Select internal reference (fast IREF clock @ 1MHz) as MCG clock source.
        ORRS     R0,R0,#0x44
//  170     MCG_C1 = temp_reg;
        LDR.N    R1,??DataTable3_3  ;; 0x40064000
        STRB     R0,[R1, #+0]
//  171   
//  172     while (MCG_S & MCG_S_IREFST_MASK){};  // Wait for Reference Status bit to update.
??mcg_pee_2_blpi_2:
        LDR.N    R0,??DataTable3_4  ;; 0x40064006
        LDRB     R0,[R0, #+0]
        LSLS     R0,R0,#+27
        BMI.N    ??mcg_pee_2_blpi_2
//  173     while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x1){};  // Wait for clock status bits to update
??mcg_pee_2_blpi_3:
        LDR.N    R0,??DataTable3_4  ;; 0x40064006
        LDRB     R0,[R0, #+0]
        UBFX     R0,R0,#+2,#+2
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        CMP      R0,#+1
        BNE.N    ??mcg_pee_2_blpi_3
//  174     
//  175     // Step 4: FBI -> BLPI
//  176     MCG_C1 |= MCG_C1_IREFSTEN_MASK;  // Keep internal reference clock running in STOP modes.
        LDR.N    R0,??DataTable3_3  ;; 0x40064000
        LDRB     R0,[R0, #+0]
        ORRS     R0,R0,#0x1
        LDR.N    R1,??DataTable3_3  ;; 0x40064000
        STRB     R0,[R1, #+0]
//  177     MCG_C2 |= MCG_C2_LP_MASK;  // FLL remains disabled in bypassed modes.
        LDR.N    R0,??DataTable3  ;; 0x40064001
        LDRB     R0,[R0, #+0]
        ORRS     R0,R0,#0x2
        LDR.N    R1,??DataTable3  ;; 0x40064001
        STRB     R0,[R1, #+0]
//  178     while (!(MCG_S & MCG_S_IREFST_MASK)){};  // Wait for Reference Status bit to update.
??mcg_pee_2_blpi_4:
        LDR.N    R0,??DataTable3_4  ;; 0x40064006
        LDRB     R0,[R0, #+0]
        LSLS     R0,R0,#+27
        BPL.N    ??mcg_pee_2_blpi_4
//  179     while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x1){};  // Wait for clock status bits to update.
??mcg_pee_2_blpi_5:
        LDR.N    R0,??DataTable3_4  ;; 0x40064006
        LDRB     R0,[R0, #+0]
        UBFX     R0,R0,#+2,#+2
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        CMP      R0,#+1
        BNE.N    ??mcg_pee_2_blpi_5
//  180   
//  181 } // end MCG PEE to BLPI
        BX       LR               ;; return
//  182 /********************************************************************/

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
//  183 void mcg_blpi_2_pee(void)
//  184 {
mcg_blpi_2_pee:
        PUSH     {R7,LR}
//  185     uint8 temp_reg;
//  186     // Transition from BLPI to PEE: BLPI -> FBI -> FEI -> FBE -> PBE -> PEE
//  187   
//  188     // Step 1: BLPI -> FBI
//  189     MCG_C2 &= ~MCG_C2_LP_MASK;  // FLL remains active in bypassed modes.
        LDR.N    R0,??DataTable3  ;; 0x40064001
        LDRB     R0,[R0, #+0]
        ANDS     R0,R0,#0xFD
        LDR.N    R1,??DataTable3  ;; 0x40064001
        STRB     R0,[R1, #+0]
//  190     while (!(MCG_S & MCG_S_IREFST_MASK)){};  // Wait for Reference Status bit to update.
??mcg_blpi_2_pee_0:
        LDR.N    R0,??DataTable3_4  ;; 0x40064006
        LDRB     R0,[R0, #+0]
        LSLS     R0,R0,#+27
        BPL.N    ??mcg_blpi_2_pee_0
//  191     while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x1){};  // Wait for clock status bits to update
??mcg_blpi_2_pee_1:
        LDR.N    R0,??DataTable3_4  ;; 0x40064006
        LDRB     R0,[R0, #+0]
        UBFX     R0,R0,#+2,#+2
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        CMP      R0,#+1
        BNE.N    ??mcg_blpi_2_pee_1
//  192     
//  193     // Step 2: FBI -> FEI
//  194     MCG_C2 &= ~MCG_C2_LP_MASK;  // FLL remains active in bypassed modes.
        LDR.N    R0,??DataTable3  ;; 0x40064001
        LDRB     R0,[R0, #+0]
        ANDS     R0,R0,#0xFD
        LDR.N    R1,??DataTable3  ;; 0x40064001
        STRB     R0,[R1, #+0]
//  195     temp_reg = MCG_C2;  // assign temporary variable of MCG_C2 contents
        LDR.N    R0,??DataTable3  ;; 0x40064001
        LDRB     R0,[R0, #+0]
//  196     temp_reg &= ~MCG_C2_RANGE_MASK;  // set RANGE field location to zero
        ANDS     R0,R0,#0xCF
//  197     temp_reg |= (0x2 << 0x4);  // OR in new values
        ORRS     R0,R0,#0x20
//  198     MCG_C2 = temp_reg;  // store new value in MCG_C2
        LDR.N    R1,??DataTable3  ;; 0x40064001
        STRB     R0,[R1, #+0]
//  199     MCG_C4 = 0x0E;  // Low-range DCO output (~10MHz bus).  FCTRIM=%0111.
        LDR.N    R0,??DataTable3_7  ;; 0x40064003
        MOVS     R1,#+14
        STRB     R1,[R0, #+0]
//  200     MCG_C1 = 0x04;  // Select internal clock as MCG source, FRDIV=%000, internal reference selected.
        LDR.N    R0,??DataTable3_3  ;; 0x40064000
        MOVS     R1,#+4
        STRB     R1,[R0, #+0]
//  201  
//  202     while (!(MCG_S & MCG_S_IREFST_MASK)){};   // Wait for Reference Status bit to update 
??mcg_blpi_2_pee_2:
        LDR.N    R0,??DataTable3_4  ;; 0x40064006
        LDRB     R0,[R0, #+0]
        LSLS     R0,R0,#+27
        BPL.N    ??mcg_blpi_2_pee_2
//  203     while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x0){}; // Wait for clock status bits to update
??mcg_blpi_2_pee_3:
        LDR.N    R0,??DataTable3_4  ;; 0x40064006
        LDRB     R0,[R0, #+0]
        UBFX     R0,R0,#+2,#+2
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        CMP      R0,#+0
        BNE.N    ??mcg_blpi_2_pee_3
//  204     
//  205     // Handle FEI to PEE transitions using standard clock initialization routine.
//  206     core_clk_mhz = pll_init(CORE_CLK_MHZ, REF_CLK); 
        MOVS     R1,#+3
        MOVS     R0,#+2
        BL       pll_init
        LDR.N    R1,??DataTable3_8
        STR      R0,[R1, #+0]
//  207 
//  208     /* Use the value obtained from the pll_init function to define variables
//  209     * for the core clock in kHz and also the peripheral clock. These
//  210     * variables can be used by other functions that need awareness of the
//  211     * system frequency.
//  212     */
//  213     core_clk_khz = core_clk_mhz * 1000;
        LDR.N    R0,??DataTable3_8
        LDR      R0,[R0, #+0]
        MOV      R1,#+1000
        MULS     R0,R1,R0
        LDR.N    R1,??DataTable3_9
        STR      R0,[R1, #+0]
//  214     periph_clk_khz = core_clk_khz / (((SIM_CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK) >> 24)+ 1);        
        LDR.N    R0,??DataTable3_9
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable3_10  ;; 0x40048044
        LDR      R1,[R1, #+0]
        UBFX     R1,R1,#+24,#+4
        ADDS     R1,R1,#+1
        UDIV     R0,R0,R1
        LDR.N    R1,??DataTable3_11
        STR      R0,[R1, #+0]
//  215 } // end MCG BLPI to PEE
        POP      {R0,PC}          ;; return
//  216 /********************************************************************/
//  217 

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
//  218 void mcg_pbe_2_pee(void)
//  219 {  
//  220   MCG_C1 &= ~MCG_C1_CLKS_MASK; // select PLL as MCG_OUT
mcg_pbe_2_pee:
        LDR.N    R0,??DataTable3_3  ;; 0x40064000
        LDRB     R0,[R0, #+0]
        ANDS     R0,R0,#0x3F
        LDR.N    R1,??DataTable3_3  ;; 0x40064000
        STRB     R0,[R1, #+0]
//  221   // Wait for clock status bits to update 
//  222   while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x3){}; 
??mcg_pbe_2_pee_0:
        LDR.N    R0,??DataTable3_4  ;; 0x40064006
        LDRB     R0,[R0, #+0]
        UBFX     R0,R0,#+2,#+2
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        CMP      R0,#+3
        BNE.N    ??mcg_pbe_2_pee_0
//  223 
//  224   switch (CORE_CLK_MHZ) {
//  225     case PLL50:
//  226       core_clk_khz = 50000;
//  227       break;
//  228     case PLL100:
//  229       core_clk_khz = 100000;
//  230       break;
//  231     case PLL96:
//  232       core_clk_khz = 96000;
        LDR.N    R0,??DataTable3_9
        LDR.N    R1,??DataTable3_12  ;; 0x17700
        STR      R1,[R0, #+0]
//  233       break;  
//  234     case PLL48:
//  235       core_clk_khz = 48000;
//  236       break;  
//  237   }
//  238 }
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable3:
        DC32     0x40064001

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable3_1:
        DC32     0x40048034

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable3_2:
        DC32     0x4007c008

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable3_3:
        DC32     0x40064000

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable3_4:
        DC32     0x40064006

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable3_5:
        DC32     0x40064004

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable3_6:
        DC32     0x40064005

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable3_7:
        DC32     0x40064003

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable3_8:
        DC32     core_clk_mhz

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable3_9:
        DC32     core_clk_khz

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable3_10:
        DC32     0x40048044

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable3_11:
        DC32     periph_clk_khz

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable3_12:
        DC32     0x17700

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)

        END
// 
// 636 bytes in section .text
//  76 bytes in section .textrw
// 
// 712 bytes of CODE memory
//
//Errors: none
//Warnings: none
