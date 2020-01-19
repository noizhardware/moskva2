#ifndef __SETUP_MOSCOW_H__
  #define __SETUP_MOSCOW_H__
  #define __SETUP_MOSCOW_H__VERSION 202001172036

  // project-specific bindings //

  #define DEBUG_MODE_a 0 // 0 or 1
  #define DEBUG_MODE_b 0 // 0 or 1
  #define DEBUG_MODE_c 0 // 0 or 1
  #define DEBUG_MODE_d 0 // 0 or 1
  #define DEBUG_MODE_e 0 // 0 or 1

  #define DEBUG_CALIB 1 // 0 or 1
  #define MAXVAL_CALIB_INIT_DELAY 0 //milliseconds
  #define MAXVAL_CALIB_DELAY 0 //milliseconds

  // prints to serial the result of SENSEDVALUE  >= POTVALUE to see if triggering does actually happen
  #define DEBUG_ORDER_a 0 // 0 or 1
  #define DEBUG_ORDER_b 0 // 0 or 1
  #define DEBUG_ORDER_c 0 // 0 or 1
  #define DEBUG_ORDER_d 0 // 0 or 1
  #define DEBUG_ORDER_e 0 // 0 or 1


  #define SMOOTHING_a 1
  #define SMOOTHING_b 1
  #define SMOOTHING_c 1
  #define SMOOTHING_d 1
  #define SMOOTHING_e 1

  #define smoothFactor_a .50
  #define smoothFactor_b .50
  #define smoothFactor_c .50
  #define smoothFactor_d .50
  #define smoothFactor_e .50

  
  //#define DEBOUNCE_ON //not used for now

  #define AUTOCAL // enable CapacitiveSensor library startup autocalibration
  //#define MAX_SENSE 100000. // max sensitivity settable by the hardware pot

  #define MAXPOT_LOOPS 50 // how many averaged samples for initial reading
  #define POTENTIOMETER_MAX_SENSE_MULT 100 // for autogeneration of MAX_SENSE (potentiometer's max sentitivity value, relative to the minimum)

  #define debounceMore 300 // set additional debounce time here (can be positive or negative)
  #define debounceMore_pre 0 // set additional PRE debounce time here (can be positive or negative)

#endif // __SETUP_MOSCOW_H__
