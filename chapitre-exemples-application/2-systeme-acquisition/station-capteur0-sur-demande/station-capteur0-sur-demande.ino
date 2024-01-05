#ifndef ARDUINO_XIAO_ESP32S3
#error "Ce croquis doit être compilé pour la carte XIAO ESP32S3"
#endif

#include <ACAN_ESP32.h>

void setup () {
  pinMode (LED_BUILTIN, OUTPUT);
  ACAN_ESP32_Settings settings (1000 * 1000);
  const uint32_t errorCode = ACAN_ESP32::can.begin (settings) ;
  if (0 != errorCode) {
    while (1) {
      delay(50);
      digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN));
    }
  }
}

static uint32_t gDateClignotement = 0 ;
static bool gEmettre = false ;

void loop () {
  if (int32_t (millis () - gDateClignotement) >= int32_t (500)) {
    gDateClignotement += 500 ;
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN));
  }
  CANMessage message ;
  if (ACAN_ESP32::can.receive (message)) {
    if (!message.ext && !message.rtr && (message.id == 0x200) && (message.len == 0)) {
      gEmettre = true ;
    }
  }
  if (gEmettre) {
    message.id = 0x400 ; // 0x401 pour CAPTEUR1
    message.ext = false ;
    message.rtr = false ;
    message.len = 0 ;
    gEmettre = !ACAN_ESP32::can.tryToSend (message) ;
  }
}
