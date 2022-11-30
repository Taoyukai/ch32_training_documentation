/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2021/06/06
 * Description        : Main program body.
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *******************************************************************************/

/*
 *@Note
 ��������LED��ʵ��
 KEY����LED�����𣬴��ڴ�ӡ i=0��ʾLED������i=1��ʾLED����
*/
#include "debug.h"
#include "led.h"
#include "key.h"

int main(void)
{
    u8 t=0;
    static u8 i=1;

    USART_Printf_Init(115200);
    Delay_Init();   //��ʱ������ʼ��
    LED_Init();     //LED��ʼ��
    KEY_Init();     //������ʼ��

    printf("SystemClk:%d\r\n",SystemCoreClock);

    while(1)
    {
        t=KEY_Scan(0);      //�õ���ֵ

        switch(t)
        {
            case KEY_PRESS:
                GPIO_WriteBit(GPIOA, GPIO_Pin_0, (i==0) ? (i=Bit_SET):(i=Bit_RESET));
                printf("i=%d\n",i);
                break;
            default:
                Delay_Ms(10);
        }
    }
}

