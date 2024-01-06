#ifndef ARDUINO_TEENSY41
  #error "Ce croquis doit être compilé pour la Teensy 4.1"
#endif

#include <ACAN_T4.h>

void setup () {
  pinMode (LED_BUILTIN, OUTPUT) ;
  ACAN_T4_Settings settings (1000 * 1000) ;
  settings.mTxPinIsOpenCollector = true ;
  settings.mRxPinConfiguration = ACAN_T4_Settings::PULLUP_22k ;
  const uint32_t errorCode = ACAN_T4::can1.begin (settings) ;
   if (0 != errorCode) {
    while (1) {
      delay (50) ;
      digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN)) ;
    }
  }
}

static uint32_t gDateClignotement = 0 ;
static uint32_t gDateEmission = 1000 ;

void loop () {
  if (gDateClignotement <= millis ()) { //Echec du test après le débordement de millis()
    gDateClignotement += 500 ;
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN)) ;
  }
  if (gDateEmission <= millis ()) { // Echec du test après le débordement de millis ()
    gDateEmission += 1 ;
    CANMessage message ; // Message M0
    ACAN_T4::can1.tryToSend (message) ;
  }
}

