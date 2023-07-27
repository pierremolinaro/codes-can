//··································································································

#include <iostream>

//··································································································

const uint32_t MCP2515_QUARTZ_FREQUENCY = 20 * 1000 * 1000 ;
static const uint32_t REQUESTED_BIT_RATE = 375 * 1000 ;
static const uint32_t TOLERANCE_PPM = 1000 ;

//··································································································
//    compute_BRP_N
//··································································································

static void compute_BRP_N (uint32_t & outBestBRP, uint32_t & outBestN) {
  std::cout << "Best D_BRP, best N for " << REQUESTED_BIT_RATE
            << " bit/s and " << MCP2515_QUARTZ_FREQUENCY << " Hz quartz\n" ;
  const uint32_t F_can = MCP2515_QUARTZ_FREQUENCY / 2 ;
  uint32_t N = 25 ; // N: 5 ... 25
  uint32_t smallestDiff = UINT32_MAX ;
  outBestBRP = 64 ; // Setting for slowest bit rate
  outBestN = 25 ; // Setting for slowest bit rate
  uint32_t D_BRP = F_can / (REQUESTED_BIT_RATE * N) ;
//--- Loop for finding best D_BRP and best N
  while ((N >= 5) && (D_BRP <= 64) && (smallestDiff > 0)) {
  //--- Compute diff using D_BRP (caution: D_BRP should be > 0)
    if (D_BRP > 0) {
      const uint32_t diff = F_can - REQUESTED_BIT_RATE * N * D_BRP ; // diff is always >= 0
      if (diff < smallestDiff) {
        smallestDiff = diff ;
        outBestBRP = D_BRP ;
        outBestN = N ;
        const uint32_t actualBitRate = F_can / (D_BRP * N) ;
        const uint32_t distance = actualBitRate - REQUESTED_BIT_RATE ;
        std::cout << "  Distance: " << distance << " bit/s, BR: " << actualBitRate
                  << " bit/s, D_BRP: " << outBestBRP << ", N: " << outBestN << "\n" ;
      }
    }
  //--- Compute diff using D_BRP+1 (caution: D_BRP+1 should be <= 64)
    if (D_BRP < 64) {
      const uint32_t diff = REQUESTED_BIT_RATE * N * (D_BRP + 1) - F_can ; // diff is always >= 0
      if (diff < smallestDiff) {
        smallestDiff = diff ;
        outBestBRP = D_BRP + 1 ;
        outBestN = N ;
        const uint32_t actualBitRate = F_can / ((D_BRP + 1) * N) ;
        const uint32_t distance = REQUESTED_BIT_RATE - actualBitRate ;
        std::cout << "  Distance: " << distance << " bit/s, BR: " << actualBitRate
                  << " bit/s, D_BRP: " << outBestBRP << ", N: " << outBestN << "\n" ;
      }
    }
  //--- Continue with next value of N
    N -= 1 ;
    D_BRP = F_can / (REQUESTED_BIT_RATE * N) ;
  }
//--- Close enough?
  const uint32_t actualBitRate = F_can / (outBestBRP * outBestN) ;
  const uint32_t diff = (actualBitRate < REQUESTED_BIT_RATE)
    ? (REQUESTED_BIT_RATE - actualBitRate)
    : (actualBitRate - REQUESTED_BIT_RATE)
  ;
  const uint32_t diff_ppm = (diff * 1000 * 1000) / REQUESTED_BIT_RATE ;
  const bool closeEnough = diff_ppm <= TOLERANCE_PPM ;
  std::cout << "Distance ppm: " << diff_ppm
            << ", close enough: " << (closeEnough ? "yes" : "no") << "\n" ;
}

//··································································································
//    computeBitFields
//··································································································

static void computeBitFields (const uint32_t inBestN) { // 5 ... 25
//--- Compute N_PS2
  const uint32_t N_PS2 = (inBestN + 1) / 3 ; // Always 2 <= N_PS2 <= 8
//--- Compute the remaining number of best N once PS2 and SyncSeg are removed
  const uint32_t propSegmentPlusPhaseSegment1 = inBestN - N_PS2 - 1 /* Sync Seg */ ;
//--- Set N_PS1 to half of remaining number
  const uint32_t N_PS1 = propSegmentPlusPhaseSegment1 / 2 ; //Always 1 <= N_PS1 <= 8
//--- Set N_PropSeg to what is left (always 1 <= N_PropSeg <= 8)
  const uint32_t N_PropSeg = propSegmentPlusPhaseSegment1 - N_PS1 ; 
//--- Set SJW (always 1 <= SJW <= 4, and SJW < N_PS2)
  const uint32_t SJW = (N_PS2 <= 4) ? (N_PS2 - 1) : 4 ;
//---
  std::cout << "N_PropSeg: " << N_PropSeg << ", N_PS1: " << N_PS1 
            << ", N_PS2: " << N_PS2
            << ", SJW: " << SJW << "\n" ;
}

//··································································································

int main (void) {
//--- Step 1: compute best D_BRP and best N
  uint32_t bestBRP = 0 ;
  uint32_t bestN = 0 ;
  compute_BRP_N (bestBRP, bestN) ;
//--- Step 2: compute PropSeg, PS1, PS2
  computeBitFields (bestN) ;
//---
  return 0 ;
}

//··································································································
