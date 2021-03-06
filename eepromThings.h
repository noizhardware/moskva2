#ifndef __EEPROMTHINGS_H__
  #define __EEPROMTHINGS_H__
  #define __EEPROMTHINGS_H__VERSION 202001272142

#include <EEPROM.h>

void wipeEEPROM(){
  for (unsigned int i = 0 ; i < EEPROM.length() ; i++) {
      EEPROM.update(i, 0);} //write 0 to address i
  Serial.println("EEPROM has been wiped");}

static inline bool getBit (const unsigned char source, const unsigned char bitpos){ // bitpos=[0..7] [LSB..MSB]
  return ((source & (1 << bitpos)) >> bitpos)!=0;}

static inline unsigned char setBit (const unsigned char source, const unsigned char bitpos, const bool bittoset){ // bitpos=[0..7] [LSB..MSB]
  return (bittoset) ? (source | (1 << bitpos)) : (source & ~(1 << bitpos));}

static inline unsigned char flipBit (const unsigned char source, const unsigned char bitpos){ // bitpos=[0..7] [LSB..MSB]
  return source ^ (1 << bitpos);}

static inline bool EEPROMaddrisOK(const unsigned int eepromAddr){
  return ((eepromAddr <= EEPROM.length()));} // eepromAddr is UNSIGNED, so got rid of "&& (eepromAddr >= 0)"

static inline bool setEEPROMbit (const unsigned int eepromAddr, const unsigned char bitpos, const bool bittoset){ // bitpos=[0..7] [LSB..MSB]
    EEPROM.update(eepromAddr, setBit(EEPROM.read(eepromAddr), bitpos, bittoset)); // do it only if necessary (update checks identity before writing)
    return 1;}

static inline bool getEEPROMbit (const unsigned int eepromAddr, const unsigned char bitpos){ // bitpos=[0..7] [LSB..MSB]
  return getBit(EEPROM.read(eepromAddr), bitpos);}

static inline bool setEEPROMbyte (const unsigned int eepromAddr, const unsigned char bytetoset){ // eepromAddr is uint16_t(unsigned int)
    if(EEPROMaddrisOK(eepromAddr)){
      EEPROM.update(eepromAddr, bytetoset);
      return 1;}
    else{
      return 0;}}

static inline unsigned char getEEPROMbyte (const unsigned int eepromAddr){
  if(EEPROMaddrisOK(eepromAddr)){
    return EEPROM.read(eepromAddr);}
  else{
    return 0;/* ERROR!!! address out of range!*/}}
    //cacca: faccio return 0, ma è in realtà un errore, dovrebbe returnare un NULLA, visto che l'input alla funzione non è valido...

static inline long int getEEPROMlongint (const unsigned int eepromAddr){ // 8 bytes -> sizeof(long int)
  if(EEPROMaddrisOK(eepromAddr)){
      long int value;
      EEPROM.get(eepromAddr, value);
      return value;}
  else{
    return 0;/* ERROR!!! address out of range!*/}}
    //cacca: faccio return 0, ma è in realtà un errore, dovrebbe returnare un NULLA, visto che l'input alla funzione non è valido...

static inline long int setEEPROMlongint (const unsigned int eepromAddr, const long int value){ // eepromAddr is uint16_t(unsigned int)
    if(EEPROMaddrisOK(eepromAddr)){
      long int oldValue;
      EEPROM.get(eepromAddr, oldValue);
      if(value != oldValue){ // write only if necessary
        EEPROM.put(eepromAddr, value);}
      return 1;}
    else{
      return 0;}} /* ERROR!!! address out of range!*/

// (0) 1-bit EEPROMflags [0..7]
// (1) 1-bit EEPROMflags [8..15]
// (2) 1-bit EEPROMflags [16..23]
// (3) 1-bit EEPROMflags [24..31]

static inline bool setEEPROMflag(const unsigned char flagID, const bool bittoset){ // flagID=[0..31]
    if(flagID <= 7){
      setEEPROMbit(0, flagID, bittoset);
      return 1;}
    else if((flagID > 7) && (flagID <= 15)){
      setEEPROMbit(1, (flagID - 8), bittoset);
      return 1;}
    else if((flagID > 15) && (flagID <= 23)){
      setEEPROMbit(2, (flagID - 16), bittoset);
      return 1;}
    else if((flagID > 23) && (flagID <= 31)){
      setEEPROMbit(3, (flagID - 24), bittoset);
      return 1;}
    else{
      /* ERROR!!! flagID out of range! */
      return 0;}}

static inline bool EEPROMflag(const unsigned char flagID){ // flagID=[0..31]
  if(flagID <= 7){
    return getEEPROMbit(0, flagID);}
  else if((flagID > 7) && (flagID <= 15)){
    return getEEPROMbit(1, (flagID - 8));}
  else if((flagID > 15) && (flagID <= 23)){
    return getEEPROMbit(2, (flagID - 16));}
  else if((flagID > 23) && (flagID <= 31)){
    return getEEPROMbit(3, (flagID - 24));}
  else{return false;/* ERROR!!! flagID out of range!*/}} 
  //cacca: faccio return false, ma è in realtà un errore, dovrebbe returnare un NULLA, visto che l'input alla funzione non è valido...

#endif // __EEPROMTHINGS_H__
  
