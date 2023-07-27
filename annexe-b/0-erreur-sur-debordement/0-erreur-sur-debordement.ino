//-----------------------------------------------------------------
//
// Ce premier code montre que le test
//   if (gEcheance <= monComptageTemps ()) {
// ne respecte pas la période après débordement du compteur de temps
//
//-----------------------------------------------------------------

uint32_t monComptageTemps (void) {
  return millis () * 200 * 1000 ;
}

const uint32_t PERIODE = 1UL * 1000 * 200 * 1000 ; // 1 s

//-----------------------------------------------------------------

void setup () {
  pinMode (LED_BUILTIN, OUTPUT);
  Serial.begin (19200) ;

  while (!Serial) {
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN)) ;
    delay (50) ;
  }

  Serial.print ("Periode : ") ;
  Serial.println (PERIODE) ;
}

//-----------------------------------------------------------------

uint32_t gEcheance = PERIODE ;
uint8_t gPhase = 0 ; // Toutes les 4 itérations, dépassement de la période

//-----------------------------------------------------------------

void loop () {
  if (gEcheance <= monComptageTemps ()) {
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN)) ;
    Serial.print (gEcheance) ;
    Serial.print (", millis : ") ;
    Serial.print (millis ()) ;
    Serial.println (" ms") ;
    gEcheance += PERIODE ;
    gPhase += 1 ;
    if (gPhase == 4) {
      gPhase = 0 ;
      delay (1111) ;
    }
  }
}

//-----------------------------------------------------------------
