//
//  main.cpp
//  test-collision-canfd-non-iso-no-data
//
//  Created by Pierre Molinaro on 09/05/2020.
//  Copyright © 2020 Pierre Molinaro. All rights reserved.
//
//--------------------------------------------------------------------------------------------------

#include <iostream>
using namespace std ;

//--------------------------------------------------------------------------------------------------

#include "CANFDFrameGenerator.h"
#include "Multiset.h"

//--------------------------------------------------------------------------------------------------

static CANFDFrameGenerator * gFrameArray [2048 * 2] ;

//--------------------------------------------------------------------------------------------------

int main (int argc, const char * argv[]) {
  uint32_t idx = 0 ;
  for (uint32_t idf = 0 ; idf < 2048 ; idf++) {
    gFrameArray [idx] = new CANFDFrameGenerator (idf,
                                                 FrameFormat::standard,
                                                 BitRateSwitch::no,
                                                 ESI::dominant,
                                                 FrameProtocol::ISO,
                                                 0, nullptr,
                                                 AckSlot::dominant) ;
    idx += 1 ;
    gFrameArray [idx] = new CANFDFrameGenerator (idf,
                                                 FrameFormat::standard,
                                                 BitRateSwitch::no,
                                                 ESI::recessive,
                                                 FrameProtocol::ISO,
                                                 0, nullptr,
                                                 AckSlot::dominant) ;
    idx += 1 ;
  }
  Multiset results ;
  for (uint32_t idx1 = 1 ; idx1 < 4096 ; idx1++) {
    for (uint32_t idx2 = 0 ; idx2 < idx1 ; idx2++) {
      const uint32_t d = gFrameArray [idx1]->hammingDistanceFrom (*gFrameArray [idx2]) ;
      results.insert (d) ;
      if ((d == 6) && (gFrameArray [idx1]->esi () != gFrameArray [idx2]->esi ())) {
        cout << "Distance " << d << ": "
             << hex << "(0x" << gFrameArray [idx1]->identifier () << ", "
             << ((gFrameArray [idx1]->esi () == ESI::dominant) ? "ESI::dominant" : "ESI::recessive")
             << "), 0x" << gFrameArray [idx2]->identifier () << ", "
             << ((gFrameArray [idx2]->esi () == ESI::dominant) ? "ESI::dominant" : "ESI::recessive")
             << ")" << dec << endl ;
      }
    }
  }
  cout << "Hamming distances:\n" ;
  for (auto it=results.cbegin(); it != results.cend(); it++) {
    cout << "  " << it << " --> " << results.count (it) << " tuples\n" ;
  }
  gFrameArray [0x03F]->latexDisplay2 (*gFrameArray [0x07C]) ;
  return 0;
}
//--------------------------------------------------------------------------------------------------
