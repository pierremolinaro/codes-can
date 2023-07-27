//
//  testInversionsBitSurTramesStandardRequete.cpp
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
  cout << "TEST INVERSIONS DE BITS SUR TRAMES STANDARDS DE REQUÊTE" << endl ;
//--- Construction du tableau
  const uint32_t NOMBRE_IDENTIFICATEURS = 1U << 11 ;
//--- Construire le multiset du nombre de bits flips
  Multiset bitFlipsMultiSet ;
  for (uint32_t idf1 = 0 ; idf1 < (NOMBRE_IDENTIFICATEURS - 1) ; idf1 ++) {
    for (uint8_t lg1 = 0 ; lg1 <= 15 ; lg1 ++) {
      const CANFrameGenerator f1 (idf1, FrameFormat::standard, lg1, NULL, FrameType::remote, AckSlot::dominant) ;
      for (uint32_t idf2 = idf1 + 1 ; idf2 < NOMBRE_IDENTIFICATEURS ; idf2 ++) {
        for (uint8_t lg2 = 0 ; lg2 <= 15 ; lg2 ++) {
          const CANFrameGenerator f2 (idf2, FrameFormat::standard, lg2, NULL, FrameType::remote, AckSlot::dominant) ;
          const uint32_t bitFlipCount = f1.hammingDistanceFrom (f2) ;
          bitFlipsMultiSet.insert (bitFlipCount) ;
          if (bitFlipCount <= 3) {
            cout << "  " << bitFlipCount
                 << " inversions pour 0x" << hex << idf1 << " DLC " << unsigned (lg1)
                 << " et Ox" << idf2 << " DLC " << unsigned (lg2) << dec << "\n" ;
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
//--- Afficher deux trames
//   const uint32_t idf1 = 0x02B ;
//   const uint8_t lg1 = 5 ;
//   const uint32_t idf2 = 0x03B ;
//   const uint8_t lg2 = 1 ;
//   cout << "  Affichage des trames pour 0x" << hex << idf1 << " DLC " << unsigned (lg1)
//        << " et Ox" << idf2 << " DLC " << unsigned (lg2) << dec << "\n" ;
//   const CANFrameGenerator f1 (idf1, FrameFormat::standard, lg1, NULL, FrameType::remote) ;
//   const CANFrameGenerator f2 (idf2, FrameFormat::standard, lg2, NULL, FrameType::remote) ;
//   const uint32_t length1 = f1.frameLength () ;
//   const uint32_t length2 = f2.frameLength () ;
//   const uint32_t maxLength = (length1 > length2) ? length1 : length2 ;
//   for (uint32_t i=0 ; i < maxLength ; i++) {
//     const bool b1 = f1.bitAtIndex (i) ;
//     const bool b2 = f2.bitAtIndex (i) ;
//     cout << ((b1 != b2) ? "$\\rightarrow$" : "")
//          << " & \\tt " << f1.nameAtIndex (i)
//          << " & \\tt " << f1.bitAtIndex (i)
//          << " & \\tt " << f1.fieldTypeAtIndex (i)
//          << " & \\tt " << i
//          << " & \\tt " << f2.nameAtIndex (i)
//          << " & \\tt " << f2.bitAtIndex (i)
//          << " & \\tt " << f2.fieldTypeAtIndex (i)
//          << " \\\\" << endl ;
//   }
  const chrono::time_point<chrono::system_clock> end = chrono::system_clock::now();
  const chrono::duration<double> elapsed_seconds = end - start;
  cout << "Elapsed time: " << elapsed_seconds.count() << "s\n" ;
  return 0 ;
}

//----------------------------------------------------------------------------------------------------------------------
