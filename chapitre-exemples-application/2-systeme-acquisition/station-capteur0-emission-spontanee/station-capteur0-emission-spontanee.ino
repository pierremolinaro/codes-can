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
static const uint32_t PERIODE_EMISSION = 2 ; // En millisecondes
static const uint32_t DATE_PREMIERE_EMISSION = 1000 ;
static uint32_t gDateDerniereEmission = DATE_PREMIERE_EMISSION - PERIODE_EMISSION ;

void loop () {
  if (int32_t (millis() - gDateClignotement) >= int32_t (500)) {
    gDateClignotement += 500 ;
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
  if (int32_t (millis () - gDateDerniereEmission) >= int32_t (PERIODE_EMISSION)) {
    CANMessage message ; // Par défaut, message.len est égal à 0
    message.id = 0x400 ; // 0x401 pour CAPTEUR1
    const bool ok = ACAN_ESP32::can.tryToSend (message) ;
    if (ok) {
      gDateDerniereEmission += PERIODE_EMISSION ;
    }
  }
}
