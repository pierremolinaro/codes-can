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
  cout << "TEST INVERSIONS DE BITS SUR TRAMES STANDARD D'UN OCTET DE DONNÉE" << endl ;
//--- Construire le multiset du nombre de bits flips
  Multiset bitFlipsMultiSet ;
  for (uint32_t idf1 = 0 ; idf1 < 2048 ; idf1 ++) {
    for (uint32_t data1 = 0 ; data1 < 256 ; data1++) {
      const uint8_t d1 = uint8_t (data1) ;
      const CANFrameGenerator f1 (idf1, FrameFormat::standard,
                                  1, &d1, FrameType::data, AckSlot::dominant) ;
      for (uint32_t idf2 = idf1 ; idf2 < 2048 ; idf2 ++) {
        for (uint32_t data2 = 0 ; data2 < 256 ; data2++) {
          if ((idf2 != idf1) || (data1 < data2)) {
            const uint8_t d2 = uint8_t (data2) ;
            const CANFrameGenerator f2 (idf2, FrameFormat::standard,
                                        1, &d2, FrameType::data, AckSlot::dominant) ;
            const uint32_t bitFlipCount = f1.hammingDistanceFrom (f2) ;
            if (bitFlipCount <= 5) {
              bitFlipsMultiSet.insert (bitFlipCount) ;
            }
            if (bitFlipCount <= 4) {
              const chrono::time_point<chrono::system_clock> end = chrono::system_clock::now() ;
              const chrono::duration<double> elapsed_seconds = end - start ;
              cout << int (elapsed_seconds.count ()) << "s:  " << bitFlipCount
                   << " inversions entre 0x" << hex << idf1 << ", data 0x" << data1
                   << " et 0x" << idf2 << ", data 0x" << data2 << dec << "\n" ;
            }
          }
        }
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
