//··································································································

#include <iostream>

//··································································································
//    computeBitFields
//··································································································

static void computeAndCheckBitFields (const uint32_t inN) { // 5 ... 25
//--- Compute N_PS2
  const uint32_t N_PS2 = (inN + 1) / 3 ; // Always 2 <= N_PS2 <= 8
//  const uint32_t N_PS2 = 1 + 2 * bestTQCount / 7 ; // Always 2 <= PS2 <= 8
//--- Compute the remaining number of best N once PS2 and SyncSeg are removed
  const uint32_t propSegmentPlusPhaseSegment1 = inN - N_PS2 - 1 /* Sync Seg */ ;
//--- Set N_PS1 to half of remaining number
  const uint32_t N_PS1 = propSegmentPlusPhaseSegment1 / 2 ; //Always 1 <= N_PS1 <= 8
//--- Set N_PropSeg to what is left (always 1 <= N_PropSeg <= 8)
  const uint32_t N_PropSeg = propSegmentPlusPhaseSegment1 - N_PS1 ; 
//--- Set SJW (always 1 <= SJW <= 4, and SJW < N_PS2)
  const uint32_t SJW = (N_PS2 <= 4) ? (N_PS2 - 1) : 4 ;
//--- Set SP (sampling point)
  const uint32_t SP = 100 * (inN - N_PS2) / inN ;
//--- Set SP (sampling point)
  const uint32_t Df = (1000 * 1000 * SJW) / (20 * inN + SJW) ;
//---
  std::cout << "    \\tt " << inN
            << " & \\tt " << N_PropSeg
            << " & \\tt " << N_PS1 
            << " & \\tt " << N_PS2
            << " & \\tt " << SJW
            << " & \\tt " << SP
            << " \\% & \\tt " << Df
            << " ppm \\\\\n" ;
//--- Check
  if (N_PropSeg == 0) {
    std::cout << "Error, N_PropSeg == 0\n" ;
    exit (1) ;
  }
  if (N_PropSeg > 8) {
    std::cout << "Error, N_PropSeg > 8\n" ;
    exit (1) ;
  }
  if (N_PS1 == 0) {
    std::cout << "Error, N_PS1 == 0\n" ;
    exit (1) ;
  }
  if (N_PS1 > 8) {
    std::cout << "Error, N_PS1 > 8\n" ;
    exit (1) ;
  }
  if (N_PS2 == 0) {
    std::cout << "Error, N_PS2 == 0\n" ;
    exit (1) ;
  }
  if (N_PS2 > 8) {
    std::cout << "Error, N_PS2 > 8\n" ;
    exit (1) ;
  }
  if (N_PS2 > (N_PropSeg + N_PS1)) {
    std::cout << "Error, N_PS2 > (N_PropSeg + N_PS1)\n" ;
    exit (1) ;
  }
  if (SJW == 0) {
    std::cout << "Error, SJW == 0\n" ;
    exit (1) ;
  }
  if (SJW > 4) {
    std::cout << "Error, SJW > 4\n" ;
    exit (1) ;
  }
  if (SJW >= N_PS2) {
    std::cout << "Error, SJW >= N_PS2\n" ;
    exit (1) ;
  }
}

//··································································································

int main (void) {
  for (uint32_t N=5 ; N <= 25 ; N++) {
    computeAndCheckBitFields (N) ;
  }
  return 0 ;
}

//··································································································
