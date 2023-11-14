#ifndef ARDUINO_XIAO_ESP32S3
#error "Ce croquis doit être compilé pour la carte XIAO ESP32S3"
#endif

#include <ACAN_ESP32.h>

void setup () {
  pinMode(LED_BUILTIN, OUTPUT);
  ACAN_ESP32_Settings settings (1000 * 1000);
  const uint32_t errorCode = ACAN_ESP32::can.begin(settings);
  if (0 != errorCode) {
    while (1) {
      delay(50);
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    }
  }
}

static uint32_t gDateClignotement = 0 ;
static uint32_t gDateEmission = 0 ;
static const uint32_t PERIODE_EMISSION = 5 ; // En millisecondes

void loop () {
  if ((millis() - gDateClignotement) >= 500) {
    gDateClignotement += 500 ;
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
  if (0 <= (millis () - gDateEmission)) {
    CANMessage message ;
    message.id = 0x400 ; // 0x401 pour CAPTEUR1, 0x402 pour CAPTEUR2, ...
    message.len = 1 ;
    message.data [0] = 0x01 ;
    const bool ok = ACAN_ESP32::can.tryToSend (message) ;
    if (ok) {
      gDateEmission += PERIODE_EMISSION ;
    }
  }
}
