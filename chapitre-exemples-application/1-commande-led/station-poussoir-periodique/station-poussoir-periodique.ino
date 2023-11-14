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
static const uint32_t PERIODE_EMISSION = 20 ; // En millisecondes
static uint32_t gDateEmission = 0 ;

void loop () {
  if ((millis() - gDateClignotement) >= 500) {
    gDateClignotement += 500 ;
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN)) ;
  }
  if ((millis () - gDateEmission) >= PERIODE_EMISSION) {
    const bool poussoirAppuye = digitalRead (POUSSOIR) == LOW ;
    const bool sent = emettreMessage (poussoirAppuye) ;
    if (sent) {
      gDateEmission += PERIODE_EMISSION ;
    }
  }
}

