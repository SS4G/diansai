#include <msp430g2553.h>
#define CPU_CLOCK       8000000
#define delay_us(us)    __delay_cycles(CPU_CLOCK/1000000*(us))
#define delay_ms(ms)    __delay_cycles(CPU_CLOCK/1000*(ms))
#include "I2C_Sium.h"
typedef unsigned char uint8_t;


int demo_I2C() 
{
  WDTCTL = WDTPW + WDTHOLD; //看门�?必须关掉 //
  DCOCTL = CALDCO_16MHZ; //
  BCSCTL1= CALBC1_16MHZ; //
  //BCSCTL2|=SELM_1+DIVM_0;
  	I2C_Init();                                        // I2C初始�?    
	I2C_Start();
    I2C_SetAdd(0x7e);				// 选择 SLAVE ADDRESS
    I2C_SendByte(0x00);					// 控制字节：Co=0,A0=0,表示以下传输�?N 个字节是指令
    I2C_SendByte(0x70);					// 控制字节：Co=0,A0=0,表示以下传输�?N 个字节是指令
	//I2C_SendByte(0x38);					// Function set
	//I2C_SendByte(0x0C);					// Display ON/OFF
	//I2C_SendByte(0x01);					// Clear display
	//I2C_SendByte(0x06);					// Entry mode set
	I2C_Stop();	
  while(1);
  return 0;
}

#define RW          0x01
#define RS          0x02
#define E           0x04
#define LIGHT       0x08

int demo_LCD1602() {	
    WDTCTL = WDTPW + WDTHOLD; //看门￿必须关�?//
    DCOCTL = CALDCO_16MHZ; //
    BCSCTL1= CALBC1_16MHZ; //
    //BCSCTL2|=SELM_1+DIVM_0;
    I2C_Init(); 
    I2C_Start();// I2C初始�?    I2C_Start();
    I2C_SetAdd(0x7e);	
    I2C_SendByte(0x30);
    I2C_Stop();
    while(1);
}

void LCD1602_SendData(uint8_t cmd) {
    uint8_t highPart = cmd & 0xf0;
    uint8_t lowPart = ((cmd & 0x0f) << 4) & 0xf0;
    I2C_Start();
	I2C_SetAdd(0x7e);				// 选择 SLAVE ADDRESS
    //I2C_SendByte(0x00);					// 控制字节：Co=0,A0=0,表示以下传输皿N 个字节是指令

    I2C_SendByte(RS | highPart | LIGHT);
    I2C_SendByte(RS | highPart | E | LIGHT);
    I2C_SendByte(RS | highPart | LIGHT);
    
    I2C_SendByte(RS | lowPart | LIGHT);
    I2C_SendByte(RS | lowPart | E | LIGHT);
    I2C_SendByte(RS | lowPart | LIGHT);
    
    I2C_Stop();
}

void LCD1602_SendCmd(uint8_t cmd) {
    uint8_t highPart = cmd & 0xf0;
    uint8_t lowPart = ((cmd & 0x0f) << 4) & 0xf0;
    I2C_Start();
	I2C_SetAdd(0x7e);				// 选择 SLAVE ADDRESS
    //I2C_SendByte(0x00);					// 控制字节：Co=0,A0=0,表示以下传输皿N 个字节是指令

    I2C_SendByte(highPart | LIGHT);
    I2C_SendByte(highPart | E | LIGHT);
    I2C_SendByte(highPart | LIGHT);
    
    I2C_SendByte(lowPart | LIGHT);
    I2C_SendByte(lowPart | E | LIGHT);
    I2C_SendByte(lowPart | LIGHT);
    
    I2C_Stop();
}


void LCD1602Init() {
    I2C_Init();   
    I2C_Start();
	I2C_SetAdd(0x7e);				// 选择 SLAVE ADDRESS
    //I2C_SendByte(0x00);					// 控制字节：Co=0,A0=0,表示以下传输皿N 个字节是指令
    I2C_SendByte(0x20 | LIGHT);
    I2C_SendByte(0x20 | E | LIGHT);
    I2C_SendByte(0x20 | LIGHT);
    I2C_Stop();
}






