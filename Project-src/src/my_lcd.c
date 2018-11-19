#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_timer.h"

#include "r_cg_userdefine.h"

char oldstate;
char uart1RxBuf[41];
char displayArr[30];
int displayLength;


void startTMR0(int delay)
{
	TMIF00 = 0U; // Clear any interrupt flag
	TDR00  = delay; // Load register (in 1 usec units)
	TS0    = 1U; // Start timer
}
void delayNoInt(uint16_t delay)
{
	startTMR0(delay); // Start timer with delay value
	while(TMIF00 == 0) // Wait for timer flag
		NOP();
	R_TAU0_Channel0_Stop(); // Stop timer and clear all flags
}


void initializelcd()
{
	RS = 0;
	RNW = 0;

	delayNoInt(15000);

	LCD_write_Nibble(0,0,1,1);
	delayNoInt(4100);

	LCD_write_Nibble(0,0,1,1);
	delayNoInt(200);

	LCD_write_Nibble(0,0,1,1);
	delayNoInt(70);

	//4 bit mode
	LCD_write_Nibble(0,0,1,0);
	delayNoInt(70);

	//func set
	LCD_write_Nibble(0,0,1,0);
	//delayNoInt(70);
	LCD_write_Nibble(1,0,0,0);
	delayNoInt(70);

	//cursor on
	LCD_write_Nibble(0,0,0,0);
	//delayNoInt(70);
	LCD_write_Nibble(1,1,1,0);
	delayNoInt(70);

	LCD_write_Nibble(0,0,0,0);
	//delayNoInt(70);
	LCD_write_Nibble(0,0,0,1);
	delayNoInt(2000);

	//cursor mode inc
	LCD_write_Nibble(0,0,0,0);
	//delayNoInt(200);
	LCD_write_Nibble(0,1,1,0);

	//delayNoInt(70);

	//Clear
	LCD_write_Nibble(0,0,0,0);
	//delayNoInt(70);
	LCD_write_Nibble(0,0,0,1);
	delayNoInt(3000);

}

void LCD_write_Nibble(char a, char b, char c, char d)
{

	RNW = 0;

	NOP();NOP();

	EN = 1;

	DB7 = a;
	DB6 = b;
	DB5 = c;
	DB4 = d;

	NOP();NOP();NOP();NOP();NOP();NOP();NOP();

	EN = 0;

	NOP();NOP();

	delayNoInt(100);


}

void LCD_write_Byte(char data)
{

	char d = (data & 0b10000000) >> 7;
	char c = (data & 0b01000000) >> 6;
	char b = (data & 0b00100000) >> 5;
	char a = (data & 0b00010000) >> 4;

	char h = (data & 0b00001000) >> 3;
	char g = (data & 0b00000100) >> 2;
	char f = (data & 0b00000010) >> 1;
	char e = (data & 0b00000001);


	LCD_write_Nibble(d,c,b,a);
	LCD_write_Nibble(h,g,f,e);

}

void LCD_shift_display()
{
	int b;

	RS = 0;
	RNW = 0;


	LCD_write_Nibble(0,0,0,1);
	LCD_write_Nibble(1,0,0,0);
	RS = 1;

	for(b=0;b<1000;b++)
	{
		delayNoInt(1000);
	}






}


void powerCheck()
{
	if((S == 0) && (oldstate != 0))
	{

		RS = 0;
		RNW = 0;
		LCD_write_Nibble(0,0,0,0);
		LCD_write_Nibble(0,0,0,1);
		delayNoInt(1640);
		RS = 1;

		LCD_write_Byte('M');

		LCD_write_Byte('a');

		LCD_write_Byte('i');

		LCD_write_Byte('n');


		oldstate = 0;
		powerFlag = 1;
		powerCount = 0;

	}

	else if((S == 1) && (oldstate != 1))
	{
		RS = 0;
		RNW = 0;
		LCD_write_Nibble(0,0,0,0);
		LCD_write_Nibble(0,0,0,1);
		delayNoInt(1640);
		RS = 1;
		LCD_write_Byte('B');

		LCD_write_Byte('a');

		LCD_write_Byte('t');

		LCD_write_Byte('t');

		LCD_write_Byte('e');

		LCD_write_Byte('r');

		LCD_write_Byte('y');


		oldstate = 1;
		powerFlag = 1;
		powerCount = 0;
	}

}



void Start(char Arr[], int len)
{
	int a,b,c;
	char s;
	char t[7] = {};
	RS=0;

	//Clear
	LCD_write_Nibble(0,0,0,0);
	LCD_write_Nibble(0,0,0,1);
	delayNoInt(3000);

	// Cursor home
	LCD_write_Nibble(0,0,0,0);
	LCD_write_Nibble(0,0,1,0);
	delayNoInt(1640);

	RS = 1;

	if(len<=8)
	{
		for(a=0;a<len;a++)
		{
			LCD_write_Byte(Arr[a]);
		}

	}


	else if(len<=16)
	{
		for(a=0;a<8;a++)
		{
			LCD_write_Byte(Arr[a]);
		}



		RS = 0;
		RNW = 0;

		LCD_write_Nibble(1,1,0,0);
		LCD_write_Nibble(0,0,0,0);
		delayNoInt(40);

		RS=1;

		for(a=8;a<len;a++)
		{
			LCD_write_Byte(Arr[a]);
		}

	}

	// 3de geval
	else
	{
		for(a=0;a<len;a++)
		{
			LCD_write_Byte(Arr[a]);
		}


		RS = 0;
		RNW = 0;

		LCD_write_Nibble(1,1,0,0);
		LCD_write_Nibble(0,0,0,0);
		delayNoInt(40);

		RS=1;

		for(a=8;a<len;a++)
		{
			LCD_write_Byte(Arr[a]);
		}
		delay(2);

		for(a = 0; a< len-16 ; a++)
		{
			LCD_shift_display();

			if (S != oldstate)
			{
				powerCheck();
				break;
			}
		}
		delay(1);

	}




}

void delay(int sec)
{
	int a,b;
	for(a=0;a<sec;a++)
	{
		for(b=0;b<1000;b++)
		{
			delayNoInt(1000);
		}
	}
}

void clearBuff(int count)
{
	int a;
	for(a=0;a<count;a++)
	{
		uart1RxBuf[a] = 0x00;
	}
}

void writeNumber(char* buf, char num)
{


	buf[0] = ('0' + num/200);
	buf[1] = ('0' + (num%200)/20);
	buf[2] = ('0' + (num%20)/2);
	buf[3] = ('.');
	buf[4] = ((num%2)?'5':'0');

}

void writeNumber2(char* buf, char num)
{

	buf[0] = ('0' + (num%200)/20);
	buf[1] = ('0' + (num%20)/2);

}

