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
static uint32_t gDonneesRecues [4] ;
static uint32_t gDateAcquisition = 0 ;
static uint8_t gMasqueReception = 0 ;
static const uint32_t PERIODE_ACQUISITION = 5 ; // En millisecondes

void loop () {
  if ((millis () - gDateClignotement) >= 500) {
    gDateClignotement += 500;
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
  if (gDateAcquisition <= millis()) {
    CANMessage message ;
    message.id = 0x300 ;
    const bool sent = ACAN_ESP32::can.tryToSend (message) ;
    if (sent) {
      gDateAcquisition += PERIODE_ACQUISITION ;
      gMasqueReception = 0 ;
    }
  }
  CANMessage message ;
  if (ACAN_ESP32::can.receive (message)) {
    if (!message.ext && !message.rtr && ((message.id & 0x7FC) == 0x400) && (message.len == 1)) {
      const uint32_t indiceCapteur = message.id & 3 ;
      gDonneesRecues [indiceCapteur] = message.data [0] ;
      gMasqueReception |= (1 << indiceCapteur) ;
      if (gMasqueReception == 0x0F) {
      // Exploitation des données reçues
      }
    }
  }
}
