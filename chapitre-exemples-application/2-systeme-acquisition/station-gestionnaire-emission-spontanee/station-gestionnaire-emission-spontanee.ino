#ifndef ARDUINO_XIAO_ESP32S3
#error "Ce croquis doit être compilé pour la carte XIAO ESP32S3"
#endif

#include <ACAN_ESP32.h>

void setup () {
  Serial.begin () ;
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
static uint32_t gNombreTramesRecues [2] ;

void loop () {
  CANMessage message ;
  if (ACAN_ESP32::can.receive (message)) {
    if (!message.ext && !message.rtr
                     && ((message.id & 0x7FE) == 0x400) && (message.len == 0)) {
      const uint32_t indiceCapteur = message.id & 1 ;
      gNombreTramesRecues [indiceCapteur] += 1 ;
    // Exploitation des données reçues
    }
  }
  if (int32_t (millis () - gDateClignotement) >= int32_t (10000)) {
    gDateClignotement += 10000 ;
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    Serial.print ("À ") ;
    Serial.print (millis () / 1000) ;
    Serial.print (" s :") ;
    for (int i=0 ; i<2 ; i++) {
      Serial.print (" ") ;
      Serial.print (gNombreTramesRecues [i]) ;
    }
    Serial.println () ;
  }
}
