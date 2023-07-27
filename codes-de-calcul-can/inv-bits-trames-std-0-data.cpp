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
  cout << "TEST INVERSIONS DE BITS SUR TRAMES STANDARD SANS DONNÉE" << endl ;
//--- Construire le multiset du nombre de bits flips
  Multiset bitFlipsMultiSet ;
  for (uint32_t idf1 = 0 ; idf1 < 2048 ; idf1 ++) {
    const CANFrameGenerator f1 (idf1, FrameFormat::standard, 0, nullptr, FrameType::data, AckSlot::dominant) ;
    for (uint32_t idf2 = idf1 + 1 ; idf2 < 2048 ; idf2 ++) {
      const CANFrameGenerator f2 (idf2, FrameFormat::standard, 0, nullptr, FrameType::data, AckSlot::dominant) ;
      const uint32_t bitFlipCount = f1.hammingDistanceFrom (f2) ;
      bitFlipsMultiSet.insert (bitFlipCount) ;
      if (bitFlipCount <= 4) {
        cout << "  " << bitFlipCount
             << " inversions pour 0x" << hex << idf1
             << " et Ox" << idf2 << dec << "\n" ;
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
