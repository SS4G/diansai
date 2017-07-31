#include <msp430g2553.h>

void io_init();
void timer0_init();
void sysTimerClkInit();
int demo_timer();
int msCnt = 0;
int demo_timer()
{
    sysTimerClkInit();
    //--LED��ʼ��----
    io_init();     

    //--��ʱ��0��ʼ��----
    timer0_init();

    //---���ж�------
    _EINT();      //���ж�

    while (1) 
    {
        if (msCnt == 500) 
        {
            msCnt = 0;
            P1OUT = ~P1OUT;
        }
    }
    return 0;
}

/*
 *    ��ʱ��0�жϺ���
 */
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0(void)    //TACCR0�ж�
{ 
    msCnt++;
    if (P2OUT & BIT5)
        P2OUT &= ~ BIT5;
    else
        P2OUT |= BIT5;
}

/*
#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer0_A1(void)    //TACCR1�ж�,TACCR2�жϺ�TA0����жϹ���һ���ж�����
{ 
  switch(TAIV)
  {
  case 2:                           //TACCR1�ж�
    P1OUT^=(1<<6);    //LED��ת
    break;
  case 4:                           //TACCR2�ж�
    P1OUT^=(1<<6);    //LED��ת
    break;
  case 10:                          //TAIFG ����ж�
    P1OUT^=(1<<6);    //LED��ת
    break;
  default:
    break;
  }

}
*/

// LED ��ʼ��
void io_init()
{
    P1DIR = 0x00;
    P2DIR |= BIT5;
    P1DIR |= BIT6;  
}

void sysTimerClkInit()
{
    // Stop watchdog timer to prevent time out reset
    WDTCTL = WDTPW + WDTHOLD;

    //--����ʱ��-----
    BCSCTL1=CALBC1_8MHZ;
    DCOCTL=CALDCO_8MHZ;
}


// ��ʱ����ʼ��
void timer0_init()   
{
  /*
   *����TIMER_A��ʱ��
   *TASSEL_0: TACLK,ʹ���ⲿ�����ź���Ϊ����
   *TASSEL_1: ACLK,����ʱ��
   *TASSEL_2: SMCLK,��ϵͳ��ʱ��
   *TASSEL_3: INCLK,�ⲿ����ʱ�� 
  */
  TACTL |= TASSEL_2;    

  /*
   *ʱ��Դ��Ƶ
   *ID_0: ����Ƶ
   *ID_1: 2��Ƶ
   *ID_2: 4��Ƶ
   *ID_3: 8��Ƶ
   */
  TACTL |= ID_3;    

  /*
   *ģʽѡ��
   *MC_0: ֹͣģʽ,���ڶ�ʱ����ͣ
   *MC_1: ������ģʽ,������������CCR0,�����������
   *MC_2: ��������ģʽ,��������������0XFFFF(65535),�����������
   *MC_3: ��������ģʽ,��������CCR0,�ټ�������0
   */
  TACTL |= MC_1;  //������ģʽ

  //----����������-----
  TACTL |= TACLR; 

  //----����TACCRx��ֵ-----
  //TACCR0=32768-1;     //ʱ����32.768K,��ôֵ��Ϊ32678-1 
  TACCR0 = 1000;   //z�޸����ڸ�����Ĵ�������aa
  //TACCR1=10000;         //TACCR1��TACCR2ҪС��TACCR0,���򲻻�����ж� 
  //TACCR2=20000;

  //----�ж�����----
  TACCTL0 |= CCIE;      //TACCR0�ж�
  //TACCTL1 |= CCIE;      //TACCR1�ж�
  //TACCTL2 |= CCIE;      //TACCR2�ж�
  //TACTL |= TAIE;        //TA0����ж�
}