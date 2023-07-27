//
//  meilleurs-crc-8bits
//
//  Created by Pierre Molinaro on 16/11/2021.
//  Copyright © 2021 Pierre Molinaro. All rights reserved.
//
//-------------------------------------------------------------------------------------------------

#include <iostream>
#include <vector>
#include <iomanip>

//-------------------------------------------------------------------------------------------------

static void enterBitInCRC8 (const bool inBit, uint8_t & ioCRC, const uint8_t inPOLY) {
  const bool msb = (ioCRC & 0x80) != 0 ;
  ioCRC <<= 1 ;
  if (inBit ^ msb) {
    ioCRC ^= inPOLY ;
  }
}

//-------------------------------------------------------------------------------------------------

static uint8_t crcForBitIndex (const size_t inBitIndex, const uint8_t inPOLY) {
  uint8_t crc = 0 ;
  enterBitInCRC8 (true, crc, inPOLY) ;
  for (size_t i = 0 ; i < inBitIndex ; i++) {
    enterBitInCRC8 (false, crc, inPOLY) ;
  }
  return crc ;
}

//-------------------------------------------------------------------------------------------------

static uint32_t hammingDistance (const std::vector <uint32_t> & inCRCarray) {
  uint32_t result = UINT32_MAX ;
  for (size_t i = 0 ; i < inCRCarray.size () ; i++) {
    for (size_t j = i + 1 ; j < inCRCarray.size () ; j++) {
      uint32_t v = inCRCarray [i] ^ inCRCarray [j] ;
      const uint32_t d = std::__popcount (v) ;
 //     printf ("   %lu:0x%.2X, %lu:0x%.2X -> %u\n", i, inCRCarray [i], j, inCRCarray [j], d) ;
      if (d < result) {
        result = d ;
      }
    }
  }
  return result ;
}

//-------------------------------------------------------------------------------------------------

static void computeBestCRCForLength (const size_t inLength) {
  std::vector <uint8_t> bestPolys ;
  size_t bestHD = 0 ;
  for (uint8_t i = 1 ; i <= 253 ; i += 2) {
    const uint8_t poly = i + 2 ;
    std::vector <uint32_t> crcArray ;
//    printf ("POLY 0x%.2X\n", poly) ;
    size_t m = 0 ;
    for (size_t j = 0 ; j < inLength ; j++) {
      uint8_t crc = 0 ;
      for (size_t bitIndex = inLength ; bitIndex > 0 ; bitIndex --) {
        enterBitInCRC8 ((m & (1 << (bitIndex - 1))) != 0, crc, poly) ;
      }
 //     printf ("   0x%.2lX\n", (m << 8) | crc) ;
      m = (m << 1) | 1 ;
      crcArray.push_back ((m << 8) | crc) ;
    }
//     for (size_t bitIndex = 0 ; bitIndex < inLength ; bitIndex ++) {
//       crcArray.push_back (crcForBitIndex (bitIndex, poly)) ;
//     }
    const uint32_t d = hammingDistance (crcArray) ;
    if (d > bestHD) {
      bestHD = d ;
      bestPolys.clear () ;
      bestPolys.push_back (poly) ;
    }else if (d == bestHD) {
      bestPolys.push_back (poly) ;
    }
  }
  printf ("%lu bits: HD=%lu for", inLength, bestHD) ;
  for (size_t i = 0 ; i < bestPolys.size () ; i++) {
    printf (" 0x%.2X", bestPolys [i]) ;
  }
  printf ("\n") ;
}

// static void computeBestCRCForLength (const size_t inLength) {
//   std::vector <uint8_t> bestPolys ;
//   size_t bestHD = 0 ;
//   for (uint8_t i = 1 ; i <= 253 ; i += 2) {
//     const uint8_t poly = i + 2 ;
//     std::vector <uint32_t> crcArray ;
// //    printf ("POLY 0x%.2X\n", poly) ;
//     for (size_t m = 0 ; m < (1 << inLength) ; m++) {
//       uint8_t crc = 0 ;
//       for (size_t bitIndex = inLength ; bitIndex > 0 ; bitIndex --) {
//         enterBitInCRC8 ((m & (1 << (bitIndex - 1))) != 0, crc, poly) ;
//       }
//  //     printf ("   0x%.2lX\n", (m << 8) | crc) ;
//       crcArray.push_back ((m << 8) | crc) ;
//     }
// //     for (size_t bitIndex = 0 ; bitIndex < inLength ; bitIndex ++) {
// //       crcArray.push_back (crcForBitIndex (bitIndex, poly)) ;
// //     }
//     const uint32_t d = hammingDistance (crcArray) ;
//     if (d > bestHD) {
//       bestHD = d ;
//       bestPolys.clear () ;
//       bestPolys.push_back (poly) ;
//     }else if (d == bestHD) {
//       bestPolys.push_back (poly) ;
//     }
//   }
//   printf ("%lu bits: HD=%lu for", inLength, bestHD) ;
//   for (size_t i = 0 ; i < bestPolys.size () ; i++) {
//     printf (" 0x%.2X", bestPolys [i]) ;
//   }
//   printf ("\n") ;
// }

//-------------------------------------------------------------------------------------------------

int main (int argc, const char * argv[]) {
  for (size_t i = 2 ; i <= 16 ; i ++) {
    computeBestCRCForLength (i) ;
  }
  return 0 ;
}

//-------------------------------------------------------------------------------------------------
