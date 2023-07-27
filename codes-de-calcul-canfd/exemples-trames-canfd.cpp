//
//  main.cpp
//  exemples-trames-canfd
//
//  Created by Pierre Molinaro on 08/05/2020.
//  Copyright © 2020 Pierre Molinaro. All rights reserved.
//
//--------------------------------------------------------------------------------------------------

#include <iostream>
#include <iomanip>
using namespace std ;

//--------------------------------------------------------------------------------------------------

#include "CANFDFrameGenerator.h"

//--------------------------------------------------------------------------------------------------

static void display (const CANFDFrameGenerator nonisoFrame,
                     const CANFDFrameGenerator isoFrame) {
  for (uint32_t i=0 ; i<nonisoFrame.frameLength () ; i++) {
    cout << "    \\tt " << nonisoFrame.nameAtIndex (i)
         << " & \\tt " << nonisoFrame.bitAtIndex (i)
         << " & \\tt " << bitDurationString (nonisoFrame.bitDurationAtIndex (i))
         << " & \\tt " << nonisoFrame.fieldTypeAtIndex (i)
         << " & \\tt " << i
         << " & \\tt " << isoFrame.nameAtIndex (i)
         << " & \\tt " << isoFrame.bitAtIndex (i)
         << " & \\tt " << bitDurationString (isoFrame.bitDurationAtIndex (i))
         << " & \\tt " << isoFrame.fieldTypeAtIndex (i)
         << " \\\\" << endl ;
  }
  for (uint32_t i=nonisoFrame.frameLength () ; i<isoFrame.frameLength () ; i++) {
    cout << " & & &"
         << " & \\tt " << i
         << " & \\tt " << isoFrame.nameAtIndex (i)
         << " & \\tt " << isoFrame.bitAtIndex (i)
         << " & \\tt " << bitDurationString (isoFrame.bitDurationAtIndex (i))
         << " & \\tt " << isoFrame.fieldTypeAtIndex (i)
         << " \\\\" << endl ;
  }
}

//--------------------------------------------------------------------------------------------------

int main (int argc, const char * argv[]) {
  cout << "Illustration ISO / non ISO 0x7AA, data 0x55 \n" ;
  { const uint32_t idf = 0x7AA ;
    const uint8_t byte = 0x55 ;
    const CANFDFrameGenerator iso (idf,
                                   FrameFormat::standard,
                                   BitRateSwitch::yes,
                                   ESI::dominant,
                                   FrameProtocol::ISO,
                                   1, &byte,
                                   AckSlot::dominant) ;
    const CANFDFrameGenerator noniso (idf,
                                      FrameFormat::standard,
                                      BitRateSwitch::yes,
                                      ESI::dominant,
                                      FrameProtocol::nonISO,
                                      1, &byte,
                                      AckSlot::dominant) ;
    noniso.latexDisplay2 (iso) ;
  }

  cout << "Illustration ISO / non ISO 0x56F \n" ;
  { const uint32_t idf = 0x56F ;
    const CANFDFrameGenerator iso (idf,
                                   FrameFormat::standard,
                                   BitRateSwitch::yes,
                                   ESI::dominant,
                                   FrameProtocol::ISO,
                                   0, nullptr,
                                   AckSlot::dominant) ;
    const CANFDFrameGenerator noniso (idf,
                                      FrameFormat::standard,
                                      BitRateSwitch::yes,
                                      ESI::dominant,
                                      FrameProtocol::nonISO,
                                      0, nullptr,
                                      AckSlot::dominant) ;
    noniso.latexDisplay2 (iso) ;
  }
  return 0;
}

//--------------------------------------------------------------------------------------------------
