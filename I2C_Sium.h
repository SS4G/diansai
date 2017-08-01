#ifndef I2C_SIUM_H
#define I2C_SIUM_H

#include "msp430g2553.h"

/*******************************************************************************
 * LaunchPad硬件I2C驱动程序(软件模拟I2C时序，只写了I2C发送，不带缓冲区，)
 * 作者：张存
 * 使用方式：
 * 程序初始化过程中，调用I2C_Init();
 * 发送信息给从机时：I2C_Start();
 *                   I2C_SetAdd(adress);
 *                   I2C_SendByte(data);
 *                   I2C_Stop();
 *                   __delay_cycles((long)100);
 *                  完成一次发送
*******************************************************************************/


/*
**定义I2C端口
*/

#define I2C_PORT P1OUT
#define I2C_DIR  P1DIR
#define I2C_SCL  BIT2
#define I2C_SDA  BIT0

/*
**对总线两端口的拉低，拉高操作
*/

#define SetSCL()    I2C_PORT |= I2C_SCL
#define ResetSCL()  I2C_PORT &= ~I2C_SCL
#define SetSDA()    I2C_PORT |= I2C_SDA
#define ResetSDA()  I2C_PORT &= ~I2C_SDA



/*
**以下为本文件所有函数的声明，方便调用：
*/
void I2C_Init(void);
void I2C_SendByte(char I2C_Data);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_SetAdd(char adress);




/*
**函数原型：void I2C_Init()
**入口参数：无
**出口参数：无
**注意事项：无
*/
void I2C_Init(void)
{
  I2C_DIR |= I2C_SCL + I2C_SDA;
}

/*
**函数原型：void I2C_SendByte(char I2C_Data)
**入口参数：I2C_Data ：需要往I2C发送的数据
**出口参数：无
**注意事项：在初始化完成后调用。
*/
void I2C_SendByte(char I2C_Data)
{
  char i = 0;
  for (i = 0; i < 8; i++)
  {
    ResetSCL();__delay_cycles((long)100);
    if(I2C_Data&0x80)
    {SetSDA();__delay_cycles((long)100);}
    else
    {ResetSDA();__delay_cycles((long)100);}
    SetSCL();__delay_cycles((long)100);
    ResetSCL();__delay_cycles((long)100);
    I2C_Data = I2C_Data << 1;
  }
  ResetSDA();__delay_cycles((long)100);
  SetSCL();__delay_cycles((long)100);
  ResetSCL();__delay_cycles((long)100);
}
/*
**函数原型：void I2C_Start(void)
**入口参数：无
**出口参数：无
**注意事项：无
*/
void I2C_Start(void)
{
  SetSCL();  __delay_cycles((long)100);
  SetSDA();__delay_cycles((long)100);
  ResetSDA();__delay_cycles((long)100);
}
/*
**函数原型：void I2C_Stop()
**入口参数：无
**出口参数：无
**注意事项：无
*/
void I2C_Stop()
{
  SetSCL();__delay_cycles((long)100);
  ResetSDA();__delay_cycles((long)100);
  SetSDA();__delay_cycles((long)100);
}
/*
**函数原型：void I2C_SetAdd(char adress)
**入口参数：adress：从机地址
**出口参数：无
**注意事项：无
*/
void I2C_SetAdd(char adress)
{
  I2C_SendByte(adress);
}

#endif