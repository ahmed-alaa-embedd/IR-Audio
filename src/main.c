/*
 * main.c
 *
 *  Created on: 24 Aug 2020
 *      Author: ahmed alaa
 */


#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "GPIO_int.h"
#include "RCC_interface.h"
#include "NVIC_interface.h"
#include "EXTI_interface.h"
#include "STk_interface.h"
#include "AFIO_interface.h"
#include "GPTIM_interface.h"

#include "GPIO_private.h"

#include "FHello.h"
#include "FHelp.h"
#include "FHungry.h"
#include "FWater.h"

volatile u16 i=0;

volatile u8 U8StartFlag=0;
volatile u32 U32FramData[50]={0};
volatile u8 U8EdgeCounter=0;
volatile u8 U8Data=0;


void VoidSetDACHello()
{
	GPIOA_ODR = Hello_raw[i];
	i++;
	if(i==4591)
	{
		i=0;
	}
}

void VoidSetDACHelp()
{
	GPIOA_ODR = Help_raw[i];
	i++;
	if(i==3355)
	{
		i=0;
	}
}

void VoidSetDACHungry()
{

	GPIOA_ODR = Hungry_raw[i];
	i++;
	if(i==3885)
	{
		i=0;
	}
}

void VoidSetDACWater()
{
	GPIOA_ODR = Water_raw[i];
	i++;
	if(i==4150)
	{
		i=0;
	}
}

void VoidPlay(void)
{
	switch(U8Data)
	{
	case 71 :
		//sound timer
		//interrupt every 125 ms and
		TIM2_voidSetIntervalSingle(125,VoidSetDACHello);
		break;
	case 70 :
		TIM2_voidSetIntervalSingle(125,VoidSetDACHelp);
		 break;
	case 69 :
		TIM2_voidSetIntervalSingle(125,VoidSetDACHungry);
		 break;
	case 68 :
		TIM2_voidSetIntervalSingle(125,VoidSetDACWater);
		 break;
	}
}

void VoidTakeAction (void)
{
	u8 i=0;
	U8Data=0;
	if((U32FramData[0]>=10000) && (U32FramData[0]<=14000))
	{
		for(i=0;i<8;i++)
		{
			if((U32FramData[17+i]>=2000) && (U32FramData[17+i]<=2300))
			{
				SET_BIT(U8Data,i);
			}

			else
			{
				CLR_BIT(U8Data,i);
			}
		}
		VoidPlay();
	}
	else
	{
		/*invalid frame*/
	}

	U8StartFlag		= 0;
	U32FramData[0]	= 0;
	U8EdgeCounter	= 0;

}

void VoidGetFrame (void)
{
	if(U8StartFlag == 0)
	{
		//Start timer
		// high ticks which is bigger than time of any bit send to ensure no overflow
		MSTK_VoidSetIntervalSingle(1000000,VoidTakeAction);
		U8StartFlag=1;
	}
	else
	{
		U32FramData[U8EdgeCounter]=MSTK_U32GetElapsedTime();
		MSTK_VoidSetIntervalSingle(1000000,VoidTakeAction);
		U8EdgeCounter++;
	}
}


void main()
{
	/*initiat clock*/
	RCC_voidInitSysClock();

	RCC_voidEnableClock(RCC_APB2, 0); // Enable clock for AFIO
	RCC_voidEnableClock(RCC_APB2, 2); // Enable clock for GPIOA
	RCC_voidEnableClock(RCC_APB2, 3); // Enable clock for GPIOB
	RCC_voidEnableClock(RCC_APB1,0);/*Enable clock for TIM2*/

	/*PIN mode*/
	//IR PINS
	MGPIO_VoidSetPinDirection(GPIOB , PIN0 , INPUT_PULLUP_DOWN); //no pull up or down sensor
	MGPIO_VoidSetPinValue(GPIOB, PIN0,HIGH);
	//DAC PINS
	MGPIO_VoidSetPinDirection(GPIOA,PIN8,OUTPUT_SPEED_2MHZ_PP);
	MGPIO_VoidSetPinDirection(GPIOA,PIN1,OUTPUT_SPEED_2MHZ_PP);
	MGPIO_VoidSetPinDirection(GPIOA,PIN2,OUTPUT_SPEED_2MHZ_PP);
	MGPIO_VoidSetPinDirection(GPIOA,PIN3,OUTPUT_SPEED_2MHZ_PP);
	MGPIO_VoidSetPinDirection(GPIOA,PIN4,OUTPUT_SPEED_2MHZ_PP);
	MGPIO_VoidSetPinDirection(GPIOA,PIN5,OUTPUT_SPEED_2MHZ_PP);
	MGPIO_VoidSetPinDirection(GPIOA,PIN6,OUTPUT_SPEED_2MHZ_PP);
	MGPIO_VoidSetPinDirection(GPIOA,PIN7,OUTPUT_SPEED_2MHZ_PP);

	/*AFIO*/
	MAFIO_voidSetEXTIConfiguration(0,MGPIOB);

	/*EXTI Initialization*/
	MEXTI_VoidSetCallBack(VoidGetFrame);
	MEXTI_VoidInit();

	/*Initialise MNVIC EXTI0*/
	MNVIC_VoidInit();
	MNVIC_VoidEnableInterrupt(6); //EXTI0
	MNVIC_VoidEnableInterrupt(28);/* TIM2 Enable Interrupt */

	/*Initialise Timers*/
	MSTK_VoidInit();	// clk=AHB/8 = 1MHz
	TIM2_voidInit();







	while(1)
	{

	}
}

