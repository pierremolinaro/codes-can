//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

#ifndef __MK20DX256__
  #error "Ce sketch doit être compilé pour la Teensy 3.1 / 3.2"
#endif

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

#include <ACAN.h>
#include <TimerOne.h>

#include "CANFrameGenerator.h"

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

static uint8_t PORT_EMISSION = 5 ; // D5

static const uint32_t BIT_RATE = 100 * 1000 ;

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

void setup() {
  pinMode (LED_BUILTIN, OUTPUT) ;
  Serial.begin (9600) ;
//--- Use D5 pin for output frame
  pinMode (PORT_EMISSION, OUTPUT) ;
  digitalWrite (PORT_EMISSION, HIGH) ;
//--- Configure can0 for receiving frames
  ACANSettings settings (BIT_RATE) ;
  const uint32_t r = ACAN::can0.begin (settings) ;
  if (r != 0) {
    Serial.print ("Erreur configuration 0x");
    Serial.println (r, HEX) ;
    while (1) {
      digitalWrite (LED_BUILTIN, ! digitalRead (LED_BUILTIN)) ;
      delay (50) ;
    }
  }
//--- Timer 1 generates periodic interrupt
  Timer1.initialize ((1000 * 1000) / BIT_RATE) ; // Time unit is 1 µs
  Timer1.attachInterrupt (periodicISR) ;
 //--- Délai pour laisser le temps au contrôleur CAN de devenir actif
 // Soit 128 occurrences de la condition "bus libre"
 // Donc 128 * 11 * 10 µs  = 14, 08 ms
  delay (100) ;
//--- 
  randomSeed (0) ;
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

static volatile bool gBitArray [CAN_FRAME_MAX_LENGTH] ;
static volatile uint32_t gBitArrayLength = 0 ;
static volatile uint32_t gIndex = CAN_FRAME_MAX_LENGTH ;

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

static void periodicISR (void) {
  if (gIndex < gBitArrayLength) {
    digitalWrite (PORT_EMISSION, gBitArray [gIndex]) ;
    gIndex += 1 ;
  }else{
    digitalWrite (PORT_EMISSION, HIGH) ; // RÉCESSIF
  }
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

static uint32_t gRecusNonConformes = 0 ;
static uint32_t gNonRecus = 0 ;
static uint32_t gEmis = 0 ;
static uint32_t gInstantAffichage = 1000 ;

static uint32_t gTramesStandardsDonnees [9] ;
static uint32_t gTramesStandardsRequetes [9] ;
static uint32_t gTramesEtenduesDonnees [9] ;
static uint32_t gTramesEtenduesRequetes [9] ;
static const CANFrameGenerator * gFrame = nullptr ;

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

void loop() {
//--- Affichage ?
  if (gInstantAffichage <= millis ()) {
    digitalWrite (LED_BUILTIN, ! digitalRead (LED_BUILTIN)) ;
    Serial.print ("Émis ") ;
    Serial.print (gEmis) ;
    Serial.print (", non conformes ") ;
    Serial.print (gRecusNonConformes) ;
    Serial.print (", non reçus ") ;
    Serial.println (gNonRecus) ;
    if ((gInstantAffichage % 60000) == 0) {
      Serial.print (gInstantAffichage / 60000) ;
      Serial.println (" min") ;
      Serial.print ("Trames standards données :") ;
      for (uint32_t i=0 ; i<9 ; i++) {
        Serial.print (" ") ;
        Serial.print (gTramesStandardsDonnees [i]) ;
      }
      Serial.println ("") ;
      Serial.print ("Trames standards requête :") ;
      for (uint32_t i=0 ; i<9 ; i++) {
        Serial.print (" ") ;
        Serial.print (gTramesStandardsRequetes [i]) ;
      }
      Serial.println ("") ;
      Serial.print ("Trames étendues données  :") ;
      for (uint32_t i=0 ; i<9 ; i++) {
        Serial.print (" ") ;
        Serial.print (gTramesEtenduesDonnees [i]) ;
      }
      Serial.println ("") ;
      Serial.print ("Trames étendues requête  :") ;
      for (uint32_t i=0 ; i<9 ; i++) {
        Serial.print (" ") ;
        Serial.print (gTramesEtenduesRequetes [i]) ;
      }
      Serial.println ("") ;
    }
    gInstantAffichage += 1000 ;
  }
  if (gFrame == nullptr) {
  //--- Caractéristiques de la trame à émettre
    const bool extended = (random () & 1) != 0 ;
    const FrameFormat format = extended ? FrameFormat::extended : FrameFormat::standard ;
    const uint32_t identifier = uint32_t (random ()) & (extended ? 0x1FFFFFFF : 0x7FF) ;
    const uint8_t dataLength = uint8_t (random ()) % 9 ;
    uint8_t data [8] ;
    for (uint32_t i=0 ; i<dataLength ; i++) {
      data [i] = uint8_t (random ()) ;
    }
    const bool remoteFrame = (random () & 1) != 0 ;
    const FrameType type = remoteFrame ? FrameType::remote : FrameType::data ;
  //--- Construire la trame  à émettre
    gFrame = new CANFrameGenerator (identifier, format, dataLength, data, type, AckSlot::dominant) ;
  //--- Construire les tableau des statistiques
    if (extended) {
      if (remoteFrame) {
        gTramesEtenduesRequetes [dataLength] ++ ;
      }else{
        gTramesEtenduesDonnees [dataLength] ++ ;
      }
    }else if (remoteFrame) {
      gTramesStandardsRequetes [dataLength] ++ ;
    }else{
      gTramesStandardsDonnees [dataLength] ++ ;
    }
  }
//--- Copier la trame et lancer l'émission
  gIndex = CAN_FRAME_MAX_LENGTH ; // Ceci bloque l'émission de bits DOMINANT
  for (size_t i=0 ; i<gFrame->frameLength () ; i++) {
    gBitArray [i] = gFrame->bitAtIndex (i) ;
  }
  gBitArrayLength = gFrame->frameLength () ;
  gIndex = 0 ; // Ceci lance l'émission
  gEmis += 1 ;
//--- Attendre la réception 3ms maximum
  const uint32_t finAttente = millis () + 3 ;
  while ((millis () <= finAttente) && !ACAN::can0.available ()) {
  }
//--- Réception ?
  CANMessage trame ;
  if (ACAN::can0.receive (trame)) {
    bool ok = gFrame->identifier () == trame.id ;
    if (ok) {
      ok = gFrame->dataLength () == trame.len ;
    }
    if (ok) {
      ok = gFrame->remoteFrame () == trame.rtr ;
    }
    if (ok && !trame.rtr) {
      for (uint32_t i=0 ; (i<trame.len) && ok ; i++) {
        ok = gFrame->dataAtIndex (i) == trame.data [i] ;
      }
    }
    if (!ok) {
      gRecusNonConformes ++ ;
    }
    delete gFrame ;
    gFrame = nullptr ;
  }else{
    gNonRecus ++ ;
  }
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
