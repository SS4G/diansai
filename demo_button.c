#include<msp430g2553.h>
int demo_button() {
  P1DIR = BIT0;
  //unsigned char x = 0;
  while (1) {
    if (!(BIT3 & P1IN)) { // on
      delay_ms(30);
      if (!(BIT3 & P1IN))
        P1OUT = BIT0;
      else
        P1OUT = 0;
    }
    else
      P1OUT = 0;
  }
  return 0;
}
