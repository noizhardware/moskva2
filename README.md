# MOSKVA v.2
##### v.201911051601
##### try https://github.com/sainoky/serial_port_monitor

## Procedura di setup:
  * avvita il sensore alla piastra e alla superficie da usare, tipo la tavola di legno
    - la posizione di tutte le cose deve rimanere invariata
  * collega l'arduino al sensore col cavetto minijack
  * collega l'arduino al pc
    - in questo momento il programmino verrà avviato, non devi toccare nulla, nè l'arduino ne il sensore, perchè all'avvio c'è una routine di calibrazione di alcuni parametri
  * ora devi calibrare la _sensibilità_ del sensore:
    1. se, senza toccare la superficie il led è acceso, diminuisci la sensibilità finche non si spegne
    2. se il led è spento, appoggia la mano sulla superficie, nel punto corretto, e alza la sensibilità finchè non si accende il led
    3. a questo punto, quando levi la mano dalla superficie, il led dovrebbe spegnersi, se non si spegne, ripeti la procedura dal punto 1.
    
## Dati dal SERIAL:
  * `start` quando il seriale è online
  * `READY` quando ha finito la calibrazione. **INIZIA A LEGGERE DOPO IL READY**
  * vengono trasmessi solo i cambi di stato, con stringhe in formato `nomepin-stato`
    - _nomepin_ è il nome di uno dei 5 pin, chiamati (a, b, c, d, e)
    - _stato_ è un BOOL, è `1` quando il sensore viene toccato, e `0` quando viene rilasciato
  * **OLD** se invii "brk"(newline) al serial, ti stampa una linea "-------" tipo segnalibro. non ha alcun altro effetto
  * se invii "debug-nomesensore" in serial, accende/spegne il debug per quel sensore.
    - es: debug-b

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


