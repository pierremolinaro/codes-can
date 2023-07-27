//--------------------------------------------------------------------------------------------------
//  Ce croquis teste le généateur de trames CANFD en mode non ISO
//
//  Connexion :
//    relier le port PORT_EMISSION au port #31 (CANRX du CAN3)
//--------------------------------------------------------------------------------------------------

#include <ACAN_T4.h>
#include <TimerOne.h>

#include "CANFDFrameGenerator.h"

//--------------------------------------------------------------------------------------------------

#ifndef __IMXRT1062__
  #error "Ce sketch doit être compilé pour la Teensy 4.0"
#endif

//--------------------------------------------------------------------------------------------------

static const uint8_t PORT_EMISSION = 28 ;

//--------------------------------------------------------------------------------------------------

static const uint32_t ARBITRATION_BIT_RATE = 10 * 1000 ;
static const DataBitRateFactor DATA_BIT_RATE_FACTOR = DataBitRateFactor::x4 ;

//--------------------------------------------------------------------------------------------------

static uint32_t gBitRatePrescaler = 0 ;
static uint32_t gArbitrationBeforeSP = 0 ;
static uint32_t gArbitrationAfterSP = 0 ;
static uint32_t gDataBeforeSP = 0 ;
static uint32_t gDataAfterSP = 0 ;

//--------------------------------------------------------------------------------------------------

void setup () {
  pinMode (LED_BUILTIN, OUTPUT) ;
  Serial.begin (9600) ;
  while (!Serial) {
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN)) ;
    delay (50) ;
  }
//--- Use D28 pin for output frame
  pinMode (PORT_EMISSION, OUTPUT) ;
  digitalWriteFast (PORT_EMISSION, HIGH) ;
  pinMode (PORT_EMISSION - 1, OUTPUT) ;
  digitalWriteFast (PORT_EMISSION - 1, HIGH) ;
//--- Configure can3 for receiving frames
  setCANRootClock (ACAN_CAN_ROOT_CLOCK::CLOCK_60MHz, 60) ; // CAN clock : 1 MHz
  ACAN_T4FD_Settings settings (ARBITRATION_BIT_RATE, DATA_BIT_RATE_FACTOR) ;
  settings.mISOCRCEnabled = false ;
  Serial.print ("Bit Rate Prescaler: ") ;
  Serial.println (settings.mBitRatePrescaler) ;
  Serial.print ("Arbitration Propagation Segment: ") ;
  Serial.println (settings.mArbitrationPropagationSegment) ;
  Serial.print ("Arbitration Phase segment 1: ") ;
  Serial.println (settings.mArbitrationPhaseSegment1) ;
  Serial.print ("Arbitration Phase segment 2: ") ;
  Serial.println (settings.mArbitrationPhaseSegment2) ;
  Serial.print ("Data Propagation Segment: ") ;
  Serial.println (settings.mDataPropagationSegment) ;
  Serial.print ("Data Phase segment 1: ") ;
  Serial.println (settings.mDataPhaseSegment1) ;
  Serial.print ("Data Phase segment 2: ") ;
  Serial.println (settings.mDataPhaseSegment2) ;
  gBitRatePrescaler = settings.mBitRatePrescaler ;
  gArbitrationBeforeSP = 1 + settings.mArbitrationPropagationSegment + settings.mArbitrationPhaseSegment1 ;
  gArbitrationAfterSP = settings.mArbitrationPhaseSegment2 ;
  gDataBeforeSP = 1 + settings.mDataPropagationSegment + settings.mDataPhaseSegment1 ;
  gDataAfterSP = settings.mDataPhaseSegment2 ;
  // Serial.println (gArbitrationBeforeSP) ;
  // Serial.println (gArbitrationAfterSP) ;
  // Serial.println (gDataBeforeSP) ;
  // Serial.println (gDataAfterSP) ;
  const uint32_t r = ACAN_T4::can3.beginFD (settings) ;
  if (r != 0) {
    Serial.print ("Erreur configuration 0x");
    Serial.println (r, HEX) ;
    while (1) {
      digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN)) ;
      delay (50) ;
    }
  }
//--- Timer 1 generates periodic interrupt gBitRatePrescaler
  Timer1.initialize (gBitRatePrescaler) ; // Timer unit is 1 µs
  Timer1.attachInterrupt (periodicISR) ;
 //--- Délai pour laisser le temps au contrôleur CAN de devenir actif
 // Soit 128 occurrences de la condition "bus libre"
  delay (100) ;
//--- 
  randomSeed (0) ;
}

//--------------------------------------------------------------------------------------------------

static const uint32_t BIT_BUFFER_SIZE = CANFD_FRAME_MAX_LENGTH * 256 ;
static volatile bool gBitBuffer [BIT_BUFFER_SIZE] ;
static volatile uint32_t gBitBufferLength = 0 ;
static volatile uint32_t gBitBufferIndex = BIT_BUFFER_SIZE ;

//--------------------------------------------------------------------------------------------------

static void periodicISR (void) {
  const uint32_t bitBufferIndex = gBitBufferIndex ;
  if (bitBufferIndex < gBitBufferLength) {
    digitalWriteFast (PORT_EMISSION, gBitBuffer [bitBufferIndex]) ;
    digitalWriteFast (PORT_EMISSION - 1, gBitBuffer [bitBufferIndex]) ;
    gBitBufferIndex = bitBufferIndex + 1 ;
  }else{
    digitalWriteFast (PORT_EMISSION, HIGH) ;
    digitalWriteFast (PORT_EMISSION - 1, HIGH) ;
  }
}

//--------------------------------------------------------------------------------------------------

static uint32_t gRecusNonConformes = 0 ;
static uint32_t gNonRecus = 0 ;
static uint32_t gEmis = 0 ;
static uint32_t gInstantAffichage = 1000 ;

class Result {

  public: uint32_t m_noEXT_noBRS_noESI = 0 ;
  public: uint32_t m_noEXT_noBRS_ESI = 0 ;
  public: uint32_t m_noEXT_BRS_noESI = 0 ;
  public: uint32_t m_noEXT_BRS_ESI = 0 ;
  public: uint32_t m_EXT_noBRS_noESI = 0 ;
  public: uint32_t m_EXT_noBRS_ESI = 0 ;
  public: uint32_t m_EXT_BRS_noESI = 0 ;
  public: uint32_t m_EXT_BRS_ESI = 0 ;

  void add (const bool inExtended,
            const bool inESI,
            const bool inBRS) {
    if (inExtended) {
      if (inESI) {
        if (inBRS) {
         m_EXT_BRS_ESI += 1 ;
        }else{
          m_EXT_noBRS_ESI += 1 ;
        }
      }else if (inBRS) {
        m_EXT_BRS_noESI += 1 ;
      }else{
        m_EXT_noBRS_noESI += 1 ;
      }
    }else{
      if (inESI) {
        if (inBRS) {
         m_noEXT_BRS_ESI += 1 ;
        }else{
          m_noEXT_noBRS_ESI += 1 ;
        }
      }else if (inBRS) {
        m_noEXT_BRS_noESI += 1 ;
      }else{
        m_noEXT_noBRS_noESI += 1 ;
      }
    }
  }
} ;

static Result gTramesEnvoyees [16] ;

//--------------------------------------------------------------------------------------------------

void loop () {
//--- Affichage ?
  if (gInstantAffichage <= millis ()) {
    digitalWrite (LED_BUILTIN, ! digitalRead (LED_BUILTIN)) ;
    Serial.print ("Non ISO, émis ") ;
    Serial.print (gEmis) ;
    Serial.print (", non conformes ") ;
    Serial.print (gRecusNonConformes) ;
    Serial.print (", non reçus ") ;
    Serial.println (gNonRecus) ;
    if ((gInstantAffichage % 60000) == 0) {
      Serial.print (gInstantAffichage / 60000) ;
      Serial.println (" min") ;
      for (uint32_t i=0 ; i<16 ; i++) {
        Serial.print (i) ;
        Serial.print (" : ") ;
        Serial.print (gTramesEnvoyees [i].m_noEXT_noBRS_noESI) ;
        Serial.print (" ") ;
        Serial.print (gTramesEnvoyees [i].m_noEXT_noBRS_ESI) ;
        Serial.print (" ") ;
        Serial.print (gTramesEnvoyees [i].m_noEXT_BRS_noESI) ;
        Serial.print (" ") ;
        Serial.print (gTramesEnvoyees [i].m_noEXT_BRS_ESI) ;
        Serial.print (" ") ;
        Serial.print (gTramesEnvoyees [i].m_EXT_noBRS_noESI) ;
        Serial.print (" ") ;
        Serial.print (gTramesEnvoyees [i].m_EXT_noBRS_ESI) ;
        Serial.print (" ") ;
        Serial.print (gTramesEnvoyees [i].m_EXT_BRS_noESI) ;
        Serial.print (" ") ;
        Serial.println (gTramesEnvoyees [i].m_EXT_BRS_ESI) ;
      }
    }
    gInstantAffichage += 1000 ;
  }
//--- Construire la trame à émettre
  const bool extended = (random () & 1) != 0 ;
  const bool esi = (random () & 1) != 0 ;
  const bool bitRateSwitch = (random () & 1) != 0 ;
  const FrameFormat format = extended ? FrameFormat::extended : FrameFormat::standard ;
  const uint32_t identifier = uint32_t (random ()) & (extended ? 0x1FFFFFFF : 0x7FF) ;
  const uint8_t dataLengthCode = uint8_t (random ()) % 16 ;
  const uint8_t frameByteLength = CANFDFrameGenerator::lengthForCode (dataLengthCode) ;
  uint8_t data [64] ;
  for (uint32_t i=0 ; i<frameByteLength ; i++) {
    data [i] = uint8_t (random ()) ;
  }
//--- Construire la trame  à émettre
  const CANFDFrameGenerator * frame = new CANFDFrameGenerator (identifier,
                                                               format,
                                                               bitRateSwitch ? BitRateSwitch::yes : BitRateSwitch::no,
                                                               esi ? ESI::dominant : ESI::recessive,
                                                               FrameProtocol::nonISO,
                                                               dataLengthCode,
                                                               data,
                                                               AckSlot::dominant) ;
//--- Attendre la fin de l'émission de la trame en cours (champ INTERMISSION)
  while (gBitBufferIndex < gBitBufferLength) {}
//--- Copier la trame et lancer l'émission
  gBitBufferIndex = BIT_BUFFER_SIZE ; // Ceci bloque l'émission de bits DOMINANT
  uint32_t idx = 0 ;
  for (size_t i=0 ; i<frame->frameLength () ; i++) {
    uint32_t bitDuration = gArbitrationBeforeSP + gArbitrationAfterSP ;
    switch (frame->bitDurationAtIndex (i)) {
    case BitDuration::arbitration :
      break ;        
    case BitDuration::arbitrationToData :
      bitDuration = gArbitrationBeforeSP + gDataAfterSP ;
      break ;
    case BitDuration::dataToArbitration :
      bitDuration = gDataBeforeSP + gArbitrationAfterSP ;
      break ;
    case BitDuration::data :
      bitDuration = gDataBeforeSP + gDataAfterSP ;
      break ;
    }
    const bool bit = frame->bitAtIndex (i) ;
    for (uint32_t s = 1 ; s <= bitDuration ; s++) {
      gBitBuffer [idx] = bit ;
      idx += 1 ;
    }    
  }
  gBitBufferLength = idx ;
  gBitBufferIndex = 0 ; // Ceci lance l'émission
  gEmis += 1 ;
  delete frame ;
//--- Construire les tableau des statistiques
  gTramesEnvoyees [dataLengthCode].add (extended, esi, bitRateSwitch) ;
//--- Attendre la réception
  const uint32_t finAttente = millis () + 1000 ;
  while ((millis () <= finAttente) && !ACAN_T4::can3.availableFD ()) {
  }
//--- Réception ?
  CANFDMessage trame ;
  if (ACAN_T4::can3.receiveFD (trame)) {
    bool ok = identifier == trame.id ;
    if (ok) {
      ok = frameByteLength == trame.len ;
    }
    for (uint32_t i=0 ; (i<frameByteLength) && ok ; i++) {
      ok = data [i] == trame.data [i] ;
    }
    if (!ok) {
      gRecusNonConformes ++ ;
    }
  }else{
    gNonRecus ++ ;
  }
}

//--------------------------------------------------------------------------------------------------
