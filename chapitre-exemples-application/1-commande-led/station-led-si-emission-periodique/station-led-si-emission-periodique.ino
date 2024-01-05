#ifndef ARDUINO_XIAO_ESP32S3
#error "Ce croquis doit être compilé pour la carte XIAO ESP32S3"
#endif

#include <ACAN_ESP32.h>

static const byte LED = 9 ;

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

static uint32_t gDateClignotement = 0 ;
static uint32_t gDateDerniereReception = 0 ;
static const int32_t DELAI_ALERTE_RECEPTION = 25 ; // ms
static const int32_t CLIGNOTEMENT_ALERTE = 50 ; // ms
static uint32_t gDateClignotementErreurReception = 0 ;
static bool gReceptionOk = true ;

void loop() {
  if (gReceptionOk && (int32_t (millis() - gDateClignotement) >= int32_t (500))) {
    gDateClignotement += 500 ;
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
  CANMessage message;
  if (ACAN_ESP32::can.receive (message)) {
    if (!message.ext && !message.rtr && (message.id == 0x120) && (message.len == 1)) {
      switch (message.data[0]) {
      case 0x00 : digitalWrite (LED, LOW); break ;
      case 0x01 : digitalWrite (LED, HIGH); break ;
      default: break ;
      }
      gDateDerniereReception = millis () ;
      gReceptionOk = true ;
    }
  }
  if (gReceptionOk
       && (int32_t (millis () - gDateDerniereReception) >= DELAI_ALERTE_RECEPTION)) {
    gReceptionOk = false ;
    gDateClignotementErreurReception = millis () ;
  }
  if (!gReceptionOk
       && (int32_t(millis ()-gDateClignotementErreurReception)>=CLIGNOTEMENT_ALERTE)) {
    gDateClignotementErreurReception += uint32_t (CLIGNOTEMENT_ALERTE) ;
    digitalWrite(LED, !digitalRead(LED));
  }
}
