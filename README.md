# MOSKVA v.2
##### v-202001211117
##### Try https://github.com/sainoky/serial_port_monitor for a decent serial monitor

## Procedura di setup:
  * repo: https://github.com/noizhardware/moskva2
  * avvita tutti i sensori al legno (7 viti su ognuno)
  * trova i centri dei sensori
    - guardando la console trova il centro scorrendo la mano prima orizzontalmente, poi verticalmente(e segnali con del nastro)
  * per mettere in debug un canale il comando è d[nomecanale], esempio: `da` per debuggare _A_
  * fai un riavvio(reupload sketch) pulito (senza robe strane che succedono) e lascia che faccia la sua calibrazione delle baselines all'avvio
  * di base ora puoi salvare i dati di calibrazione con `ssb` (save baselines), cosi al successivo avvio non si ricalibra, ma usa i dati di questa calibrazione
    - se vuoi de-salvare la calibrazione usa `usb` (unsave baselines)
  * qua ora fai i settaggi di sensibilità per i canali singoli:
    - metti tutto al minimo di sensibilità
    - metti in debug
    - metti qualcuno che tocca la parete
    - alza la sensibilità giusto appena perchè triggeri, non di più
    - ripeti per gli altri
    - se vuoi cambiare il range di valori dei potenziometri(ad es., diminuendo il range per avere più precisione) usa `max-[nomesensore] [valore]` ad esempio `max-a 1000`
      + poi per salvare questi range usa `ss`
  * tutti i settaggi _extra_ li trovi in `setup_moscow.h`
    - di base l'unico di cui potresti aver bisogno è `smoothFactor_[nomesensore]`, che è la lentezza del filtro(piu il valore è alto (valore tra 0 e 1), piu aumenta il ritardo perchè fa la media tra più valori)
    - con `debounceMore` e `debounceMore_pre` setti il valore globale di debounce in attacco e in rilascio
  * бог свиней
    
## Dati dal SERIAL:
  * `start` quando il seriale è online
  * `READY` quando ha finito la calibrazione. **INIZIA A LEGGERE DOPO IL READY**
  * vengono trasmessi solo i cambi di stato, con stringhe in formato `nomepin-stato`
    - _nomepin_ è il nome di uno dei 5 pin, chiamati (a, b, c, d, e)
    - _stato_ è un BOOL, è `1` quando il sensore viene toccato, e `0` quando viene rilasciato
  * **OLD** se invii "brk"(newline) al serial, ti stampa una linea "-------" tipo segnalibro. non ha alcun altro effetto
  * se invii "debug-nomesensore" in serial, accende/spegne il debug per quel sensore.
    - es: debug-b
    
## <u>Serial monitor commands:</u>
  * `debug-nomesensore` attiva/disattiva il debug per quel sensore.
    - es: `debug-b` - shorthand `db`
  * `reboot` or `rr` reboots the arduino, same as pushing the reset button or power-cycling
  * `break` or `brk` or `br` writes a break to console "---------------------"
  * `wipe` - wipes EEPROM, initializing all to 0. _**USE WITH CAUTION**_
  * `save` or `ss` saves all calibration data to EEPROM, will be loaded as/is on next reboot
  * `unsave` or `uu` sets the saveflag to 0, so on the next reboot saved data will be ignored
  * `gimme` or `gc` shows current calibration data
  * `gimmesaved` or `gs` shows saved calibration data
  * `autocal` or `ac`
  * `noautocal` or `nac`
  * `max-sensorname customvalue`
    - example: `max-b 10000`
  * `pot-sensorname customvalue`
    - example: `pot-c 5438`

## setup_moscow.h
  E' il file di settaggio per il **progetto**:
  `#define DEBUG_MODE_nomepin 1` per stampare su serial i valori grezzi dei sensori e altre info. per spegnere settarlo a 0
     - esempio: DEBUG_MODE_a 1 abilita il debug per il pin "a" 
  `#define POTENTIOMETER_MAX_SENSE_MULT 10` questo è il moltiplicatore che genera il valore massimo del potenziometro. **10x** mi sembra un buon numero per ora.
  `#define debounceMore 0` qui puoi aggiungere o togliere debounce, in millisecondi

## mozg2_proto0.h (per ora non serve cambiare nulla qua)
  E' il file di configurazione per l'**hardware**: (per ora usiamo questo, poi useremo `mozg2.h` per il circuito definitivo)

  `#define CAP_SAMPLES 30` la precisione del sensore capacitivo. per ora lasciamo a 30

  `#define DEBOUNCE_BASE 200` questo è il debounce base, in millisecondi

  Qui vengono inizializzati i 5 sensori:
  ~~~~
  status_t sense_a;
  status_t sense_b;
  status_t sense_c;
  status_t sense_d;
  status_t sense_e;
  ~~~~

  `#define PIN_COMMON A4` questo è il pin comune del sensore capacitivo

  Qua definiamo i pin di ogni sensore, col rispettivo led indicatore e potenziometro di regolazione:

  ~~~~
  #define PIN_SENSE_a A5
  #define LED_a A0
  #define POT_a A1
  ~~~~