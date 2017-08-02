//********************************************************************
// 文件名称： I2C.h
// 创建日期： 2016-10-11
// 最新更改： 2016-10-11
// 描述： I2C初始化
//********************************************************************
 //
#define _XTAL_FREQ 2000000 // 延时函数delay_us/ms使用此值
#define Slave_Add 0x7E // 从器件地址,PCF8574A,0x7E
 // PCF8574,0x27,左移1位,0x4E,
#define I2C_BPS 0x18 // I2C波特率Fclock = Fosc/((I2C_BPS+1)*4)
 // 2MHz,20k,0x18
#define Idle !(SSP1STATbits.R_nW|(0x1F & SSP1CON2)) // 空闲
void I2CInit ( );
void I2CStart ( );
void I2CStop ( );
void ReStart ( );
void I2CSendByte(unsigned char I2CSnBy);
void WriteCommand(unsigned char Command);
void WriteData (unsigned char Data);
void LCD1602Init(void);
void Dip_Single_char(unsigned char col,unsigned char row,unsigned char sign);
void DisDec(unsigned char col_D,unsigned char row_D,unsigned int Temp_k );
/*********************************************************************
** The End
*********************************************************************/
I2C.c 文件
//********************************************************************
// 文件名称： I2C.c
// 创建日期： 2016-10-11
// 最新更改： 2016-10-11
// 描述： I2C初始化
//********************************************************************
 //
#include "xc.h"
#include "I2C.h"
/*********************************************************************
** Function name: I2CInit
** Descriptions: 注意：必须将SDA、SCL引脚配置为输入引脚，<<DS P293>>
** input parameters: 无
** output parameters: 无
** Returned value: 无
**********************************************************************/
void I2CInit (void){

 SSP1STATbits.SMP = 1; // 禁止标准速度模式下的压摆率控制
 SSP1STATbits.CKE = 0; // 禁止SMBus特定输入
 SSP1CON1bits.SSPEN = 1; // 使能I2C,并将SDA 和SCL引脚配
 SSP1CON1bits.SSPM = 0x8; // I2C主模式
 SSP1ADD = I2C_BPS; // Fclock = Fosc / ((SSP1ADD + 1)*4)

}
/*********************************************************************
** Function name: I2CStart ( )
** Descriptions: I2C开始
** input parameters: 无
** output parameters: 无
** Returned value: 无
**********************************************************************/
void I2CStart ( ){
 while (!Idle);
 SSP1CON2bits.SEN = 1; // 在SDA和SCL引脚上发出启动条件，硬件自动清零
 while(SSP1CON2bits.SEN); // 启动条件发送完成
 while(!Idle);
}
/*********************************************************************
** Function name: I2CStop ( )
** Descriptions: I2C停止
** input parameters: 无
** output parameters: 无
** Returned value: 无
**********************************************************************/
void I2CStop(){
 while (!Idle);
 SSP1CON2bits.PEN = 1; // 在SDA和SCL引脚上发出停止条件，硬件自动清零
 while(SSP1CON2bits.PEN); // 停止条件发送完成
 while(!Idle);
}
/*********************************************************************
** Function name: ReStart()
** Descriptions: I2C ，ReStart
** input parameters: 无
** output parameters: 无
** Returned value: 无
**********************************************************************/
void ReStart(){
 while (!Idle);
 SSP1CON2bits.RSEN = 1; // 在SDA和SCL引脚上发出重复启动条件，硬件自动清
零
 while(SSP1CON2bits.RSEN); // 重复启动条件发送完成
 while(!Idle);
}
/*********************************************************************
** Function name: I2CSendByte()
** Descriptions: SSP1BUF中写入字节
** input parameters: I2CSnBy，要发送的数据
** output parameters: 无
** Returned value: 无
**********************************************************************/
void I2CSendByte(unsigned char I2CSnBy) {
 while (!Idle);
 SSP1BUF = I2CSnBy; // SSP1BUF中写入字节
 while(!Idle);
}
/*********************************************************************
** Function name:
** Descriptions:
** input parameters: 
** output parameters: 无
** Returned value: 无
**********************************************************************/
void WriteCommand(unsigned char Command){
 I2CStart(); // I2C开始
 I2CSendByte(Slave_Add); // 从器件地址
 unsigned char Temp_C;
 Temp_C = Command & 0xF0;
 Temp_C |= 0x0C; // P3=1 EN=1 RW=0 RS=0
 I2CSendByte(Temp_C);
 Temp_C &= 0xF8; // P3=1 EN=0 RW=0 RS=0
 I2CSendByte(Temp_C);


 Temp_C = (Command & 0x0F)<< 4;
 Temp_C |= 0x0C; // P3=1 EN=1 RW=0 RS=0
 I2CSendByte(Temp_C);
 Temp_C &= 0xF8; // P3=1 EN=0 RW=0 RS=0
 I2CSendByte(Temp_C);

 I2CStop(); // I2C停止
}
/*********************************************************************
** Function name:
** Descriptions:
** input parameters:
** output parameters: 无
** Returned value: 无
**********************************************************************/
void WriteData (unsigned char Data){
 I2CStart(); // I2C开始
 I2CSendByte(Slave_Add); // 从器件地址
 unsigned char Temp_D;
 Temp_D = Data & 0xF0;
 Temp_D |= 0x0D; // P3=1 EN=1 RW=0 RS=1
 I2CSendByte(Temp_D);
 Temp_D &= 0xF9; // P3=1 EN=0 RW=0 RS=1
 I2CSendByte(Temp_D);

 Temp_D = (Data & 0x0F)<< 4;
 Temp_D |= 0x0D; // P3=1 EN=1 RW=0 RS=1
 I2CSendByte(Temp_D);
 Temp_D &= 0xF9; // P3=1 EN=0 RW=0 RS=1
 I2CSendByte(Temp_D);

 I2CStop(); // I2C停止
}
/*********************************************************************
** Function name: LCD1602Init(void),LCD1602初始化
** Descriptions: 写一次，偶尔不能正常显示，重复2次
** input parameters:
** output parameters: 无
** Returned value: 无
**********************************************************************/
void LCD1602Init(void){
 __delay_ms(10);
WriteCommand(0x33); __delay_ms(5);
WriteCommand(0x32); __delay_ms(5);
WriteCommand(0x28); __delay_ms(5);
WriteCommand(0x0C); __delay_ms(5);
WriteCommand(0x06); __delay_ms(5);
 WriteCommand(0x01); __delay_ms(5); // 清屏

 __delay_ms(10);
WriteCommand(0x33); __delay_ms(5);
WriteCommand(0x32); __delay_ms(5);
WriteCommand(0x28); __delay_ms(5);
WriteCommand(0x0C); __delay_ms(5);
WriteCommand(0x06); __delay_ms(5);
 WriteCommand(0x01); __delay_ms(5); // 清屏
}
/********************************************************************
** Function name: L1602_char(uchar col,uchar row,char sign)
** Descriptions: 改变液晶中某位的值，如果要让第一行，第五个字符显示"b" ，
 调用该函数如,Dip_Single_char(1,5,'A');
** input parameters: 行，列，需要输入1602的数据
** output parameters: 无
** Returned value: 无
*********************************************************************/
void Dip_Single_char(unsigned char col,unsigned char row,unsigned char sign){
 unsigned char a;
 if(col == 1) a = 0x80;
 if(col == 2) a = 0xc0;
 a = a + row - 1;
 WriteCommand(a);
 WriteData(sign);
}
/********************************************************************
** Function name: Dip_Single_char
** Descriptions: 显示int型数据,5位
** input parameters: 行，列，数据
** output parameters: 无
** Returned value: 无
*********************************************************************/
void DisDec(unsigned char col_D,unsigned char row_D,unsigned int Temp_k ){

 if(Temp_k>=65535) Temp_k=65535;
 unsigned int Temp_Ts;
 unsigned char Table[5]; // 数字与1602显示码转换

 Table[0] = Temp_k/10000+48; // 万位
 Temp_Ts = Temp_k%10000; // 取余，0-9999
 Table[1] = Temp_Ts/1000+48; // 千位
 Temp_Ts = Temp_Ts%1000; // 取余，0-999
 Table[2] = Temp_Ts/100+48; // 百位
 Temp_Ts = Temp_Ts%100; // 取余，0-99
 Table[3] = Temp_Ts/10+48; // 十位
 Table[4] = Temp_Ts%10+48; // 个位

 unsigned char q;

 for(q=0;q<5;q++){ // 显示
 Dip_Single_char(col_D,q+row_D,Table[q]);
 }
}
/*********************************************************************
** The End
*********************************************************************/