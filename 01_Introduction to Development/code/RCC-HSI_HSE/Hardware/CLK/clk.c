
#include "clk.h"

/*
 * ʹ��HSEʱ������ϵͳʱ�ӵĲ���
 * 1������HSE �����ȴ� HSE �ȶ�
 * 2������ AHB��APB2��APB1��Ԥ��Ƶ����
 * 3������PLL��ʱ����Դ����PLL�ı�Ƶ���ӣ����ø���Ƶ����Ҫ��������������
 * 4������PLL�����ȴ�PLL�ȶ�
 * 5����PLLCK�л�Ϊϵͳʱ��SYSCLK
 * 6����ȡʱ���л�״̬λ��ȷ��PLLCLK��ѡΪϵͳʱ��
 */

/* ���� ϵͳʱ��:SYSCLK, AHB����ʱ��:HCLK, APB2����ʱ��:PCLK2, APB1����ʱ��:PCLK1
 * PCLK2 = HCLK = SYSCLK
 * PCLK1 = HCLK
 * ����˵����pllmul��PLL�ı�Ƶ���ӣ��ڵ��õ�ʱ������ǣ�RCC_PLLMul_x , x:[2,3,...16]
 * ������User_SetSysClock(RCC_PLLMul_9);  ������ϵͳʱ��Ϊ��8MHZ * 9 = 72MHZ
 * HSE��Ϊʱ����Դ������PLL��Ƶ��Ϊϵͳʱ�ӣ�����ͨ��������
 */

void HSE_SetSysClock(uint32_t pllmul)
{
    __IO uint32_t HSEStartUpStatus = 0;

  // ��RCC�����ʼ���ɸ�λ״̬
  RCC_DeInit();

  //ʹ��HSE�������ⲿ����Ұ�𿪷����õ���8M
  RCC_HSEConfig(RCC_HSE_ON);

  // �ȴ� HSE �����ȶ�
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

    // ֻ�� HSE �ȶ�֮�����������ִ��
  if (HSEStartUpStatus == READY)
  {
    // AHBԤ��Ƶ��������Ϊ1��Ƶ��HCLK = SYSCLK
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

    // APB2Ԥ��Ƶ��������Ϊ1��Ƶ��PCLK2 = HCLK
    RCC_PCLK2Config(RCC_HCLK_Div1);

    // APB1Ԥ��Ƶ��������Ϊ1��Ƶ��PCLK1 = HCLK/2
    RCC_PCLK1Config(RCC_HCLK_Div2);

//-----------------���ø���Ƶ����Ҫ��������������-------------------//
    // ����PLLʱ����ԴΪHSE������PLL��Ƶ����
    // PLLCLK = 8MHz * pllmul
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, pllmul);
//------------------------------------------------------------------//

    // ����PLL
    RCC_PLLCmd(ENABLE);

    // �ȴ� PLL�ȶ�
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }

    // ��PLL�ȶ�֮�󣬰�PLLʱ���л�Ϊϵͳʱ��SYSCLK
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    // ��ȡʱ���л�״̬λ��ȷ��PLLCLK��ѡΪϵͳʱ��
    while (RCC_GetSYSCLKSource() != 0x08)
    {
    }
  }
  else
  {
    // ���HSE����ʧ�ܣ���ô����ͻ���������û�����������ӳ���Ĵ��봦��
    // ��HSE����ʧ�ܻ��߹��ϵ�ʱ�򣬵�Ƭ�����Զ���HSI����Ϊϵͳʱ�ӣ�HSI���ڲ��ĸ���ʱ�ӣ�8MHZ
    while (1)
    {
    }
  }
}
/*
 * ʹ��HSIʱ������ϵͳʱ�ӵĲ���
 * 1������HSI �����ȴ� HSI �ȶ�
 * 2������ AHB��APB2��APB1��Ԥ��Ƶ����
 * 3������PLL��ʱ����Դ����PLL�ı�Ƶ���ӣ����ø���Ƶ����Ҫ��������������
 * 4������PLL�����ȴ�PLL�ȶ�
 * 5����PLLCK�л�Ϊϵͳʱ��SYSCLK
 * 6����ȡʱ���л�״̬λ��ȷ��PLLCLK��ѡΪϵͳʱ��
 */

/* ���� ϵͳʱ��:SYSCLK, AHB����ʱ��:HCLK, APB2����ʱ��:HCLK2, APB1����ʱ��:PCLK1
 * PCLK2 = HCLK = SYSCLK
 * PCLK1 = HCLK
 * ����˵����pllmul��PLL�ı�Ƶ���ӣ��ڵ��õ�ʱ������ǣ�RCC_PLLMul_x , x:[2,3,...16]
 *
 * HSI��Ϊʱ����Դ������PLL��Ƶ��Ϊϵͳʱ�ӣ�������HSE���ϵ�ʱ���ʹ�õķ���
 * HSI����Ϊ�¶ȵ�ԭ�����Ư�ƣ����ȶ���һ�㲻����HSI��Ϊʱ����Դ���������Ȳ����ѵ����
 * ���HSIҪ��ΪPLLʱ�ӵ���Դ�Ļ�����ѡ�����Ƶ���߲���Ƶֱ������PLL��оƬ������ʹ��HSIʱ������72MHz��Ƶ
 * HSI��ΪPLL����ʱ�ӣ���ʾϵͳʱ��Ϊ 24MHz��48MHz��72MHz��
 * HSI/2��ΪPLL����ʱ�ӣ���ʾϵͳʱ��Ϊ 24MHz��48MHz
 */
void HSI_SetSysClock(uint32_t pllmul)
{

#if (PLL_Source == HSI)
  EXTEN->EXTEN_CTR |= EXTEN_PLL_HSI_PRE;  //HSIʱ����ΪPLL����ʱ��
#endif

  __IO uint32_t HSIStartUpStatus = 0;

  // ��RCC�����ʼ���ɸ�λ״̬
  RCC_DeInit();

  //ʹ��HSI
  RCC_HSICmd(ENABLE);

  // �ȴ� HSI ����
  HSIStartUpStatus = RCC->CTLR & RCC_HSIRDY;

  // ֻ�� HSI����֮�����������ִ��
  if (HSIStartUpStatus == RCC_HSIRDY)
  {
   // AHBԤ��Ƶ��������Ϊ1��Ƶ��HCLK = SYSCLK
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

    // APB2Ԥ��Ƶ��������Ϊ1��Ƶ��PCLK2 = HCLK
    RCC_PCLK2Config(RCC_HCLK_Div1);

    // APB1Ԥ��Ƶ��������Ϊ1��Ƶ��PCLK1 = HCLK/2
    RCC_PCLK1Config(RCC_HCLK_Div2);

//-----------------���ø���Ƶ����Ҫ��������������-------------------//
    // ������PLLʱ����ԴΪHSI������PLL��Ƶ����
    // PLLCLK = 8MHz * pllmul
    // ������PLLʱ����ԴΪHSI/2������PLL��Ƶ����
    // PLLCLK = 4MHz * pllmul
    RCC_PLLConfig(RCC_PLLSource_HSI_Div2, pllmul);
//------------------------------------------------------------------//

    // ����PLL
    RCC_PLLCmd(ENABLE);

    // �ȴ� PLL�ȶ�
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }

    // ��PLL�ȶ�֮�󣬰�PLLʱ���л�Ϊϵͳʱ��SYSCLK
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    // ��ȡʱ���л�״̬λ��ȷ��PLLCLK��ѡΪϵͳʱ��
    while (RCC_GetSYSCLKSource() != 0x08)
    {
    }
  }
  else
  {
    // ���HSI����ʧ�ܣ���ô����ͻ���������û�����������ӳ���Ĵ��봦��
    // ��HSE����ʧ�ܻ��߹��ϵ�ʱ�򣬵�Ƭ�����Զ���HSI����Ϊϵͳʱ�ӣ�HSI���ڲ��ĸ���ʱ�ӣ�8MHZ
    while (1)
    {
    }
  }
}


