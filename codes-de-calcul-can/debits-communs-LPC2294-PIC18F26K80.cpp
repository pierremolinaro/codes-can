//--------------------------------------------------------------------------------------------------
//
//  debits-communs-LPC2294-PIC18F26K80
//
//--------------------------------------------------------------------------------------------------

#include <iostream>     // std::cout
#include <algorithm>    // std::sort
#include <vector>       // std::vector
#include <cstdlib>      // abs

//--------------------------------------------------------------------------------------------------

static const int BIT_RATE_LOWER_BOUND = 500000 ; // kbit/s
static const int BIT_RATE_UPPER_BOUND = 1000000 ; // kbit/s
static const int LPC2294_CAN_CLOCK = 58982400 ; // Hz
static const int PIC18F26K80_CAN_CLOCK = 32000000 ; // Hz
static const double RESULT_MAX_DISTANCE = 1500.0 ; // ppm

//--------------------------------------------------------------------------------------------------

class Configuration {
  public: int mActualBitRate ;
  public: int mBRP ;
  public: int mN ;
} ;

//--------------------------------------------------------------------------------------------------

class Result {
  public: Configuration mLPC2294Configuration ;
  public: Configuration m18F26K80Configuration ;
  public: double mDistancePPM ;

  public: static bool ascending (const Result & inLeft, const Result & inRight) {
    return inLeft.mDistancePPM < inRight.mDistancePPM ;
  }
} ;

//--------------------------------------------------------------------------------------------------

static std::vector <Configuration> computeLPC2294Configurations (void) {
  std::vector <Configuration> configurationArray ;
  for (int brp = 1 ; brp <= 1024 ; brp++) {
    for (int N = 4 ; N <= 25 ; N++) {
      const int actualBitRate = LPC2294_CAN_CLOCK / (brp * N) ;
      if ((actualBitRate >= BIT_RATE_LOWER_BOUND) && (actualBitRate <= BIT_RATE_UPPER_BOUND)) {
        Configuration r ;
        r.mActualBitRate = actualBitRate ;
        r.mBRP = brp ;
        r.mN = N ;
        configurationArray.push_back (r) ;
        std::cout << "  LPC2294: " << actualBitRate << " bit/s, brp: " << brp << ", N: " << N << "\n" ;
      }
    }
  }
  return configurationArray ;
}

//--------------------------------------------------------------------------------------------------

static std::vector <Configuration> compute18F26K80Configurations (void) {
  std::vector <Configuration> configurationArray ;
  for (int brp = 1 ; brp <= 32 ; brp++) {
    for (int N = 5 ; N <= 25 ; N++) {
      const int actualBitRate = PIC18F26K80_CAN_CLOCK / (brp * N) ;
      if ((actualBitRate >= BIT_RATE_LOWER_BOUND) && (actualBitRate <= BIT_RATE_UPPER_BOUND)) {
        Configuration r ;
        r.mActualBitRate = actualBitRate ;
        r.mBRP = brp ;
        r.mN = N ;
        configurationArray.push_back (r) ;
        std::cout << "  PIC18F26K80: " << actualBitRate << " bit/s, brp: " << brp << ", N: " << N << "\n" ;
      }
    }
  }
  return configurationArray ;
}

//--------------------------------------------------------------------------------------------------

static std::vector <Result> selectBestConfigurations (const std::vector <Configuration> & inLpc2294Configurations,
                                                      const std::vector <Configuration> & in26K80Configurations) {
  std::vector <Result> resultArray ;
  for (int i=0 ; i<inLpc2294Configurations.size () ; i++) {
    const Configuration lpc2294Config = inLpc2294Configurations [i] ;
    for (int j=0 ; j<in26K80Configurations.size () ; j++) {
      const Configuration pic18F26K80Config = in26K80Configurations [j] ;
      const double distance = double (abs (lpc2294Config.mActualBitRate - pic18F26K80Config.mActualBitRate)) ;
      const double meanBitRate = double (lpc2294Config.mActualBitRate + pic18F26K80Config.mActualBitRate) / 2.0 ;
      const double distance_ppm = distance * 1000000.0 / meanBitRate ;
      if (distance_ppm <= RESULT_MAX_DISTANCE) {
        Result r ;
        r.mLPC2294Configuration = lpc2294Config ;
        r.m18F26K80Configuration = pic18F26K80Config ;
        r.mDistancePPM = distance_ppm ;
        resultArray.push_back (r) ;
      }
    }
  }
//--- Sort result array
  std::sort (resultArray.begin (), resultArray.end (), Result::ascending) ;
//---
  return resultArray ;
}

//--------------------------------------------------------------------------------------------------

static void displayResults (const std::vector <Result> & inResultArray) {
  for (int i=0 ; i<inResultArray.size () ; i++) {
    const Result r = inResultArray [i] ;
    std::cout << "  " << ((r.mLPC2294Configuration.mActualBitRate + r.m18F26K80Configuration.mActualBitRate) / 2)
              << " bit/s, " << int (r.mDistancePPM)
              << " ppm, LPC2294: " << r.mLPC2294Configuration.mActualBitRate
              << " bit/s, BRP: " << r.mLPC2294Configuration.mBRP
              << ", N: " << r.mLPC2294Configuration.mN
              << ", PIC: " << r.m18F26K80Configuration.mActualBitRate
              << " bit/s, BRP: " << r.m18F26K80Configuration.mBRP
              << ", N: " << r.m18F26K80Configuration.mN << "\n" ;
  }
//--- Latex table
//   for (int i=0 ; i<inResultArray.size () ; i++) {
//     const Result r = inResultArray [i] ;
//     std::cout << "    \\tt " << ((r.mLPC2294Configuration.mActualBitRate + r.m18F26K80Configuration.mActualBitRate) / 2)
//               << " bit/s & \\tt " << int (r.mDistancePPM)
//               << " ppm & \\tt " << r.mLPC2294Configuration.mActualBitRate
//               << " bit/s & \\tt " << r.mLPC2294Configuration.mBRP
//               << " & \\tt " << r.mLPC2294Configuration.mN
//               << " & \\tt " << r.m18F26K80Configuration.mActualBitRate
//               << " bit/s & \\tt " << r.m18F26K80Configuration.mBRP
//               << " & \\tt " << r.m18F26K80Configuration.mN << " \\\\\n" ;
//   }
}

//--------------------------------------------------------------------------------------------------

int main (int argc, const char * argv[]) {
  std::vector <Configuration> lpc2294Configurations = computeLPC2294Configurations () ;
  std::cout << "LPC2294: " << lpc2294Configurations.size () << " configurations\n";
  std::vector <Configuration> pic26K80Configurations = compute18F26K80Configurations () ;
  std::cout << "18F26K80: " << pic26K80Configurations.size () << " configurations\n" ;
  const std::vector <Result> results = selectBestConfigurations (lpc2294Configurations, pic26K80Configurations) ;
  std::cout << results.size () << " results\n" ;
  displayResults (results) ;
  return 0;
}

//--------------------------------------------------------------------------------------------------
