#include <msp430g2553.h>

void io_init();
void timer0_init();
void sysTimerClkInit();
int demo_timer();
int msCnt = 0;
int demo_timer()
{
    sysTimerClkInit();
    //--LED初始化----
    io_init();     

    //--定时器0初始化----
    timer0_init();

    //---开中断------
    _EINT();      //开中断

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
 *    定时器0中断函数
 */
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0(void)    //TACCR0中断
{ 
    msCnt++;
    if (P2OUT & BIT5)
        P2OUT &= ~ BIT5;
    else
        P2OUT |= BIT5;
}

/*
#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer0_A1(void)    //TACCR1中断,TACCR2中断和TA0溢出中断共用一个中断向量
{ 
  switch(TAIV)
  {
  case 2:                           //TACCR1中断
    P1OUT^=(1<<6);    //LED反转
    break;
  case 4:                           //TACCR2中断
    P1OUT^=(1<<6);    //LED反转
    break;
  case 10:                          //TAIFG 溢出中断
    P1OUT^=(1<<6);    //LED反转
    break;
  default:
    break;
  }

}
*/

// LED 初始化
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

    //--配置时钟-----
    BCSCTL1=CALBC1_8MHZ;
    DCOCTL=CALDCO_8MHZ;
}


// 定时器初始化
void timer0_init()   
{
  /*
   *设置TIMER_A的时钟
   *TASSEL_0: TACLK,使用外部引脚信号作为输入
   *TASSEL_1: ACLK,辅助时钟
   *TASSEL_2: SMCLK,子系统主时钟
   *TASSEL_3: INCLK,外部输入时钟 
  */
  TACTL |= TASSEL_2;    

  /*
   *时钟源分频
   *ID_0: 不分频
   *ID_1: 2分频
   *ID_2: 4分频
   *ID_3: 8分频
   */
  TACTL |= ID_3;    

  /*
   *模式选择
   *MC_0: 停止模式,用于定时器暂停
   *MC_1: 增计数模式,计数器计数到CCR0,再清零计数器
   *MC_2: 连续计数模式,计数器增计数到0XFFFF(65535),再清零计数器
   *MC_3: 增减计数模式,增计数到CCR0,再减计数到0
   */
  TACTL |= MC_1;  //增计数模式

  //----计数器清零-----
  TACTL |= TACLR; 

  //----设置TACCRx的值-----
  //TACCR0=32768-1;     //时钟是32.768K,那么值设为32678-1 
  TACCR0 = 1000;   //z修改周期该这个寄存器即可aa
  //TACCR1=10000;         //TACCR1和TACCR2要小于TACCR0,否则不会产生中断 
  //TACCR2=20000;

  //----中断允许----
  TACCTL0 |= CCIE;      //TACCR0中断
  //TACCTL1 |= CCIE;      //TACCR1中断
  //TACCTL2 |= CCIE;      //TACCR2中断
  //TACTL |= TAIE;        //TA0溢出中断
}