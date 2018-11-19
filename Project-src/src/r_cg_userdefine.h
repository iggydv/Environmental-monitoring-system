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
 * File Name    : r_cg_userdefine.h
 * Version      : CodeGenerator for RL78/G13 V2.02.00.02 [11 Feb 2014]
 * Device(s)    : R5F100LE
 * Tool-Chain   : GCCRL78
 * Description  : This file includes user definition.
 * Creation Date: 5/4/2015
 ***********************************************************************************************************************/

#ifndef _USER_DEF_H
#define _USER_DEF_H

/***********************************************************************************************************************
User definitions
 ***********************************************************************************************************************/
#include "r_cg_rtc.h"
/* Start user code for function. Do not edit comment generated here */
typedef struct
{
	char type;
	uint8_t time[5];
	uint8_t value;
} menu_alarm_entries;

extern rtc_counter_value_t my_time;
extern rtc_counter_value_t* my_time_point;

extern char uart1RxBuf[41];
extern char dataBuf[41];
extern char time_send[5];
extern int uart1RxFlag;
extern char IIC_Buf[352];
extern int IIC_Flag;
extern char uart1TxBuf[40];
extern int uart1TxFlag;
extern MD_STATUS uart1Status;
extern MD_STATUS rtcStatus;
extern int transmit;
extern int rx_count;
extern int tx_count;
extern int data_count;
extern int last_received;
extern uint8_t uart1RxErrFlag, temp, timer;
extern int buzzerON;
extern uint16_t pComp;
extern uint16_t presMplValue;
extern uint16_t tempMplValue;
extern uint16_t pressValue;

extern int wind_count1;
extern int wind_count2;
extern int sec_flag;
extern uint16_t WindSpeed_16;
extern uint16_t WindOldTurns;

extern int fiveMilFlag, fiveMilCount;
extern int tenMilFlag;
extern int valuesTimer;
extern int cycleTimer;
extern int cycleMode;
extern int twoFlag;
extern int oneFlag;
extern int welcomeFlag;

extern char Temperature;
extern char Humidity;
extern char Pressure;
extern char WindSpeed;

extern char displayArr[30];
extern int displayLength;

extern int buzTime, buzFlag, buzTimeFlag, buzState, buzCount;

extern char statusByte;

extern int buzzalarm;

void initializelcd(void);
void LCD_write_Nibble(char e, char f, char g, char h);
void LCD_write_Byte(char data);
void LCD_shift_display();
void powerCheck();
void Start(char[], int);
void delay(int sec);
void clearBuff(int);
char convertTemp(char*);
char convertHumid(char* rawT);
void calCoef();
uint16_t calcPComp(uint16_t mplPadc, uint16_t mplTadc);
void calWind();
void writeNumber(char* buf, char num);
void writeNumber2(char* buf, char num);

extern char oldstate;
void BUZZ_LOOP();
void BUZZ_DOUBLE();
void BUZZ_SHORT();


#define S P3_bit.no1
#define RS P7_bit.no6
#define RNW P7_bit.no5
#define EN P7_bit.no4
#define DB7 P7_bit.no3
#define DB6 P7_bit.no2
#define DB5 P7_bit.no1
#define DB4 P7_bit.no0

#define up P13_bit.no7
#define down P1_bit.no6
#define left P5_bit.no0
#define right P14_bit.no1
#define centre P5_bit.no1

extern char centreBuf[3];
extern int centreFlag;
extern int centreSet;

extern char leftBuf[3];
extern int leftFlag;
extern int leftSet;

extern char rightBuf[3];
extern int rightFlag;
extern int rightSet;

extern char upBuf[3];
extern int upFlag;
extern int upSet;

extern char downBuf[3];
extern int downFlag;
extern int downSet;

extern int menuState;
extern int valueView;
extern int valueDisplay;

extern char Mode;

extern int cycleStart;

extern int powerFlag;
extern int powerCount;

extern menu_alarm_entries alarmsArr[50];
extern int alarmCount;
extern int alarmCurrent;

extern char alarmConditionWind;
extern char alarmConditionHumidity;
extern char alarmConditionPressure;
extern int8_t alarmConditionLowTemp;
extern int8_t alarmConditionTemp;

extern int alarmFlag[5];

#define modeTemp 0x00
#define modeHum 0x01
#define modePres 0x02
#define modeWind 0x03
#define modeDT 0x04

#define modeLowTemp 0x04

#define buzzOff 0x00
#define buzzLong 0x01
#define buzzDouble 0x02
#define buzzShort 0x03

#define CYCLE 0x00
#define VALUES 0x01
#define ALARMS 0x02

#define alarmTypeTemp 0xE1
#define alarmTypeLowTemp 0xE2
#define alarmTypeHum 0xE3
#define alarmTypePres 0xE4
#define alarmTypeWind 0xE6

#define HALT_DEFAULT 0x00
#define HALT_NO_HALT 0x01
#define HALT_HALT 0x02

extern char haltState;




/* End user code. Do not edit comment generated here */
#endif
