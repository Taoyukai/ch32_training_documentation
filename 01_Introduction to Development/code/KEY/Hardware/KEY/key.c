#include "key.h"
#include "debug.h"

//������ʼ������
//PA2���ó�����
void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);//ʹ��PORTA,PORTCʱ��

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2|GPIO_Pin_3; //����GPIO��������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;         //���ó���������
    GPIO_Init(GPIOA, &GPIO_InitStructure);                //��ʼ��GPIOA15

}

//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//����ֵ��
//0��û���κΰ�������
//KEY_PRES��KEY����
u8 KEY_Scan(u8 mode)
{
    static u8 key_up=1;//�������ɿ���־
    if(mode)  key_up=1;  //֧������
    if(key_up&&(KEY==0))
    {
        Delay_Ms(10);//ȥ����
        key_up=0;
        if(KEY==0)      return KEY_PRESS;
    }else if(KEY==1)  key_up=1;
    return 0;// �ް�������
}
