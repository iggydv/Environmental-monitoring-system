
#include "my_functions.h"
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_serial.h"
#include "r_cg_timer.h"
#include "r_cg_rtc.h"
#include "r_cg_pclbuz.h"
#include "r_cg_userdefine.h"
#include <string.h>

int cycleTimer = 0;
int cycleMode = 0;
char centreBuf[3] = {1,1,1};
int centreFlag = 0, centreSet = 1;

char upBuf[3] = {1,1,1};
int upFlag = 0, upSet = 1;

char downBuf[3] = {1,1,1};
int downFlag = 0, downSet = 1;

char leftBuf[3] = {1,1,1};
int leftFlag = 0, leftSet = 1;

char rightBuf[3] = {1,1,1};
int rightFlag = 0, rightSet = 1;

char Temperature =0,Humidity=0,Pressure=0,WindSpeed=0;


char tempArr1[] = "Temperature";
char humArr1[] = "Humidity";
char presArr1[] = "Pressure";
char windArr1[] = "Wind Speed";
char DTArr1[] = "Date/Time";
uint8_t display;

uint8_t wind_8bit;


//temp, hum, press, wind, time
void cycle()
{
	if(cycleStart)
	{
		R_TAU0_Channel3_Stop();
		R_TAU0_Channel3_Start();
		cycleMode = modeTemp;
		cycleTimer = 0;
		cycleStart = 0;
		oneFlag = 0;
		twoFlag = 0;
	}

	RS = 0;
	//Clear
	LCD_write_Nibble(0,0,0,0);
	LCD_write_Nibble(0,0,0,1);
	delayNoInt(3000);

	// Cursor home
	LCD_write_Nibble(0,0,0,0);
	LCD_write_Nibble(0,0,1,0);
	delayNoInt(1640);

	RS = 1;

	if((cycleTimer<9)&&((twoFlag)))
	{
		twoFlag = 0;
		cycleMode++;
	}
	else if(cycleTimer>=60)
	{
		cycleTimer = 0;
		cycleMode = 0;
		oneFlag = 0;
		twoFlag = 0;
	}

	switch(cycleMode)
	{

	case modeTemp :
	{
		strcpy(displayArr, "Temp ");
		writeNumber(displayArr + 5, Temperature);
		displayArr[10] = '\'';
		displayArr[11] = 'C';
		Start(displayArr,12);
	}
	break;

	case modeHum :

		strcpy(displayArr, "Humidity ");
		writeNumber(displayArr + 9, Humidity);
		displayArr[14] = '%';
		Start(displayArr,15);

		break;

	case modePres :

		strcpy(displayArr, "Press ");
		writeNumber(displayArr + 6, Pressure);
		displayArr[11] = 'k';
		displayArr[12] = 'P';
		displayArr[13] = 'a';

		Start(displayArr,14);
		break;

	case modeWind :

		strcpy(displayArr, "Wind/S ");
		writeNumber(displayArr + 7, WindSpeed);
		displayArr[12] = 'm';
		displayArr[13] = '/';
		displayArr[14] = 's';
		Start(displayArr,15);
		break;

	case modeDT :
	{

		uart1TxBuf[5] = BCD_to_Dec(my_time.sec);
		uart1TxBuf[4] = BCD_to_Dec(my_time.min);
		uart1TxBuf[3] = BCD_to_Dec(my_time.hour);
		uart1TxBuf[2] = BCD_to_Dec(my_time.day);
		uart1TxBuf[1] = BCD_to_Dec(my_time.month);

		strcpy(displayArr, "");
		writeNumber2(displayArr, uart1TxBuf[2]*2);
		displayArr[2] = '/';
		writeNumber2(displayArr+3, uart1TxBuf[1]*2);
		displayArr[5] = ' ';
		writeNumber2(displayArr+6, uart1TxBuf[3]*2);
		displayArr[8] = ':';
		writeNumber2(displayArr+9, uart1TxBuf[4]*2);
		displayArr[11] = ':';
		writeNumber2(displayArr+12, uart1TxBuf[5]*2);



		Start(displayArr,14);

	}
	break;


	}
}

void IIC_Receive()
{

	switch(valuesTimer)
	{
	//Temperature
	case 1 :

		timer = 0;

		IIC_Buf[0] = 0xF3;

		while(R_IICA0_Busy_Check());

		R_IICA0_Master_Send(0x80, IIC_Buf, 1, 0);

		while(IIC_Flag == 0)
			NOP();
		IIC_Flag = 0;
		break;
	case 2 :
		R_IICA0_StopCondition();
		break;
	case 11 :
		R_IICA0_Master_Receive(0x81, IIC_Buf, 2, 0);

		while(IIC_Flag == 0)
			NOP();
		IIC_Flag = 0;
		break;
	case 12 :
		R_IICA0_StopCondition(); //Send stop condition

		IIC_Buf[1] = convertTemp(IIC_Buf);
		IIC_Buf[0] = 0xF7;

		Temperature = IIC_Buf[1];

		if((Temperature>alarmConditionTemp)&&(!alarmFlag[modeTemp]))
		{
			alarmFlag[modeTemp] = 1;
			alarmsArr[alarmCount].type = alarmTypeTemp;
			alarmsArr[alarmCount].time[0] = my_time.month;
			alarmsArr[alarmCount].time[1] = my_time.day;
			alarmsArr[alarmCount].time[2] = my_time.hour;
			alarmsArr[alarmCount].time[3] = my_time.min;
			alarmsArr[alarmCount].time[4] = my_time.sec;
			alarmsArr[alarmCount].value = Temperature;
			alarmCount++;
			alarm_new();
		}
		else if((Temperature<alarmConditionTemp)&&(alarmFlag[modeTemp]))
		{
			alarmFlag[modeTemp] = 0;
		}

		if((Temperature<alarmConditionLowTemp)&&(!alarmFlag[modeLowTemp]))
		{
			alarmFlag[modeLowTemp] = 1;
			alarmsArr[alarmCount].type = alarmTypeLowTemp;
			alarmsArr[alarmCount].time[0] = my_time.month;
			alarmsArr[alarmCount].time[1] = my_time.day;
			alarmsArr[alarmCount].time[2] = my_time.hour;
			alarmsArr[alarmCount].time[3] = my_time.min;
			alarmsArr[alarmCount].time[4] = my_time.sec;
			alarmsArr[alarmCount].value = Temperature;
			alarmCount++;
			alarm_new();
		}
		else if((Temperature>alarmConditionLowTemp)&&(alarmFlag[modeLowTemp]))
		{
			alarmFlag[modeLowTemp] = 0;
		}

		//R_UART1_Send(IIC_Buf, 2);

		break;
		//Humidity
	case 13 :
		timer = 0;
		IIC_Flag = 0;
		//Send data to slave
		IIC_Buf[0] = 0xF5;
		R_IICA0_Master_Send(0x80, IIC_Buf, 1, 0);
		while(IIC_Flag == 0)
			NOP();
		IIC_Flag = 0;
		break;
	case 22 :
		R_IICA0_Master_Receive(0x81, IIC_Buf, 2, 0);

		timer = 0;
		//Wait for received end
		while(IIC_Flag == 0)
			NOP();
		IIC_Flag = 0;
		R_IICA0_StopCondition(); //Send stop condition

		IIC_Buf[1] = convertHumid(IIC_Buf);
		IIC_Buf[0] = 0xF8;

		Humidity = IIC_Buf[1];

		if((Humidity>alarmConditionHumidity)&&(!alarmFlag[modeHum]))
		{
			alarmFlag[modeHum] = 1;
			alarmsArr[alarmCount].type = alarmTypeHum;
			alarmsArr[alarmCount].time[0] = my_time.month;
			alarmsArr[alarmCount].time[1] = my_time.day;
			alarmsArr[alarmCount].time[2] = my_time.hour;
			alarmsArr[alarmCount].time[3] = my_time.min;
			alarmsArr[alarmCount].time[4] = my_time.sec;
			alarmsArr[alarmCount].value = Humidity;
			alarmCount++;
			alarm_new();
		}
		else if((Humidity<alarmConditionHumidity)&&(alarmFlag[modeHum]))
		{
			alarmFlag[modeHum] = 0;
		}

		break;

		//Pressure
	case 23 :
		IIC_Buf[0] = 0x12;
		IIC_Buf[1] = 0x00;

		while(R_IICA0_Busy_Check());

		R_IICA0_Master_Send(0xC0, IIC_Buf, 2, 0);
		break;
	case 29 :
		IIC_Buf[0] = 0x00;
		R_IICA0_Master_Send(0xC0, IIC_Buf, 1, 0);
		break;
	case 35 :
		R_IICA0_Master_Receive(0xC1, IIC_Buf, 4, 0);
		break;
	case 41 :
		presMplValue = (IIC_Buf[0]*256U) + IIC_Buf[1];
		tempMplValue = (IIC_Buf[2]*256U) + IIC_Buf[3];

		// Calculate pComp using a fixed-point calculation - return is a scaled pComp
		pComp = calcPComp(presMplValue, tempMplValue);
		// Calculate the pressure range factor (115 - 50) = 65 and
		// then the pressure from Pressure = pComp * (115-50)/1024 + 50
		// The product 65*pComp must be shifted by 13 shifts i.e. (1024 x 4 times x 0.5) to obtain
		// pressValue in 0.5 kPa units (we need to transmit to the PC in 0.5 kPa units)
		pressValue = (uint16_t) (((int32_t) pComp * 65) >> 13) + 100;



		IIC_Buf[1] = pressValue;
		IIC_Buf[0] = 0xF9;

		Pressure = IIC_Buf[1];

		if((Pressure>alarmConditionPressure)&&(!alarmFlag[modePres]))
		{
			alarmFlag[modePres] = 1;
			alarmsArr[alarmCount].type = alarmTypePres;
			alarmsArr[alarmCount].time[0] = my_time.month;
			alarmsArr[alarmCount].time[1] = my_time.day;
			alarmsArr[alarmCount].time[2] = my_time.hour;
			alarmsArr[alarmCount].time[3] = my_time.min;
			alarmsArr[alarmCount].time[4] = my_time.sec;
			alarmsArr[alarmCount].value = Pressure;
			alarmCount++;
			alarm_new();
		}
		else if((Pressure<alarmConditionPressure)&&(alarmFlag[modePres]))
		{
			alarmFlag[modePres] = 0;
		}

		//R_UART1_Send(IIC_Buf, 2);
		break;

		//Wind speed
	case 42 :
		if(sec_flag)
		{
			calWind();
			wind_8bit = (uint8_t)WindSpeed_16;
		}


		IIC_Buf[1] = WindSpeed_16;
		IIC_Buf[0] = 0xFE;

		while(R_IICA0_Busy_Check());

		WindSpeed = IIC_Buf[1];

		if((WindSpeed>alarmConditionWind)&&(!alarmFlag[modeWind]))
		{
			alarmFlag[modeWind] = 1;
			alarmsArr[alarmCount].type = alarmTypeWind;
			alarmsArr[alarmCount].time[0] = my_time.month;
			alarmsArr[alarmCount].time[1] = my_time.day;
			alarmsArr[alarmCount].time[2] = my_time.hour;
			alarmsArr[alarmCount].time[3] = my_time.min;
			alarmsArr[alarmCount].time[4] = my_time.sec;
			alarmsArr[alarmCount].value = WindSpeed;
			alarmCount++;
			alarm_new();
		}
		else if((WindSpeed<alarmConditionWind)&&(alarmFlag[modeWind]))
		{
			alarmFlag[modeWind] = 0;
		}




		break;
	case 51 :
		valueDisplay = 1;
		do{

			rtcStatus = R_RTC_Get_CounterValue(my_time_point); // Update the RTC

		} while(rtcStatus != MD_OK);
		break;

	}

}


void buttons()
{
	centreBuf[0] = centre;
	if((!centreSet)&&(centreBuf[0]==1)&&(centreBuf[1]==1)&&(centreBuf[2]==1))
		centreSet = 1;

	leftBuf[0] = left;
	if((!leftSet)&&(leftBuf[0]==1)&&(leftBuf[1]==1)&&(leftBuf[2]==1))
		leftSet = 1;

	rightBuf[0] = right;
	if((!rightSet)&&(rightBuf[0]==1)&&(rightBuf[1]==1)&&(rightBuf[2]==1))
		rightSet = 1;

	upBuf[0] = up;
	if((!upSet)&&(upBuf[0]==1)&&(upBuf[1]==1)&&(upBuf[2]==1))
		upSet = 1;

	downBuf[0] = down;
	if((!downSet)&&(downBuf[0]==1)&&(downBuf[1]==1)&&(downBuf[2]==1))
		downSet = 1;

	if(centreSet)
	{
		if((((centreBuf[0]==0)&&(centreBuf[1]==0))||((centreBuf[1]==0)&&(centreBuf[2]==0))))
		{
			centreFlag = 1;
			centreSet = 0;

			if(buzFlag)
			{
				buzzerON = 0;
				buzState = buzzOff;
				buzFlag = 0;
				R_TAU0_Channel2_Stop();
				buzTime=10;
				buzzalarm = 0;
				R_PCLBUZ0_Stop();
			}

		}
		else
			centreFlag = 0;
	}
	else
		centreFlag = 0;

	if(leftSet)
	{
		if((((leftBuf[0]==0)&&(leftBuf[1]==0))||((leftBuf[1]==0)&&(leftBuf[2]==0))))
		{
			leftFlag = 1;
			leftSet = 0;
			if(Mode=='N')
			{
				if(menuState==CYCLE)
					menuState = ALARMS;
				else
					menuState--;

				oneFlag = 0;
				twoFlag = 0;
				cycleTimer = 0;
				cycleStart = 1;

				cycleMode = modeTemp;
				valueDisplay = 1;


			}

		}
		else
			leftFlag = 0;
	}
	else
		leftFlag = 0;

	if(rightSet)
	{
		if((((rightBuf[0]==0)&&(rightBuf[1]==0))||((rightBuf[1]==0)&&(rightBuf[2]==0))))
		{
			rightFlag = 1;
			rightSet = 0;
			if(Mode=='N')
			{
				if(menuState==ALARMS)
					menuState = CYCLE;
				else
					menuState++;

				oneFlag = 0;
				twoFlag = 0;
				cycleTimer = 0;
				cycleStart = 1;

				cycleMode = modeTemp;
				valueDisplay = 1;


			}

		}
		else
			rightFlag = 0;
	}
	else
		rightFlag = 0;
	if(upSet)
	{
		if((((upBuf[0]==0)&&(upBuf[1]==0))||((upBuf[1]==0)&&(upBuf[2]==0))))
		{
			upFlag = 1;
			upSet = 0;
			if((Mode=='N')&&(menuState==VALUES))
			{
				if(cycleMode==modeTemp)
					cycleMode = modeDT;
				else
					cycleMode--;

				valueView = 0;


			}
			else if((Mode=='N')&&(menuState==ALARMS))
			{
				if(alarmCurrent==0)
				{
					alarmCurrent = alarmCount-1;
				}
				else
				{
					alarmCurrent--;
				}
			}


		}
		else
			upFlag = 0;
	}
	else
		upFlag = 0;

	if(downSet)
	{
		if((((downBuf[0]==0)&&(downBuf[1]==0))||((downBuf[1]==0)&&(downBuf[2]==0))))
		{
			downFlag = 1;
			downSet = 0;
			if((Mode=='N')&&(menuState==VALUES))
			{
				if(cycleMode==modeDT)
					cycleMode = modeTemp;
				else
					cycleMode++;

				valueView = 0;
			}
			else if((Mode=='N')&&(menuState==ALARMS))
			{
				if(alarmCurrent==(alarmCount-1))
				{
					alarmCurrent = 0;
				}
				else
				{
					alarmCurrent++;
				}
			}

		}
		else
			downFlag = 0;
	}
	else
		downFlag = 0;

	centreBuf[2] = centreBuf[1];
	centreBuf[1] = centreBuf[0];

	leftBuf[2] = leftBuf[1];
	leftBuf[1] = leftBuf[0];

	rightBuf[2] = rightBuf[1];
	rightBuf[1] = rightBuf[0];

	upBuf[2] = upBuf[1];
	upBuf[1] = upBuf[0];

	downBuf[2] = downBuf[1];
	downBuf[1] = downBuf[0];



}

void flagCheck()
{
	if ((S != oldstate)&&(welcomeFlag))
	{
		powerCheck();

	}
	if(fiveMilFlag)
	{
		Buzzer();
		fiveMilFlag = 0;
	}
	if(centreFlag)
	{
		centreFlag = 0;
		//BUZZ_SHORT();
		int a;
		for(a=0;a<3;a++)
			centreBuf[a] = 1;
	}
	if(tenMilFlag)
	{
		tenMilFlag = 0;
		buttons();

		if((!powerFlag)&&(Mode == 'N'))
		{

			if(menuState==VALUES)
				values_menu();
			else if(menuState==ALARMS)
				alarms_menu();
		}
		if(valuesTimer>=100)
			valuesTimer = 0;
		IIC_Receive();
	}

	if(sec_flag)
	{
		calWind();
	}
	if(buzFlag&&buzTimeFlag)
	{

		buzTimeFlag = 0;
		BUZZ_LOOP();
	}
}

void values_menu()
{
	switch(cycleMode)
	{
	case modeTemp :
	{
		if(centreFlag)
		{
			valueView = 1;
			valueDisplay = 1;
		}
		if((!valueView)&&(valueDisplay))
		{
			Start(tempArr1,sizeof(tempArr1)-1);
			valueDisplay = 0;
		}
		else if((valueView)&&(valueDisplay))
		{
			valueDisplay = 0;
			strcpy(displayArr, "");
			writeNumber(displayArr, Temperature);
			displayArr[5] = '\'';
			displayArr[6] = 'C';
			Start(displayArr,7);
		}


	}
	break;

	case modeHum :
	{
		if(centreFlag)
		{
			valueView = 1;
			valueDisplay = 1;
		}
		if((!valueView)&&(valueDisplay))
		{
			Start(humArr1,sizeof(humArr1)-1);
			valueDisplay = 0;
		}
		else if((valueView)&&(valueDisplay))
		{
			valueDisplay = 0;
			strcpy(displayArr, "");
			writeNumber(displayArr, Humidity);
			displayArr[5] = '%';
			Start(displayArr,6);
		}
	}
	break;

	case modePres :
	{
		if(centreFlag)
		{
			valueView = 1;
			valueDisplay = 1;
		}
		if((!valueView)&&(valueDisplay))
		{
			Start(presArr1,sizeof(presArr1)-1);
			valueDisplay = 0;
		}
		else if((valueView)&&(valueDisplay))
		{
			valueDisplay = 0;
			strcpy(displayArr, "");
			writeNumber(displayArr, Pressure);
			displayArr[5] = 'k';
			displayArr[6] = 'P';
			displayArr[7] = 'a';
			Start(displayArr,8);
		}
	}
	break;

	case modeWind :
	{
		if(centreFlag)
		{
			valueView = 1;
			valueDisplay = 1;
		}
		if((!valueView)&&(valueDisplay))
		{
			Start(windArr1,sizeof(windArr1)-1);
			valueDisplay = 0;
		}
		else if((valueView)&&(valueDisplay))
		{
			valueDisplay = 0;
			strcpy(displayArr, "");
			writeNumber(displayArr, WindSpeed);
			displayArr[5] = 'm';
			displayArr[6] = '/';
			displayArr[7] = 's';
			Start(displayArr,8);
		}
	}
	break;

	case modeDT :
	{
		if(centreFlag)
		{
			valueView = 1;
			valueDisplay = 1;
		}
		if((!valueView)&&(valueDisplay))
		{
			Start(DTArr1,sizeof(DTArr1)-1);
			valueDisplay = 0;
		}
		else if((valueView)&&(valueDisplay))
		{
			valueDisplay = 0;
			uart1TxBuf[5] = BCD_to_Dec(my_time.sec);
			uart1TxBuf[4] = BCD_to_Dec(my_time.min);
			uart1TxBuf[3] = BCD_to_Dec(my_time.hour);
			uart1TxBuf[2] = BCD_to_Dec(my_time.day);
			uart1TxBuf[1] = BCD_to_Dec(my_time.month);

			strcpy(displayArr, "");
			writeNumber2(displayArr, uart1TxBuf[2]*2);
			displayArr[2] = '/';
			writeNumber2(displayArr+3, uart1TxBuf[1]*2);
			displayArr[5] = ' ';
			writeNumber2(displayArr+6, uart1TxBuf[3]*2);
			displayArr[8] = ':';
			writeNumber2(displayArr+9, uart1TxBuf[4]*2);
			displayArr[11] = ':';
			writeNumber2(displayArr+12, uart1TxBuf[5]*2);



			Start(displayArr,14);
		}
	}
	break;

	}
}

void alarms_menu()
{
	if((!alarmCount)&&(valueDisplay))
	{
		Start("No Alarms",9);
		valueDisplay = 0;
	}
	else if((alarmCount>0)&&(valueDisplay))
	{
		valueDisplay=0;
		switch(alarmsArr[alarmCurrent].type)
		{
		case alarmTypeTemp :
			Start("High Temperature",16);
			break;
		case alarmTypeLowTemp :
			Start("Low Temperature",15);
			break;
		case alarmTypeHum :
			Start("High Humidity",13);
			break;
		case alarmTypePres :
			Start("High Pressure",13);
			break;
		case alarmTypeWind :
			Start("High Wind Speed",15);
			break;
		}
	}

}

void alarm_new()
{
	if(Mode == 'N')
	{
		menuState = ALARMS;
		alarmCurrent = alarmCount-1;

	}
	else
	{
		switch(alarmsArr[alarmCount-1].type)
		{
		case alarmTypeTemp :
			Start("High Temperature",16);
			break;
		case alarmTypeLowTemp :
			Start("Low Temperature",15);
			break;
		case alarmTypeHum :
			Start("High Humidity",13);
			break;
		case alarmTypePres :
			Start("High Pressure",13);
			break;
		case alarmTypeWind :
			Start("High Wind Speed",15);
			break;
		}
	}
	buzzerON = 0;
	buzState = buzzOff;
	buzFlag = 0;
	R_TAU0_Channel2_Stop();
	buzzalarm = 1;
	BUZZ_LOOP();
}

void readStatus()
{
	int a,flag=0;
	statusByte = 0;
	while((a<5)&&(!flag))
	{
		flag = alarmFlag[a];
		a++;
	}
	if(flag)
	{

		statusByte = (0x80)+(alarmFlag[modeWind]<<5)+(alarmFlag[modeHum]<<3)+(alarmFlag[modePres]<<2)+(alarmFlag[modeLowTemp]<<1)+(alarmFlag[modeTemp]);
	}
}

