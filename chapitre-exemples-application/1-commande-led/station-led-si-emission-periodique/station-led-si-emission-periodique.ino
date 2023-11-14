#ifndef ARDUINO_XIAO_ESP32S3
#error "Ce croquis doit être compilé pour la carte XIAO ESP32S3"
#endif

#include <ACAN_ESP32.h>

static const byte LED = 0;

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  ACAN_ESP32_Settings settings(1000 * 1000);
  const uint32_t errorCode = ACAN_ESP32::can.begin(settings);
  if (0 != errorCode) {
    while (1) {
      delay(50);
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    }
  }
}

static uint32_t gDateClignotement = 0;
static const uint32_t DELAI_ALERTE_RECEPTION = 25 ; // ms
static bool gReceptionOk = true ;
static uint32_t gEcheanceReceptionMessage = 0;

void loop() {
  if (gReceptionOk && ((millis () - gEcheanceReceptionMessage) >= 0)) {
    gReceptionOk = false ;
    gDateClignotement = millis () ;
  }
  if ((millis() - gDateClignotement) >= 500) {
    gDateClignotement += gReceptionOk ? 500 : DELAI_ALERTE_RECEPTION ;
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
  CANMessage message;
  if (ACAN_ESP32::can.receive (message)) {
    if (!message.ext && !message.rtr && (message.id == 0x123) && (message.len == 1)) {
      const bool etatPoussoir = message.data [0] == 0x01 ;
      digitalWrite (LED, etatPoussoir) ;
      gEcheanceReceptionMessage = millis () + DELAI_ALERTE_RECEPTION ;
      gReceptionOk = true ;
    }
  }
 }
