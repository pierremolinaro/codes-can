//
//  testInversionsBitSurTramesStandardDonnees.cpp
//  compute-frame-length
//
//  Created by Pierre Molinaro on 27/03/2020.
//  Copyright © 2020 Pierre Molinaro. All rights reserved.
//
//----------------------------------------------------------------------------------------------------------------------

#include "CANFrameGenerator.h"
#include "Multiset.h"

//----------------------------------------------------------------------------------------------------------------------

#include <iostream>

//----------------------------------------------------------------------------------------------------------------------

using namespace std ;

//----------------------------------------------------------------------------------------------------------------------

int main (void) {
  const chrono::time_point<chrono::system_clock> start = chrono::system_clock::now();
  cout << "TEST INVERSIONS DE BITS SUR TRAMES ÉTENDUES SANS DONNÉE" << endl ;
//--- Construire le multiset du nombre de bits flips
  const uint32_t IDENTIFICATEUR_MAX = 0xFFFFU ;
  Multiset bitFlipsMultiSet ;
  for (uint32_t idf1 = 0 ; idf1 < (IDENTIFICATEUR_MAX - 1) ; idf1 ++) {
    const CANFrameGenerator f1 (idf1, FrameFormat::extended, 0, nullptr, FrameType::data, AckSlot::dominant) ;
    for (uint32_t idf2 = idf1 + 1 ; idf2 < IDENTIFICATEUR_MAX ; idf2 ++) {
      const CANFrameGenerator f2 (idf2, FrameFormat::extended, 0, nullptr, FrameType::data, AckSlot::dominant) ;
      const uint32_t bitFlipCount = f1.hammingDistanceFrom (f2) ;
      if (bitFlipCount <= 5) {
        bitFlipsMultiSet.insert (bitFlipCount) ;
      }
      if (bitFlipCount <= 4) {
        const chrono::time_point<chrono::system_clock> end = chrono::system_clock::now() ;
        const chrono::duration<double> elapsed_seconds = end - start ;
        cout << int (elapsed_seconds.count()) << "s :  " << bitFlipCount
             << " inversions entre 0x" << hex << idf1
             << " et 0x" << idf2 << dec << "\n" ;
      }
    }
  }
//--- Afficher le tableau des bit flips
  cout << "  Récapitulatif\n" ;
  for (auto it=bitFlipsMultiSet.cbegin(); it != bitFlipsMultiSet.cend(); it++) {
    cout << "    " << it << " inversions --> " << bitFlipsMultiSet.count (it) << "\n" ;
  }
  const chrono::time_point<chrono::system_clock> end = chrono::system_clock::now() ;
  const chrono::duration<double> elapsed_seconds = end - start ;
  cout << "Elapsed time: " << elapsed_seconds.count() << "s\n" ;
  return 0 ;
}

//----------------------------------------------------------------------------------------------------------------------
