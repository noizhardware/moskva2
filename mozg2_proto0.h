// breadboard & veroboard(mauro) - 2019k02

#ifndef __MOZG2_PROTO0_H__
  #define __MOZG2_PROTO0_H__
  #define __MOZG2_PROTO0_H__VERSION 201911051601

  #define CAP_SAMPLES 10 // capacitive sensor accuracy, tradeoff: the higher the more accurate, the lower the faster (default was 30)
  
  #define DEBOUNCE_BASE 150 // set hardware-bound global debounce time here
  
  // create status objects for 5 sensors
  status_t sense_a;
  status_t sense_b;
  status_t sense_c;
  status_t sense_d;
  status_t sense_e;
  
  #define PIN_COMMON_a 3
  #define PIN_SENSE_a 2
  #define LED_a A0
  #define POT_a A1

  #define PIN_COMMON_b 3
  #define PIN_SENSE_b 4
  #define LED_b A2
  #define POT_b A3

#endif // __MOZG2_PROTO0_H__
