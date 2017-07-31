#include <msp430g2553.h>
#define USE_INTERRUPT

void SysCtlClockInit();
int UARTPutstring(unsigned char *str);
int UARTGetChar(void);
void UARTPutChar(unsigned char cTX);
void UartInit();
void UartGpioCfg();
void UartRegCfg();
void enableUartRxInterrupt();
int readRxBuf(unsigned char* dstBuf); 
int demo_uart();
#ifdef USE_INTERRUPT
    #define RXBUF_SIZE 32
    unsigned char rxbuf[RXBUF_SIZE];
    unsigned char reciveComplete = 0;
    unsigned int rxbufMsgLen = 0; 
#endif
//P1.2  UART_TX
//P1.1  UART_RX
int demo_uart() {
    WDTCTL = WDTPW + WDTHOLD;
    SysCtlClockInit();
    UartInit();
    enableUartRxInterrupt();
    unsigned char buf[32];
    while (1) {
        if (reciveComplete) {
            readRxBuf(buf);
            UARTPutstring(buf);
        }
    }
    
    return 0;
}

void UartRegCfg()
{
UCA0CTL1 |=UCSWRST;      //reset UART module,as well as enable UART module
UCA0CTL1 |=UCSSEL_2;     //UART clock is SMCLK
UCA0BR0  |=65;           //Baud N=BCLK/rate,rate=9600,BCLK=SMCLK=8M
UCA0BR1  |=3;
UCA0MCTL  = UCBRS1;      //UCBRSx=2
UCA0CTL1 &=~UCSWRST;     //UART reset end
}
void UartGpioCfg()
{
P1DIR  |= BIT2;           //P1.2  UART_TX
P1DIR  &= ~BIT1;           //P1.1  UART_RX
P1SEL  |= (BIT1+BIT2);      //select P1.1 and P1.2 as UART port
P1SEL2 |= (BIT1+BIT2);
}
void UartInit()
{
UartRegCfg();
UartGpioCfg();
}
/************************************************************************
* Function Name : UARTPutChar
* Create Date : 2012/07/27
* Author  : 
*
* Description :send a character
*
* Param : cTX is willing to send character
************************************************************************/
void UARTPutChar(unsigned char cTX)
{
UCA0TXBUF=cTX;
while (!(IFG2&UCA0TXIFG));  //waiting UCA0TXBUF is empty
    IFG2&=~UCA0TXIFG;           //clear TX interrupt flag
}
/************************************************************************
* Function Name : UARTGetChar
* Create Date : 2012/07/27
* Author  : 
*
* Description :get a character
*
* Param : cRX is willing to get character
************************************************************************/
int UARTGetChar(void)
{
int GetChar=0;
while (!(IFG2&UCA0RXIFG));  //UCA1RXBUF has received a complete character
IFG2&=~UCA0RXIFG;           //clear RX interrupt flag
UCA0TXBUF=UCA0RXBUF;        //back to display
GetChar  =UCA0RXBUF;
while (!(IFG2&UCA0TXIFG));  //waiting UCA0TXBUF is empty
IFG2&=~UCA0TXIFG;           //clear TX interrupt flag
return GetChar;
}
/************************************************************************
* Function Name : UARTPutstring
* Create Date : 2012/07/27
* Author  : 
*
* Description :output string
*
* Param : char *str point send string
* return: the length of string
************************************************************************/
int UARTPutstring(unsigned char *str)
{
   unsigned int uCount=0;
   do
   {
    UARTPutChar(str[uCount]);
    uCount++;
   }
   while(str[uCount]!='\0');
   UARTPutChar('\n');
   return uCount;
}

void SysCtlClockInit()
{
    WDTCTL = WDTPW + WDTHOLD;
    DCOCTL=0;
    BCSCTL1=CALBC1_16MHZ;
    DCOCTL =CALDCO_16MHZ;
    BCSCTL1|=DIVA_1;    //ACLK =MCLK/2=8M
    BCSCTL2|=DIVS_1;    //SMCLK=MCLK/2=8M
}

#ifdef USE_INTERRUPT
    void enableUartRxInterrupt()
    {
        IE2 |= UCA0RXIE;// Enable USCI_A0 RX 
        _EINT(); //enable global interrupt
    }

    //rxbuf -> dstBuf
    int readRxBuf(unsigned char* dstBuf)
    {
        _DINT();
        int i = 0;
        while (rxbufMsgLen > i) {
            dstBuf[i] = rxbuf[i];
            i ++;
        }
        rxbufMsgLen = 0; //clear the counter
        reciveComplete = 0;
        dstBuf[i] = '\0';
        _EINT();
        return i;
    }

    #pragma vector=USCIAB0RX_VECTOR
    //recive the messege from uart and 
    //save the messege to rxbuf
    //if the messege is too long , abort data directly
    __interrupt void USCI0RX_ISR(void)
    {
        if (rxbufMsgLen < RXBUF_SIZE) {
            rxbuf[rxbufMsgLen] = UCA0RXBUF;
            if (rxbuf[rxbufMsgLen] == '#')
                reciveComplete = 1;
            rxbufMsgLen++;
        }
    }
#endif



