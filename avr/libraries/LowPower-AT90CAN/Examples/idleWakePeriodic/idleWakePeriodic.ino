// **** INCLUDES *****

#include "LowPowerAT90CAN.h"

unsigned long Timer_5000 = 0;

void setup()
{
  // No setup is required for this library
  Serial1.begin(57600);       // start serial port
  Serial1.println("Start...");
  Timer_5000 = millis();
}

void loop() 
{
  // After every 5 seconds of operation, the microcontroller is switched to the Power_save mode for 3 seconds.
  
  Serial1.println("Loop...");
  
  if ((Timer_5000+5000) < millis()) {
	 Serial1.print("---> Start slipping "); 
	 LowPowerAT90CAN.powerSave(3000); 
	 Serial1.print("---> WAKE UP !!!! "); 
	 Timer_5000 = millis();
  }	  


}

