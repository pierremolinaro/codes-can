//
//  CANFrameGenerator.h
//  can-in-software
//
//  Created by Pierre Molinaro on 24/05/2016.
//  Copyright © 2016 Pierre Molinaro. All rights reserved.
//
//----------------------------------------------------------------------------------------

#pragma once

//----------------------------------------------------------------------------------------

#ifdef ARDUINO
  #include <Arduino.h>
  static inline String to_string (const unsigned inValue) { return String (inValue) ; }
#else
  #include <string>
  typedef std::string String ;
  #include <iostream>
  #include <iomanip>
  using namespace std ;
#endif

//----------------------------------------------------------------------------------------

enum class FrameFormat : uint8_t {standard, extended} ;

//----------------------------------------------------------------------------------------

enum class FrameType : uint8_t {data, remote} ;

//----------------------------------------------------------------------------------------

enum class AckSlot : uint8_t {dominant, recessive} ;

//----------------------------------------------------------------------------------------

enum class FrameField : uint8_t {
  sof,
  arbitration,
  control,
  data,
  crc,
  ack,
  eof,
  intermission
} ;

//----------------------------------------------------------------------------------------

enum class FrameBitType : uint8_t {
  sof,
  identifier,
  data,
  crc,
  crcDelimiter,
  recessiveAckSlot,
  dominantAckSlot,
  ackDelimiter,
  eof,
  intermission,
  stuff,
  srr,
  ide,
  rtr,
  reserved,
  dlc
} ;

//----------------------------------------------------------------------------------------

static const uint32_t CAN_FRAME_MAX_LENGTH = 160 ;

//----------------------------------------------------------------------------------------

inline String frameFormatString (const FrameFormat inFormat) {
  return (inFormat == FrameFormat::standard) ? "standard" : "extended" ;
}

//----------------------------------------------------------------------------------------

class cFrameBit {
  public : cFrameBit (void) :
  mFieldType (FrameField::intermission),
  mBitType (FrameBitType::intermission),
  mDataIndex (0),
  mBitIndex (0) {
  }

  public : cFrameBit (const FrameField inType,
                      const FrameBitType inBitType,
                      const uint8_t inDataIndex,
                      const uint8_t inBitIndex) :
  mFieldType (inType),
  mBitType (inBitType),
  mDataIndex (inDataIndex),
  mBitIndex (inBitIndex) {
  }

  private: FrameField mFieldType ;
  private: FrameBitType mBitType;
  private: uint8_t mDataIndex ;
  private: uint8_t mBitIndex ;

  public: String name (void) const {
    String result ;
    switch (mBitType) {
    case FrameBitType::sof : result = "SOF" ; break ;
    case FrameBitType::identifier : result = "IDF" ; break ;
    case FrameBitType::data : result = "D" ; break ;
    case FrameBitType::crc : result = "CRC" ; break ;
    case FrameBitType::crcDelimiter : result = "CRC DEL" ; break ;
    case FrameBitType::recessiveAckSlot : result = "ACK SLOT" ; break ;
    case FrameBitType::dominantAckSlot : result = "ACK SLOT*" ; break ;
    case FrameBitType::ackDelimiter : result = "ACK DEL" ; break ;
    case FrameBitType::eof : result = "EOF" ; break ;
    case FrameBitType::intermission : result = "IFS" ; break ;
    case FrameBitType::stuff : result = "S" ; break ;
    case FrameBitType::srr : result = "SRR" ; break ;
    case FrameBitType::ide : result = "IDE" ; break ;
    case FrameBitType::rtr : result = "RTR" ; break ;
    case FrameBitType::reserved : result = "R" ; break ;
    case FrameBitType::dlc : result = "DLC" ; break ;
    }
    switch (mBitType) {
    case FrameBitType::identifier :
    case FrameBitType::data :
    case FrameBitType::crc :
    case FrameBitType::eof :
    case FrameBitType::intermission :
    case FrameBitType::reserved :
    case FrameBitType::dlc :
      result.append (to_string (mDataIndex)) ;
      break ;
    case FrameBitType::crcDelimiter :
    case FrameBitType::dominantAckSlot :
    case FrameBitType::recessiveAckSlot :
    case FrameBitType::ackDelimiter :
    case FrameBitType::stuff :
    case FrameBitType::srr :
    case FrameBitType::ide :
    case FrameBitType::rtr :
    case FrameBitType::sof :
      break ;
    }
    if (mBitType == FrameBitType::data) {
      result.append (".") ;
      result.append (to_string (mBitIndex)) ;
    }
    return result ;
  }

  public: String fieldName (void) const {
    String result ;
    switch (mFieldType) {
    case FrameField::sof : result = "START OF FRAME" ; break ;
    case FrameField::arbitration  : result = "ARBITRATION" ; break ;
    case FrameField::control : result = "CONTROL" ; break ;
    case FrameField::data : result = "DATA" ; break ;
    case FrameField::crc : result = "CRC" ; break ;
    case FrameField::ack : result = "ACK" ; break ;
    case FrameField::eof : result = "END OF FRAME" ; break ;
    case FrameField::intermission : result = "INTERMISSION" ; break ;
    }
    return result ;
  }
} ;

//----------------------------------------------------------------------------------------

class CANFrameGenerator {
  public : CANFrameGenerator (const uint32_t inIdentifier,
                              const FrameFormat inFrameFormat,
                              const uint8_t inDataLength,
                              const uint8_t inData [8],
                              const FrameType inFrameType,
                              const AckSlot inAckSlot) :
  mFrame (),
  mData (),
  mBits (),
  mIdentifier (inIdentifier),
  mFrameCRC (0),
  mStuffBitCount (0),
  mDataLength ((inDataLength > 15) ? 15 : inDataLength),
  mFrameLength (0),
  mFrameFormat (inFrameFormat),
  mFrameType (inFrameType),
  mAckSlot (inAckSlot),
  mConsecutiveBitCount (1),
  mLastBitValue (true),
  mCRCAccumulator (0) {
    for (uint32_t i=0 ; i<5 ; i++) {
      mBits [i] = UINT32_MAX ;
    }
    if (mFrameType == FrameType::data) {
      const uint32_t maxLength = (mDataLength > 8) ? 8 : mDataLength ;
      for (uint32_t i=0 ; i<maxLength ; i++) {
        mData [i] = inData [i] ;
      }
    }
  //--- Generate frame
    enterBitAppendStuff (false, FrameField::sof, FrameBitType::sof, 0, 0) ;
    switch (mFrameFormat) {
    case FrameFormat::extended :
      for (uint8_t idx = 28 ; idx >= 18 ; idx--) { // Identifier
        const bool bit = (mIdentifier & (1 << idx)) != 0 ;
        enterBitAppendStuff (bit, FrameField::arbitration, FrameBitType::identifier, idx, 0) ;
      }
      enterBitAppendStuff (true, FrameField::arbitration, FrameBitType::srr, 0, 0) ;
      enterBitAppendStuff (true, FrameField::arbitration, FrameBitType::ide, 0, 0) ;
      for (int idx = 17 ; idx >= 0 ; idx--) { // Identifier
        const bool bit = (mIdentifier & (1 << idx)) != 0 ;
        enterBitAppendStuff (bit, FrameField::arbitration, FrameBitType::identifier, uint8_t (idx), 0) ;
      }
      break ;
    case FrameFormat::standard :
      for (int idx = 10 ; idx >= 0 ; idx--) { // Identifier
        const bool bit = (mIdentifier & (1 << idx)) != 0 ;
        enterBitAppendStuff (bit, FrameField::arbitration, FrameBitType::identifier, uint8_t (idx), 0) ;
      }
      break ;
    }
    enterBitAppendStuff (mFrameType == FrameType::remote, FrameField::arbitration, FrameBitType::rtr, 0, 0) ;
    enterBitAppendStuff (false, FrameField::control, FrameBitType::reserved, 1, 0) ;
    enterBitAppendStuff (false, FrameField::control, FrameBitType::reserved, 0, 0) ;
    enterBitAppendStuff ((mDataLength & 8) != 0, FrameField::control, FrameBitType::dlc, 3, 0) ;
    enterBitAppendStuff ((mDataLength & 4) != 0, FrameField::control, FrameBitType::dlc, 2, 0) ;
    enterBitAppendStuff ((mDataLength & 2) != 0, FrameField::control, FrameBitType::dlc, 1, 0) ;
    enterBitAppendStuff ((mDataLength & 1) != 0, FrameField::control, FrameBitType::dlc, 0, 0) ;
  //--- Enter DATA
    if (mFrameType == FrameType::data) {
      const uint8_t maxLength = (mDataLength > 8) ? 8 : mDataLength ;
      for (uint8_t dataIdx = 0 ; dataIdx < maxLength ; dataIdx ++) {
        for (int bitIdx = 7 ; bitIdx >= 0 ; bitIdx--) {
          enterBitAppendStuff ((mData [dataIdx] & (1 << bitIdx)) != 0, FrameField::data, FrameBitType::data, dataIdx, uint8_t (bitIdx)) ;
        }
      }
    }
  //--- Enter CRC SEQUENCE
    mFrameCRC = mCRCAccumulator ;
    for (int idx = 14 ; idx >= 0 ; idx--) {
      const bool bit = (mFrameCRC & (1 << idx)) != 0 ;
      enterBitAppendStuff (bit, FrameField::crc, FrameBitType::crc, uint8_t (idx), 0) ;
    }
  //--- Enter ACK, EOF, INTERMISSION
    enterBitNoStuff (true, FrameField::crc, FrameBitType::crcDelimiter, 0) ;
    switch (mAckSlot) {
    case AckSlot::dominant :
      enterBitNoStuff (false, FrameField::ack, FrameBitType::dominantAckSlot, 0) ;
      break ;
    case AckSlot::recessive :
      enterBitNoStuff (true, FrameField::ack, FrameBitType::recessiveAckSlot, 0) ;
      break ;
    }
    enterBitNoStuff (true, FrameField::ack, FrameBitType::ackDelimiter, 0) ;
    for (uint8_t i=0 ; i<7 ; i++) {
      enterBitNoStuff (true, FrameField::eof, FrameBitType::eof, i) ;
    }
    for (uint8_t i=0 ; i<3 ; i++) {
      enterBitNoStuff (true, FrameField::intermission, FrameBitType::intermission, i) ;
    }
  //---
    #ifndef ARDUINO
      if (mCRCAccumulator != 0) {
        std::cout << "CRC error for idf 0x" << std::hex << mIdentifier << ": "
                  << mCRCAccumulator << std::dec << std::endl ;
        exit (1) ;
      }
    #endif
  }

//--- Public methods
  public: inline uint32_t identifier (void) const { return mIdentifier ; }

  public: inline bool extendedFrame (void) const { return mFrameFormat == FrameFormat::extended ; }

  public: inline bool remoteFrame (void) const { return mFrameType == FrameType::remote ; }

  public: inline uint8_t dataLength (void) const { return mDataLength ; }

  public: inline uint8_t dataAtIndex (const uint32_t inDataIndex) const { return mData [inDataIndex] ; }

  public: inline uint32_t frameLength (void) const { return mFrameLength ; }

  public: inline uint32_t stuffBitCount (void) const { return mStuffBitCount ; }

  public: inline uint16_t frameCRC (void) const { return mFrameCRC ; }

  public: bool bitAtIndex (const uint32_t inIndex) const {
    bool result = true ; // RECESSIF
    if (inIndex < mFrameLength) {
      const uint32_t idx = inIndex / 32 ;
      const uint32_t offset = inIndex % 32 ;
      result = (mBits [idx] & (1U << offset)) != 0 ;
    }
    return result ;
  }


  public: String nameAtIndex (const uint32_t inIndex) const {
    String result = "IDLE" ;
    if (inIndex < mFrameLength) {
      result = mFrame [inIndex].name () ;
    }
    return result ;
  }

  public: String fieldTypeAtIndex (const uint32_t inIndex) const {
    String result ;
    if (inIndex < mFrameLength) {
      result = mFrame [inIndex].fieldName () ;
    }
    return result ;
  }


  public: uint32_t hammingDistance (const CANFrameGenerator & inFrame) const {
    uint32_t result = 0 ;
    for (int i=0 ; i<5 ; i++) {
      uint32_t x = mBits [i] ^ inFrame.mBits [i] ;
      while (x > 0) {
        if ((x & 1) != 0) {
          result += 1 ;
        }
        x >>= 1 ;
      }
    }
    return result ;
  }


//--- Private methods (used during frame generation)
  private: void enterBitAppendStuff (const bool inBit,
                                     const FrameField inFieldType,
                                     const FrameBitType inBitType,
                                     const uint8_t inIndex0,
                                     const uint8_t inIndex1) {
  //--- Compute CRC
    const bool bit14 = (mCRCAccumulator & (1 << 14)) != 0 ;
    const bool crc_nxt = inBit ^ bit14 ;
    mCRCAccumulator <<= 1 ;
    mCRCAccumulator &= 0x7FFF ;
    if (crc_nxt) {
      mCRCAccumulator ^= 0x4599 ;
    }
  //--- Emit bit
    if (!inBit) {
      const uint32_t idx = mFrameLength / 32 ;
      const uint32_t offset = mFrameLength % 32 ;
      mBits [idx] &= ~ (1U << offset) ;
    }
    mFrame [mFrameLength] = cFrameBit (inFieldType, inBitType, inIndex0, inIndex1) ;
    mFrameLength ++ ;
  //--- Add a stuff bit ?
    if (mLastBitValue == inBit) {
      mConsecutiveBitCount += 1 ;
      if (mConsecutiveBitCount == 5) {
        mStuffBitCount += 1 ;
        mLastBitValue ^= true ;
        if (!mLastBitValue) {
          const uint32_t idx = mFrameLength / 32 ;
          const uint32_t offset = mFrameLength % 32 ;
          mBits [idx] &= ~ (1U << offset) ;
        }
        mFrame [mFrameLength] = cFrameBit (inFieldType, FrameBitType::stuff, 0, 0) ;
        mFrameLength ++ ;
        mConsecutiveBitCount = 1 ;
      }
    }else{
      mLastBitValue = inBit ;
      mConsecutiveBitCount = 1 ;
    }
  }

 private: void enterBitNoStuff (const bool inBit,
                                const FrameField inFieldType,
                                const FrameBitType inBitType,
                                const uint8_t inIndex0) {
  //--- Emit bit
    if (!inBit) {
      const uint32_t idx = mFrameLength / 32 ;
      const uint32_t offset = mFrameLength % 32 ;
      mBits [idx] &= ~ (1U << offset) ;
    }
    mFrame [mFrameLength] = cFrameBit (inFieldType, inBitType, inIndex0, 0) ;
    mFrameLength ++ ;
  }

//--- Distance de Hamming avec une autre trame CAN
  public: uint32_t hammingDistanceFrom (const CANFrameGenerator & inFrame) const {
    uint32_t result = 0 ;
    for (int i=0 ; i<5 ; i++) {
      uint32_t x = mBits [i] ^ inFrame.mBits [i] ;
      while (x > 0) {
        if ((x & 1) != 0) {
          result += 1 ;
        }
        x >>= 1 ;
      }
    }
    return result ;
  }

//--- Private properties
  private: cFrameBit mFrame [CAN_FRAME_MAX_LENGTH] ;
  private: uint8_t mData [8] ;
  private: uint32_t mBits [5] ;
  private: const uint32_t mIdentifier ;
  private: uint16_t mFrameCRC ;
  private: uint8_t mStuffBitCount ;
  private: const uint8_t mDataLength ;
  private: uint8_t mFrameLength ;
  private: const FrameFormat mFrameFormat ;
  private: const FrameType mFrameType ;
  private: const AckSlot mAckSlot ;

  private : uint32_t mConsecutiveBitCount ;
  private : bool mLastBitValue ;
  private : uint16_t mCRCAccumulator ;
} ;

//----------------------------------------------------------------------------------------
