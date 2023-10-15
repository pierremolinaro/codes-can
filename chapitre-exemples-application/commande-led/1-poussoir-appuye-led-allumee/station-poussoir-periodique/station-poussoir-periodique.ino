#ifndef ARDUINO_TEENSY41
  #error "Ce croquis doit être compilé pour la Teensy 4.1"
#endif

#include <ACAN_T4.h>

static const byte POUSSOIR = 0 ;

void setup () {
  pinMode (POUSSOIR, INPUT_PULLUP) ;
  pinMode (LED_BUILTIN, OUTPUT) ;
  ACAN_T4_Settings settings (1000 * 1000) ;
  const uint32_t errorCode = ACAN_T4::can1.begin (settings) ;
   if (0 != errorCode) {
    while (1) {
      delay (50) ;
      digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN)) ;
    }
  }
}

static uint32_t gDateClignotement = 0 ;
static const uint32_t PERIODE_EMISSION = 20 ; // En millisecondes
static uint32_t gDateEmission = 0 ;

void loop () {
  if (gDateClignotement <= millis ()) {
    gDateClignotement += 500 ;
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN)) ;
  }
  if (gDateEmission <= millis ()) {
    const bool poussoirAppuye = digitalRead (POUSSOIR) == LOW ;
    CANMessage message ;
    message.id = 0x120 ;
    message.len = 1 ;
    message.data [0] = poussoirAppuye ;
    const bool sent = ACAN_T4::can1.tryToSend (message) ;
    if (sent) {
      gDateEmission += PERIODE_EMISSION ;
    }
  }
}

