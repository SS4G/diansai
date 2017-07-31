#include <msp430g2553.h>
#define CPU_CLOCK       8000000
#define delay_us(us)    __delay_cycles(CPU_CLOCK/1000000*(us))
#define delay_ms(ms)    __delay_cycles(CPU_CLOCK/1000*(ms))
int led_demo() 
{
  WDTCTL = WDTPW + WDTHOLD; //看门�?必须关掉 //
  DCOCTL = CALDCO_8MHZ; //
  BCSCTL1= CALBC1_8MHZ; //
  //BCSCTL2|=SELM_1+DIVM_0;
  P1DIR = 0x00;
  P1DIR |= (BIT0 + BIT6); //BIT0 = 00000001
  while (1) {
    P1OUT |= (BIT0 + BIT6);
    delay_ms(100);
    P1OUT &= ~(BIT0 + BIT6);
    delay_ms(100);
  }
  //while(1);
  return 0;
}