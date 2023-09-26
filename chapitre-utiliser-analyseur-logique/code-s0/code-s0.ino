#ifndef ARDUINO_TEENSY41
  #error "Ce croquis doit être compilé pour la Teensy 4.1"
#endif

#include <ACAN_T4.h>

static const byte POUSSOIR = 0 ;

void setup () {
  pinMode (LED_BUILTIN, OUTPUT) ;
  pinMode (POUSSOIR, INPUT_PULLUP) ;
  ACAN_T4_Settings settings (1000 * 1000) ;
  const uint32_t errorCode = ACAN_T4::can1.begin (settings) ;
   if (0 != errorCode) {
    while (1) {
      delay (50) ;
      digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN)) ;
    }
  }
}

enum class ÉtatPoussoir {relâché, filtrageAppui, appuyé, filtrageRelachement} ;

static ÉtatPoussoir gÉtatPoussoir = ÉtatPoussoir::relâché ;
static uint32_t gDateClignotement = 0 ;
static uint32_t gDateFinFiltrage = 0 ;

void loop () {
  if (gDateClignotement <= millis ()) {
    gDateClignotement += 500 ;
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN)) ;
  }
  switch (gÉtatPoussoir) {
  case ÉtatPoussoir::relâché :
    if (!digitalRead (POUSSOIR)) {
      gÉtatPoussoir = ÉtatPoussoir::filtrageAppui ;
      gDateFinFiltrage = millis () + 100 ;
      CANMessage message ; // Message M0
      ACAN_T4::can1.tryToSend (message) ;
    }
    break ;
  case ÉtatPoussoir::filtrageAppui :
    if (gDateFinFiltrage <= millis ()) {
      gÉtatPoussoir = ÉtatPoussoir::appuyé ;
    }
    break ;
  case ÉtatPoussoir::appuyé :
    if (digitalRead (POUSSOIR)) {
      gÉtatPoussoir = ÉtatPoussoir::filtrageRelachement ;
      gDateFinFiltrage = millis () + 100 ;
    }
    break ;
  case ÉtatPoussoir::filtrageRelachement :
    if (gDateFinFiltrage <= millis ()) {
      gÉtatPoussoir = ÉtatPoussoir::relâché ;
    }
    break ;
  }
}

