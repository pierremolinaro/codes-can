#ifndef ARDUINO_TEENSY41
#error "Ce croquis doit être compilé pour la Teensy 4.1"
#endif

#include <ACAN_T4.h>

static const byte LED = 0;

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  ACAN_T4_Settings settings(1000 * 1000);
  const uint32_t errorCode = ACAN_T4::can1.begin(settings);
  if (0 != errorCode) {
    while (1) {
      delay(50);
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    }
  }
}

static uint32_t gDateClignotement = 0;

void loop() {
  if (gDateClignotement <= millis()) {
    gDateClignotement += 500;
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
  CANMessage message;
  if (ACAN_T4::can1.receive (message)) {
    if (!message.ext && !message.rtr && (message.id == 0x123) && (message.len == 1)) {
      const bool etatPoussoir = message.data [0] == 0x01 ;
      digitalWrite (LED, etatPoussoir) ;
    }
  }
 }
