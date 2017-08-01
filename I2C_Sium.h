#ifndef I2C_SIUM_H
#define I2C_SIUM_H

#include "msp430g2553.h"

/*******************************************************************************
 * LaunchPadӲ��I2C��������(���ģ��I2Cʱ��ֻд��I2C���ͣ�������������)
 * ���ߣ��Ŵ�
 * ʹ�÷�ʽ��
 * �����ʼ�������У�����I2C_Init();
 * ������Ϣ���ӻ�ʱ��I2C_Start();
 *                   I2C_SetAdd(adress);
 *                   I2C_SendByte(data);
 *                   I2C_Stop();
 *                   __delay_cycles((long)100);
 *                  ���һ�η���
*******************************************************************************/


/*
**����I2C�˿�
*/

#define I2C_PORT P1OUT
#define I2C_DIR  P1DIR
#define I2C_SCL  BIT2
#define I2C_SDA  BIT0

/*
**���������˿ڵ����ͣ����߲���
*/

#define SetSCL()    I2C_PORT |= I2C_SCL
#define ResetSCL()  I2C_PORT &= ~I2C_SCL
#define SetSDA()    I2C_PORT |= I2C_SDA
#define ResetSDA()  I2C_PORT &= ~I2C_SDA



/*
**����Ϊ���ļ����к�����������������ã�
*/
void I2C_Init(void);
void I2C_SendByte(char I2C_Data);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_SetAdd(char adress);




/*
**����ԭ�ͣ�void I2C_Init()
**��ڲ�������
**���ڲ�������
**ע�������
*/
void I2C_Init(void)
{
  I2C_DIR |= I2C_SCL + I2C_SDA;
}

/*
**����ԭ�ͣ�void I2C_SendByte(char I2C_Data)
**��ڲ�����I2C_Data ����Ҫ��I2C���͵�����
**���ڲ�������
**ע������ڳ�ʼ����ɺ���á�
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
**����ԭ�ͣ�void I2C_Start(void)
**��ڲ�������
**���ڲ�������
**ע�������
*/
void I2C_Start(void)
{
  SetSCL();  __delay_cycles((long)100);
  SetSDA();__delay_cycles((long)100);
  ResetSDA();__delay_cycles((long)100);
}
/*
**����ԭ�ͣ�void I2C_Stop()
**��ڲ�������
**���ڲ�������
**ע�������
*/
void I2C_Stop()
{
  SetSCL();__delay_cycles((long)100);
  ResetSDA();__delay_cycles((long)100);
  SetSDA();__delay_cycles((long)100);
}
/*
**����ԭ�ͣ�void I2C_SetAdd(char adress)
**��ڲ�����adress���ӻ���ַ
**���ڲ�������
**ע�������
*/
void I2C_SetAdd(char adress)
{
  I2C_SendByte(adress);
}

#endif