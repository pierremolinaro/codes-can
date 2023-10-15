#ifndef ARDUINO_XIAO_ESP32S3
#error "Ce croquis doit être compilé pour la carte XIAO ESP32S3"
#endif

#include <ACAN_ESP32.h>

static const byte POUSSOIR = 0 ;

void setup () {
  pinMode (POUSSOIR, INPUT_PULLUP) ;
  pinMode (LED_BUILTIN, OUTPUT) ;
  ACAN_ESP32_Settings settings (1000 * 1000) ;
  const uint32_t errorCode = ACAN_ESP32::can.begin (settings) ;
   if (0 != errorCode) {
    while (1) {
      delay (50) ;
      digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN)) ;
    }
  }
}

static uint32_t gDateClignotement = 0 ;
enum class EtatPoussoir { relâché, aprèsAppui, appuyé, aprèsRelâchement } ;

static EtatPoussoir gEtatPoussoir = EtatPoussoir::relâché ;
static uint32_t gEcheancePoussoir = 0 ;
static const uint32_t DELAI_REBONDS = 10 ; // ms

void loop () {
  if (gDateClignotement <= millis ()) {
    gDateClignotement += 500 ;
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN)) ;
  }
  const bool poussoirAppuye = digitalRead (POUSSOIR) == LOW ;
  switch (gEtatPoussoir) {
  case EtatPoussoir::relâché :
    if (poussoirAppuye) {
      CANMessage message ;
      message.id = 0x120 ;
      message.len = 1 ;
      message.data [0] = poussoirAppuye ;
      const bool sent = ACAN_ESP32::can.tryToSend (message) ;
      if (sent) {
        gEtatPoussoir = EtatPoussoir::aprèsAppui ;
        gEcheancePoussoir = millis () + DELAI_REBONDS ;
      }
    }
    break ;
  case EtatPoussoir::aprèsAppui :
    if (gEcheancePoussoir <= millis ()) {
      gEtatPoussoir = EtatPoussoir::appuyé ;
    }
    break ;
  case EtatPoussoir::appuyé :
   if (!poussoirAppuye) {
      gEtatPoussoir = EtatPoussoir::aprèsRelâchement ;
      gEcheancePoussoir = millis () + DELAI_REBONDS ;
   }
    break ;
  case EtatPoussoir::aprèsRelâchement :
     if (gEcheancePoussoir <= millis ()) {
      gEtatPoussoir = EtatPoussoir::relâché ;
    }
    break ;
  }
}

