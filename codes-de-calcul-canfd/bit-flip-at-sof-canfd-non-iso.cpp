//
//  testInversionsBitSurTramesStandardDonnees.cpp
//  compute-frame-length
//
//  Created by Pierre Molinaro on 30/05/2023.
//  Copyright © 2020 Pierre Molinaro. All rights reserved.
//
//----------------------------------------------------------------------------------------

#include "CANFDFrameGenerator.h"
#include "Multiset.h"

//----------------------------------------------------------------------------------------

#include <iostream>

//----------------------------------------------------------------------------------------

using namespace std ;

//----------------------------------------------------------------------------------------

int main (void) {
  const chrono::time_point<chrono::system_clock> start = chrono::system_clock::now();
  cout << "TEST BIT FLIP AT START OF FRAME" << endl ;
  uint32_t matchCount = 0 ;
  for (uint32_t idf1 = 0 ; idf1 < 2048 ; idf1 ++) {
    const CANFDFrameGenerator f1 (
      idf1,
      FrameFormat::standard,
      BitRateSwitch::no,
      ESI::dominant,
      FrameProtocol::nonISO,
      0, nullptr,
      AckSlot::dominant
    ) ;
    for (uint32_t idf2 = idf1 + 1 ; idf2 < 2048 ; idf2 ++) {
      const CANFDFrameGenerator f2 (
        idf2,
        FrameFormat::standard,
        BitRateSwitch::no,
        ESI::dominant,
        FrameProtocol::nonISO,
        0,
        nullptr,
        AckSlot::dominant
      ) ;
      if (f1.frameLength () == (f2.frameLength () + 1)) {
        bool same = true ;
        for (uint32_t i=0 ; (i<(f2.frameLength () + 10)) && same ; i++) {
          same = f1.bitAtIndex (i+1) == f2.bitAtIndex (i) ;
        }
        if (same) {
          matchCount += 1 ;
          cout << "f1 0x" << hex << idf1 << " (length " << dec << f1.frameLength ()
                << "), f2 0x" << hex << idf2 << " (length " << dec << f2.frameLength () << ")\n" ;
        }
      }
      if ((f1.frameLength () + 1) == f2.frameLength ()) {
        bool same = true ;
        for (uint32_t i=0 ; (i<(f1.frameLength () + 10)) && same ; i++) {
          same = f1.bitAtIndex (i) == f2.bitAtIndex (i+1) ;
        }
        if (same) {
          matchCount += 1 ;
          cout << "f1 0x" << hex << idf1 << " (length " << dec << f1.frameLength ()
                << "), f2 0x" << hex << idf2 << " (length " << dec << f2.frameLength () << ")\n" ;
        }
      }
    }
  }
//--- Afficher le nombre de couples trouvés
  cout << matchCount << " found\n" ;
  const chrono::time_point<chrono::system_clock> end = chrono::system_clock::now() ;
  const chrono::duration<double> elapsed_seconds = end - start ;
  cout << "Elapsed time: " << elapsed_seconds.count() << "s\n" ;
//--- Afficher un couple représentatif
  const CANFDFrameGenerator f1 (
    0x0,
    FrameFormat::standard,
    BitRateSwitch::no,
    ESI::dominant,
    FrameProtocol::nonISO,
    0, nullptr,
    AckSlot::dominant
  ) ;
  const CANFDFrameGenerator f2 (
    0x80,
    FrameFormat::standard,
    BitRateSwitch::no,
    ESI::dominant,
    FrameProtocol::nonISO,
    0, nullptr,
    AckSlot::dominant
  ) ;
  f1.latexDisplay2 (f2, 1) ;
//---
  return 0 ;
}

//----------------------------------------------------------------------------------------
