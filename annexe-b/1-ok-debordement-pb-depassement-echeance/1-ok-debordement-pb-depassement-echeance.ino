//-----------------------------------------------------------------
//
// Cette nouvelle écriture du test présente l'inconvénient suivant :
//   - la période n'est plus respectée lors d'un dépassement occasionnel de l'exécution
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

uint32_t gEcheance = 0 ;
uint8_t gPhase = 0 ; // Toutes les 4 itérations, dépassement de la période

//-----------------------------------------------------------------

void loop () {
  const uint32_t date = monComptageTemps () ;
  if ((date - gEcheance) >= PERIODE) {
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN)) ;
    Serial.print (gEcheance) ;
    Serial.print (", millis : ") ;
    Serial.print (millis ()) ;
    Serial.println (" ms") ;
    gEcheance = date ;
    gPhase += 1 ;
    if (gPhase == 4) {
      gPhase = 0 ;
      delay (1111) ;
    }
  }
}

//-----------------------------------------------------------------
