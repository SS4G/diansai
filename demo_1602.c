#include <msp430g2553.h>
#include <string.h>
#define CPU_CLOCK       16000000
#define delay_us(us)    __delay_cycles(CPU_CLOCK/1000000*(us))
#define delay_ms(ms)    __delay_cycles(CPU_CLOCK/1000*(ms))


//以下的声明是用来修改I2C通信端口的 该声明必须出现在 #include "I2C_Sium.h" 之前
#define I2C_PORT P1OUT
#define I2C_DIR  P1DIR
#define I2C_SCL  BIT2
#define I2C_SDA  BIT0

#include "I2C_Sium.h"//you must include this header file for I2C bus



typedef unsigned char uint8_t;
void LCD1602Init();
void LCD1602_SendCmd(uint8_t cmd);
void LCD1602_SendData(uint8_t cmd);
void Dip_Single_char(unsigned char col,unsigned char row,unsigned char sign);
void display(const char* str, int row);
void clearScreen(int row);
int demo_I2C();

/*
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
	I2C_Stop();	
  while(1);
  return 0;
}
*/

#define PCF8574A_W_ADDR 0x7e  //for PCF8574AT 
#define PCF8574A_R_ADDR 0x7f  //for PCF8574AT 
#define PCF8574_W_ADDR 0x4e   //for PCF8574T their I2C address are not equal
#define PCF8574_R_ADDR 0x4f   //for PCF8574T their I2C address are not equal
#define RW          0x01
#define RS          0x02
#define E           0x04
#define LIGHT       0x08


int demo_LCD1602() {	
    WDTCTL = WDTPW + WDTHOLD; //看门￿必须关�?//
    DCOCTL = CALDCO_16MHZ; //
    BCSCTL1= CALBC1_16MHZ; //
    I2C_Init(); //Initialization of  I2C bus invoked only once
    LCD1602Init(); // Initialization of LCD invoked only once
    display("ABCDEFG", 1); //
    display("123456", 2);
    display("0123456789abcdef", 2); //
    display("fedcba9876543210", 1); //
    //clearScreen(2);
    while(1);
}

/********************************
** Function name:
** Descriptions:
** input parameters: 
    str: string buffer whose maxium length is 16
    row: display position of LCD 1 or 2  
** output parameters: None 
*********************************/
void display(const char* str, int row) {
    char len = strlen(str);
    len = 16 >= len ? len :16;
    char idx = 0;
    for (idx = 0; idx < 16; idx++) {
        Dip_Single_char(row, idx + 1, ' ');
    }
    for (idx = 0; idx < len; idx++) {
        Dip_Single_char(row, idx + 1, str[idx]);
    }
}

void clearScreen(int row) {
    display("                ", row);
}


/*********************************************************************
** Function name:
** Descriptions:
** input parameters: 
** output parameters: �?** Returned value: �?
**********************************************************************/
void WriteCommand(unsigned char Command){
 I2C_Start(); // I2C开�? 
 I2C_SendByte(PCF8574_W_ADDR); // 从器件地址
 unsigned char Temp_C;
 Temp_C = Command & 0xF0;
 Temp_C |= 0x0C; // P3=1 EN=1 RW=0 RS=0
 I2C_SendByte(Temp_C);
 Temp_C &= 0xF8; // P3=1 EN=0 RW=0 RS=0
 I2C_SendByte(Temp_C);


 Temp_C = (Command & 0x0F)<< 4;
 Temp_C |= 0x0C; // P3=1 EN=1 RW=0 RS=0
 I2C_SendByte(Temp_C);
 Temp_C &= 0xF8; // P3=1 EN=0 RW=0 RS=0
 I2C_SendByte(Temp_C);

 I2C_Stop(); // I2C停止
}

/*********************************************************************
** Function name:
** Descriptions:
** input parameters:
** output parameters: �?** Returned value: �?**********************************************************************/
void WriteData (unsigned char Data){
 I2C_Start(); // I2C开�? 
 I2C_SendByte(PCF8574_W_ADDR); // 从器件地址
 unsigned char Temp_D;
 Temp_D = Data & 0xF0;
 Temp_D |= 0x0D; // P3=1 EN=1 RW=0 RS=1
 I2C_SendByte(Temp_D);
 Temp_D &= 0xF9; // P3=1 EN=0 RW=0 RS=1
 I2C_SendByte(Temp_D);

 Temp_D = (Data & 0x0F)<< 4;
 Temp_D |= 0x0D; // P3=1 EN=1 RW=0 RS=1
 I2C_SendByte(Temp_D);
 Temp_D &= 0xF9; // P3=1 EN=0 RW=0 RS=1
 I2C_SendByte(Temp_D);

 I2C_Stop(); // I2C停止
}

/*********************************************************************
** Function name: LCD1602Init(void),LCD1602初始�?** Descriptions: 写一次，偶尔不能正常显示，重�?�?** input parameters:
** output parameters: �?** Returned value: �?**********************************************************************/
void LCD1602Init(void){
delay_ms(10);
WriteCommand(0x33); delay_ms(5);
WriteCommand(0x32); delay_ms(5);
WriteCommand(0x28); delay_ms(5);
WriteCommand(0x0C); delay_ms(5);
WriteCommand(0x06); delay_ms(5);
WriteCommand(0x01); delay_ms(5); // 清屏

delay_ms(10);
WriteCommand(0x33); delay_ms(5);
WriteCommand(0x32); delay_ms(5);
WriteCommand(0x28); delay_ms(5);
WriteCommand(0x0C); delay_ms(5);
WriteCommand(0x06); delay_ms(5);
WriteCommand(0x01); delay_ms(5); // 清屏
}
/********************************************************************
** Function name: L1602_char(uchar col,uchar row,char sign)
** Descriptions: 改变液晶中某位的值，如果要让第一行，第五个字符显�?b" �? 调用该函数如,Dip_Single_char(1,5,'A');
** input parameters: 行，列，需要输�?602的数�?** output parameters: �?** Returned value: �?*********************************************************************/
void Dip_Single_char(unsigned char col,unsigned char row,unsigned char sign){
 unsigned char a;
 if(col == 1) a = 0x80;
 if(col == 2) a = 0xc0;
 a = a + row - 1;
 WriteCommand(a);
 WriteData(sign);
}






