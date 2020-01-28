#ifndef __MOSKVA2_H__
#define __MOSKVA2_H__
#define __MOSKVA2_H__VERSION 202001191516

#include <string.h>
#include <stdlib.h>

#include "eepromThings.h"
// FLAGS(1-bit address)
#define SAVEFLAG 0
#define AUTOCALFLAG 1
#define POT_A_SAVED 2
#define POT_B_SAVED 3
#define POT_C_SAVED 4
#define POT_D_SAVED 5
#define POT_E_SAVED 6
#define SAVEBASELINESFLAG 7
// DATA(absolute EEPROM address)
#define SAVEADDRESS_a 4 // 4bytes each (for long ints)
#define SAVEADDRESS_b 8
#define SAVEADDRESS_c 12
#define SAVEADDRESS_d 16
#define SAVEADDRESS_e 24
#define POT_A_ADDRESS 32 // 4bytes each (for long ints)
#define POT_B_ADDRESS 36
#define POT_C_ADDRESS 40
#define POT_D_ADDRESS 44
#define POT_E_ADDRESS 48
#define BASELINE_A_ADDRESS 52 // 4bytes each (for long ints)
#define BASELINE_B_ADDRESS 56
#define BASELINE_C_ADDRESS 60
#define BASELINE_D_ADDRESS 64
#define BASELINE_E_ADDRESS 68

// TODO:
// (save-unsave single maxVal)
// (save-unsave single baselineR)
// save pot values "save current pot status" LOL

#ifndef F
  #define F(string_literal) (reinterpret_cast<const __FlashStringHelper*>(PSTR(string_literal)))
#endif
//#define SERIPRINT(text) if(Serial){Serial.print(text);}
//#define NEWLINE if(Serial){Serial.println("");}
#define SERIPRINT(text) Serial.print(text);
#define SERIPRINT_CONST(text) Serial.print(F(text));/*this one forces data in progmem, so not to clog RAM, then copies it on-the-fly byte-by-byte when needed. works only with constant text/values */
#define NEWLINE Serial.println(F(""));

#define SAVEDATA_INIT \
  if(EEPROMflag(SAVEFLAG)){\
    SERIPRINT_CONST("Loading maxVal saved calibration data...");\
    potMaxval_a = getEEPROMlongint(SAVEADDRESS_a);\
    potMaxval_b = getEEPROMlongint(SAVEADDRESS_b);\
    potMaxval_c = getEEPROMlongint(SAVEADDRESS_c);\
    potMaxval_d = getEEPROMlongint(SAVEADDRESS_d);\
    potMaxval_e = getEEPROMlongint(SAVEADDRESS_e);\
    SERIPRINT_CONST("DONE!"); NEWLINE;\
  }else{\
    SERIPRINT_CONST("no saved data"); NEWLINE;\
  }\
  if(EEPROMflag(SAVEBASELINESFLAG)){\
    SERIPRINT_CONST("Loading baselineR saved calibration data...");\
    touchVal_a.baselineR = getEEPROMlongint(BASELINE_A_ADDRESS);\
    touchVal_b.baselineR = getEEPROMlongint(BASELINE_B_ADDRESS);\
    touchVal_c.baselineR = getEEPROMlongint(BASELINE_C_ADDRESS);\
    touchVal_d.baselineR = getEEPROMlongint(BASELINE_D_ADDRESS);\
    touchVal_e.baselineR = getEEPROMlongint(BASELINE_E_ADDRESS);\
    SERIPRINT_CONST("DONE!"); NEWLINE;\
  }else{\
    SERIPRINT_CONST("no saved baselineR data"); NEWLINE;\
  }

#define SAVEDATA_NOW \
  SERIPRINT_CONST("saving calibration data...");\
  /* all long ints - 8bytes */\
  setEEPROMlongint(SAVEADDRESS_a, potMaxval_a);\
  setEEPROMlongint(SAVEADDRESS_b, potMaxval_b);\
  setEEPROMlongint(SAVEADDRESS_c, potMaxval_c);\
  setEEPROMlongint(SAVEADDRESS_d, potMaxval_d);\
  setEEPROMlongint(SAVEADDRESS_e, potMaxval_e);\
  setEEPROMflag(SAVEFLAG, true);\
  SERIPRINT_CONST("DONE!"); NEWLINE

#define SAVEBASELINES_NOW \
  SERIPRINT_CONST("saving baselineR data...");\
  setEEPROMlongint(BASELINE_A_ADDRESS, touchVal_a.baselineR);\
  setEEPROMlongint(BASELINE_B_ADDRESS, touchVal_b.baselineR);\
  setEEPROMlongint(BASELINE_C_ADDRESS, touchVal_c.baselineR);\
  setEEPROMlongint(BASELINE_D_ADDRESS, touchVal_d.baselineR);\
  setEEPROMlongint(BASELINE_E_ADDRESS, touchVal_e.baselineR);\
  setEEPROMflag(SAVEBASELINESFLAG, true);\
  SERIPRINT_CONST("DONE!"); NEWLINE

#define UNSAVE \
  setEEPROMflag(SAVEFLAG, false);\
  SERIPRINT_CONST("the SAVEFLAG has been set to 0, reboot to create new calibration data"); NEWLINE

#define UNSAVEBASELINES \
  setEEPROMflag(SAVEBASELINESFLAG, false);\
  SERIPRINT_CONST("the SAVEBASELINESFLAG has been set to 0, baselineR calibration will happen on reboot"); NEWLINE




/////////////////// set faster clock ////////////////////////
// defines for setting and clearing register bits
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

// set prescale to 16 : ADC clock = 16MHz / scale >> 16MHz/16=1MHz | ADC takes 13cycles >> 1/1M*13= 13uS
#define FASTADC \
  sbi(ADCSRA,ADPS2);\
  cbi(ADCSRA,ADPS1);\
  cbi(ADCSRA,ADPS0)
///////////////// END set faster clock //////////////////////


#define BIT16 const int16_t PROGMEM
#define vBIT16 int8_t
#define BOO const bool PROGMEM

#define ON 1
#define OFF 0

#define MAX_INPUT 20

bool debug_a = DEBUG_MODE_a;
bool debug_b = DEBUG_MODE_b;
bool debug_c = DEBUG_MODE_c;
bool debug_d = DEBUG_MODE_d;
bool debug_e = DEBUG_MODE_e;

#define TAB SERIPRINT(F("\t"))

/*
// the function is defined as: long readTouch::readTouch(uint8_t samples)
#define CAP_INIT(sensorname, COMMONPIN, SENSEPIN) \
  readTouch touchVal_##sensorname = readTouch(COMMONPIN,SENSEPIN);\
  long int potMaxval_##sensorname = 0;
  */

// with Paul Badger's library: 
#define CAP_INIT(sensorname, COMMONPIN, SENSEPIN) \
  CapTouch touchVal_##sensorname = CapTouch(COMMONPIN,SENSEPIN);\
  long int potMaxval_##sensorname = 0;\
  float smooth_##sensorname = 0;


#define change(a, b) (a != b)
#define NOW millis()
#define RESTART(timer) timer = NOW

#define SERIALBOOT \
  Serial.begin(9600);\
  vBIT16 startTime = millis();\
  while(!Serial){ /* that alone would stall the system if no serial connection is present :( */\
    if ((millis()- startTime) >= 2000){ /* if it takes more than 2seconds to find the Serial, fuck it, we'll do without it. */\
      break; }}\
  digitalWrite(LED_BUILTIN, 1); /* lights up the builtin LED to say everything is ready! */\
  if(Serial){\
    Serial.println("start"); } /* prints "start" to serial out, if available */

typedef struct {
  bool current;
  bool last;
  bool debounce;
  bool debounce_pre;
  unsigned long debounceTimer;
  unsigned long debounceTimer_pre;
  bool changeBuffer = OFF;
  bool led_status; // not sure if used
} status_t;

#define POTENTIOMETER_MAXVAL_CALIB(sensename) \
  SERIPRINT_CONST("CALIBRATING MAXVAL " #sensename " ..."); NEWLINE;\
  delay(MAXVAL_CALIB_INIT_DELAY);\
  for(int i = 0; i < MAXPOT_LOOPS; i++){\
    long int temp = touchVal_##sensename.readTouch(CAP_SAMPLES);\
    if(DEBUG_CALIB){SERIPRINT(#sensename " reading: "); SERIPRINT(temp); NEWLINE;}\
    potMaxval_##sensename += temp * POTENTIOMETER_MAX_SENSE_MULT;\
    delay(MAXVAL_CALIB_DELAY);}\
  potMaxval_##sensename /= MAXPOT_LOOPS;\
  SERIPRINT_CONST("max value " #sensename ":"); SERIPRINT(potMaxval_##sensename); NEWLINE

// debounce shit
#define debounceDelay DEBOUNCE_BASE + debounceMore
#define debounceDelay_pre DEBOUNCE_PRE + debounceMore_pre

// simple lowpass filter
// requires recycling the output in the "smoothedVal" param
static inline float smooth(float data, float filterVal, float smoothedVal){
  if (filterVal > 1){      // check to make sure param's are within range
    filterVal = .99;}
  else if (filterVal <= 0){
    filterVal = 0;}

  smoothedVal = (data * (1 - filterVal)) + (smoothedVal  *  filterVal);

  return smoothedVal;
}

#define PINLOOP(sensename) \
  \
  float senseValNow_##sensename = touchVal_##sensename.readTouch(CAP_SAMPLES);/*only actual reading of sensor here*/\
  if(SMOOTHING_##sensename){smooth_##sensename = smooth(senseValNow_##sensename, smoothFactor_##sensename, smooth_##sensename);}\
  /*if(EEPROMflag(POT_A_SAVED)){}*/ /*CACCA qui devo implementare il salvataggio del valore totale del pot*/\
  if(debug_##sensename){ /* print raw values to serial */\
    if(SMOOTHING_##sensename){SERIPRINT_CONST(#sensename); SERIPRINT_CONST(":"); SERIPRINT(smooth_##sensename); TAB; TAB;}\
    else{SERIPRINT_CONST(#sensename); SERIPRINT_CONST(":"); SERIPRINT(senseValNow_##sensename); TAB; TAB;}\
    SERIPRINT_CONST("p"); SERIPRINT_CONST(#sensename); SERIPRINT_CONST(":"); SERIPRINT( ((1. - (analogRead(POT_##sensename)/ 1023.)) * potMaxval_##sensename) );\
    TAB; TAB; SERIPRINT_CONST("debounce: ");\
    if(sense_##sensename.debounce){\
      SERIPRINT_CONST("ON");}\
    else{\
      SERIPRINT_CONST("off");}\
    NEWLINE;}\
  /*from here begins the actual reading and debounce: */\
  /*cacca - here I should put the saved pots loader???*/\
  if(SMOOTHING_##sensename){\
    if(smooth_##sensename >= ((1. - (analogRead(POT_##sensename)/ 1023.)) * potMaxval_##sensename) ){/* POT reading and comparation here */\
        sense_##sensename.current = ON;\
    if(DEBUG_ORDER_##sensename){SERIPRINT_CONST(#sensename); SERIPRINT_CONST(" sensor triggering"); NEWLINE;}}\
    else{\
      sense_##sensename.current = OFF; } /* sensor status has been read and stored*/\
  }else{\
    if(senseValNow_##sensename >= ((1. - (analogRead(POT_##sensename)/ 1023.)) * potMaxval_##sensename) ){/* POT reading and comparation here */\
      sense_##sensename.current = ON;\
      if(DEBUG_ORDER_##sensename){SERIPRINT_CONST(#sensename); SERIPRINT_CONST(" sensor triggering"); NEWLINE;}}\
    else{\
    sense_##sensename.current = OFF; } /* sensor status has been read and stored*/\
  }\
  \
  /*DEBOUNCE PRE starts here*/\
  if(sense_##sensename.debounce_pre){\
    if(sense_##sensename.current){\
      if(NOW >= (sense_##sensename.debounceTimer_pre + debounceDelay_pre)){/*timer-pre is fulfilled (bao)*/\
        if(sense_##sensename.debounce){ /* if debounce is running */\
          if(sense_##sensename.current){ RESTART(sense_##sensename.debounceTimer); } /* if there is pressure on the sensor, restart debounceTimer */\
          if(NOW >= (sense_##sensename.debounceTimer + debounceDelay)){ /* if debounceTime has expired - CACCA è qui il debounce*/\
            sense_##sensename.debounce = OFF; /* turn it off! */\
          } \
          if(change(sense_##sensename.last, sense_##sensename.current)){ sense_##sensename.changeBuffer = ON; } /* store if there has been a pin status change */\
        } \
      else{ /* if debounce is NOT running */\
        if(sense_##sensename.current){  /* key pressed */\
          sense_##sensename.debounce = ON; /* store debounce status ON */\
          RESTART(sense_##sensename.debounceTimer); /* start debounce timer */\
          SERIPRINT_CONST(#sensename); /* print status change to serial */\
          SERIPRINT_CONST("-");\
          SERIPRINT(ON); NEWLINE;\
          digitalWrite(LED_##sensename, ON); /* and to status LED */\
          if(change(sense_##sensename.last, sense_##sensename.current)){ sense_##sensename.changeBuffer = ON; }} /* store if there has been a pin status change */\
        else{                          /* key NOT pressed */\
          if((change(sense_##sensename.current, sense_##sensename.last)) || sense_##sensename.changeBuffer){/* was it pressed before? */\
            SERIPRINT_CONST(#sensename);\
            SERIPRINT_CONST("-");\
            SERIPRINT(OFF); NEWLINE;\
            digitalWrite(LED_##sensename, OFF);\
            sense_##sensename.changeBuffer = OFF; }}}/*here ends the post-debounce block*/\
      }}\
      else{\
        RESTART(sense_##sensename.debounceTimer_pre);\
        /*cacca - qui debounce-pre spegne il led*/\
        if((change(sense_##sensename.current, sense_##sensename.last)) || sense_##sensename.changeBuffer){/* was it pressed before? */\
            SERIPRINT_CONST(#sensename);\
            SERIPRINT_CONST("-");\
            SERIPRINT(OFF); NEWLINE;\
            digitalWrite(LED_##sensename, OFF);\
            sense_##sensename.changeBuffer = OFF;\
            sense_##sensename.debounce = OFF; }\
      }\
  }\
  else{\
    if(sense_##sensename.current){\
      RESTART(sense_##sensename.debounceTimer_pre);\
      sense_##sensename.debounce_pre = ON;}}\
  /*DEBOUNCE PRE ends here*/\
  



#include <avr/io.h>
#include <avr/wdt.h>
#define reset_AVR() wdt_enable(WDTO_15MS); while(1) {}

static inline void removeSubstring (char *string, char *sub) {
    char* match;
    int len = strlen(sub);
    while ((match = strstr(string, sub))) {
        *match = '\0';
        strcat(string, match+len);
    }
}



#endif // __MOSKVA2_H__
