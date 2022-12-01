// ��������main����֮ǰ�������ļ���statup_ch32v20x.S�Ѿ�����SystemInit()������ϵͳʱ�ӳ�ʼ����72MHZ
// SystemInit()��system_ch32v10x.c�ж���
// ����û����޸�ϵͳʱ�ӣ������б�д�����޸�,��ʵ����Ҫ������α�д�����޸�ϵͳʱ���Լ�ͨ��PA8����������ʱ��

#include "debug.h"
#include "clk.h"

#define HSI      0   //HSI
#define HSI_1_2  1   //HSI/2

#define PLL_Source   HSI
//#define PLL_Source   HSI_1_2



//  �����ʱ������ʹ�ò�ͬ��ϵͳʱ�ӣ���ʱ��һ��
void Delay(__IO uint32_t nCount)
{
    for(; nCount != 0; nCount--);
}


void MCO_GPIO_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    // ����GPIOA��ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // ѡ��GPIO8����
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;

    //����Ϊ���ù����������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;

    //����IO�ķ�ת����Ϊ50M
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    // ��ʼ��GPIOA8
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}


void LED_Init(void)
{

    GPIO_InitTypeDef  GPIO_InitStructure;                     //����һ��GPIO_InitTypeDef���͵Ľṹ��

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);     //ʹ����LED��ص�GPIO�˿�ʱ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;      //����GPIO����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;          //����GPIOģʽΪ�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;         //����GPIO������ٶ�
    GPIO_Init(GPIOA, &GPIO_InitStructure);                    //���ÿ⺯������ʼ��GPIOA

    GPIO_SetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_1);                //������������ߵ�ƽ

}

/*******************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Return         : None
*******************************************************************************/
int main(void)
{


    USART_Printf_Init(115200);
    MCO_GPIO_Init();
    LED_Init();

    printf("This is HSE&HSI example\r\n");

    // ��������ϵͳʱ�ӣ�������Ҫ�޸ģ�CH32V103��߿�����Ϊ80MH����ͨ��ʹ��72M��ΪĬ������
    // SYSCLK = 8M * RCC_PLLMul_x, x:[2,3,...16]
    HSE_SetSysClock(RCC_PLLMul_2);
    //HSI_SetSysClock(RCC_PLLMul_9);

    // ����MCO�������ʱ�ӣ���ʾ����������PA8�����������ʱ���ź�
    // MCO�������������HSE,HSI,PLLCLK/2,SYSCLK
    //RCC_MCOConfig(RCC_MCO_HSE);             // 8M
    //RCC_MCOConfig(RCC_MCO_HSI);             // 8M
    //RCC_MCOConfig(RCC_MCO_PLLCLK_Div2);     // 36M
    RCC_MCOConfig(RCC_MCO_SYSCLK);            // 72M

    while (1)
    {
        GPIO_SetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_1);        // ��
        Delay(0x0FFFFF);
        GPIO_ResetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_1);      // ��
        Delay(0x0FFFFF);
    }
}

