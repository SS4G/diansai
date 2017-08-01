#ifndef LCD1602_SIUM_H
#define LCD1602_SIUM_H

#include "I2C_Sium.h"


/**************************************************************************************
 * LCD1602_I2C��������
 * ���ߣ�PurpleSword
 * 2014.8.7�Ľ����Ľ�˵�����˳���Ӧ��ģ��I2C����ʹ�ã���һЩϸ���������޸�
 * ʹ�÷�ʽ��
 * �����ʼ�������У�����LCD_Init();
 * Ҫ��ʾ�ַ���ʱ��  LCD_Display(0,0,lcd_disp)
 *                   ע����һ������Ϊ�������ڶ�����Ϊ���������һ������ΪҪ��ʾ���ַ�������Ԫ�ص�ַ��ֱ���ô������ɣ�
 *                  ���һ�η���
 *************************************************************************************/


#define LCD_ClearLine0()    LCD_Display(0, 0, "                ")
#define LCD_ClearLine1()    LCD_Display(1, 0, "                ")


/*
**����Ϊ���ļ����к�����������������ã�
*/

void LCD_Init(void);
unsigned char LCD_GenPos(unsigned char line, unsigned char add);
void LCD_Blink(unsigned char line, unsigned char add);
void LCD_BlinkOff();
void LCD_Display(unsigned char line, unsigned char add, char *str);


/****************************************************************
* ��    �ƣ�LCD_Init()
* ��    �ܣ�LCD��ʼ��
* ��ڲ�������
* ���ڲ�������
****************************************************************/
void LCD_Init(void)
{
	I2C_Init();                                        // I2C��ʼ��
        I2C_Start();                                     // ��ʼ��־
	I2C_SetAdd(0x78);				// ѡ�� SLAVE ADDRESS
							
	I2C_SendByte(0x00);					// �����ֽڣ�Co=0,A0=0,��ʾ���´���� N ���ֽ���ָ��
	I2C_SendByte(0x38);					// Function set
	I2C_SendByte(0x0C);					// Display ON/OFF
	I2C_SendByte(0x01);					// Clear display
	I2C_SendByte(0x06);					// Entry mode set
	I2C_Stop();							// ������־
    __delay_cycles((long)16000);
}


/****************************************************************
* ��    �ƣ�LCD_GenPos()
* ��    �ܣ��ڲ���������ָ�������м����Ӧ��LCD��ʾ��ַ
* ��ڲ�������(0-1)����(0-15)
* ���ڲ�������ַ
****************************************************************/
unsigned char LCD_GenPos(unsigned char line, unsigned char add)
{
	return line ? (add + 0x80 + 0x40) : (add + 0x80);
}


/****************************************************************
* ��    �ƣ�LCD_Display()
* ��    �ܣ�LCD��ʾ�ַ���
* ��ڲ�������(0-1)����(0-15)���ַ���
* ���ڲ�������
****************************************************************/
void LCD_Display(unsigned char line, unsigned char add, char *str)
{
							// ѡ�� SLAVE ADDRESS
	I2C_Start();
        I2C_SetAdd(0x78);
	I2C_SendByte(0x80);							// �����ֽڣ�Co=1,A0=0,��ʾ���´���� 1 ���ֽ���ָ��
	I2C_SendByte(LCD_GenPos(line, add));		// ���� DDRAM ADDRESS���ڼ��У��ڼ���
	I2C_SendByte(0x40);							// �����ֽڣ�Co=0,A0=1,��ʾ���´���� n ���ֽ�������
	while(*str && (add++ < 16) ) {
		I2C_SendByte(*str++);
	}
	I2C_Stop();
    __delay_cycles((long)100);
}
/****************************************************************
* ��    �ƣ�LCD_Blink()
* ��    �ܣ�LCD�����˸
* ��ڲ�������(0-1)����(0-15)
* ���ڲ�������
****************************************************************/
void LCD_Blink(unsigned char line, unsigned char add)
{
							// ѡ�� SLAVE ADDRESS
	I2C_Start();
        I2C_SetAdd(0x78);
	I2C_SendByte(0x00);							// �����ֽڣ�Co=0,A0=0,��ʾ���´���� N ���ֽ���ָ��
	I2C_SendByte(LCD_GenPos(line, add));		// ���� DDRAM ADDRESS���ڼ��У��ڼ���
	I2C_SendByte(0x0F);
	I2C_Stop();
    __delay_cycles((long)100);
}


/****************************************************************
* ��    �ƣ�LCD_BlinkOff()
* ��    �ܣ�LCD�����˸�ر�
* ��ڲ�������
* ���ڲ�������
****************************************************************/
void LCD_BlinkOff()
{
							// ѡ�� SLAVE ADDRESS
	I2C_Start();
        I2C_SetAdd(0x78);
	I2C_SendByte(0x80);							// �����ֽڣ�Co=1,A0=0,��ʾ���´���� 1 ���ֽ���ָ��
	I2C_SendByte(0x0C);
	I2C_Stop();
    __delay_cycles((long)100);
}


#endif