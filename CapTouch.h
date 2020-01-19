/* CapTouch.h part of v.04 - Capacitive Sensing Library for 'duino / Wiring
 * by Paul Badger 2-3-2012
 * <paul@moderndevice.com> http://opensource.org/licenses/mit-license.php
 *
 * This capacitive sensing method requires two microcontroller pins (send pin, receive pin)
 * with a high value resistor between them (1M is a starting point)
 * a small capacitor (20-40 pf) from the receive pin (the sensor is connected to this pin)
 * to ground will result in more stable readings. Sensor is any wire or conductive foil 
 * on the receive pin.
 */

// ensure this library description is only included once
#ifndef CapTouch_h
#define CapTouch_h
#define US  166UL  // period of four US powerline cycles in uS   ( 1/60 * 10 ) * 1000
#define EU  200UL  // period of four EU powerline cycles in uS   ( 1/50 * 10 ) * 1000

#define calibrateTime EU

#define NO_NEGATIVES // prevent negative numbers for output

// count after which functions timeout - arbitrary units
// ground one pin through a 1k resistor to see how long this takes

#define timeoutCount 40000000L 

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

// library interface description
class CapTouch
{
  // user-accessible "public" interface
  public:
  
  // variables
	int error;
	long  total;
	unsigned long baselineR;
	uint8_t is_calibrated_Flag;
	unsigned int lastSamples;

	uint8_t sBit;   // send pin's ports and bitmask
	uint8_t sPort;
	volatile uint8_t *sReg;
	volatile uint8_t *sOut;
	
	uint8_t rBit;    // receive pin's ports and bitmask 
	uint8_t rPort;
	volatile uint8_t *rReg;
	volatile uint8_t *rIn;
	volatile uint8_t *rOut;
	
    // methods
	
	// constructor
    CapTouch(uint8_t sendPin, uint8_t receivePin);
	
	long readTouch(uint8_t samples);
		
	// calibrate the baseline value for Slider method
	long calibrateTouch(uint8_t samples);

};

// actual functions form here (used to be inside CapTouch.cpp)
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#include "pins_arduino.h"
#include "WConstants.h"
#endif

// TODO: rewrite in functional style, fuck classes


// Constructor /////////////////////////////////////////////////////////////////
// Function that handles the creation and setup of instances

CapTouch::CapTouch(uint8_t sendPin, uint8_t receivePin){
  
  // get pin mapping and port for send Pin - from PinMode function in core
  sBit =  digitalPinToBitMask(sendPin);            // get send pin's ports and bitmask
  sPort = digitalPinToPort(sendPin);
  sOut = portOutputRegister(sPort);                // get pointer to output register   
  
   rBit = digitalPinToBitMask(receivePin);         // get receive pin's ports and bitmask 
   rPort = digitalPinToPort(receivePin);
  
    rOut = portOutputRegister(rPort);
  
  if (sPort == NOT_A_PORT) {error = -1;           // this does not appear to work
  // Serial.println("bad pin");
  }
  sReg = portModeRegister(sPort);
  
  // get pin mapping and port for receive Pin - from digital pin functions in Wiring.c
  if (rPort == NOT_A_PORT){ error = -1;          // this does not appear to work
  // Serial.println("bad pin");
  }
  rReg = portModeRegister(rPort);
  rOut = portOutputRegister(rPort);
  rIn  = portInputRegister(rPort);
  
    *sReg |= sBit;              // set sendpin to OUTPUT
  is_calibrated_Flag = 0;          // uncalibrated on startup
}

// Public Methods //////////////////////////////////////////////////////////////
// Functions available in Wiring sketches, this library, and other libraries

long CapTouch::readTouch(uint8_t samples)
{
  int i;
  total = 0;
  
  // calibratess the baseline value
  // first time after powerup or reset calibrates the sensor with baseline
  // so sensor should be in "untouched" state at powerup

  if(!EEPROMflag(SAVEBASELINESFLAG)){ // do baseline calibration only if there is no saved baselineR data
    if (is_calibrated_Flag == 0 || samples != lastSamples){
      calibrateTouch(samples);
      lastSamples = samples;
      is_calibrated_Flag = 1;}
  }
    
//  *sReg |= sBit;             // set Send pin Output
  
  for (i = 0; i < samples; i++) {
    noInterrupts();        // turn off interrupts to prevent collisions with other software
    *rReg &= ~rBit;        // set receive pin to input
    *rOut &= ~rBit;        // set receive pin low to make sure pullups are off
    
    *rReg |= rBit;         // receive pin is now LOW AND OUTPUT to discharge circuit
    *rReg &= ~rBit;        // set pin to INPUT 
    
    *sOut |= sBit;         // set send pin High
    interrupts();
    
    while( !(*rIn & rBit)  && (total < timeoutCount )){  // while receive pin is LOW AND total is positive value
      total++;
    }
    
    if (total >= timeoutCount){
      return -2;             //  total variable over timeout
    }
    
    // set receive pin HIGH briefly to charge up fully - because the while loop above will exit when pin is ~ 2.5V 
    noInterrupts();
    *rOut  |= rBit;        // set receive pin HIGH - turns on pullup 
    *rReg |= rBit;         // set pin to OUTPUT - pin is now HIGH AND OUTPUT
    *rReg &= ~rBit;        // set pin to INPUT 
    *rOut  &= ~rBit;       // turn off pullup
    
    *sOut &= ~sBit;        // set send Pin LOW
    interrupts();
    while( (*rIn & rBit) && (total < timeoutCount)){        // while receive pin is HIGH  AND total is less than timeout
      total++;
    }
    
  }
  

  
  
  if (total >= timeoutCount){
    return -2;     // total variable over timeout
    }
    else
    {
    total = total - baselineR;
    
#ifdef NO_NEGATIVES
    if (total < 0)   return 0;
#endif
    
        return total;
    }
}


long CapTouch::calibrateTouch(uint8_t samples){
  
  int i = 0, j = 0;
  
  *sReg |= sBit;             // set Send pin Output
  
  // the idea here is to calibrate for the same number of samples that are specified
  // but to make sure that the value is over a certain number of powerline cycles to 
  // average out powerline errors
  
  unsigned long start = millis();
  while (millis() - start < calibrateTime){
    
    for (i = 0; i < samples; i++) {     
      noInterrupts();
      *rReg &= ~rBit;        // set receive pin to input
      *rOut &= ~rBit;        // set receive pin low to make sure pullups are off
      
      *rReg |= rBit;         // receive pin is now LOW AND OUTPUT to discharge circuit
      *rReg &= ~rBit;        // set pin to INPUT 
      
      *sOut |= sBit;         // set send pin High
      interrupts();
      while( !(*rIn & rBit)  && (total < timeoutCount )){  // while receive pin is LOW AND total is positive value
        total++;
      }
      
      if (total >= timeoutCount){
        return -2;             //  total variable over timeout
      }
      
      // set receive pin HIGH briefly to charge up fully - because the while loop above will exit when pin is ~ 2.5V
      noInterrupts();
      *rOut  |= rBit;        // set receive pin HIGH - turns on pullup 
      *rReg |= rBit;         // set pin to OUTPUT - pin is now HIGH AND OUTPUT
      *rReg &= ~rBit;        // set pin to INPUT 
      *rOut  &= ~rBit;       // turn off pullup
      
      *sOut &= ~sBit;        // set send Pin LOW
      interrupts();
      while( (*rIn & rBit) && (total < timeoutCount)){        // while receive pin is HIGH  AND total is less than timeout
        total++;
      }
      
    }
    j++;
    
  }
  
  
   
  
  if (total >= timeoutCount){
    return -2;     // total variable over timeout
    Serial.println("calibrate routine over timeout, check wiring or raise  'timeoutCount' in CapTouch.h");
    }
    else
    {   Serial.print("total = ");
    Serial.print(total);
    baselineR = total / j;
    Serial.print(" | baselineR = ");
    Serial.println(baselineR);
    }
    // here I should load saved baseline values, overwriting the freshly calibrated new ones
  
  
}

#endif
