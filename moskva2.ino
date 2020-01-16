#include "CapacitiveSensor.h"
#include "setup_moscow.h"
#include "moskva2.h"
#include "listen_serial.h"
#include "mozg2.h"
//#include "mozg2_proto0.h"

#ifndef __MOSKVA2_INO__VERSION
#define __MOSKVA2_INO__VERSION 202001160133

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


  if(!EEPROMflag(AUTOCALFLAG)){ // turn off autocalibrate if AUTOCALFLAG says so
    touchVal_a.set_CS_AutocaL_Millis(0xFFFFFFFF); // turn off autocalibrate on touchVal_a
    touchVal_b.set_CS_AutocaL_Millis(0xFFFFFFFF); // turn off autocalibrate on touchVal_b
    touchVal_c.set_CS_AutocaL_Millis(0xFFFFFFFF); // turn off autocalibrate on touchVal_c
    touchVal_d.set_CS_AutocaL_Millis(0xFFFFFFFF); // turn off autocalibrate on touchVal_d
    touchVal_e.set_CS_AutocaL_Millis(0xFFFFFFFF); // turn off autocalibrate on touchVal_e
  }
 
  
  #ifndef AUTOCAL
    touchVal_a.set_CS_AutocaL_Millis(0xFFFFFFFF); // turn off autocalibrate on touchVal_a
    touchVal_b.set_CS_AutocaL_Millis(0xFFFFFFFF); // turn off autocalibrate on touchVal_b
    touchVal_c.set_CS_AutocaL_Millis(0xFFFFFFFF); // turn off autocalibrate on touchVal_c
    touchVal_d.set_CS_AutocaL_Millis(0xFFFFFFFF); // turn off autocalibrate on touchVal_d
    touchVal_e.set_CS_AutocaL_Millis(0xFFFFFFFF); // turn off autocalibrate on touchVal_e
  #endif

  

  LIGHTSHOW;                   
  
  SERIALBOOT;

  SAVEDATA_INIT;

  if(!EEPROMflag(SAVEFLAG)){
    //delay(5000);
    POTENTIOMETER_MAXVAL_CALIB(a);
    //delay(5000);
    POTENTIOMETER_MAXVAL_CALIB(b);
    //delay(5000);
    POTENTIOMETER_MAXVAL_CALIB(c);
    //delay(5000);
    POTENTIOMETER_MAXVAL_CALIB(d);
    //delay(5000);
    POTENTIOMETER_MAXVAL_CALIB(e);
  }
  


  digitalWrite(LED_BUILTIN, 0); delay(300); digitalWrite(LED_BUILTIN, 1);
  SERIPRINT("READY\n");

}

void loop(){

  
  
    PINLOOP(a);
    PINLOOP(b);
    PINLOOP(c);
    PINLOOP(d);
    PINLOOP(e);
    
    LISTEN_SERIAL;
      
}

#endif // __MOSKVA2_INO__VERSION
