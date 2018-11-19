#include "my_buzzer.h"
#include "r_cg_userdefine.h"
int buzFlag = 0, buzCount = 0, five_milsec_counter = 0;

void Buzzer()
{
	five_milsec_counter++;
	switch(buzState)
	{
	case buzzOff :

		if(buzzerON)
		{
			R_PCLBUZ0_Stop();
			buzzerON = 0;
		}
		break;
	case buzzShort :
		if(buzState==buzzLong)
		{

		}
		else if(!buzzerON)
		{
			R_TAU0_Channel5_Stop();
			R_TAU0_Channel5_Start();
			BUZZER_FREQ = BUZZER_FREQ_LOW;
			R_PCLBUZ0_Start();
			buzzerON = 1;
			five_milsec_counter = 0;
		}
		else if(five_milsec_counter==50)
		{
			R_PCLBUZ0_Stop();
			buzzerON = 0;
			buzState = buzzOff;
		}

		break;
	case buzzDouble :
		if(buzState==buzzLong)
		{

		}
		else if(!buzzerON)
		{
			R_TAU0_Channel5_Stop();
			R_TAU0_Channel5_Start();
			BUZZER_FREQ = BUZZER_FREQ_LOW;
			R_PCLBUZ0_Start();
			buzzerON = 1;
			five_milsec_counter = 0;
		}
		else if(five_milsec_counter==50)
		{

			R_PCLBUZ0_Stop();

		}
		else if(five_milsec_counter==55)
		{
			R_PCLBUZ0_Start();
		}
		else if(five_milsec_counter==105)
		{
			R_PCLBUZ0_Stop();
			buzzerON = 0;
			buzState = buzzOff;
		}



		break;
	case buzzLong :
		break;
	}
}

void BUZZ_LOOP()
{
	R_PCLBUZ0_Stop();
	if(buzFlag==0)
	{
		BUZZER_FREQ = BUZZER_FREQ_HIGH;
		buzTime = 0;
		buzTimeFlag = 0;
		buzzerON = 1;
		R_TAU0_Channel2_Start();
	}
	else
	{
		if(BUZZER_FREQ == BUZZER_FREQ_LOW)
			BUZZER_FREQ = BUZZER_FREQ_HIGH;
		else
			BUZZER_FREQ = BUZZER_FREQ_LOW;
	}
	buzFlag = 1;
	if((buzTime==10)&&(!buzzalarm))
	{
		buzzerON = 0;
		buzState = buzzOff;
		buzFlag = 0;
		R_TAU0_Channel2_Stop();
	}
	else
		R_PCLBUZ0_Start();
}

void BUZZ_DOUBLE()
{
	int a;
	BUZZER_FREQ = BUZZER_FREQ_LOW;
	R_PCLBUZ0_Start();
	for(a=0;a<25;a++)
	{
		delayNoInt(10000);
	}
	R_PCLBUZ0_Stop();

	for(a=0;a<25;a++)
	{
		delayNoInt(1000);
	}

	R_PCLBUZ0_Start();
	for(a=0;a<25;a++)
	{
		delayNoInt(10000);
	}
	R_PCLBUZ0_Stop();
}

void BUZZ_SHORT()
{
	int a;
	BUZZER_FREQ = BUZZER_FREQ_LOW;
	R_PCLBUZ0_Start();
	for(a=0;a<25;a++)
	{
		delayNoInt(10000);
	}
	R_PCLBUZ0_Stop();
}


