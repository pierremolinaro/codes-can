//
//  main.cpp
//  decomposition-bit-canfd
//
//  Created by Pierre Molinaro on 26/07/2023.
//
//----------------------------------------------------------------------------------------

#include <iostream>

//----------------------------------------------------------------------------------------

static const uint32_t FDCAN_CLOCK_FREQUENCY = 168 * 1000 * 1000 ; // In Hz

//----------------------------------------------------------------------------------------

static const uint32_t MIN_ARBITRATION_PS1 = 2 ;
static const uint32_t MAX_ARBITRATION_PS1 = 256 ;
static const uint32_t MIN_ARBITRATION_PS2 = 1 ;
static const uint32_t MAX_ARBITRATION_PS2 = 128 ;

static const uint32_t MAX_ARBITRATION_SJW = MAX_ARBITRATION_PS2 ;

static const uint32_t MAX_ARBITRATION_TQ_COUNT = 1 + MAX_ARBITRATION_PS1 + MAX_ARBITRATION_PS2 ;

//----------------------------------------------------------------------------------------

static const uint32_t MIN_DATA_PS1 = 2 ;
static const uint32_t MAX_DATA_PS1 = 32 ;
static const uint32_t MIN_DATA_PS2 = 1 ;
static const uint32_t MAX_DATA_PS2 = 16 ;

static const uint32_t MAX_DATA_SJW = MAX_DATA_PS2 ;

static const uint32_t MIN_DATA_TQ_COUNT = 1 + MIN_DATA_PS1 + MIN_DATA_PS2 ;
static const uint32_t MAX_DATA_TQ_COUNT = 1 + MAX_DATA_PS1 + MAX_DATA_PS2 ;

//----------------------------------------------------------------------------------------

static const uint32_t MAX_BRP = 32 ;

//----------------------------------------------------------------------------------------

class CANFD_Settings {

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //    Constructor
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  public: CANFD_Settings (const uint32_t inDesiredArbitrationBitRate,
                          const uint32_t inDesiredArbitrationSamplePoint,
                          const uint32_t inDataBitRateFactor,
                          const uint32_t inDesiredDataSamplePoint,
                          const uint32_t inTolerancePPM = 1000) ;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //    Properties
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//--- CAN FD bit timing
  public: const uint32_t mDesiredArbitrationBitRate ; // In bit/s
  public: const uint32_t mDataBitRateFactor ;
//--- bitrate prescaler is common to arbitration bitrate and data bitrate
  public: uint32_t mBitRatePrescaler = MAX_BRP ;
//--- Arbitration segments
  public: uint32_t mArbitrationPhaseSegment1 = MAX_ARBITRATION_PS1 ;
  public: uint32_t mArbitrationPhaseSegment2 = MAX_ARBITRATION_PS2 ;
  public: uint32_t mArbitrationSJW = MAX_ARBITRATION_SJW ;
//--- Data segments
  public: uint32_t mDataPhaseSegment1 = MAX_DATA_PS1 ;
  public: uint32_t mDataPhaseSegment2 = MAX_DATA_PS2 ;
  public: uint32_t mDataSJW = MAX_DATA_SJW ;
//--- Transceiver Delay Compensation
  public: uint32_t mTransceiverDelayCompensation = 0 ; // 0 ... 127

  public: bool mTripleSampling = false ; // true -> triple sampling, false -> single
  protected: bool mActualBitRateClosedToDesiredBitRate = false ;

  public: uint32_t checkBitSettingConsistency (void) const ;
  public: bool actualBitRateClosedToDesiredBitRate (void) const {
    return mActualBitRateClosedToDesiredBitRate ;
  }
} ;

//----------------------------------------------------------------------------------------

CANFD_Settings::CANFD_Settings (const uint32_t inDesiredArbitrationBitRate,
                                const uint32_t inDesiredArbitrationSamplePoint,
                                const uint32_t inDataBitRateFactor,
                                const uint32_t inDesiredDataSamplePoint,
                                const uint32_t inTolerancePPM) :
mDesiredArbitrationBitRate (inDesiredArbitrationBitRate),
mDataBitRateFactor (inDataBitRateFactor) {
//---------------------------------------------- Configure CANFD bit decomposition
  const uint32_t dataBitRate = inDesiredArbitrationBitRate * inDataBitRateFactor ;
  uint32_t dataTQCount = std::min (MAX_DATA_TQ_COUNT, MAX_ARBITRATION_TQ_COUNT / inDataBitRateFactor) ;
  uint32_t smallestError = UINT32_MAX ;
  uint32_t bestBRP = MAX_BRP ; // Setting for slowest bitrate
  uint32_t bestDataTQCount = dataTQCount ; // Setting for slowest bitrate
  uint32_t BRP = FDCAN_CLOCK_FREQUENCY / (dataBitRate * dataTQCount) ;
//--- Loop for finding best BRP and best TQCount
  while ((smallestError > 0) && (dataTQCount >= MIN_DATA_TQ_COUNT) && (BRP <= MAX_BRP)) {
  //--- Compute error using BRP (caution: BRP should be > 0)
    if (BRP > 0) {
      const uint32_t error = FDCAN_CLOCK_FREQUENCY - dataBitRate * dataTQCount * BRP ; // error is always >= 0
      if (error < smallestError) {
        smallestError = error ;
        bestBRP = BRP ;
        bestDataTQCount = dataTQCount ;
      }
    }
  //--- Compute error using BRP+1 (caution: BRP+1 should be <= MAX_BRP)
    if (BRP < MAX_BRP) {
      const uint32_t error = dataBitRate * dataTQCount * (BRP + 1) - FDCAN_CLOCK_FREQUENCY ; // error is always >= 0
      if (error < smallestError) {
        smallestError = error ;
        bestBRP = BRP + 1 ;
        bestDataTQCount = dataTQCount ;
      }
    }
  //--- Continue with next value of TQCount
    dataTQCount -= 1 ;
    BRP = FDCAN_CLOCK_FREQUENCY / (dataBitRate * dataTQCount) ;
  }
//-------------------------- Set the BRP
  mBitRatePrescaler = bestBRP ;
//-------------------------- Set Data segment lengthes
//--- Compute PS1
  mDataPhaseSegment1 = (inDesiredDataSamplePoint * bestDataTQCount) / 100 - 1 ;
  if (mDataPhaseSegment1 > MAX_DATA_PS1) {
    mDataPhaseSegment1 = MAX_DATA_PS1 ; // Always 1 <= PS1 <= MAX_DATA_PS1
  }
//--- Set PS2 to remaining TQCount
  mDataPhaseSegment2 = bestDataTQCount - mDataPhaseSegment1 - 1 ;
  if (mDataPhaseSegment2 > MAX_DATA_PS2) {
    mDataPhaseSegment1 -= mDataPhaseSegment2 - MAX_DATA_PS2 ;
    mDataPhaseSegment2 = MAX_DATA_PS2 ;
  }
//--- Set RJW to PS2
  mDataSJW = mDataPhaseSegment2 ;
//-------------------------- Set TDCO
  mTransceiverDelayCompensation = (dataBitRate <= 1000000)
    ? 0
    : (bestBRP * bestDataTQCount / 2)
  ;
//-------------------------- Set Arbitration segment lengthes
  const uint32_t bestArbitrationTQCount = bestDataTQCount * uint32_t (inDataBitRateFactor) ;
//--- Compute PS1
  mArbitrationPhaseSegment1 = (inDesiredArbitrationSamplePoint * bestArbitrationTQCount) / 100 - 1 ;
  if (mArbitrationPhaseSegment1 > MAX_ARBITRATION_PS1) {
    mArbitrationPhaseSegment1 = MAX_ARBITRATION_PS1 ; // Always 1 <= PS1 <= 256
  }
//--- Set PS2 to remaining TQCount
  mArbitrationPhaseSegment2 = bestArbitrationTQCount - mArbitrationPhaseSegment1 - 1 ;
  if (mArbitrationPhaseSegment2 > MAX_ARBITRATION_PS2) {
    mArbitrationPhaseSegment1 -= mArbitrationPhaseSegment2 - MAX_ARBITRATION_PS2 ;
    mArbitrationPhaseSegment2 = MAX_ARBITRATION_PS2 ;
  }
//--- Set RJW to PS2
  mArbitrationSJW = mArbitrationPhaseSegment2 ;
//--- Triple sampling ?
  mTripleSampling = (mDesiredArbitrationBitRate <= 125000) && (mArbitrationPhaseSegment1 >= 2) ;
//-------------------------- Final check of the configuration
  const uint32_t W = bestArbitrationTQCount * mDesiredArbitrationBitRate * mBitRatePrescaler ;
  const uint64_t diff = (FDCAN_CLOCK_FREQUENCY > W) ? (FDCAN_CLOCK_FREQUENCY - W) : (W - FDCAN_CLOCK_FREQUENCY) ;
  const uint64_t ppm = uint64_t (1000 * 1000) ;
  mActualBitRateClosedToDesiredBitRate = (diff * ppm) <= (uint64_t (W) * inTolerancePPM) ;
} ;

//----------------------------------------------------------------------------------------

static const uint32_t kBitRatePrescalerIsZero                       = 1U <<  0 ;
static const uint32_t kBitRatePrescalerIsTooLarge                   = 1U <<  1 ;
static const uint32_t kArbitrationPhaseSegment1TooSmall             = 1U <<  2 ;
static const uint32_t kArbitrationPhaseSegment1IsTooLarge           = 1U <<  3 ;
static const uint32_t kArbitrationPhaseSegment2IsTooSmall           = 1U <<  4 ;
static const uint32_t kArbitrationPhaseSegment2IsTooLarge           = 1U <<  5 ;
static const uint32_t kArbitrationSJWIsZero                         = 1U <<  6 ;
static const uint32_t kArbitrationSJWIsLooLarge                     = 1U <<  7 ;
static const uint32_t kArbitrationSJWIsGreaterThanPhaseSegment2     = 1U <<  8 ;
static const uint32_t kArbitrationPhaseSegment1Is1AndTripleSampling = 1U <<  9 ;
static const uint32_t kDataPhaseSegment1IsTooSmall                  = 1U << 10 ;
static const uint32_t kDataPhaseSegment1IsTooLarge                  = 1U << 11 ;
static const uint32_t kDataPhaseSegment2IsTooSmall                  = 1U << 12 ;
static const uint32_t kDataPhaseSegment2IsTooLarge                  = 1U << 13 ;
static const uint32_t kDataSJWIsZero                                = 1U << 14 ;
static const uint32_t kDataSJWIsTooLarge                            = 1U << 15 ;
static const uint32_t kDataSJWIsGreaterThanPhaseSegment2            = 1U << 16 ;

//----------------------------------------------------------------------------------------

uint32_t CANFD_Settings::checkBitSettingConsistency (void) const {
  uint32_t errorCode = 0 ; // Means no error
  if (mBitRatePrescaler == 0) {
    errorCode |= kBitRatePrescalerIsZero ;
  }else if (mBitRatePrescaler > MAX_BRP) {
    errorCode |= kBitRatePrescalerIsTooLarge ;
  }
  if (mArbitrationPhaseSegment1 < MIN_ARBITRATION_PS1) {
    errorCode |= kArbitrationPhaseSegment1TooSmall ;
  }else if ((mArbitrationPhaseSegment1 == 1) && mTripleSampling) {
    errorCode |= kArbitrationPhaseSegment1Is1AndTripleSampling ;
  }else if (mArbitrationPhaseSegment1 > MAX_ARBITRATION_PS1) {
    errorCode |= kArbitrationPhaseSegment1IsTooLarge ;
  }
  if (mArbitrationPhaseSegment2 < MIN_ARBITRATION_PS2) {
    errorCode |= kArbitrationPhaseSegment2IsTooSmall ;
  }else if (mArbitrationPhaseSegment2 > MAX_ARBITRATION_PS2) {
    errorCode |= kArbitrationPhaseSegment2IsTooLarge ;
  }
  if (mArbitrationSJW == 0) {
    errorCode |= kArbitrationSJWIsZero ;
  }else if (mArbitrationSJW > MAX_ARBITRATION_SJW) {
    errorCode |= kArbitrationSJWIsLooLarge ;
  }
  if (mArbitrationSJW > mArbitrationPhaseSegment2) {
    errorCode |= kArbitrationSJWIsGreaterThanPhaseSegment2 ;
  }
  if (mDataPhaseSegment1 < MIN_DATA_PS1) {
    errorCode |= kDataPhaseSegment1IsTooSmall ;
  }else if (mDataPhaseSegment1 > MAX_DATA_PS1) {
    errorCode |= kDataPhaseSegment1IsTooLarge ;
  }
  if (mDataPhaseSegment2 < MIN_DATA_PS2) {
    errorCode |= kDataPhaseSegment2IsTooSmall ;
  }else if (mDataPhaseSegment2 > MAX_DATA_PS2) {
    errorCode |= kDataPhaseSegment2IsTooLarge ;
  }
  if (mDataSJW == 0) {
    errorCode |= kDataSJWIsZero ;
  }else if (mDataSJW > MAX_DATA_SJW) {
    errorCode |= kDataSJWIsTooLarge ;
  }
  if (mDataSJW > mDataPhaseSegment2) {
    errorCode |= kDataSJWIsGreaterThanPhaseSegment2 ;
  }
  return errorCode ;
}

//----------------------------------------------------------------------------------------

int main (int argc, const char * argv[]) {
  for (uint32_t arbitrationBitRate = 1 ; arbitrationBitRate <= 1000000 ; arbitrationBitRate++) {
    for (uint32_t dataBitRateFactor = 1 ; dataBitRateFactor <= 10 ; dataBitRateFactor++) {
      for (uint32_t sp_a = 50 ; sp_a <= 90 ; sp_a++) {
        for (uint32_t sp_d = 50 ; sp_d <= 90 ; sp_d++) {
          const CANFD_Settings settings (arbitrationBitRate, sp_a, dataBitRateFactor, sp_d) ;
          const uint32_t errorCode = settings.checkBitSettingConsistency () ;
          if (errorCode != 0) {
            std::cout << "Error 0x" << std::hex << errorCode << std::dec << "\n" ;
            std::cout << "  Arbitration bit rate : " << arbitrationBitRate << "\n" ;
            std::cout << "  Arbitration Sample Point : " << sp_a << "\n" ;
            std::cout << "  Data bit rate : " << (arbitrationBitRate * dataBitRateFactor) << "\n" ;
            std::cout << "  Data Sample Point : " << sp_d << "\n" ;
            std::cout << "  BRP : " << settings.mBitRatePrescaler << "\n" ;
            std::cout << "  PS1_A : " << settings.mArbitrationPhaseSegment1 << "\n" ;
            std::cout << "  PS2_A : " << settings.mArbitrationPhaseSegment2 << "\n" ;
            std::cout << "  SJW_A : " << settings.mArbitrationSJW << "\n" ;
            std::cout << "  PS1_D : " << settings.mDataPhaseSegment1 << "\n" ;
            std::cout << "  PS2_D : " << settings.mDataPhaseSegment2 << "\n" ;
            std::cout << "  SJW_D : " << settings.mDataSJW << "\n" ;
            std::exit (1) ;
          }
        }
      }
    }
  }
  std::cout << "Done.\n";
  return 0;
}

//----------------------------------------------------------------------------------------
