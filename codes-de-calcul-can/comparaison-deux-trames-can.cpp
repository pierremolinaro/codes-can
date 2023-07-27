//--------------------------------------------------------------------------------------------------

#include <iostream>    // std::cout
#include <algorithm>   // std::min
#include "CANFrameGenerator.h"

using namespace std ;

//--------------------------------------------------------------------------------------------------

static void compareAndDisplayFrames (const CANFrameGenerator & inLeftFrame,
                                     const CANFrameGenerator & inRightFrame) {
  const uint32_t minLength = min (inLeftFrame.frameLength (), inRightFrame.frameLength ()) ;
  for (uint32_t i= 0 ; i < minLength ; i++) {
    cout << "   "
         << ((inLeftFrame.bitAtIndex (i) == inRightFrame.bitAtIndex (i)) ? "" : "$\\rightarrow$")
         << " & \\tt " << inLeftFrame.nameAtIndex (i)
         << " & \\tt " << inLeftFrame.bitAtIndex (i)
         << " & \\tt "  << inLeftFrame.fieldTypeAtIndex (i)
         << " & \\tt " << i
         << " & \\tt " << inRightFrame.nameAtIndex (i)
         << " & \\tt " << inRightFrame.bitAtIndex (i)
         << " & \\tt "  << inRightFrame.fieldTypeAtIndex (i)
         << " \\\\\n" ;
  }
//--- Left frame shorter than right frame ?
  for (uint32_t i= minLength ; i < inRightFrame.frameLength () ; i++) {
    cout << "   $\\rightarrow$ & & & & \\tt " << i
         << " & \\tt " << inRightFrame.nameAtIndex (i)
         << " & \\tt " << inRightFrame.bitAtIndex (i)
         << " & \\tt "  << inRightFrame.fieldTypeAtIndex (i)
         << " \\\\\n" ;
  }
//--- Right frame shorter than left frame ?
  for (uint32_t i= minLength ; i < inLeftFrame.frameLength () ; i++) {
    cout << "    $\\rightarrow$ & \\tt " << inLeftFrame.nameAtIndex (i)
         << " & \\tt " << inLeftFrame.bitAtIndex (i)
         << " & \\tt "  << inLeftFrame.fieldTypeAtIndex (i)
         << " & \\tt " << i << " & & & \\\\\n" ;
  }
}

//--------------------------------------------------------------------------------------------------

int main (int argc, const char * argv[]) {
//   const CANFrameGenerator f0 (0x40E, FrameFormat::standard, 0, nullptr, FrameType::data, AckSlot::dominant) ;
//   const CANFrameGenerator f1 (0x513, FrameFormat::standard, 0, nullptr, FrameType::data, AckSlot::dominant) ;

//   const CANFrameGenerator f0 (0x10B, FrameFormat::standard, 4, nullptr, FrameType::remote, AckSlot::dominant) ;
//   const CANFrameGenerator f1 (0x18F, FrameFormat::standard, 8, nullptr, FrameType::remote, AckSlot::dominant) ;

  const uint8_t d0 = 0x11 ;
  const CANFrameGenerator f0 (0x00E, FrameFormat::standard, 1, &d0, FrameType::data, AckSlot::dominant) ;
  const uint8_t d1 = 0x88 ;
  const CANFrameGenerator f1 (0x457, FrameFormat::standard, 1, &d1, FrameType::data, AckSlot::dominant) ;

  compareAndDisplayFrames (f0, f1) ;
  return 0 ;
}

//--------------------------------------------------------------------------------------------------
