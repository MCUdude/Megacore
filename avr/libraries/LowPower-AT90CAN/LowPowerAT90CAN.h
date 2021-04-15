#ifndef LowPowerAT90CAN_h
#define LowPowerAT90CAN_h

#include "Arduino.h"

class LowPowerAT90CANClass
{
	public:
		#if defined (__AVR__)

			#if defined (__AVR_AT90CAN32__) || defined(__AVR_AT90CAN64__) || defined(__AVR_AT90CAN128__)
				void powerSave(unsigned long SLEEP_MS; byte SLEEP_MODE);				
			#else
				#error "Please ensure chosen MCU is AT90CAN."
			#endif

		#else

			#error "Processor architecture is not supported."

		#endif
};

extern LowPowerAT90CANClass LowPowerAT90CAN;
#endif
