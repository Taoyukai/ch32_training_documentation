#ifndef __KEY_H
#define __KEY_H

#include "ch32v20x_conf.h"

#define KEY  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)  //��ȡ����0

#define KEY_PRESS   1       //KEY


void KEY_Init(void);    //����IO��ʼ��
u8 KEY_Scan(u8 mode);   //����ɨ�躯��

#endif
