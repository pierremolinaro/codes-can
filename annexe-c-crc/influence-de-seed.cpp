//
//  influence-de-seed
//
//  Created by Pierre Molinaro on 16/11/2021.
//  Copyright © 2021 Pierre Molinaro. All rights reserved.
//
//-------------------------------------------------------------------------------------------------

#include <iostream>

//-------------------------------------------------------------------------------------------------

static void enterBitInCRC8 (const bool inBit, uint8_t & ioCRC, const uint8_t inPOLY) {
  const bool msb = (ioCRC & 0x80) != 0 ;
  ioCRC <<= 1 ;
  if (inBit ^ msb) {
    ioCRC ^= inPOLY ;
  }
}

//-------------------------------------------------------------------------------------------------

int main (int argc, const char * argv []) {
  const uint8_t POLY  = 0xF3 ;
  const uint8_t SEED1 = 0x47 ;
  const uint8_t SEED2 = 0x01 ;
  uint8_t crc0 = 0 ;
  uint8_t crc1 = SEED1 ;
  uint8_t crc2 = SEED2 ;
  std::string s ;
  for (size_t nombreBits = 1 ; nombreBits <= 16 ; nombreBits++) {
    s.insert (0, "0") ;
    if ((nombreBits % 4) == 0) {
      s.insert (0, " ") ;
    }
    enterBitInCRC8 (false, crc0, POLY) ;
    enterBitInCRC8 (false, crc1, POLY) ;
    enterBitInCRC8 (false, crc2, POLY) ;
    printf ("%lu bits à 0 : \\tt %s & \\tt 0x%.2X & \\tt 0x%.2X & \\tt 0x%.2X \\\\\n",
            nombreBits, s.c_str (), crc0, crc1, crc2) ;
  }
  return 0 ;
}

//-------------------------------------------------------------------------------------------------
