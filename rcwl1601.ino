//*****************************************************************
// Test program for the 3V/5V Sonic sensor RCWL-1601
// Make sure I2C jumper (zero-ohm) is installed on RWCL-1601 board
// and that you have pull-ups on the SCA/SCL lines
//*****************************************************************
//
// Terje Nilsen
// Feb 12, 2024
//*****************************************************************
#include "Particle.h"

SerialLogHandler logHandler(LOG_LEVEL_ALL);

#define RCWL1601_I2C_ADDR  0x57  //I2C address for the RCWL-1601 sensor

//*************************
//Starts a new sonic sample
//*************************
void start_sonic_sample(void) 
{
  Wire.beginTransmission(RCWL1601_I2C_ADDR);
  Wire.write(1);
  Wire.endTransmission();
}
//**********
//Initialize
//**********
void setup() 
{
  //***********************
  //Slow down bus if needed
  //***********************
  //Wire.setSpeed(20000);
  //*****************************
  //Initialize I2C pins (SDA/SCL)
  //*****************************
  Wire.begin();
  //******************
  //Start first sample
  //******************
  start_sonic_sample();
}
//***************
//Background loop
//***************
void loop() 
{
  unsigned long int micrometers;
  double mm, inches;

  //*******************************************
  //Delay a little before processing new sample
  //*******************************************
	delay(50);

	//******************************
	//Ask for the 3-byte measurement
	//******************************
	Wire.requestFrom(RCWL1601_I2C_ADDR, 3);
	//******************************************
	//Make sure we got a measurement transferred
	//******************************************
	if (Wire.available() == 3)
	{
	    //*****************
	    //Read 3-byte value
	    //*****************
	    micrometers  = (unsigned long int)Wire.read() << 16;
	    micrometers += (unsigned long int)Wire.read() << 8;
	    micrometers += (unsigned long int)Wire.read();
	    //******************************
	    //Compute millimeters and inches
	    //******************************
	    mm = (double)micrometers / 1000.0;
	    inches = mm * 0.0393701;
	    //*******************
	    //Send to serial port
	    //*******************
        Log.info("Distance mm=%.1f, inches=%.2f",mm,inches);
	}
	else
	{
        Log.info("****** MEASUREMENT NOT TRANSFERRED ******");
	}

  //************************
  //Pace the sensor readings
  //************************
  delay(500);

  //***********************************
  //Start a new measurement before exit
  //***********************************
  start_sonic_sample();
}
