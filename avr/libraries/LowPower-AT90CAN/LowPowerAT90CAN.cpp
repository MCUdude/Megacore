/*******************************************************************************
* LowPower for AT90CAN Library
* Version: 1.00
* Date: 15-04-2021
* Author: Eael_Vadim, Belarus.
*
* Unported License.
*
* Revision  Description
* ========  ===========
* 1.00      Initial public release.
*******************************************************************************/
#if defined (__AVR__)
	#include <avr/sleep.h>
	#include <avr/interrupt.h>
#else
	#error "Processor architecture is not supported."
#endif

#include "LowPowerAT90CAN.h"

unsigned long TCounter = 0;

#if defined (__AVR_AT90CAN32__) || defined(__AVR_AT90CAN64__) || defined(__AVR_AT90CAN128__)

void	LowPowerAT90CANClass::powerSave(unsigned long SLEEP_MS)
{
  if (SLEEP_MS > 15) {	
    sleepMode(SLEEP_POWER_SAVE);   
    TCounter = 0;
    TCNT2=0; //счётный регистр обнулён
    TIFR2=TIFR2; // TIFR2&=0xff; //отчистить флаги прерываний
    OCR2A=255; // считать до 255
    TIMSK2=(1<<OCIE2A); //прерывание по совпадению с OCR2A
    TCCR2A=(1<<CS22)|(1<<CS21)|(1<<CS20)|(1<<WGM21); //запуск с таймера с делителем на 1024

    while (TCounter < (int(SLEEP_MS / 16)) ) {
      TCNT2=0; 
      sleep();    
    }
  }

}
#endif


/*******************************************************************************
* Name: ISR (TIMER2_COMP_vect)
* Description: TIMER2 interrupt service routine. 
*******************************************************************************/
ISR (TIMER2_COMP_vect)
{
  TCounter++;
  TCNT2=0;
  TIFR2=TIFR2;
}


LowPowerAT90CANClass LowPowerAT90CAN;
