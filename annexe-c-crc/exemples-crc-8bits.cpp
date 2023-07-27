//
//  main.cpp
//  exemples-crc-8bits
//
//  Created by Pierre Molinaro on 15/07/2020.
//  Copyright © 2020 Pierre Molinaro. All rights reserved.
//
//-------------------------------------------------------------------------------------------------

#include <iostream>
#include <iomanip>
using namespace std ;

//-------------------------------------------------------------------------------------------------

void enterBitInCRC8 (const bool inBit, uint8_t & ioCRC, const uint8_t inPOLY) {
  const bool msb = (ioCRC & 0x80) != 0 ;
  const bool crc_nxt = inBit ^ msb ;
  ioCRC <<= 1 ;
  if (crc_nxt) {
    ioCRC ^= inPOLY ;
  }
}

//-------------------------------------------------------------------------------------------------

static inline void enterByteInCRC8 (const uint16_t inData, uint8_t & ioCRC, const uint8_t inPOLY) {
  uint16_t data = inData ;
  for (uint8_t i=0 ; i<16 ; i++) {
    const bool bit = (data & 0x8000U) != 0 ;
    data <<= 1 ;
    enterBitInCRC8 (bit, ioCRC, inPOLY) ;
  }
}

//-------------------------------------------------------------------------------------------------

uint8_t computeCRC (const uint8_t inSEED, const uint8_t inPOLY, const uint16_t inData) {
  uint8_t crc = inSEED ;
  enterByteInCRC8 (inData, crc, inPOLY) ;
  return crc ;
}

//-------------------------------------------------------------------------------------------------

void printLineForByte (const uint16_t inData) {
  printf ("      \\tt 0x%.4X", inData) ;
  printf (" & \\tt 0x%.2X", computeCRC (0x00, 0xCF, inData)) ;
  printf (" & \\tt 0x%.2X", computeCRC (0x47, 0xCF, inData)) ;
  printf (" & \\tt 0x%.2X", computeCRC (0xD0, 0xCF, inData)) ;
  printf (" & \\tt 0x%.2X", computeCRC (0x00, 0x07, inData)) ;
  printf (" & \\tt 0x%.2X", computeCRC (0x00, 0x08, inData)) ;
  printf (" & \\tt 0x%.2X", computeCRC (0x00, 0xCF, inData)) ;
  printf (" & \\tt 0x%.2X", computeCRC (0x00, 0xD7, inData)) ;
  printf (" \\\\\n") ;
}

//-------------------------------------------------------------------------------------------------

int main (int argc, const char * argv[]) {
  for (uint16_t data=0 ; data<=0x08 ; data++) {
    printLineForByte (data) ;
  }
  printLineForByte (0x10) ;
  printLineForByte (0x20) ;
  printLineForByte (0x40) ;
  printLineForByte (0x80) ;
  printLineForByte (0x100) ;
  printLineForByte (0x200) ;
  printLineForByte (0x400) ;
  printLineForByte (0x800) ;
  printLineForByte (0x1000) ;
  printLineForByte (0x2000) ;
  printLineForByte (0x4000) ;
  printLineForByte (0x8000) ;
  return 0;
}

//-------------------------------------------------------------------------------------------------
