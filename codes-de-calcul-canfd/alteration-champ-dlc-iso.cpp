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

//--------------------------------------------------------------------------------------------------

int main (int argc, const char * argv[]) {
 const CANFDFrameGenerator trameZeroData (0x555,
                                          FrameFormat::standard,
                                          BitRateSwitch::no,
                                          ESI::recessive,
                                          FrameProtocol::ISO,
                                          0, nullptr,
                                          AckSlot::dominant) ;
 const uint8_t datas [8] = {0x87, 0x2E, 0xCB, 0xB7, 0xFF, 0, 0, 0} ;
 const CANFDFrameGenerator trame8Datas (0x555,
                                        FrameFormat::standard,
                                        BitRateSwitch::no,
                                        ESI::recessive,
                                        FrameProtocol::ISO,
                                        8, datas,
                                        AckSlot::dominant) ;

  trameZeroData.latexDisplay2 (trame8Datas) ;
  cout << "Hamming distance: " << trame8Datas.hammingDistanceFrom (trameZeroData) << endl ;
  return 0;
}

//--------------------------------------------------------------------------------------------------
