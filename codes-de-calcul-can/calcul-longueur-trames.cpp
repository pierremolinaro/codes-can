//
//  main.cpp
//  can-in-software
//
//  Created by Pierre Molinaro on 24/05/2016.
//  Copyright © 2016 Pierre Molinaro. All rights reserved.
//
//--------------------------------------------------------------------------------------------------

#include <iostream>
#include <iomanip>

//--------------------------------------------------------------------------------------------------

#include "CANFrameGenerator.h"

//--------------------------------------------------------------------------------------------------

class CountedSet {
//--- Constructor
  public: CountedSet (void)  :
  mFirstEntry (0),
  mEntryCount (0),
  mEntries (NULL) {
  }

//--- Destructor
  public: virtual ~ CountedSet (void) {
    delete [] mEntries ;
  }

//--- Insert
  public: void insert (const uint32_t inValue) {
    if (0 == mEntryCount) {
      mFirstEntry = inValue ;
      mEntryCount = 1 ;
      mEntries = new uint64_t [1] ;
      mEntries [0] = 1 ;
    }else if (inValue < mFirstEntry) { // Insert before first entry
      const uint32_t currentLast = mFirstEntry + mEntryCount - 1 ;
      uint64_t * newEntries = new uint64_t [currentLast - inValue + 1] ;
      newEntries [0] = 1 ;
      for (uint32_t i=1 ; i < (mFirstEntry - inValue) ; i++) {
        newEntries [i] = 0 ;
      }
      for (uint32_t i=0 ; i<mEntryCount ; i++) {
        newEntries [mFirstEntry - inValue + i] = mEntries [i] ;
      }
      delete [] mEntries ; mEntries = newEntries ;
      mFirstEntry = inValue ;
      mEntryCount = currentLast - inValue + 1 ;
    }else if (inValue >= (mFirstEntry + mEntryCount)) { // Insert after last entry
      uint64_t * newEntries = new uint64_t [inValue - mFirstEntry + 1] ;
      for (uint32_t i=0 ; i<mEntryCount ; i++) {
        newEntries [i] = mEntries [i] ;
      }
      for (uint32_t i=mEntryCount ; i< (inValue - mFirstEntry) ; i++) {
        newEntries [i] = 0 ;
      }
      newEntries [inValue - mFirstEntry] = 1 ;
      delete [] mEntries ; mEntries = newEntries ;
      mEntryCount = inValue - mFirstEntry + 1 ;
    }else{
      mEntries [inValue - mFirstEntry] += 1 ;
    }
  }

//--- Count
  public : uint64_t count (const size_t inValue) const {
    uint64_t result = 0 ;
    if ((inValue >= mFirstEntry) && (inValue < (mFirstEntry + mEntryCount))) {
      result = mEntries [inValue - mFirstEntry] ;
    }
    return result ;
  }

  public : inline uint32_t cbegin (void) const { return mFirstEntry ; }

  public : inline uint32_t cend (void) const { return mFirstEntry + mEntryCount ; }

//--- Private properties
  private : uint32_t mFirstEntry ;
  private : uint32_t mEntryCount ;
  private : uint64_t * mEntries ;

//--- No copy
  private : CountedSet (const CountedSet &) = delete ;
  private : CountedSet & operator = (const CountedSet &) = delete ;
} ;

//--------------------------------------------------------------------------------------------------

using namespace std ;

//--------------------------------------------------------------------------------------------------

static const int RESULT_COUNT = 20 ;

//--------------------------------------------------------------------------------------------------

class ZeroByteResult {
  public : uint32_t mIdentifier ;
  public : uint32_t mFrameLength ;

  public : ZeroByteResult (void) : mIdentifier (0), mFrameLength (0) {}

  public : ZeroByteResult (const uint32_t inIdentifier, const uint32_t inFrameLength) :
  mIdentifier (inIdentifier),
  mFrameLength (inFrameLength) {
  }
} ;

//--------------------------------------------------------------------------------------------------

class RemoteFrameResult {
  public : uint32_t mIdentifier ;
  public : uint8_t mDataLength ;
  public : uint32_t mFrameLength ;

  public : RemoteFrameResult (void) : mIdentifier (0), mDataLength (0), mFrameLength (0) {}

  public : RemoteFrameResult (const uint32_t inIdentifier, const uint8_t inDataLength, const uint32_t inFrameLength) :
  mIdentifier (inIdentifier),
  mDataLength (inDataLength),
  mFrameLength (inFrameLength) {
  }
} ;

//--------------------------------------------------------------------------------------------------

static void compareAllExtendedRemoteFrames (void) {
  cout << "-------------------------------------- Extended remote frames:\n" ;
  const chrono::time_point<chrono::system_clock> start = chrono::system_clock::now();
  CountedSet lengthCountedSet ;
  RemoteFrameResult shortestFrames [RESULT_COUNT] ;
  RemoteFrameResult longestFrames  [RESULT_COUNT] ;
  for (int i=0 ; i<RESULT_COUNT ; i++) {
    shortestFrames [i].mFrameLength = INT_MAX ;
  }
  for (uint32_t identifier = 0 ; identifier < (1 << 29) ; identifier ++) {
    for (uint8_t dataLength = 0 ; dataLength <= 8 ; dataLength ++) {
      CANFrameGenerator f (identifier, FrameFormat::extended, dataLength, NULL, FrameType::remote, AckSlot::dominant) ;
      lengthCountedSet.insert (f.frameLength ()) ;
      if (f.frameLength () < shortestFrames [RESULT_COUNT - 1].mFrameLength) {
        RemoteFrameResult r (identifier, dataLength, f.frameLength ()) ;
        for (int i=0 ; i<RESULT_COUNT ; i++) {
          if (r.mFrameLength < shortestFrames [i].mFrameLength) {
            swap (r, shortestFrames [i]) ;
          }
        }
      }
      if (f.frameLength () > longestFrames [RESULT_COUNT - 1].mFrameLength) {
        RemoteFrameResult r (identifier, dataLength, f.frameLength ()) ;
        for (int i=0 ; i<RESULT_COUNT ; i++) {
          if (r.mFrameLength > longestFrames [i].mFrameLength) {
            swap (r, longestFrames [i]) ;
          }
        }
      }
    }
  }
//--- Display shortest frames
  cout << "Shortest frames:\n" ;
  for (int i=0 ; i<RESULT_COUNT ; i++) {
    cout << shortestFrames [i].mFrameLength << " for identifier 0x"
         << hex << shortestFrames [i].mIdentifier << dec << ", DLC "
         << unsigned (shortestFrames [i].mDataLength) << endl ;
  }
//--- Display longest frames
  cout << "Longest frames:\n" ;
  for (int i=0 ; i<RESULT_COUNT ; i++) {
    cout << longestFrames [i].mFrameLength << " for identifier 0x"
         << hex << longestFrames [i].mIdentifier << dec << ", DLC "
         << unsigned (longestFrames [i].mDataLength) << endl ;
  }
//--- Display frame length distribution
  cout << "Frame lengthes:\n" ;
  for (auto it=lengthCountedSet.cbegin(); it != lengthCountedSet.cend(); it++) {
    cout << "  " << it << " --> " << lengthCountedSet.count (it) << " frames\n" ;
  }
  const chrono::time_point<chrono::system_clock> end = chrono::system_clock::now();
  const chrono::duration<double> elapsed_seconds = end - start;
  cout << "Elapsed time: " << elapsed_seconds.count() << "s\n";
}

//--------------------------------------------------------------------------------------------------

static void compareAllStandardRemoteFrames (void) {
  cout << "-------------------------------------- Standard remote frames:\n" ;
  const chrono::time_point<chrono::system_clock> start = chrono::system_clock::now();
  CountedSet lengthCountedSet ;
  RemoteFrameResult shortestFrames [RESULT_COUNT] ;
  RemoteFrameResult longestFrames  [RESULT_COUNT] ;
  for (int i=0 ; i<RESULT_COUNT ; i++) {
    shortestFrames [i].mFrameLength = INT_MAX ;
  }
  for (uint32_t identifier = 0 ; identifier < (1 << 11) ; identifier ++) {
    for (uint8_t dataLength = 0 ; dataLength <= 8 ; dataLength ++) {
      CANFrameGenerator f (identifier, FrameFormat::standard, dataLength, NULL, FrameType::remote, AckSlot::dominant) ;
      lengthCountedSet.insert (f.frameLength ()) ;
      if (f.frameLength () < shortestFrames [RESULT_COUNT - 1].mFrameLength) {
        RemoteFrameResult r (identifier, dataLength, f.frameLength ()) ;
        for (int i=0 ; i<RESULT_COUNT ; i++) {
          if (r.mFrameLength < shortestFrames [i].mFrameLength) {
            swap (r, shortestFrames [i]) ;
          }
        }
      }
      if (f.frameLength () > longestFrames [RESULT_COUNT - 1].mFrameLength) {
        RemoteFrameResult r (identifier, dataLength, f.frameLength ()) ;
        for (int i=0 ; i<RESULT_COUNT ; i++) {
          if (r.mFrameLength > longestFrames [i].mFrameLength) {
            swap (r, longestFrames [i]) ;
          }
        }
      }
    }
  }
//--- Display shortest frames
  cout << "Shortest frames:\n" ;
  for (int i=0 ; i<RESULT_COUNT ; i++) {
    cout << shortestFrames [i].mFrameLength << " for identifier 0x"
         << hex << shortestFrames [i].mIdentifier << dec << ", DLC "
         << unsigned (shortestFrames [i].mDataLength) << endl ;
  }
//--- Display longest frames
  cout << "Longest frames:\n" ;
  for (int i=0 ; i<RESULT_COUNT ; i++) {
    cout << longestFrames [i].mFrameLength << " for identifier 0x"
         << hex << longestFrames [i].mIdentifier << dec << ", DLC "
        << unsigned (longestFrames [i].mDataLength) << endl ;
  }
//--- Display frame length distribution
  cout << "Frame lengthes:\n" ;
  for (auto it=lengthCountedSet.cbegin(); it != lengthCountedSet.cend(); it++) {
    cout << "  " << it << " --> " << lengthCountedSet.count (it) << " frames\n" ;
  }
  const chrono::time_point<chrono::system_clock> end = chrono::system_clock::now();
  const chrono::duration<double> elapsed_seconds = end - start;
  cout << "Elapsed time: " << elapsed_seconds.count() << "s\n";
}

//--------------------------------------------------------------------------------------------------

static void compareAllStandardFramesZeroData (void) {
  cout << "-------------------------------------- Standard frames without data:\n" ;
  const chrono::time_point<chrono::system_clock> start = chrono::system_clock::now();
  CountedSet lengthCountedSet ;
  ZeroByteResult shortestFrames [RESULT_COUNT] ;
  ZeroByteResult longestFrames  [RESULT_COUNT] ;
  for (int i=0 ; i<RESULT_COUNT ; i++) {
    shortestFrames [i].mFrameLength = INT_MAX ;
  }
  for (uint32_t identifier = 0 ; identifier < (1 << 11) ; identifier ++) {
    CANFrameGenerator f (identifier, FrameFormat::standard, 0, NULL, FrameType::data, AckSlot::dominant) ;
    lengthCountedSet.insert (f.frameLength ()) ;
    if (f.frameLength () < shortestFrames [RESULT_COUNT - 1].mFrameLength) {
      ZeroByteResult r (identifier, f.frameLength ()) ;
      for (int i=0 ; i<RESULT_COUNT ; i++) {
        if (r.mFrameLength < shortestFrames [i].mFrameLength) {
          swap (r, shortestFrames [i]) ;
        }
      }
    }
    if (f.frameLength () > longestFrames [RESULT_COUNT - 1].mFrameLength) {
      ZeroByteResult r (identifier, f.frameLength ()) ;
      for (int i=0 ; i<RESULT_COUNT ; i++) {
        if (r.mFrameLength > longestFrames [i].mFrameLength) {
          swap (r, longestFrames [i]) ;
        }
      }
    }
  }
//--- Display shortest frames
  cout << "Shortest frames:\n" ;
  for (int i=0 ; i<RESULT_COUNT ; i++) {
    cout << shortestFrames [i].mFrameLength << " for identifier 0x"
         << hex << shortestFrames [i].mIdentifier << dec  << endl ;
  }
//--- Display longest frames
  cout << "Longest frames:\n" ;
  for (int i=0 ; i<RESULT_COUNT ; i++) {
    cout << longestFrames [i].mFrameLength << " for identifier 0x"
         << hex << longestFrames [i].mIdentifier << dec << endl ;
  }
//--- Display frame length distribution
  cout << "Frame lengthes:\n" ;
  for (auto it=lengthCountedSet.cbegin(); it != lengthCountedSet.cend(); it++) {
    cout << "  " << it << " --> " << lengthCountedSet.count (it) << " frames\n" ;
  }
  const chrono::time_point<chrono::system_clock> end = chrono::system_clock::now();
  const chrono::duration<double> elapsed_seconds = end - start;
  cout << "Elapsed time: " << elapsed_seconds.count() << "s\n";
}

//--------------------------------------------------------------------------------------------------

static void compareAllExtendedFramesZeroData (void) {
  cout << "-------------------------------------- Extended frames without data:\n" ;
  const chrono::time_point<chrono::system_clock> start = chrono::system_clock::now();
  CountedSet lengthCountedSet ;
  ZeroByteResult shortestFrames [RESULT_COUNT] ;
  ZeroByteResult longestFrames  [RESULT_COUNT] ;
  for (int i=0 ; i<RESULT_COUNT ; i++) {
    shortestFrames [i].mFrameLength = INT_MAX ;
  }
  for (uint32_t identifier = 0 ; identifier < (1 << 29) ; identifier ++) {
    CANFrameGenerator f (identifier, FrameFormat::extended, 0, NULL, FrameType::data, AckSlot::dominant) ;
    lengthCountedSet.insert (f.frameLength ()) ;
    if (f.frameLength () < shortestFrames [RESULT_COUNT - 1].mFrameLength) {
      ZeroByteResult r (identifier, f.frameLength ()) ;
      for (int i=0 ; i<RESULT_COUNT ; i++) {
        if (r.mFrameLength < shortestFrames [i].mFrameLength) {
          swap (r, shortestFrames [i]) ;
        }
      }
    }
    if (f.frameLength () > longestFrames [RESULT_COUNT - 1].mFrameLength) {
      ZeroByteResult r (identifier, f.frameLength ()) ;
      for (int i=0 ; i<RESULT_COUNT ; i++) {
        if (r.mFrameLength > longestFrames [i].mFrameLength) {
          swap (r, longestFrames [i]) ;
        }
      }
    }
  }
//--- Display shortest frames
  cout << "Shortest:\n" ;
  for (int i=0 ; i<RESULT_COUNT ; i++) {
    cout << shortestFrames [i].mFrameLength << " for identifier 0x"
         << hex << shortestFrames [i].mIdentifier  << dec << endl ;
  }
//--- Display longest frames
  cout << "Longest frames:\n" ;
  for (int i=0 ; i<RESULT_COUNT ; i++) {
    cout << longestFrames [i].mFrameLength << " for identifier 0x"
         << hex << longestFrames [i].mIdentifier << dec << endl ;
  }
//--- Display frame length distribution
  cout << "Frame lengthes:\n" ;
  for (auto it=lengthCountedSet.cbegin(); it != lengthCountedSet.cend(); it++) {
    cout << "  " << it << " --> " << lengthCountedSet.count (it) << " frames\n" ;
  }
  const chrono::time_point<chrono::system_clock> end = chrono::system_clock::now();
  const chrono::duration<double> elapsed_seconds = end - start;
  cout << "Elapsed time: " << elapsed_seconds.count() << "s\n";
}

//--------------------------------------------------------------------------------------------------

class OneByteResult {
  public : uint32_t mIdentifier ;
  public : int mByte ;
  public : uint32_t mFrameLength ;
  
  public : OneByteResult (void) : mIdentifier (0), mByte (0), mFrameLength (0) {}
  
  public : OneByteResult (const uint32_t inIdentifier, const int inByte, const uint32_t inFrameLength) :
  mIdentifier (inIdentifier),
  mByte (inByte),
  mFrameLength (inFrameLength) {
  }
} ;

//--------------------------------------------------------------------------------------------------

static void compareAllExtendedFramesOneByteData (void) {
  cout << "-------------------------------------- One byte extended frames:\n" ;
  const chrono::time_point<chrono::system_clock> start = chrono::system_clock::now();
  CountedSet lengthCountedSet ;
  OneByteResult shortestFrames [RESULT_COUNT] ;
  OneByteResult longestFrames  [RESULT_COUNT] ;
  for (int i=0 ; i<RESULT_COUNT ; i++) {
    shortestFrames [i].mFrameLength = INT_MAX ;
  }
  for (int byte = 0 ; byte < 256 ; byte ++) {
    const chrono::time_point<chrono::system_clock> end = chrono::system_clock::now();
    const chrono::duration<double> elapsed_seconds = end - start ;
    cout << "Byte 0x" << hex << byte << dec << ", at " << elapsed_seconds.count ()
         << "s, shortest " << shortestFrames [0].mFrameLength
         << ", longest "  << longestFrames [0].mFrameLength << endl ;
    for (uint32_t identifier = 0 ; identifier < (1 << 29) ; identifier ++) {
      const uint8_t b = uint8_t (byte) ;
      CANFrameGenerator f (identifier, FrameFormat::extended, 1, &b, FrameType::data, AckSlot::dominant) ;
      lengthCountedSet.insert (f.frameLength ()) ;
      if (f.frameLength () < shortestFrames [RESULT_COUNT - 1].mFrameLength) {
        OneByteResult r (identifier, byte, f.frameLength ()) ;
        for (int i=0 ; i<RESULT_COUNT ; i++) {
          if (r.mFrameLength < shortestFrames [i].mFrameLength) {
            swap (r, shortestFrames [i]) ;
          }
        }
      }
      if (f.frameLength () > longestFrames [RESULT_COUNT - 1].mFrameLength) {
        OneByteResult r (identifier, byte, f.frameLength ()) ;
        for (int i=0 ; i<RESULT_COUNT ; i++) {
          if (r.mFrameLength > longestFrames [i].mFrameLength) {
            swap (r, longestFrames [i]) ;
          }
        }
      }
    }
  }
//--- Display shortest frames
  cout << "Shortest frames:\n" ;
  for (int i=0 ; i<RESULT_COUNT ; i++) {
    cout << shortestFrames [i].mFrameLength << " for identifier 0x"
         << hex << shortestFrames [i].mIdentifier
         << ", byte 0x" << shortestFrames [i].mByte
          << dec << endl ;
  }
//--- Display longest frames
  cout << "Longest frames:\n" ;
  for (int i=0 ; i<RESULT_COUNT ; i++) {
    cout << longestFrames [i].mFrameLength << " for identifier 0x"
         << hex << longestFrames [i].mIdentifier
         << ", byte 0x" << longestFrames [i].mByte
         << dec << endl ;
  }
  cout << "Frame lengthes:\n" ;
  for (auto it=lengthCountedSet.cbegin(); it != lengthCountedSet.cend(); it++) {
    cout << "  " << it << " --> " << lengthCountedSet.count (it) << " frames\n" ;
  }
  const chrono::time_point<chrono::system_clock> end = chrono::system_clock::now();
  const chrono::duration<double> elapsed_seconds = end - start;
  cout << "Elapsed time: " << elapsed_seconds.count() << "s\n";
}

//--------------------------------------------------------------------------------------------------

static void compareAllStandardFramesOneByteData (void) {
  cout << "-------------------------------------- One byte standard frames:\n" ;
  const chrono::time_point<chrono::system_clock> start = chrono::system_clock::now();
  CountedSet lengthCountedSet ;
  OneByteResult shortestFrames [RESULT_COUNT] ;
  OneByteResult longestFrames  [RESULT_COUNT] ;
  for (int i=0 ; i<RESULT_COUNT ; i++) {
    shortestFrames [i].mFrameLength = INT_MAX ;
  }
  for (uint16_t identifier = 0 ; identifier < (1 << 11) ; identifier ++) {
    for (int byte = 0 ; byte < 256 ; byte ++) {
      const uint8_t b = uint8_t (byte) ;
      CANFrameGenerator f (identifier, FrameFormat::standard, 1, &b, FrameType::data, AckSlot::dominant) ;
      lengthCountedSet.insert (f.frameLength ()) ;
      if (f.frameLength () < shortestFrames [RESULT_COUNT - 1].mFrameLength) {
        OneByteResult r (identifier, b, f.frameLength ()) ;
        for (int i=0 ; i<RESULT_COUNT ; i++) {
          if (r.mFrameLength < shortestFrames [i].mFrameLength) {
            swap (r, shortestFrames [i]) ;
          }
        }
      }
      if (f.frameLength () > longestFrames [RESULT_COUNT - 1].mFrameLength) {
        OneByteResult r (identifier, b, f.frameLength ()) ;
        for (int i=0 ; i<RESULT_COUNT ; i++) {
          if (r.mFrameLength > longestFrames [i].mFrameLength) {
            swap (r, longestFrames [i]) ;
          }
        }
      }
    }
  }
//--- Display shortest frames
  cout << "Shortest frames:\n" ;
  for (int i=0 ; i<RESULT_COUNT ; i++) {
    cout << shortestFrames [i].mFrameLength << " for identifier 0x"
         << hex << shortestFrames [i].mIdentifier
         << ", byte 0x" << shortestFrames [i].mByte
         << dec << endl ;
  }
//--- Display longest frames
  cout << "Longest frames:\n" ;
  for (int i=0 ; i<RESULT_COUNT ; i++) {
    cout << longestFrames [i].mFrameLength << " for identifier 0x"
         << hex << longestFrames [i].mIdentifier
         << ", byte 0x" << longestFrames [i].mByte
         << dec << endl ;
  }
  cout << "Frame lengthes:\n" ;
  for (auto it=lengthCountedSet.cbegin(); it != lengthCountedSet.cend(); it++) {
    cout << "  " << it << " --> " << lengthCountedSet.count (it) << " frames\n" ;
  }
  const chrono::time_point<chrono::system_clock> end = chrono::system_clock::now();
  const chrono::duration<double> elapsed_seconds = end - start ;
  cout << "Elapsed time: " << elapsed_seconds.count() << "s\n";
}

//--------------------------------------------------------------------------------------------------

class TwoBytesResult {
  public : uint32_t mIdentifier ;
  public : int mByte0 ;
  public : int mByte1 ;
  public : uint32_t mFrameLength ;
  
  public : TwoBytesResult (void) : mIdentifier (0), mByte0 (0), mByte1 (0), mFrameLength (0) {}
  
  public : TwoBytesResult (const uint32_t inIdentifier, const int inByte0, const int inByte1, const uint32_t inFrameLength) :
  mIdentifier (inIdentifier),
  mByte0 (inByte0),
  mByte1 (inByte1),
  mFrameLength (inFrameLength) {
  }
} ;

//--------------------------------------------------------------------------------------------------

static void compareAllStandardFramesTwoBytesData (void) {
  cout << "-------------------------------------- Two bytes standard frames:\n" ;
  const chrono::time_point<chrono::system_clock> start = chrono::system_clock::now();
  CountedSet lengthCountedSet ;
  TwoBytesResult shortestFrames [RESULT_COUNT] ;
  TwoBytesResult longestFrames  [RESULT_COUNT] ;
  for (int i=0 ; i<RESULT_COUNT ; i++) {
    shortestFrames [i].mFrameLength = INT_MAX ;
  }
  for (uint16_t identifier = 0 ; identifier < (1 << 11) ; identifier ++) {
    for (int byte0 = 0 ; byte0 < 256 ; byte0 ++) {
      for (int byte1 = 0 ; byte1 < 256 ; byte1 ++) {
        const uint8_t b [2] = {uint8_t (byte0), uint8_t (byte1)} ;
        CANFrameGenerator f (identifier, FrameFormat::standard, 2, b, FrameType::data, AckSlot::dominant) ;
        lengthCountedSet.insert (f.frameLength ()) ;
        if (f.frameLength () < shortestFrames [RESULT_COUNT - 1].mFrameLength) {
          TwoBytesResult r (identifier, byte0, byte1, f.frameLength ()) ;
          for (int i=0 ; i<RESULT_COUNT ; i++) {
            if (r.mFrameLength < shortestFrames [i].mFrameLength) {
              swap (r, shortestFrames [i]) ;
            }
          }
        }
        if (f.frameLength () > longestFrames [RESULT_COUNT - 1].mFrameLength) {
          TwoBytesResult r (identifier, byte0, byte1, f.frameLength ()) ;
          for (int i=0 ; i<RESULT_COUNT ; i++) {
            if (r.mFrameLength > longestFrames [i].mFrameLength) {
              swap (r, longestFrames [i]) ;
            }
          }
        }
      }
    }
  }
//--- Display frames
  cout << "Shortest frames:\n" ;
  for (int i=0 ; i<RESULT_COUNT ; i++) {
    cout << shortestFrames [i].mFrameLength << " for identifier 0x"
         << hex << shortestFrames [i].mIdentifier
         << ", bytes 0x" << shortestFrames [i].mByte0
         << ", 0x" << shortestFrames [i].mByte1
          << dec << endl ;
  }
//--- Display longest frames
  cout << "Longest frames:\n" ;
  for (int i=0 ; i<RESULT_COUNT ; i++) {
    cout << longestFrames [i].mFrameLength << " for identifier 0x"
         << hex << longestFrames [i].mIdentifier
         << ", bytes 0x" << longestFrames [i].mByte0
         << ", 0x" << longestFrames [i].mByte1
         << dec << endl ;
  }
  cout << "Frame lengthes:\n" ;
  for (auto it=lengthCountedSet.cbegin(); it != lengthCountedSet.cend(); it++) {
    cout << "  " << it << " --> " << lengthCountedSet.count (it) << " frames\n" ;
  }
  const chrono::time_point<chrono::system_clock> end = chrono::system_clock::now();
  const chrono::duration<double> elapsed_seconds = end - start;
  cout << "Elapsed time: " << elapsed_seconds.count() << "s\n";
}

//--------------------------------------------------------------------------------------------------

int main (int /* argc */, const char * /* argv */ []) {
  compareAllStandardRemoteFrames () ;
  compareAllStandardFramesZeroData () ;
  compareAllStandardFramesOneByteData () ;
  compareAllStandardFramesTwoBytesData () ;
  compareAllExtendedFramesZeroData () ;
  compareAllExtendedRemoteFrames () ;
  compareAllExtendedFramesOneByteData () ;
  return 0 ;
}

//--------------------------------------------------------------------------------------------------
