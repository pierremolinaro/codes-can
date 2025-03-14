//
//  main.cpp
//  calcul-debit-utile-can
//
//  Created by Pierre Molinaro on 16/10/2023.
//
//--------------------------------------------------------------------------------------------------

#include <iostream>
#include <vector>

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

std::string separatedString (const uint32_t inValue) {
  std::string result ;
  const uint32_t milliers =  inValue / 1000 ;
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

static const uint32_t BIT_RATE = 1'000'000 ; // bit/s

//--------------------------------------------------------------------------------------------------

int main(int argc, const char * argv[]) {
  std::vector <Bounds *> vector ;
  vector.push_back (new Bounds ( 47,  55,  67,  80)) ; // 0
  vector.push_back (new Bounds ( 55,  65,  75,  90)) ; // 1
  vector.push_back (new Bounds ( 63,  75,  83, 100)) ; // 2
  vector.push_back (new Bounds ( 71,  85,  91, 110)) ; // 3
  vector.push_back (new Bounds ( 79,  95,  99, 120)) ; // 4
  vector.push_back (new Bounds ( 87, 105, 107, 130)) ; // 5
  vector.push_back (new Bounds ( 95, 115, 115, 140)) ; // 6
  vector.push_back (new Bounds (103, 125, 123, 150)) ; // 7
  vector.push_back (new Bounds (111, 135, 131, 160)) ; // 8
  for (uint32_t i=0 ; i <= 8 ; i++) {
    const uint32_t minStandard = BIT_RATE / vector [i]->mStandardMax ;
    const uint32_t maxStandard = BIT_RATE / vector [i]->mStandardMin ;
    std::cout << " & \\tt " << i << " & " << separatedString (minStandard)
              << " à " << separatedString (maxStandard) << " trames/s & " ;
    if (i == 0) {
      std::cout << " -- " ;
    }else{
      const uint32_t minData = minStandard * i ;
      const uint32_t maxData = maxStandard * i ;
      std::cout << separatedString (minData) << " à " << separatedString (maxData) << " octets/s " ;
    }
    std::cout << "\\\\\n" ;
  }
  for (uint32_t i=0 ; i <= 8 ; i++) {
    const uint32_t minStandard = BIT_RATE / vector [i]->mExtendedMax ;
    const uint32_t maxStandard = BIT_RATE / vector [i]->mExtendedMin ;
    std::cout << " & \\tt " << i << " & " << separatedString (minStandard)
              << " à " << separatedString (maxStandard) << " trames/s & " ;
    if (i == 0) {
      std::cout << " -- " ;
    }else{
      const uint32_t minData = minStandard * i ;
      const uint32_t maxData = maxStandard * i ;
      std::cout << separatedString (minData)
                << " à " << separatedString (maxData) << " octets/s " ;
    }
    std::cout << "\\\\\n" ;
  }
  return 0;
}

//--------------------------------------------------------------------------------------------------
