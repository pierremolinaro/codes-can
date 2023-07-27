//··································································································

#include <iostream>

//··································································································

static const uint32_t REQUESTED_BIT_RATE = 615 * 1000 ;

//··································································································
//    compute_BRP_N
//··································································································

static void compute_BRP_N (const uint32_t inQuartzFrequency,
                           uint32_t & outBestBRP,
                           uint32_t & outBestN,
                           uint32_t & outActualBitRate,
                           uint32_t & outDiff) {
//   std::cout << "Best D_BRP, best N for " << REQUESTED_BIT_RATE
//             << " bit/s and " << inQuartzFrequency << " Hz quartz\n" ;
  const uint32_t F_can = inQuartzFrequency / 2 ;
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
      }
    }
  //--- Compute diff using D_BRP+1 (caution: D_BRP+1 should be <= 64)
    if (D_BRP < 64) {
      const uint32_t diff = REQUESTED_BIT_RATE * N * (D_BRP + 1) - F_can ; // diff is always >= 0
      if (diff < smallestDiff) {
        smallestDiff = diff ;
        outBestBRP = D_BRP + 1 ;
        outBestN = N ;
      }
    }
  //--- Continue with next value of N
    N -= 1 ;
    D_BRP = F_can / (REQUESTED_BIT_RATE * N) ;
  }
//--- Close enough?
  outActualBitRate = F_can / (outBestBRP * outBestN) ;
  outDiff = (outActualBitRate < REQUESTED_BIT_RATE)
    ? (REQUESTED_BIT_RATE - outActualBitRate)
    : (outActualBitRate - REQUESTED_BIT_RATE)
  ;
}

//··································································································

int main (void) {
  uint32_t bestBRP = 0 ;
  uint32_t bestN = 0 ;
  uint32_t bestQuartzMHz = 0 ;
  uint32_t bestDiff = UINT32_MAX ;
  uint32_t bestActualBitRate = UINT32_MAX ;
  for (uint32_t quartzMHz = 1 ; quartzMHz <= 20 ; quartzMHz ++) {
    uint32_t BRP = 0 ;
    uint32_t N = 0 ;
    uint32_t actualBitRate = 0 ;
    uint32_t diff = UINT32_MAX ;
    compute_BRP_N (quartzMHz * 1000 * 1000, BRP, N, actualBitRate, diff) ;
    if (diff <= bestDiff) {
      bestDiff = diff ;
      bestBRP = BRP ;
      bestN = N ;
      bestActualBitRate = actualBitRate ;
      bestQuartzMHz = quartzMHz ;
      std::cout << "Diff: " << diff << " bit/s, bit rate: "
                << actualBitRate << " bit/s, quartz: "
                << quartzMHz << " MHz, BRP: "
                << BRP << ", N: " << N << "\n" ;
    }
  }
//--- 
  const uint32_t diff_ppm = (bestDiff * 1000 * 1000) / REQUESTED_BIT_RATE ;
  std::cout << "Best solution diff ppm: " << diff_ppm << "\n" ;
//---
  return 0 ;
}

//··································································································
