uint32_t monComptageTemps (void) {
  return millis () * 200 * 1000 ;
}

void setup () {
  pinMode (LED_BUILTIN, OUTPUT);
  Serial.begin (19200) ;
  while (!Serial) {
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN)) ;
    delay (50) ;
  }
}

static const uint32_t PERIODE = 1UL * 1000 * 200 * 1000 ; // 1 s
static uint32_t gDateProchaineExecution = PERIODE ;
static uint8_t gPhase = 0 ; // Toutes les 4 itérations, dépassement de la période

void loop () {
  if (gDateProchaineExecution <= monComptageTemps ()) {
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN)) ;
    Serial.print ("A ") ;
    Serial.print (millis ()) ;
    Serial.print (" ms, gDateProchaineExecution = ") ;
    Serial.println (gDateProchaineExecution) ;
    gDateProchaineExecution += PERIODE ;
    gPhase += 1 ;
    if (gPhase == 4) {
      gPhase = 0 ;
      delay (1111) ;
    }
  }
}
