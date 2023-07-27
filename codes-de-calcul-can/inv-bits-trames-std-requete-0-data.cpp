//
//  testInversionsBitSurTramesStandardRequeteEtDonnees.cpp
//  compute-frame-length
//
//  Created by Pierre Molinaro on 27/03/2020.
//  Copyright © 2020 Pierre Molinaro. All rights reserved.
//
//----------------------------------------------------------------------------------------

#include "CANFrameGenerator.h"
#include "Multiset.h"

//----------------------------------------------------------------------------------------

#include <iostream>

//----------------------------------------------------------------------------------------

using namespace std ;

//----------------------------------------------------------------------------------------

int main (void) {
  const chrono::time_point<chrono::system_clock> start = chrono::system_clock::now();
  cout << "TEST INVERSIONS DE BITS ENTRE TRAMES STANDARDS DE REQUÊTE ET DE DONNÉES" << endl ;
//--- Construction du tableau
  const uint32_t NOMBRE_IDENTIFICATEURS = 1U << 11 ;
//--- Construire le multiset du nombre de bits flips
  Multiset bitFlipsMultiSet ;
  for (uint32_t idf1 = 0 ; idf1 < NOMBRE_IDENTIFICATEURS ; idf1 ++) {
    for (uint8_t lg1 = 0 ; lg1 <= 15 ; lg1 ++) {
      const CANFrameGenerator f1 (idf1, FrameFormat::standard,
                                  lg1, NULL, FrameType::remote, AckSlot::dominant) ;
      for (uint32_t idf2 = 0 ; idf2 < NOMBRE_IDENTIFICATEURS ; idf2 ++) {
        const CANFrameGenerator f2 (idf2, FrameFormat::standard,
                                    0, NULL, FrameType::data, AckSlot::dominant) ;
        const uint32_t bitFlipCount = f1.hammingDistanceFrom (f2) ;
        bitFlipsMultiSet.insert (bitFlipCount) ;
        if (bitFlipCount <= 3) {
          cout << "  " << bitFlipCount
               << " inversions entre 0x" << hex << idf1 << " DLC " << unsigned (lg1)
               << " et 0x" << idf2 << dec << "\n" ;
        }
      }
    }
  }
//--- Afficher le tableau des bit flips
  cout << "  Récapitulatif\n" ;
  for (auto it=bitFlipsMultiSet.cbegin(); it != bitFlipsMultiSet.cend(); it++) {
    cout << "    " << it << " inversions --> " << bitFlipsMultiSet.count (it) << "\n" ;
  }
  const chrono::time_point<chrono::system_clock> end = chrono::system_clock::now();
  const chrono::duration<double> elapsed_seconds = end - start;
  cout << "Elapsed time: " << elapsed_seconds.count() << "s\n" ;
  return 0 ;
}

//----------------------------------------------------------------------------------------
