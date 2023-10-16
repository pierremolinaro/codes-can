//
//  main.cpp
//  calcul-debit-utile-canfd
//
//  Created by Pierre Molinaro on 16/10/2023.
//
//--------------------------------------------------------------------------------------------------

#include <iostream>

//--------------------------------------------------------------------------------------------------

class Bounds {
  public: const uint32_t mStandardMin ;
  public: const uint32_t mStandardMax ;
  public: const uint32_t mExtendedMin ;
  public: const uint32_t mExtendedMax ;

  public: Bounds (const uint32_t inStandardMin,
                  const uint32_t inStandardMax,
                  const uint32_t inExtendedMin,
                  const uint32_t inExtendedMax) :
  mStandardMin (inStandardMin),
  mStandardMax (inStandardMax),
  mExtendedMin (inExtendedMin),
  mExtendedMax (inExtendedMax) {
  }

} ;

//--------------------------------------------------------------------------------------------------

std::string separatedString (const uint64_t inValue) {
  std::string result ;
  const uint64_t milliers =  inValue / 1000 ;
  if (milliers > 0) {
    result.append (std::to_string (milliers)) ;
    result.append (" ") ;
    result.append (std::to_string ((inValue / 100) % 10)) ;
    result.append (std::to_string ((inValue / 10) % 10)) ;
    result.append (std::to_string (inValue % 10)) ;
  }else{
    result.append (std::to_string (inValue % 1000)) ;
  }
  return result ;
}

//--------------------------------------------------------------------------------------------------

static const uint64_t ARBITRATION_BIT_RATE = 1'000'000 ; // bit/s
static const uint64_t DATA_BIT_RATE = 8'000'000 ; // bit/s

//--------------------------------------------------------------------------------------------------

static const uint32_t DLC [16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20, 24, 32, 48, 64} ;

//--------------------------------------------------------------------------------------------------

int main (int argc, const char * argv[]) {
  std::cout << "TRAME DE BASE, SANS CHANGEMENT DE DEBIT\n" ;
  for (uint32_t i=0 ; i < 16 ; i++) {
    const uint32_t minStandard = 29 + ((i >= 11) ? 38 : 33) + 8 * DLC [i] ;
    const uint32_t maxStandard = minStandard + 4 + 1 + 2 * DLC [i] ; ;
    std::cout << " & \\texttt{" << i << "} (" << DLC [i] << " octets)"
              << " & " << separatedString (ARBITRATION_BIT_RATE / maxStandard)
              << " à " << separatedString (ARBITRATION_BIT_RATE / minStandard) << " trames/s & " ;
    if (i == 0) {
      std::cout << "0" ;
    }else{
    std::cout << separatedString ((ARBITRATION_BIT_RATE / maxStandard) * DLC [i])
              << " à " << separatedString ((ARBITRATION_BIT_RATE / minStandard) * DLC [i]) << " octets/s" ;
    }
    std::cout << " \\\\\n" ;
  }
  std::cout << "TRAME ETENDUES, SANS CHANGEMENT DE DEBIT\n" ;
  for (uint32_t i=0 ; i < 16 ; i++) {
    const uint32_t minStandard = 49 + ((i >= 11) ? 38 : 33) + 8 * DLC [i] ;
    const uint32_t maxStandard = minStandard + 9 + 1 + 2 * DLC [i] ; ;
    std::cout << " & \\texttt{" << i << "} (" << DLC [i] << " octets)"
              << " & " << separatedString (ARBITRATION_BIT_RATE / maxStandard)
              << " à " << separatedString (ARBITRATION_BIT_RATE / minStandard) << " trames/s & " ;
    if (i == 0) {
      std::cout << "0" ;
    }else{
    std::cout << separatedString ((ARBITRATION_BIT_RATE / maxStandard) * DLC [i])
              << " à " << separatedString ((ARBITRATION_BIT_RATE / minStandard) * DLC [i]) << " octets/s" ;
    }
    std::cout << " \\\\\n" ;
  }
  std::cout << "TRAME DE BASE, AVEC CHANGEMENT DE DEBIT\n" ;
  for (uint32_t i=0 ; i < 16 ; i++) {
    const uint64_t minStandardArbitration = 29 ;
    const uint64_t minStandardData = ((i >= 11) ? 38 : 33) + 8 * DLC [i] ;
    const uint64_t maxStandardArbitration = minStandardArbitration + 4 ;
    const uint64_t maxStandardData = minStandardData + 1 + 2 * DLC [i] ;
    const uint64_t minTrames = (ARBITRATION_BIT_RATE * DATA_BIT_RATE) / (DATA_BIT_RATE * maxStandardArbitration + ARBITRATION_BIT_RATE * maxStandardData) ;
    const uint64_t maxTrames = (ARBITRATION_BIT_RATE * DATA_BIT_RATE) / (DATA_BIT_RATE * minStandardArbitration + ARBITRATION_BIT_RATE * minStandardData) ;
    std::cout << " & \\texttt{" << i << "} (" << DLC [i] << " octets)"
              << " & " << separatedString (minTrames)
              << " à " << separatedString (maxTrames) << " trames/s & " ;
    if (i == 0) {
      std::cout << "0" ;
    }else{
    std::cout << separatedString (minTrames * DLC [i])
              << " à " << separatedString (maxTrames * DLC [i]) << " octets/s" ;
    }
    std::cout << " \\\\\n" ;
  }
  std::cout << "TRAME ETENDUE, AVEC CHANGEMENT DE DEBIT\n" ;
  for (uint32_t i=0 ; i < 16 ; i++) {
    const uint64_t minStandardArbitration = 49 ;
    const uint64_t minStandardData = ((i >= 11) ? 38 : 33) + 8 * DLC [i] ;
    const uint64_t maxStandardArbitration = minStandardArbitration + 9 ;
    const uint64_t maxStandardData = minStandardData + 1 + 2 * DLC [i] ;
    const uint64_t minTrames = (ARBITRATION_BIT_RATE * DATA_BIT_RATE) / (DATA_BIT_RATE * maxStandardArbitration + ARBITRATION_BIT_RATE * maxStandardData) ;
    const uint64_t maxTrames = (ARBITRATION_BIT_RATE * DATA_BIT_RATE) / (DATA_BIT_RATE * minStandardArbitration + ARBITRATION_BIT_RATE * minStandardData) ;
    std::cout << " & \\texttt{" << i << "} (" << DLC [i] << " octets)"
              << " & " << separatedString (minTrames)
              << " à " << separatedString (maxTrames) << " trames/s & " ;
    if (i == 0) {
      std::cout << "0" ;
    }else{
    std::cout << separatedString (minTrames * DLC [i])
              << " à " << separatedString (maxTrames * DLC [i]) << " octets/s" ;
    }
    std::cout << " \\\\\n" ;
  }
  return 0;
}

//--------------------------------------------------------------------------------------------------
