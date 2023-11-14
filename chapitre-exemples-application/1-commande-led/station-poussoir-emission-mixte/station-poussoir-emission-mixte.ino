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

static bool emettreMessage (const bool & inPoussoirAppuye) {
  CANMessage message ;
  message.id = 0x120 ;
  message.len = 1 ;
  message.data [0] = inPoussoirAppuye ;
  const bool sent = ACAN_ESP32::can.tryToSend (message) ;
  return sent ;
}

static uint32_t gDateClignotement = 0 ;

static const uint32_t PERIODE_EMISSION = 200 ; // En millisecondes
enum class EtatPoussoir { relache, apresAppui, appuye, apresRelachement } ;
static EtatPoussoir gEtatPoussoir = EtatPoussoir::relache ;
static uint32_t gDateDerniereEmissionEtatPoussoir = 0 ;
static const uint32_t DELAI_REBONDS = 10 ; // ms

void loop () {
  if ((millis() - gDateClignotement) >= 500) {
    gDateClignotement += 500 ;
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN)) ;
  }
  const bool poussoirAppuye = digitalRead (POUSSOIR) == LOW ;
  switch (gEtatPoussoir) {
  case EtatPoussoir::relache :
    if (poussoirAppuye) {
      const bool sent = emettreMessage (true) ;
      if (sent) {
        gDateDerniereEmissionEtatPoussoir = millis () ;
        gEtatPoussoir = EtatPoussoir::apresAppui ;
      }
    }else if ((millis () - gDateDerniereEmissionEtatPoussoir) >= PERIODE_EMISSION) {
      const bool sent = emettreMessage (true) ;
      if (sent) {
        gDateDerniereEmissionEtatPoussoir += PERIODE_EMISSION ;
      }
    }
    break ;
  case EtatPoussoir::apresAppui :
    if ((millis () - gDateDerniereEmissionEtatPoussoir) >= DELAI_REBONDS) {
      gEtatPoussoir = EtatPoussoir::appuye ;
    }
    break ;
  case EtatPoussoir::appuye :
    if (!poussoirAppuye){
      const bool sent = emettreMessage (false) ;
      if (sent) {
        gDateDerniereEmissionEtatPoussoir = millis () ;
        gEtatPoussoir = EtatPoussoir::apresRelachement ;
      }
    }else if ((millis () - gDateDerniereEmissionEtatPoussoir) >= PERIODE_EMISSION) {
      const bool sent = emettreMessage (false) ;
      if (sent) {
        gDateDerniereEmissionEtatPoussoir += PERIODE_EMISSION ;
      }
    }
    break ;
  case EtatPoussoir::apresRelachement :
    if ((millis () - gDateDerniereEmissionEtatPoussoir) >= DELAI_REBONDS) {
      gEtatPoussoir = EtatPoussoir::relache ;
    }
    break ;
  }
}

