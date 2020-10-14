/***************************/
/*Author : ahmed alaa	*/
/*date : 19/8/2020 		*/
/*version : v02			*/
/***************************/

#ifndef NVIC_INTERFACE_H
#define NVIC_INTERFACE_H


void MNVIC_VoidEnableInterrupt			(u8 Copy_u8IntNumber);
void MNVIC_VoidDisableInterrupt		(u8 Copy_u8IntNumber);
void MNVIC_VoidSetPendingFlag		(u8 Copy_u8IntNumber);
void MNVIC_VoidClearPendingFlag		(u8 Copy_u8IntNumber);
u8 MNVIC_U8GetActiveFlag			(u8 Copy_u8IntNumber);
/*void MNVIC_VoidSetPriority(s8 copy_s8IntId,u8 copy_u8GroupPriorityNum,
		u8 copy_u8SubPriorityNum, u8 copy_u32Group);*/
void MNVIC_VoidSetPriority(u8 Copy_u8PeripheralIdx ,u8 Copy_u8Priority);
void MNVIC_VoidInit();

#define GROUP3	0x05FA0300	// 4bits for group (IPR) => group
#define GROUP4	0x05FA0400  // 3bits for group & 1bit for sub group
#define GROUP5	0x05FA0500  // 2bits for group & 2bit for sub group
#define GROUP6	0x05FA0600  // 1bits for group & 3bit for sub group
#define GROUP7	0x05FA0700  // 0bits for group & 4bit for sub group


#endif

