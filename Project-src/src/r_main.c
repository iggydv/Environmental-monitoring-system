/***********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only
 * intended for use with Renesas products. No other uses are authorized. This
 * software is owned by Renesas Electronics Corporation and is protected under
 * all applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
 * LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
 * TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
 * ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
 * FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
 * ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
 * BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software
 * and to discontinue the availability of this software.  By using this software,
 * you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2011, 2014 Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * File Name    : r_main.c
 * Version      : CodeGenerator for RL78/G13 V2.02.00.02 [11 Feb 2014]
 * Device(s)    : R5F100LE
 * Tool-Chain   : GCCRL78
 * Description  : This file implements main function.
 * Creation Date: 5/4/2015
 ***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
 ***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_serial.h"
#include "r_cg_timer.h"
#include "r_cg_rtc.h"
#include "r_cg_pclbuz.h"
/* Start user code for include. Do not edit comment generated here */
#include <stdlib.h>
#include <string.h>
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
 ***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */

int a;

char uart1TxBuf[40], test;
char dataBuf[41];
char IIC_Buf[352];
char Welcome[] = "Welcome / Welkom";
char tempArr[] = "Temperature";
char humArr[] = "Humidity";
char PresArr[] = "Pressure";
char WindArr[] = "Wind Speed";
char clockSet[] = "Set Clock";
char clockGet[] = "Get Clock";

char Mode;
rtc_counter_value_t my_time = {0,0,0,0,0,0,0};
rtc_counter_value_t* my_time_point;

int uart1RxFlag;
int IIC_Flag;
int uart1TxFlag;
MD_STATUS uart1Status;
MD_STATUS rtcStatus;
int transmit;
int rx_count;
int tx_count;
int data_count;
int last_received;
int buzzerON, buzState;
int welcomeFlag = 0;
uint8_t uart1RxErrFlag, temp, timer=0;
uint16_t convTempHum;
int menuState;
int valueView;
int valueDisplay;

int cycleStart;
int powerFlag;

uint16_t pComp;
uint16_t presMplValue;
uint16_t tempMplValue;
uint16_t pressValue;

menu_alarm_entries alarmsArr[50];
int alarmCount;
int alarmCurrent;
char alarmConditionWind;
char alarmConditionHumidity;
char alarmConditionPressure;
int8_t alarmConditionLowTemp;
int8_t alarmConditionTemp;
int alarmFlag[5];

char statusByte;
int buzzalarm = 0;

char haltState;

/* End user code. Do not edit comment generated here */
void R_MAIN_UserInit(void);

/***********************************************************************************************************************
 * Function Name: main
 * Description  : This function implements main function.
 * Arguments    : None
 * Return Value : None
 ***********************************************************************************************************************/
void main(void)
{
	int a;
	R_MAIN_UserInit();
	/* Start user code. Do not edit comment generated here */


	R_PORT_Create();
	PM7 = 0x00 ;

	R_TAU0_Create();

	R_PCLBUZ0_Create();

	R_RTC_Start();

	initializelcd();

	buzzerON = 0;
	Mode = 'N';
	buzState = buzzOff;
	menuState = CYCLE;
	valueView = 0;
	valueDisplay = 0;
	powerFlag = 0;
	oldstate = 2;

	haltState = 0;

	statusByte = 0;

	cycleStart = 0;



	my_time_point = &my_time;

	alarmCount = 0;
	alarmCurrent = 0;

	alarmConditionWind = 8;
	alarmConditionHumidity = 200;
	alarmConditionPressure = 206;
	alarmConditionLowTemp = -20;
	alarmConditionTemp = 80;
	for(a=0;a<5;a++)
		alarmFlag[a] = 0;


	do{

		rtcStatus = R_RTC_Set_CounterValue(my_time); // Update the RTC

	} while(rtcStatus != MD_OK);

	R_UART1_Create();
	R_UART1_Start();

	IIC_Flag = 0;

	my_time.sec = Dec_to_BCD(0);
	my_time.min = Dec_to_BCD(15);
	my_time.hour = Dec_to_BCD(10);
	my_time.day = Dec_to_BCD(12);
	my_time.month = Dec_to_BCD(3);

	do{
		rtcStatus = R_RTC_Set_CounterValue(my_time); // Update the RTC

	} while(rtcStatus != MD_OK);

	for(a=0;a<250;a++)
		IIC_Buf[a] = 0;


	tx_count = 0;
	rx_count = 0;
	data_count = 0;
	uart1Status = R_UART1_Receive(&uart1RxBuf[rx_count],1);
	Start(Welcome, sizeof(Welcome)-1);

	R_TAU0_Channel1_Start();
	R_TAU0_Channel3_Start();
	R_TAU0_Channel4_Start();
	R_TAU0_Channel5_Start();
	R_TAU0_Channel7_Start();
	//--------------------------------------------------------------------------------------------------
	IIC_Buf[0] = 0x4;

	while(R_IICA0_Busy_Check());

	R_IICA0_Master_Send(0xC0, IIC_Buf, 1, 0);

	while(IIC_Flag == 0)
		NOP();
	IIC_Flag = 0;

	delayNoInt(300);


	delayNoInt(50000);
	delayNoInt(39000);
	R_IICA0_Master_Receive(0xC1, IIC_Buf, 8, 0);

	while(IIC_Flag == 0)
		NOP();
	IIC_Flag = 0;

	delayNoInt(1000);

	calCoef(IIC_Buf);



	//--------------------------------------------------------------------------------------------------


	while (1U)
	{

		flagCheck();
		if((welcomeFlag==0))
		{
			if((twoFlag))
			{
				oneFlag = 0;
				twoFlag = 0;
				cycleTimer = 0;
				cycleStart = 1;
				welcomeFlag = 1;
			}
		}
		if((cycleStart)||((oneFlag)&&(welcomeFlag)))
		{
			oneFlag = 0;
			if((Mode=='N')&&(menuState==CYCLE)&&(!powerFlag))
				cycle();
		}
		if (uart1RxFlag)
		{
			uart1RxFlag = 0;
			last_received = (int)uart1RxBuf[rx_count];

			if(((last_received>=0x00)&&(last_received<=0x7F)))
			{
				last_received=0x7F;
			}

			if(Mode == 'N')
			{

				switch(last_received)
				{


				case 0x7F :

					dataBuf[data_count] = uart1RxBuf[rx_count];
					if(data_count<40)
						data_count++;
					if(rx_count<40)
						rx_count++;

					break;

				case 0x81 :
					//Test&Monitor
					R_TAU0_Channel3_Stop();


					//BUZZ_SHORT();
					buzState = buzzShort;
					Buzzer();
					uart1Status = R_UART1_Send(&uart1RxBuf[rx_count], 1);
					Mode = 'T';
					char word[] = {"Test Mode"};
					int length = sizeof(word) -1;
					Start(word, length);


					break;

				case 0xF4 :

					uart1Status = R_UART1_Send(&dataBuf[data_count], 1);
					Start(dataBuf,data_count);
					data_count =0;
					break;


				}

			}
			flagCheck();
			if(Mode == 'T')
			{



				switch(last_received)
				{
				case 0xF0 :

					BUZZ_LOOP();
					buzState = buzzLong;
					R_TAU0_Channel2_Start();

					break;

				case 0xF1 :

					buzState = buzzOff;

					buzFlag = 0;
					R_TAU0_Channel2_Stop();

					break;




				case 0x7F :

					dataBuf[data_count] = uart1RxBuf[rx_count];
					if(data_count<40)
						data_count++;
					if(rx_count<40)
						rx_count++;

					break;


				case 0x80 :
					//Normal

					uart1Status = R_UART1_Send(&uart1RxBuf[rx_count], 1);
					Mode = 'N';
					char word[] = {"Normal Mode"};
					int length = sizeof(word) -1;
					Start(word, length);
					R_TAU0_Channel3_Start();
					cycleTimer = 0;
					cycleMode = 0;
					oneFlag = 0;
					twoFlag = 0;
					welcomeFlag = 0;

					rx_count = 0;
					break;

					//high temp
				case 0xE0 :
					uart1TxBuf[0] = uart1RxBuf[rx_count];
					uart1Status = R_UART1_Send(uart1TxBuf, 1);

					alarmConditionTemp = uart1RxBuf[rx_count-1];

					rx_count = 0;
					break;

					//low temp
				case 0xE1 :
					uart1TxBuf[0] = uart1RxBuf[rx_count];
					uart1Status = R_UART1_Send(uart1TxBuf, 1);

					alarmConditionLowTemp = uart1RxBuf[rx_count-1];


					rx_count = 0;
					break;
					//high Humidity
				case 0xE2 :
					uart1TxBuf[0] = uart1RxBuf[rx_count];
					uart1Status = R_UART1_Send(uart1TxBuf, 1);

					alarmConditionHumidity = uart1RxBuf[rx_count-1] + 80;


					rx_count = 0;
					break;
					//high pressure
				case 0xE3 :
					uart1TxBuf[0] = uart1RxBuf[rx_count];
					uart1Status = R_UART1_Send(uart1TxBuf, 1);

					alarmConditionPressure = uart1RxBuf[rx_count-1] + 160;


					rx_count = 0;
					break;
					//high wind/s
				case 0xE4 :
					uart1TxBuf[0] = uart1RxBuf[rx_count];
					uart1Status = R_UART1_Send(uart1TxBuf, 1);

					alarmConditionWind = uart1RxBuf[rx_count-1];


					rx_count = 0;
					break;
				case 0xE8 :
					uart1TxBuf[0] = uart1RxBuf[rx_count];
					uart1Status = R_UART1_Send(uart1TxBuf, 1);
					switch(uart1RxBuf[rx_count-1])
					{
					case HALT_DEFAULT :
						haltState = HALT_DEFAULT;
						break;
					case HALT_NO_HALT :
						haltState = HALT_NO_HALT;
						break;
					case HALT_HALT :
						haltState = HALT_HALT;
						break;

					}

					break;

					case 0xF2 :
						//Set_Clock
						uart1TxBuf[0] = uart1RxBuf[rx_count];
						uart1Status = R_UART1_Send(uart1TxBuf, 1);

						my_time.sec = Dec_to_BCD(uart1RxBuf[4]);
						my_time.min = Dec_to_BCD(uart1RxBuf[3]);
						my_time.hour = Dec_to_BCD(uart1RxBuf[2]);
						my_time.day = Dec_to_BCD(uart1RxBuf[1]);
						my_time.month = Dec_to_BCD(uart1RxBuf[0]);

						do{
							rtcStatus = R_RTC_Set_CounterValue(my_time); // Update the RTC

						} while(rtcStatus != MD_OK);


						rx_count = 0;
						Start(clockSet,sizeof(clockSet)-1);
						break;

					case 0xF3 :
						//Receive_Date/Time
						uart1TxBuf[0] = uart1RxBuf[rx_count];

						do{
							rtcStatus = R_RTC_Get_CounterValue(my_time_point); // Update the RTC

						} while(rtcStatus != MD_OK);

						uart1TxBuf[5] = BCD_to_Dec(my_time.sec);
						uart1TxBuf[4] = BCD_to_Dec(my_time.min);
						uart1TxBuf[3] = BCD_to_Dec(my_time.hour);
						uart1TxBuf[2] = BCD_to_Dec(my_time.day);
						uart1TxBuf[1] = BCD_to_Dec(my_time.month);

						uart1Status = R_UART1_Send(uart1TxBuf,6);
						Start(clockGet,sizeof(clockGet)-1);

						rx_count = 0;
						break;

					case 0xF4 :

						uart1Status = R_UART1_Send(&dataBuf[data_count], 1);
						Start(dataBuf,data_count);
						data_count =0;
						break;

					case 0xF5 :
						alarmCount = 0;
						alarmCurrent = 0;
						break;

					case 0xF6 :

						IIC_Buf[0] = 0xF6;
						memcpy((IIC_Buf+1),alarmsArr,7*alarmCount);

						R_UART1_Send(IIC_Buf, (1+7*alarmCount));


						rx_count = 0;
						break;

					case 0xF7 :

						//Read Temperature Sample
						IIC_Buf[1] = Temperature;
						IIC_Buf[0] = 0xF7;
						R_UART1_Send(IIC_Buf, 2);


						rx_count = 0;
						break;

					case 0xF8 :

						IIC_Buf[1] = Humidity;
						IIC_Buf[0] = 0xF8;
						R_UART1_Send(IIC_Buf, 2);


						rx_count = 0;
						break;


					case 0xF9 :
						IIC_Buf[1] = Pressure;
						IIC_Buf[0] = 0xF9;
						R_UART1_Send(IIC_Buf, 2);


						rx_count = 0;
						break;

					case 0xFE :
						IIC_Buf[1] = WindSpeed;
						IIC_Buf[0] = 0xFE;
						R_UART1_Send(IIC_Buf, 2);


						rx_count = 0;
						break;

					case 0xFF :
						readStatus();
						IIC_Buf[0] = 0xFF;
						IIC_Buf[1] = statusByte;
						R_UART1_Send(IIC_Buf, 2);


						rx_count = 0;
						break;
				}
			}
			flagCheck();
			uart1Status = R_UART1_Receive(&uart1RxBuf[rx_count],1);

		}



		flagCheck();



		switch(haltState)
		{
		case HALT_DEFAULT :
			if(S==1)
				HALT();
			break;
		case HALT_NO_HALT :

			break;
		case HALT_HALT :
			HALT();
			break;
		}

	}
	/* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
 * Function Name: R_MAIN_UserInit
 * Description  : This function adds user code before implementing main function.
 * Arguments    : None
 * Return Value : None
 ***********************************************************************************************************************/
void R_MAIN_UserInit(void)
{
	/* Start user code. Do not edit comment generated here */
	EI();

	/* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
