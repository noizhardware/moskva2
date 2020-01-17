#ifndef __LISTEN_SERIAL_H__
  #define __LISTEN_SERIAL_H__
  #define  __LISTEN_SERIAL_H__VERSION 202001172036

#define BRK \
  if(Serial.readString() == "brk\n"){\
    SERIPRINT_CONST("-----------------------------------------");\
    NEWLINE;}

#define LISTEN_SERIAL \
  /*SERIPRINT_CONST("gona listen to serial..."); NEWLINE;*/\
  while (Serial.available () > 0){\
    String seri = Serial.readString();\
    if(seri == "debug-a\n" or seri == "da\n"){\
      if(debug_a){\
        debug_a = 0;\
        SERIPRINT_CONST("debug A is OFF");\
        NEWLINE;\
      }else{\
        debug_a = 1;\
        SERIPRINT_CONST("debug A is ON");\
        NEWLINE;\
        /*SERIPRINT_CONST("maxVal for A is: ");\
        SERIPRINT(potMaxval_a);\
        NEWLINE;*/\
      }}\
    if(seri == "debug-b\n" or seri == "db\n"){\
      if(debug_b){\
        debug_b = 0;\
        SERIPRINT_CONST("debug B is OFF");\
        NEWLINE;\
      }else{\
        debug_b = 1;\
        SERIPRINT_CONST("debug B is ON");\
        NEWLINE;\
        /*SERIPRINT_CONST("maxVal for B is: ");\
        SERIPRINT(potMaxval_b);\
        NEWLINE;*/\
      }}\
    if(seri == "debug-c\n" or seri == "dc\n"){\
      if(debug_c){\
        debug_c = 0;\
        SERIPRINT_CONST("debug C is OFF");\
        NEWLINE;\
      }else{\
        debug_c = 1;\
        SERIPRINT_CONST("debug C is ON");\
        NEWLINE;\
        /*SERIPRINT_CONST("maxVal for C is: ");\
        SERIPRINT(potMaxval_c);\
        NEWLINE;*/\
      }}\
    if(seri == "debug-d\n" or seri == "dd\n"){\
      if(debug_d){\
        debug_d = 0;\
        SERIPRINT_CONST("debug D is OFF");\
        NEWLINE;\
      }else{\
        debug_d = 1;\
        SERIPRINT_CONST("debug D is ON");\
        NEWLINE;\
        /*SERIPRINT_CONST("maxVal for D is: ");\
        SERIPRINT(potMaxval_d);\
        NEWLINE;*/\
      }}\
    if(seri == "debug-e\n" or seri == "de\n"){\
      if(debug_e){\
        debug_e = 0;\
        SERIPRINT_CONST("debug E is OFF");\
        NEWLINE;\
      }else{\
        debug_e = 1;\
        SERIPRINT_CONST("debug E is ON");\
        NEWLINE;\
        /*SERIPRINT_CONST("maxVal for E is: ");\
        SERIPRINT(potMaxval_e);\
        NEWLINE;*/\
      }}\
    if(seri == "reboot\n" or seri == "rr\n"){\
      SERIPRINT_CONST("rebooting...\n");\
      reset_AVR();}\
    if(seri == "break\n" or seri == "br\n" or seri == "brk\n"){\
      SERIPRINT_CONST("-----------------------------------------");\
      NEWLINE;}\
    if(seri == "wipe\n"){\
      wipeEEPROM();}\
    if(seri == "save\n" or seri == "ss\n"){\
      SAVEDATA_NOW;}\
    if(seri == "unsave\n" or seri == "uu\n"){\
      UNSAVE;}\
    if(seri == "gimme\n" or seri == "gc\n"){\
      SERIPRINT_CONST("Current maxVal calibration data:"); NEWLINE;\
      SERIPRINT_CONST("potMaxval_a: "); SERIPRINT(potMaxval_a); NEWLINE;\
      SERIPRINT_CONST("potMaxval_b: "); SERIPRINT(potMaxval_b); NEWLINE;\
      SERIPRINT_CONST("potMaxval_c: "); SERIPRINT(potMaxval_c); NEWLINE;\
      SERIPRINT_CONST("potMaxval_d: "); SERIPRINT(potMaxval_d); NEWLINE;\
      SERIPRINT_CONST("potMaxval_e: "); SERIPRINT(potMaxval_e); NEWLINE;\
    }\
    if(seri == "gimmesaved\n" or seri == "gs\n"){\
      SERIPRINT_CONST("The SAVEFLAG is set to: "); SERIPRINT(EEPROMflag(SAVEFLAG)); NEWLINE;\
      SERIPRINT_CONST("Saved maxVal calibration data:"); NEWLINE;\
      SERIPRINT_CONST("potMaxval_a: "); SERIPRINT(getEEPROMlongint(SAVEADDRESS_a)); NEWLINE;\
      SERIPRINT_CONST("potMaxval_b: "); SERIPRINT(getEEPROMlongint(SAVEADDRESS_b)); NEWLINE;\
      SERIPRINT_CONST("potMaxval_c: "); SERIPRINT(getEEPROMlongint(SAVEADDRESS_c)); NEWLINE;\
      SERIPRINT_CONST("potMaxval_d: "); SERIPRINT(getEEPROMlongint(SAVEADDRESS_d)); NEWLINE;\
      SERIPRINT_CONST("potMaxval_e: "); SERIPRINT(getEEPROMlongint(SAVEADDRESS_e)); NEWLINE;\
      if(EEPROMflag(AUTOCALFLAG)){\
        SERIPRINT_CONST("CapacitiveSensor library AUTOCAL on boot is ON"); NEWLINE;}\
      else{\
        SERIPRINT_CONST("CapacitiveSensor library AUTOCAL on boot is OFF"); NEWLINE;}\
    }\
    if(seri == "autocal\n" or seri == "ac\n"){\
      setEEPROMflag(AUTOCALFLAG, true);\
      SERIPRINT_CONST("CapacitiveSensor library AUTOCAL on boot set to ON"); NEWLINE;\
    }\
    if(seri == "noautocal\n" or seri == "nac\n"){\
      setEEPROMflag(AUTOCALFLAG, false);\
      SERIPRINT_CONST("CapacitiveSensor library AUTOCAL on boot set to OFF"); NEWLINE;\
    }\
    if(seri.startsWith("max-a")){\
      char buf[sizeof(seri) + 1 + 50];/*create a buffer char array*/\
      strcpy(buf, seri.c_str());/*copy my string into it, doing the conversion String->char* */\
      removeSubstring(buf, "\n");/*gets rid of the final newline, if present*/\
      char* tmp = strtok(buf, " ");/*splits string by " " and gets first part*/\
      tmp = strtok(NULL, " ");/*then second part, the number I want!*/\
      long int mylong = atol(tmp);/*cast to long int*/\
      potMaxval_a = mylong;\
      SERIPRINT_CONST("potMaxval_a successfully set to: "); SERIPRINT(potMaxval_a); NEWLINE;\
    }\
    if(seri.startsWith("max-b")){\
      char buf[sizeof(seri) + 1 + 50];/*create a buffer char array*/\
      strcpy(buf, seri.c_str());/*copy my string into it, doing the conversion String->char* */\
      removeSubstring(buf, "\n");/*gets rid of the final newline, if present*/\
      char* tmp = strtok(buf, " ");/*splits string by " " and gets first part*/\
      tmp = strtok(NULL, " ");/*then second part, the number I want!*/\
      long int mylong = atol(tmp);/*cast to long int*/\
      potMaxval_b = mylong;\
      SERIPRINT_CONST("potMaxval_b successfully set to: "); SERIPRINT(potMaxval_b); NEWLINE;\
    }\
    if(seri.startsWith("max-c")){\
      char buf[sizeof(seri) + 1 + 50];/*create a buffer char array*/\
      strcpy(buf, seri.c_str());/*copy my string into it, doing the conversion String->char* */\
      removeSubstring(buf, "\n");/*gets rid of the final newline, if present*/\
      char* tmp = strtok(buf, " ");/*splits string by " " and gets first part*/\
      tmp = strtok(NULL, " ");/*then second part, the number I want!*/\
      long int mylong = atol(tmp);/*cast to long int*/\
      potMaxval_c = mylong;\
      SERIPRINT_CONST("potMaxval_c successfully set to: "); SERIPRINT(potMaxval_c); NEWLINE;\
    }\
    if(seri.startsWith("max-d")){\
      char buf[sizeof(seri) + 1 + 50];/*create a buffer char array*/\
      strcpy(buf, seri.c_str());/*copy my string into it, doing the conversion String->char* */\
      removeSubstring(buf, "\n");/*gets rid of the final newline, if present*/\
      char* tmp = strtok(buf, " ");/*splits string by " " and gets first part*/\
      tmp = strtok(NULL, " ");/*then second part, the number I want!*/\
      long int mylong = atol(tmp);/*cast to long int*/\
      potMaxval_d = mylong;\
      SERIPRINT_CONST("potMaxval_d successfully set to: "); SERIPRINT(potMaxval_d); NEWLINE;\
    }\
    if(seri.startsWith("max-e")){\
      char buf[sizeof(seri) + 1 + 50];/*create a buffer char array*/\
      strcpy(buf, seri.c_str());/*copy my string into it, doing the conversion String->char* */\
      removeSubstring(buf, "\n");/*gets rid of the final newline, if present*/\
      char* tmp = strtok(buf, " ");/*splits string by " " and gets first part*/\
      tmp = strtok(NULL, " ");/*then second part, the number I want!*/\
      long int mylong = atol(tmp);/*cast to long int*/\
      potMaxval_e = mylong;\
      SERIPRINT_CONST("potMaxval_e successfully set to: "); SERIPRINT(potMaxval_e); NEWLINE;\
    }\
  }

#endif // __LISTEN_SERIAL_H__
