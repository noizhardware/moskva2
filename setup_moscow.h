#ifndef __SETUP_MOSKOW_H__
  #define __SETUP_MOSKOW_H__
  #define __SETUP_MOSKOW_H__VERSION 201911051601

  // project-specific bindings //

  #define DEBUG_MODE_a 0 // 0 or 1
  #define DEBUG_MODE_b 0 // 0 or 1
  #define DEBUG_MODE_c 0 // 0 or 1
  #define DEBUG_MODE_d 0 // 0 or 1
  #define DEBUG_MODE_e 0 // 0 or 1
  
  //#define DEBOUNCE_ON //not used for now

  #define AUTOCAL // enable CapacitiveSensor library startup autocalibration
  //#define MAX_SENSE 100000. // max sensitivity settable by the hardware pot

  #define MAXPOT_LOOPS 50 // how many averaged samples for initial reading
  #define POTENTIOMETER_MAX_SENSE_MULT 10 // for autogeneration of MAX_SENSE

  #define debounceMore 0 // set additional debounce time here (can be positive or negative)

#endif // __SETUP_MOSKOW_H__
