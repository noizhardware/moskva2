//#include "CapacitiveSensor.h"
#include "CapTouch.h"
#include "setup_moscow.h"
#include "moskva2.h"
#include "listen_serial.h"
#include "mozg2.h"
//#include "mozg2_proto0.h"

#ifndef __MOSKVA2_INO__VERSION
#define __MOSKVA2_INO__VERSION 202001171535

CAP_INIT(a, PIN_COMMON_a,PIN_SENSE_a);
CAP_INIT(b, PIN_COMMON_b,PIN_SENSE_b);
CAP_INIT(c, PIN_COMMON_c,PIN_SENSE_c);
CAP_INIT(d, PIN_COMMON_d,PIN_SENSE_d);
CAP_INIT(e, PIN_COMMON_e,PIN_SENSE_e);

void setup(){
  FASTADC;
  
  pinMode(LED_a, OUTPUT);
  pinMode(LED_b, OUTPUT);
  pinMode(LED_c, OUTPUT);
  pinMode(LED_d, OUTPUT);
  pinMode(LED_e, OUTPUT);
  
  LIGHTSHOW;                   
  
  SERIALBOOT;

  SAVEDATA_INIT;

/*
  if(!EEPROMflag(AUTOCALFLAG)){ // turn off autocalibrate if AUTOCALFLAG says so
    SERIPRINT_CONST("CapacitiveSensor autocalibration is OFF\n");
    touchVal_a.set_CS_AutocaL_Millis(0xFFFFFFFF); // turn off autocalibrate on touchVal_a
    touchVal_b.set_CS_AutocaL_Millis(0xFFFFFFFF); // turn off autocalibrate on touchVal_b
    touchVal_c.set_CS_AutocaL_Millis(0xFFFFFFFF); // turn off autocalibrate on touchVal_c
    touchVal_d.set_CS_AutocaL_Millis(0xFFFFFFFF); // turn off autocalibrate on touchVal_d
    touchVal_e.set_CS_AutocaL_Millis(0xFFFFFFFF); // turn off autocalibrate on touchVal_e
  }else{
    SERIPRINT_CONST("CapacitiveSensor autocalibration is ON\n");
  }*/
 
  /*
  #ifndef AUTOCAL
    touchVal_a.set_CS_AutocaL_Millis(0xFFFFFFFF); // turn off autocalibrate on touchVal_a
    touchVal_b.set_CS_AutocaL_Millis(0xFFFFFFFF); // turn off autocalibrate on touchVal_b
    touchVal_c.set_CS_AutocaL_Millis(0xFFFFFFFF); // turn off autocalibrate on touchVal_c
    touchVal_d.set_CS_AutocaL_Millis(0xFFFFFFFF); // turn off autocalibrate on touchVal_d
    touchVal_e.set_CS_AutocaL_Millis(0xFFFFFFFF); // turn off autocalibrate on touchVal_e
  #endif
  */

  if(!EEPROMflag(SAVEFLAG)){ // autocalib maxVals, if not already saved
    POTENTIOMETER_MAXVAL_CALIB(a);
    POTENTIOMETER_MAXVAL_CALIB(b);
    POTENTIOMETER_MAXVAL_CALIB(c);
    POTENTIOMETER_MAXVAL_CALIB(d);
    POTENTIOMETER_MAXVAL_CALIB(e);
  }

  digitalWrite(LED_BUILTIN, 0); delay(300); digitalWrite(LED_BUILTIN, 1);
  SERIPRINT_CONST("READY\n");

}

void loop(){

    PINLOOP(a);
    PINLOOP(b);
    PINLOOP(c);
    PINLOOP(d);
    PINLOOP(e);
    
    LISTEN_SERIAL;
    delay(500); //added safety: time-separation between capacitive sensing and serial transmission
      
}

#endif // __MOSKVA2_INO__VERSION
