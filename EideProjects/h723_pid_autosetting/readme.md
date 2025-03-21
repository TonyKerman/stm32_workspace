# pid自动整定
原理详见《PID参数整定与复杂控制》冯少辉

用法见注释`Core\Src\pid_param_setting.c`


## M2006调速度环pid代码
```c
    FDCAN1_RX_Filter_Init();
    FDCAN1_Interrupt_Init();
    FDCAN1_Start();
    hDJI[0].motorType = M2006;
    DJI_Init();    
    TIM_Delay_init(&htim12);

    PID_Init_NoParams(&speedpid);
    PID_Set_output_limit(&speedpid,-10000,10000);
    PID_Set_integral_limit(&speedpid,1000);//设置积分限幅，效果很好
    PID_AutoSetting_Init(&pas,TYPE_INTEGRAL,1,0,75,1,0.01,10000);
    HAL_Delay(800);
    speedpid.setpoint = 500;
    while(PID_AutoSetting_Update(&pas,hDJI[0].AxisData.AxisVelocity) == PID_AUTOSETTING_PROCESS)
    {
        // TIM_Delay_Us_Start();
        CanTransmit_DJI_1234(10000,1000,1000,1000);
        TIM_Delay_Us(1000);
        // TIM_Delay_Us_Until(1000);
    }
    for(int i = 0;i<2000;i++)
    {
        CanTransmit_DJI_1234(0,0,0,0);
        if(i<pas.pv_num){
        vofa_send_data(0, pas.test_output);
        vofa_send_data(1, pas.pv[i]);
        vofa_sendframetail();            
        }
        else
        {
            vofa_send_data(0, 0);
            vofa_send_data(1, 0);
            vofa_sendframetail(); 
        }
        TIM_Delay_Us(1000);
    }
    // PID_AutoSetting_Set(&pas,&speedpid);
    PID_AutoSetting_Set_Param(&pas,&speedpid.kp,&speedpid.ki,&speedpid.kd);//kp=246,ki=12.295
    speedpid.setpoint = 300;
    for(int t = 0;t<1500;t++)
    {
        TIM_Delay_Us_Start();
        PID_update(&speedpid,hDJI[0].AxisData.AxisVelocity);
        CanTransmit_DJI_1234(speedpid.output,0,0,0);
        vofa_send_data(0, speedpid.setpoint);
        vofa_send_data(1, hDJI[0].AxisData.AxisAngle_inDegree);
        vofa_send_data(2, speedpid.output);
        vofa_send_data(3, speedpid.integral);
        vofa_send_data(4, speedpid.kp);
        vofa_send_data(5, speedpid.ki);
        vofa_sendframetail();
        TIM_Delay_Us_Until(1000);
    }
```

## M2006调位置环pid代码

main函数：
```c
    PID_AutoSetting_Buffers_t pas;
    PID_TypeDef speedpid;
    PID_TypeDef pospid;
    FDCAN1_RX_Filter_Init();
    FDCAN1_Interrupt_Init();
    FDCAN1_Start();
    hDJI[0].motorType = M2006;
    DJI_Init();    
    TIM_Delay_init(&htim12);
    
    PID_Init(&speedpid,200,10,0);//速度环pid参数已知
    PID_Set_output_limit(&speedpid,-10000,10000);
    PID_Set_integral_limit(&speedpid,500);
    PID_Init_NoParams(&pospid);
    PID_Set_output_limit(&pospid,-550,550);
    PID_Set_integral_limit(&pospid,100);
    PID_Set_deadband(&pospid,1);
    PID_AutoSetting_Init(&pas,TYPE_INTEGRAL,10,0,75,1,0.01,500);
    HAL_Delay(800);
    speedpid.setpoint = 500;
    while(PID_AutoSetting_Update(&pas,hDJI[0].AxisData.AxisAngle_inDegree) == PID_AUTOSETTING_PROCESS)
    {
        // TIM_Delay_Us_Start();
        PID_update(&speedpid,hDJI[0].AxisData.AxisVelocity);
        CanTransmit_DJI_1234(speedpid.output,1000,1000,1000);
        TIM_Delay_Us(1000);
        // TIM_Delay_Us_Until(1000);
    }
    for(int i = 0;i<2000;i++)
    {
        CanTransmit_DJI_1234(0,0,0,0);
        if(i<pas.pv_num){
        vofa_send_data(0, pas.test_output);
        vofa_send_data(1, pas.pv[i]);
        vofa_sendframetail();            
        }
        else
        {
            vofa_send_data(0, 0);
            vofa_send_data(1, 0);
            vofa_sendframetail(); 
        }
        TIM_Delay_Us(1000);
    }
    // PID_AutoSetting_Set(&pas,&pospid);
    PID_Reset(&speedpid,200,10,0);
    PID_AutoSetting_Set_Param(&pas,&pospid.kp,&pospid.ki,&pospid.kd);//kp=246,ki=12.295
    pospid.setpoint = 180+hDJI[0].AxisData.AxisAngle_inDegree;
    for(int t = 0;t<1500;t++)
    {
        TIM_Delay_Us_Start();
        PID_update(&pospid,hDJI[0].AxisData.AxisAngle_inDegree);
        speedpid.setpoint = pospid.output;
        PID_update(&speedpid,hDJI[0].AxisData.AxisVelocity);
        CanTransmit_DJI_1234(speedpid.output,0,0,0);
        vofa_send_data(0, pospid.setpoint);
        vofa_send_data(1, hDJI[0].AxisData.AxisAngle_inDegree);
        vofa_send_data(2, pospid.output);
        vofa_send_data(3, pospid.integral);
        vofa_send_data(4, pospid.kp);
        vofa_send_data(5, pospid.ki);
        vofa_sendframetail();
        TIM_Delay_Us_Until(1000);
    }
```