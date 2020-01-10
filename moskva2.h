#ifndef __MOSKVA2_H__
  #define __MOSKVA2_H__
  #define __MOSKVA2_H__VERSION 201912081359

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

  //#define SERIPRINT(text) if(Serial){Serial.print(text);}
  //#define NEWLINE if(Serial){Serial.println("");}
  #define SERIPRINT(text) Serial.print(text);
  #define NEWLINE Serial.println("");
  
  #define TAB SERIPRINT("\t")

  // the function is defined as: long CapacitiveSensor::capacitiveSensor(uint8_t samples)
  #define CAP_INIT(sensorname, COMMONPIN, SENSEPIN) \
            CapacitiveSensor touchVal_##sensorname = CapacitiveSensor(COMMONPIN,SENSEPIN);\
            long int potMaxval_##sensorname = 0;
  

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
    unsigned long debounceTimer;
    bool changeBuffer = OFF;
  } status_t;

  #define POTENTIOMETER_MAXVAL_CALIB(sensename) \
            if(1){ SERIPRINT("CALIBRATING MAXVAL " #sensename " ..."); NEWLINE; }\
            for(int i = 0; i < MAXPOT_LOOPS; i++){\
               potMaxval_##sensename += touchVal_##sensename.capacitiveSensor(CAP_SAMPLES) * POTENTIOMETER_MAX_SENSE_MULT; }\
            potMaxval_##sensename /= MAXPOT_LOOPS;\
            if(1){ SERIPRINT("max value " #sensename ":"); SERIPRINT(potMaxval_##sensename); NEWLINE; }

  // debounce shit
  #define debounceDelay DEBOUNCE_BASE + debounceMore

  #define PINLOOP(sensename) \
            if(debug_##sensename){ /* print raw values to serial */\
              SERIPRINT(#sensename); SERIPRINT(":"); SERIPRINT(touchVal_##sensename.capacitiveSensor(CAP_SAMPLES)); TAB; TAB;\
              SERIPRINT("p"); SERIPRINT(#sensename); SERIPRINT(":"); SERIPRINT( ((1. - (analogRead(POT_##sensename)/ 1023.)) * potMaxval_##sensename) );\
              TAB; TAB; SERIPRINT("debounce:");\
              if(sense_##sensename.debounce){\
                SERIPRINT("ON");}\
              else{\
                SERIPRINT("off");}\
              NEWLINE;}\
            \
            if(touchVal_##sensename.capacitiveSensor(CAP_SAMPLES) >= ((1. - (analogRead(POT_##sensename)/ 1023.)) * potMaxval_##sensename) ){\
              sense_##sensename.current = ON; }\
            else{\
              sense_##sensename.current = OFF; } /* sensor status has been read and stored*/\
            \
            if(sense_##sensename.debounce){ /* if debounce is running */\
              if(sense_##sensename.current){ RESTART(sense_##sensename.debounceTimer); } /* if there is pressure on the sensor, restart debounceTimer */\
              if(NOW >= (sense_##sensename.debounceTimer + debounceDelay)){ /* if debounceTime has expired */\
                sense_##sensename.debounce = OFF; /* turn it off! */\
                } \
              if(change(sense_##sensename.last, sense_##sensename.current)){ sense_##sensename.changeBuffer = ON; } /* store if there has been a pin status change */\
              } \
            else{ /* if debounce is NOT running */\
              if(sense_##sensename.current){  /* key pressed */\
                sense_##sensename.debounce = ON; /* store debounce status ON */\
                RESTART(sense_##sensename.debounceTimer); /* start debounce timer */\
                SERIPRINT(#sensename); /* print status change to serial */\
                SERIPRINT("-");\
                SERIPRINT(ON); NEWLINE;\
                digitalWrite(LED_##sensename, ON); /* and to status LED */\
                if(change(sense_##sensename.last, sense_##sensename.current)){ sense_##sensename.changeBuffer = ON; }} /* store if there has been a pin status change */\
              else{                          /* key NOT pressed */\
                if((change(sense_##sensename.current, sense_##sensename.last)) || sense_##sensename.changeBuffer){/* was it pressed before? */\
                  SERIPRINT(#sensename);\
                  SERIPRINT("-");\
                  SERIPRINT(OFF); NEWLINE;\
                  digitalWrite(LED_##sensename, OFF);\
                  sense_##sensename.changeBuffer = OFF; }}}
      #define BRK \
                if(Serial.readString() == "brk\n"){\
                  SERIPRINT("-----------------------------------------");\
                  NEWLINE;}
                  
      #include <avr/io.h>
      #include <avr/wdt.h>
      #define reset_AVR() wdt_enable(WDTO_30MS); while(1) {}
      #define REBOOTLISTEN \
                if(Serial.readString() == "reboot\n"){\
                SERIPRINT("rebooting...\n");\
                reset_AVR();}

      void processIncomingByte (const byte inByte)
  {
  static char input_line [MAX_INPUT];
  static unsigned int input_pos = 0;

  switch (inByte)
    {

    case '\n':   // end of text
      input_line [input_pos] = 0;  // terminating null byte

      // terminator reached! process input_line here ...
      if(input_line == "debug-a\n"){
                if(debug_a){
                  debug_a = 0;
                  SERIPRINT("debug A is OFF");
                  NEWLINE;
                }else{
                  debug_a = 1;
                  SERIPRINT("debug A is ON");
                  NEWLINE;
                }}
                if(input_line == "debug-b\n"){
                if(debug_a){
                  debug_a = 0;
                  SERIPRINT("debug B is OFF");
                  NEWLINE;
                }else{
                  debug_a = 1;
                  SERIPRINT("debug B is ON");
                  NEWLINE;
                }}
                if(input_line == "debug-c\n"){
                if(debug_a){
                  debug_a = 0;
                  SERIPRINT("debug C is OFF");
                  NEWLINE;
                }else{\
                  debug_a = 1;
                  SERIPRINT("debug C is ON");
                  NEWLINE;
                }}
                if(input_line == "debug-d\n"){
                if(debug_a){
                  debug_a = 0;
                  SERIPRINT("debug D is OFF");
                  NEWLINE;
                }else{
                  debug_a = 1;
                  SERIPRINT("debug D is ON");
                  NEWLINE;
                }}
                if(input_line == "debug-e\n"){
                if(debug_a){
                  debug_a = 0;
                  SERIPRINT("debug E is OFF");
                  NEWLINE;
                }else{
                  debug_a = 1;
                  SERIPRINT("debug E is ON");
                  NEWLINE;
                }}

      // reset buffer for next time
      input_pos = 0;  
      break;

    case '\r':   // discard carriage return
      break;

    default:
      // keep adding if not full ... allow for terminating null byte
      if (input_pos < (MAX_INPUT - 1))
        input_line [input_pos++] = inByte;
      break;

    }  // end of switch

  } // end of processIncomingByte  

      #define LISTEN_DEBUG \
                while (Serial.available () > 0){\
                  String seri = Serial.readString();\
                  if(seri == "debug-a\n"){\
                  if(debug_a){\
                    debug_a = 0;\
                    SERIPRINT("debug A is OFF");\
                    NEWLINE;\
                  }else{\
                    debug_a = 1;\
                    SERIPRINT("debug A is ON");\
                    NEWLINE;\
                  }}\
                  if(seri == "debug-b\n"){\
                  if(debug_a){\
                    debug_a = 0;\
                    SERIPRINT("debug B is OFF");\
                    NEWLINE;\
                  }else{\
                    debug_a = 1;\
                    SERIPRINT("debug B is ON");\
                    NEWLINE;\
                  }}\
                  if(seri == "debug-c\n"){\
                  if(debug_a){\
                    debug_a = 0;\
                    SERIPRINT("debug C is OFF");\
                    NEWLINE;\
                  }else{\
                    debug_a = 1;\
                    SERIPRINT("debug C is ON");\
                    NEWLINE;\
                  }}\
                  if(seri == "debug-d\n"){\
                  if(debug_a){\
                    debug_a = 0;\
                    SERIPRINT("debug D is OFF");\
                    NEWLINE;\
                  }else{\
                    debug_a = 1;\
                    SERIPRINT("debug D is ON");\
                    NEWLINE;\
                  }}\
                  if(seri == "debug-e\n"){\
                  if(debug_a){\
                    debug_a = 0;\
                    SERIPRINT("debug E is OFF");\
                    NEWLINE;\
                  }else{\
                    debug_a = 1;\
                    SERIPRINT("debug E is ON");\
                    NEWLINE;\
                }}}
                
        #define LISTEN_DBG \
                  processIncomingByte (Serial.read ());\
                if(input_line == "debug-a\n"){\
                if(debug_a){\
                  debug_a = 0;\
                  SERIPRINT("debug A is OFF");\
                  NEWLINE;\
                }else{\
                  debug_a = 1;\
                  SERIPRINT("debug A is ON");\
                  NEWLINE;\
                }}\
                if(input_line == "debug-b\n"){\
                if(debug_a){\
                  debug_a = 0;\
                  SERIPRINT("debug B is OFF");\
                  NEWLINE;\
                }else{\
                  debug_a = 1;\
                  SERIPRINT("debug B is ON");\
                  NEWLINE;\
                }}\
                if(input_line == "debug-c\n"){\
                if(debug_a){\
                  debug_a = 0;\
                  SERIPRINT("debug C is OFF");\
                  NEWLINE;\
                }else{\
                  debug_a = 1;\
                  SERIPRINT("debug C is ON");\
                  NEWLINE;\
                }}\
                if(input_line == "debug-d\n"){\
                if(debug_a){\
                  debug_a = 0;\
                  SERIPRINT("debug D is OFF");\
                  NEWLINE;\
                }else{\
                  debug_a = 1;\
                  SERIPRINT("debug D is ON");\
                  NEWLINE;\
                }}\
                if(input_line == "debug-e\n"){\
                if(debug_a){\
                  debug_a = 0;\
                  SERIPRINT("debug E is OFF");\
                  NEWLINE;\
                }else{\
                  debug_a = 1;\
                  SERIPRINT("debug E is ON");\
                  NEWLINE;\
                }}

#endif // __MOSKVA2_H__