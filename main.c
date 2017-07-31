#include <msp430g2553.h>
//extern int demo_led();
extern int led_demo();
extern int demo_uart();
extern int demo_timer();
extern int demo_button();
#define CPU_CLOCK       8000000
#define delay_us(us)    __delay_cycles(CPU_CLOCK/1000000*(us))
#define delay_ms(ms)    __delay_cycles(CPU_CLOCK/1000*(ms))
int main() {
  WDTCTL = WDTPW + WDTHOLD;
  //demo_button();
  //return 0; 
}