#ifndef ARDUINO_TEENSY41
  #error "Ce croquis doit être compilé pour la Teensy 4.1"
#endif

#if F_CPU != 24000000
  #error "La fréquence du CPU doit être 24 MHz"
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

void loop () {
  if (gDateClignotement <= millis ()) {// Le test échoue après le débordement de millis
    gDateClignotement += 500 ;
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN)) ;
  }
  CANMessage message ;
  if (ACAN_T4::can1.receive (message) && (message.id == 0)) {
    message.id = 1 ; // Message M1
    ACAN_T4::can1.tryToSend (message) ;
  }
}
