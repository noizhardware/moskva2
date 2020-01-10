// moskva-mozgMain_10 - 2019w06

#ifndef __MOZG2_H__
  #define __MOZG2_H__
  #define __MOZG2_H__VERSION 201912071534

  #define CAP_SAMPLES 10 // capacitive sensor accuracy, tradeoff: the higher the more accurate, the lower the faster (default was 30)
  
  #define DEBOUNCE_BASE 150 // set hardware-bound global debounce time here
  
  // create status objects for 5 sensors
  status_t sense_a;
  status_t sense_b;
  status_t sense_c;
  status_t sense_d;
  status_t sense_e;

  // pin 11 is common for all sense
  
  #define PIN_COMMON_a 11
  #define PIN_SENSE_a 9
  #define LED_a 2
  #define POT_a A0

  #define PIN_COMMON_b 11
  #define PIN_SENSE_b 8
  #define LED_b 3
  #define POT_b A1

  #define PIN_COMMON_c 11
  #define PIN_SENSE_c 7
  #define LED_c 4
  #define POT_c A2

  #define PIN_COMMON_d 11
  #define PIN_SENSE_d 10
  #define LED_d 5
  #define POT_d A3

  #define PIN_COMMON_e 11
  #define PIN_SENSE_e 12
  #define LED_e 6
  #define POT_e A4


  #define LIGHTSPEED 50

  #define LIGHTSHOW \
            for(unsigned char i = 0; i < 5; i++){ \
            digitalWrite(LED_a, 1); \
            delay(LIGHTSPEED);             \
            digitalWrite(LED_a, 0); \
            digitalWrite(LED_b, 1); \
            delay(LIGHTSPEED);             \
            digitalWrite(LED_b, 0); \
            digitalWrite(LED_c, 1); \
            delay(LIGHTSPEED);             \
            digitalWrite(LED_c, 0); \
            digitalWrite(LED_d, 1); \
            delay(LIGHTSPEED);             \
            digitalWrite(LED_d, 0); \
            digitalWrite(LED_e, 1); \
            delay(LIGHTSPEED);             \
            digitalWrite(LED_e, 0); \
            }                       \

#endif // __MOZG2_H__
