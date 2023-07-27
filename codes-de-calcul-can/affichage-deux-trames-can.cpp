//--------------------------------------------------------------------------------------------------

#include <iostream>    // std::cout
#include <algorithm>   // std::min
#include "CANFrameGenerator.h"

using namespace std ;

//--------------------------------------------------------------------------------------------------

static void displayFrames (const CANFrameGenerator & inLeftFrame,
                           const CANFrameGenerator & inRightFrame) {
  const uint32_t minLength = min (inLeftFrame.frameLength (), inRightFrame.frameLength ()) ;
  for (uint32_t i= 0 ; i < minLength ; i++) {
    cout << "    \\tt " << inLeftFrame.nameAtIndex (i)
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
    cout << "    & & & \\tt " << i
         << " & \\tt " << inRightFrame.nameAtIndex (i)
         << " & \\tt " << inRightFrame.bitAtIndex (i)
         << " & \\tt "  << inRightFrame.fieldTypeAtIndex (i)
         << " \\\\\n" ;
  }
//--- Right frame shorter than left frame ?
  for (uint32_t i= minLength ; i < inLeftFrame.frameLength () ; i++) {
    cout << "    \\tt " << inLeftFrame.nameAtIndex (i)
         << " & \\tt " << inLeftFrame.bitAtIndex (i)
         << " & \\tt "  << inLeftFrame.fieldTypeAtIndex (i)
         << " & \\tt " << i << " & & & \\\\\n" ;
  }
}

//--------------------------------------------------------------------------------------------------

int main (int argc, const char * argv[]) {
  const uint8_t byte = 0x55 ;
  const CANFrameGenerator f0 (0x7AA, FrameFormat::standard, 1, &byte, FrameType::data, AckSlot::dominant) ;
  const CANFrameGenerator f1 (0x12345678, FrameFormat::extended, 8, nullptr, FrameType::remote, AckSlot::dominant) ;
  displayFrames (f0, f1) ;
  return 0 ;
}

//--------------------------------------------------------------------------------------------------
