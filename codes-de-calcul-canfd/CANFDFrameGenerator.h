//
//  CANFDFrameGenerator.h
//
//  Created by Pierre Molinaro on 8/05/2020.
//  Copyright © 2020 Pierre Molinaro. All rights reserved.
//
//--------------------------------------------------------------------------------------------------

#pragma once

//--------------------------------------------------------------------------------------------------

#ifdef ARDUINO
  #include <Arduino.h>
  static inline String to_string (const unsigned inValue) { return String (inValue) ; }
#else
  #include <iostream>
  #include <iomanip>
  #include <string>
  typedef std::string String ;
  using namespace std ;
#endif

//--------------------------------------------------------------------------------------------------

static const uint32_t CANFD_FRAME_MAX_LENGTH = 960 ;

//--------------------------------------------------------------------------------------------------

enum class FrameFormat : uint8_t {standard, extended} ;

inline String frameFormatString (const FrameFormat inFormat) {
  return (inFormat == FrameFormat::standard) ? "standard" : "extended" ;
}

//--------------------------------------------------------------------------------------------------

enum class FrameProtocol : uint8_t {ISO, nonISO} ;

//--------------------------------------------------------------------------------------------------

enum class ESI : uint8_t {dominant, recessive} ;

//--------------------------------------------------------------------------------------------------

enum class AckSlot : uint8_t {dominant, recessive} ;

//--------------------------------------------------------------------------------------------------

enum class BitRateSwitch : uint8_t {no, yes} ;

//--------------------------------------------------------------------------------------------------

enum class BitDuration : uint8_t {arbitration, arbitrationToData, dataToArbitration, data} ;

inline String bitDurationString (const BitDuration inBitDuration) {
  String result = "A" ;
  switch (inBitDuration) {
  case BitDuration::arbitration :
    break ;
  case BitDuration::arbitrationToData :
    result = "AD" ;
    break ;
  case BitDuration::dataToArbitration :
    result = "DA" ;
    break ;
  case BitDuration::data :
    result = "D" ;
    break ;
  }
  return result ;
}

//--------------------------------------------------------------------------------------------------

enum class FrameFieldType : uint8_t {
  sof,
  arbitration,
  control,
  data,
  stuffCount,
  crc,
  ack,
  eof,
  intermission
} ;

//--------------------------------------------------------------------------------------------------

enum class FrameBitType : uint8_t {
  sof,
  identifier,
  data,
  crc,
  crcDel,
  recessiveACKSlot,
  dominantACKSlot,
  ackDel,
  eof,
  intermission,
  stuff,
  srr,
  ide,
  rtr,
  fdf,
  bsr,
  esi,
  fsb,
  parity,
  stuffCount,
  reserved,
  dlc
} ;

//--------------------------------------------------------------------------------------------------

class FrameBit {

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  public : FrameBit (void) :
  mFieldType (FrameFieldType::intermission),
  mBitType (FrameBitType::intermission),

  mDataIndex (0),
  mBitIndex (0) {
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  public : FrameBit (const bool inBitValue,
                     const FrameFieldType inType,
                     const FrameBitType inBitType,
                     const BitDuration inBitDuration,
                     const uint8_t inDataIndex,
                     const uint8_t inBitIndex) :
  mBitValue (inBitValue),
  mFieldType (inType),
  mBitType (inBitType),
  mBitDuration (inBitDuration),
  mDataIndex (inDataIndex),
  mBitIndex (inBitIndex) {
  }

  private: bool mBitValue ;
  private: FrameFieldType mFieldType ;
  private: FrameBitType mBitType;
  private: BitDuration mBitDuration ;
  private: uint8_t mDataIndex ;
  private: uint8_t mBitIndex ;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  public: bool bitValue (void) const { return mBitValue ; }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  public: BitDuration bitDuration (void) const { return mBitDuration ; }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  public: String name (void) const {
    String result ;
    switch (mBitType) {
    case FrameBitType::sof: result = "SOF" ; break ;
    case FrameBitType::identifier: result = "IDF" ; break ;
    case FrameBitType::data: result = "D" ; break ;
    case FrameBitType::crc: result = "CRC" ; break ;
    case FrameBitType::crcDel: result = "CRC DEL" ; break ;
    case FrameBitType::recessiveACKSlot: result = "ACK SLOT" ; break ;
    case FrameBitType::dominantACKSlot: result = "ACK SLOT*" ; break ;
    case FrameBitType::ackDel: result = "ACK DEL" ; break ;
    case FrameBitType::eof: result = "EOF" ; break ;
    case FrameBitType::intermission: result = "IFS" ; break ;
    case FrameBitType::stuff: result = "S" ; break ;
    case FrameBitType::srr: result = "SRR" ; break ;
    case FrameBitType::ide: result = "IDE" ; break ;
    case FrameBitType::rtr: result = "RTR" ; break ;
    case FrameBitType::fdf: result = "FDF" ; break ;
    case FrameBitType::bsr: result = "BSR" ; break ;
    case FrameBitType::esi: result = "ESI" ; break ;
    case FrameBitType::fsb: result = "FSB" ; break ;
    case FrameBitType::reserved: result = "R" ; break ;
    case FrameBitType::dlc: result = "DLC" ; break ;
    case FrameBitType::parity: result = "P" ; break ;
    case FrameBitType::stuffCount :  result = "SBC" ; break ;
    }
    switch (mBitType) {
    case FrameBitType::identifier:
    case FrameBitType::data:
    case FrameBitType::crc:
    case FrameBitType::eof:
    case FrameBitType::intermission:
    case FrameBitType::reserved:
    case FrameBitType::dlc:
    case FrameBitType::stuffCount:
      result.append (to_string (mDataIndex)) ;
      break ;
    case FrameBitType::crcDel:
    case FrameBitType::dominantACKSlot:
    case FrameBitType::recessiveACKSlot:
    case FrameBitType::ackDel:
    case FrameBitType::stuff:
    case FrameBitType::srr:
    case FrameBitType::ide:
    case FrameBitType::rtr:
    case FrameBitType::fdf:
    case FrameBitType::bsr:
    case FrameBitType::esi:
    case FrameBitType::sof:
    case FrameBitType::fsb:
    case FrameBitType::parity:
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
    case FrameFieldType::sof : result = "START OF FRAME" ; break ;
    case FrameFieldType::arbitration  : result = "ARBITRATION" ; break ;
    case FrameFieldType::control : result = "CONTROL" ; break ;
    case FrameFieldType::data : result = "DATA" ; break ;
    case FrameFieldType::stuffCount : result = "SBC" ; break ;
    case FrameFieldType::crc : result = "CRC" ; break ;
    case FrameFieldType::ack : result = "ACK" ; break ;
    case FrameFieldType::eof : result = "END OF FRAME" ; break ;
    case FrameFieldType::intermission : result = "INTERMISSION" ; break ;
    }
    return result ;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

} ;

//--------------------------------------------------------------------------------------------------

class CANFDFrameGenerator {

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //   Constructor
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  public : CANFDFrameGenerator (const uint32_t inIdentifier,
                                const FrameFormat inFrameFormat,
                                const BitRateSwitch inBitRateSwitch,
                                const ESI inESI,
                                const FrameProtocol inProtocol,
                                const uint8_t inDataLengthCode,
                                const uint8_t inData [64],
                                const AckSlot inAckSlot) :
  mBitArray (),
  mData (),
  mIdentifier (inIdentifier),
  mFrameCRC (0),
  mFrameLength (0),
  mCRCAccumulator17 (0),
  mCRCAccumulator21 (0),
  mStuffBitCount (0),
  mDataLengthCode (inDataLengthCode),
  mFrameFormat (inFrameFormat),
  mFrameProtocol (inProtocol),
  mESI (inESI),
  mAckSlot (inAckSlot),
  mLastBitValue (true),
  mConsecutiveBitCount (1) {
    const uint8_t dataByteCount = CANFDFrameGenerator::lengthForCode (mDataLengthCode) ;
    for (uint8_t i=0 ; i<dataByteCount ; i++) {
      mData [i] = inData [i] ;
    }
  //--- Change CRC initial value according to ISO
    switch (mFrameProtocol) {
    case FrameProtocol::nonISO :
      break ;
    case FrameProtocol::ISO :
      mCRCAccumulator17 = 1U << 16 ;
      mCRCAccumulator21 = 1U << 20 ;
      break ;
    }
  //--- Enter SOF
    enterBitComputeCRCAppendStuff (false, BitRateSwitch::no, FrameFieldType::sof, FrameBitType::sof, 0, 0) ;
  //--- Enter Identifier
    switch (mFrameFormat) {
    case FrameFormat::extended :
      for (uint8_t idx = 28 ; idx >= 18 ; idx--) { // Identifier
        const bool bit = (mIdentifier & (1 << idx)) != 0 ;
        enterBitComputeCRCAppendStuff (bit, BitRateSwitch::no, FrameFieldType::arbitration, FrameBitType::identifier, idx, 0) ;
      }
      enterBitComputeCRCAppendStuff (true, BitRateSwitch::no, FrameFieldType::arbitration, FrameBitType::srr, 0, 0) ; // SRR
      enterBitComputeCRCAppendStuff (true, BitRateSwitch::no, FrameFieldType::arbitration, FrameBitType::ide, 0, 0) ; // IDE
      for (int idx = 17 ; idx >= 0 ; idx--) { // Identifier
        const bool bit = (mIdentifier & (1 << idx)) != 0 ;
        enterBitComputeCRCAppendStuff (bit, BitRateSwitch::no, FrameFieldType::arbitration, FrameBitType::identifier, uint8_t (idx), 0) ;
      }
      break ;
    case FrameFormat::standard :
      for (int idx = 10 ; idx >= 0 ; idx--) { // Identifier
        const bool bit = (mIdentifier & (1 << idx)) != 0 ;
        enterBitComputeCRCAppendStuff (bit, BitRateSwitch::no, FrameFieldType::arbitration, FrameBitType::identifier, uint8_t (idx), 0) ;
      }
      enterBitComputeCRCAppendStuff (false, BitRateSwitch::no, FrameFieldType::arbitration, FrameBitType::reserved, 1, 0) ; // R1
      break ;
    }
  //--- Enter DLC
    enterBitComputeCRCAppendStuff (false, BitRateSwitch::no, FrameFieldType::control, FrameBitType::ide, 0, 0) ; // IDE
    enterBitComputeCRCAppendStuff (true, BitRateSwitch::no, FrameFieldType::control, FrameBitType::fdf, 0, 0) ; // FDF
    enterBitComputeCRCAppendStuff (false, BitRateSwitch::no, FrameFieldType::control, FrameBitType::reserved, 0, 0) ; // R0
    enterBitComputeCRCAppendStuff (inBitRateSwitch == BitRateSwitch::yes, inBitRateSwitch, FrameFieldType::control, FrameBitType::bsr, 0, 0) ; // BRS
    enterBitComputeCRCAppendStuff (mESI == ESI::recessive, inBitRateSwitch, FrameFieldType::control, FrameBitType::esi, 0, 0) ; // ESI
    enterBitComputeCRCAppendStuff ((mDataLengthCode & 8) != 0, inBitRateSwitch, FrameFieldType::control, FrameBitType::dlc, 3, 0) ;
    enterBitComputeCRCAppendStuff ((mDataLengthCode & 4) != 0, inBitRateSwitch, FrameFieldType::control, FrameBitType::dlc, 2, 0) ;
    enterBitComputeCRCAppendStuff ((mDataLengthCode & 2) != 0, inBitRateSwitch, FrameFieldType::control, FrameBitType::dlc, 1, 0) ;
    if (dataByteCount == 0) {
      enterBitInFrameComputeCRC ((mDataLengthCode & 1) != 0, inBitRateSwitch, FrameFieldType::control, FrameBitType::dlc, 0, 0) ;
    }else{
      enterBitComputeCRCAppendStuff ((mDataLengthCode & 1) != 0, inBitRateSwitch, FrameFieldType::control, FrameBitType::dlc, 0, 0) ;
    }
  //--- Enter DATA
    bool lastBit = mLastBitValue ;
    for (uint8_t dataIdx = 0 ; dataIdx < dataByteCount ; dataIdx ++) {
      for (int bitIdx = 7 ; bitIdx >= 0 ; bitIdx--) {
        lastBit = (inData [dataIdx] & (1 << bitIdx)) != 0 ;
        if ((dataIdx == (dataByteCount - 1)) && (bitIdx == 0)) { // Last data bit
          enterBitInFrameComputeCRC (lastBit, inBitRateSwitch, FrameFieldType::data, FrameBitType::data, dataIdx, uint8_t (bitIdx)) ;
        }else{
          enterBitComputeCRCAppendStuff (lastBit, inBitRateSwitch, FrameFieldType::data, FrameBitType::data, dataIdx, uint8_t (bitIdx)) ;
        }
      }
    }
  //--- Enter STUFF BIT COUNT
    if (mFrameProtocol == FrameProtocol::ISO) {
      enterBitInFrame (!lastBit, inBitRateSwitch, FrameFieldType::stuffCount, FrameBitType::fsb, 0, 0) ;
      const uint8_t GRAY_CODE_PARITY [8] = {0, 3, 6, 5, 12, 15, 10, 9} ;
      const uint8_t code = GRAY_CODE_PARITY [mStuffBitCount % 8] ;
      enterBitInFrameComputeCRC ((code & 8) != 0, inBitRateSwitch, FrameFieldType::stuffCount, FrameBitType::stuffCount, 2, 0) ;
      enterBitInFrameComputeCRC ((code & 4) != 0, inBitRateSwitch, FrameFieldType::stuffCount, FrameBitType::stuffCount, 1, 0) ;
      enterBitInFrameComputeCRC ((code & 2) != 0, inBitRateSwitch, FrameFieldType::stuffCount, FrameBitType::stuffCount, 0, 0) ;
      lastBit = (code & 1) != 0 ;
      enterBitInFrameComputeCRC (lastBit, inBitRateSwitch, FrameFieldType::stuffCount, FrameBitType::parity, 0, 0) ;
    }
  //--- Enter CRC SEQUENCE
    enterBitInFrame (!lastBit, inBitRateSwitch, FrameFieldType::crc, FrameBitType::fsb, 0, 0) ;
    const uint32_t frameCRC = (mDataLengthCode > 10) ? mCRCAccumulator21 : mCRCAccumulator17 ;
    mFrameCRC = frameCRC ;
    const int crcFirstBitIndex = (mDataLengthCode > 10) ? 20 : 16 ;
    uint32_t bitCount = 0 ;
    for (int idx = crcFirstBitIndex ; idx >= 0 ; idx--) {
      const bool crc_bit = (frameCRC & (1 << idx)) != 0 ;
      enterBitInFrame (crc_bit, inBitRateSwitch, FrameFieldType::crc, FrameBitType::crc, uint8_t (idx), 0) ;
      bitCount += 1 ;
      if (bitCount == 4) {
        bitCount = 0 ;
        enterBitInFrame (!crc_bit, inBitRateSwitch, FrameFieldType::crc, FrameBitType::fsb, 0, 0) ;
      }
    }
    enterBitInFrame (true, inBitRateSwitch, FrameFieldType::crc, FrameBitType::crcDel, 0, 0) ; // CRC DEL
  //--- Enter ACK, EOF, INTERMISSION
    switch (mAckSlot) {
    case AckSlot::dominant :
      enterBitInFrame (false, BitRateSwitch::no, FrameFieldType::ack, FrameBitType::dominantACKSlot, 0, 0) ;
      break ;
    case AckSlot::recessive :
      enterBitInFrame (true, BitRateSwitch::no, FrameFieldType::ack, FrameBitType::recessiveACKSlot, 0, 0) ;
      break ;
    }
    enterBitInFrame (true, BitRateSwitch::no, FrameFieldType::ack, FrameBitType::ackDel, 0, 0) ;
    for (uint8_t i=0 ; i<7 ; i++) {
      enterBitInFrame (true, BitRateSwitch::no, FrameFieldType::eof, FrameBitType::eof, i, 0) ;
    }
    for (uint8_t i=0 ; i<3 ; i++) {
      enterBitInFrame (true, BitRateSwitch::no, FrameFieldType::intermission, FrameBitType::intermission, i, 0) ;
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  public: inline ESI esi (void) const { return mESI ; }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  public: inline uint32_t frameLength (void) const { return mFrameLength ; }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  public: inline uint32_t stuffBitCount (void) const { return mStuffBitCount ; }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  public: inline uint32_t frameCRC (void) const { return mFrameCRC ; }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  public: bool bitAtIndex (const uint32_t inIndex) const {
    bool result = true ; // RECESSIF
    if (inIndex < mFrameLength) {
      result = mBitArray [inIndex].bitValue () ;
    }
    return result ;
  }

   // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  public: BitDuration bitDurationAtIndex (const uint32_t inIndex) const {
    BitDuration result = BitDuration::arbitration ;
    if (inIndex < mFrameLength) {
      result = mBitArray [inIndex].bitDuration () ;
    }
    return result ;
  }

 // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  public: String nameAtIndex (const uint32_t inIndex) const {
    String result = "IDLE" ;
    if (inIndex < mFrameLength) {
      result = mBitArray [inIndex].name () ;
    }
    return result ;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  public: String fieldTypeAtIndex (const uint32_t inIndex) const {
    String result ;
    if (inIndex < mFrameLength) {
      result = mBitArray [inIndex].fieldName () ;
    }
    return result ;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  public: uint32_t hammingDistanceFrom (const CANFDFrameGenerator & inFrame) const {
    uint32_t result = 0 ;
    const uint32_t minLength = (mFrameLength < inFrame.mFrameLength)
      ? mFrameLength
      : inFrame.mFrameLength
    ;
    for (uint32_t i=0 ; i<minLength ; i++) {
      result += (mBitArray [i].bitValue () != inFrame.mBitArray [i].bitValue ()) ;
    }
    return result ;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  public: inline uint8_t dataLengthCode (void) const { return mDataLengthCode ; }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  public: inline uint8_t dataAtIndex (const uint32_t inIndex) const { return mData [inIndex] ; }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  public: inline uint32_t identifier (void) const { return mIdentifier ; }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  #ifndef ARDUINO
    public: void display (void) const {
      for (uint32_t i=0 ; i<mFrameLength ; i++) {
        std::cout << setw (9) << nameAtIndex (i)
                  << " " << bitAtIndex (i)
                  << " " << bitDurationString (bitDurationAtIndex (i))
                  << " " << fieldTypeAtIndex (i)
                  << endl ;
      }
    }
  #endif

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  private: void enterBitComputeCRCAppendStuff (const bool inBit,
                                               const BitRateSwitch inBitRateSwitch,
                                               const FrameFieldType inFieldType,
                                               const FrameBitType inBitType,
                                               const uint8_t inIndex0,
                                               const uint8_t inIndex1) {
  //--- Enter bit in frame
    enterBitInFrameComputeCRC (inBit, inBitRateSwitch, inFieldType, inBitType, inIndex0, inIndex1) ;
  //--- Add a stuff bit ?
    if (mLastBitValue == inBit) {
      mConsecutiveBitCount += 1 ;
      if (mConsecutiveBitCount == 5) {
        mConsecutiveBitCount = 1 ;
        mStuffBitCount += 1 ;
        mLastBitValue ^= true ;
        enterBitInFrameComputeCRC (mLastBitValue, inBitRateSwitch, inFieldType, FrameBitType::stuff, 0, 0) ;
      }
    }else{
      mLastBitValue = inBit ;
      mConsecutiveBitCount = 1 ;
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  private: void enterBitInFrame (const bool inBit,
                                 const BitRateSwitch inBitRateSwitch,
                                 const FrameFieldType inFieldType,
                                 const FrameBitType inBitType,
                                 const uint8_t inIndex0,
                                 const uint8_t inIndex1) {
    BitDuration bitDuration = BitDuration::arbitration ;
    if (inBitRateSwitch == BitRateSwitch::yes) {
      switch (inBitType) {
      case FrameBitType::sof :
      case FrameBitType::identifier :
      case FrameBitType::recessiveACKSlot :
      case FrameBitType::dominantACKSlot :
      case FrameBitType::ackDel :
      case FrameBitType::eof :
      case FrameBitType::intermission :
      case FrameBitType::fdf :
      case FrameBitType::srr :
      case FrameBitType::ide :
      case FrameBitType::rtr :
      case FrameBitType::reserved :
        break ;
      case FrameBitType::bsr :
        bitDuration = BitDuration::arbitrationToData ;
        break ;
      case FrameBitType::esi :
      case FrameBitType::dlc :
      case FrameBitType::data :
      case FrameBitType::fsb :
      case FrameBitType::crc :
      case FrameBitType::stuff :
      case FrameBitType::stuffCount :
      case FrameBitType::parity :
        bitDuration = BitDuration::data ;
        break ;
      case FrameBitType::crcDel :
        bitDuration = BitDuration::dataToArbitration ;
        break ;
      }
    }
    mBitArray [mFrameLength] = FrameBit (inBit, inFieldType, inBitType, bitDuration, inIndex0, inIndex1) ;
    mFrameLength += 1 ;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  private: void enterBitInFrameComputeCRC (const bool inBit,
                                           const BitRateSwitch inBitRateSwitch,
                                           const FrameFieldType inFieldType,
                                           const FrameBitType inBitType,
                                           const uint8_t inIndex0,
                                           const uint8_t inIndex1) {
  //--- Enter bit in frame
    enterBitInFrame (inBit, inBitRateSwitch, inFieldType, inBitType, inIndex0, inIndex1) ;
  //--- Enter in CRC17
    const bool bit16 = (mCRCAccumulator17 & (1U << 16)) != 0 ;
    const bool crc17_nxt = inBit ^ bit16 ;
    mCRCAccumulator17 <<= 1 ;
    mCRCAccumulator17 &= 0x1FFFF ;
    if (crc17_nxt) {
      mCRCAccumulator17 ^= 0x1685B ;
    }
  //--- Enter in CRC21
    const bool bit20 = (mCRCAccumulator21 & (1U << 20)) != 0 ;
    const bool crc21_nxt = inBit ^ bit20 ;
    mCRCAccumulator21 <<= 1 ;
    mCRCAccumulator21 &= 0x1FFFFF ;
    if (crc21_nxt) {
      mCRCAccumulator21 ^= 0x102899 ;
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  public: static uint8_t lengthForCode (const uint8_t inDataLengthCode) {
    const uint8_t LENGTH [16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20, 24, 32, 48, 64} ;
    return LENGTH [inDataLengthCode] ;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //  Latex display: comparing two frames
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  #ifndef ARDUINO
    public: void latexDisplay2 (const CANFDFrameGenerator & rightFrame,
                                const uint32_t inRightFrameOffset = 0) const {
      const uint32_t minLength = (frameLength () < (rightFrame.frameLength () + inRightFrameOffset))
        ? frameLength ()
        : (rightFrame.frameLength () + inRightFrameOffset)
      ;
      for (uint32_t i=0 ; i<inRightFrameOffset ; i++) {
        cout << ""
            << " & \\tt " << nameAtIndex (i)
            << " & \\tt " << bitAtIndex (i)
            << " & \\tt " << bitDurationString (bitDurationAtIndex (i))
            << " & \\tt " << fieldTypeAtIndex (i)
            << " & \\tt " << i
            << " & & & &"
            << " \\\\" << endl ;
      }
      for (uint32_t i=inRightFrameOffset ; i<minLength ; i++) {
        const bool equal = bitAtIndex (i) == rightFrame.bitAtIndex (i -inRightFrameOffset) ;
        cout << (equal ? "" : "$\\rightarrow$")
            << " & \\tt " << nameAtIndex (i)
            << " & \\tt " << bitAtIndex (i)
            << " & \\tt " << bitDurationString (bitDurationAtIndex (i))
            << " & \\tt " << fieldTypeAtIndex (i)
            << " & \\tt " << i
            << " & \\tt " << rightFrame.nameAtIndex (i- inRightFrameOffset)
            << " & \\tt " << rightFrame.bitAtIndex (i - inRightFrameOffset)
            << " & \\tt " << bitDurationString (rightFrame.bitDurationAtIndex (i - inRightFrameOffset))
            << " & \\tt " << rightFrame.fieldTypeAtIndex (i - inRightFrameOffset)
            << " \\\\" << endl ;
      }
      for (uint32_t i=(rightFrame.frameLength () +inRightFrameOffset) ; i<frameLength () ; i++) {
        cout << " & \\tt " << nameAtIndex (i)
            << " & \\tt " << bitAtIndex (i)
            << " & \\tt " << bitDurationString (bitDurationAtIndex (i))
            << " & \\tt " << fieldTypeAtIndex (i)
            << " & \\tt " << i
            << " & & & &"
            << " \\\\" << endl ;
      }
      for (uint32_t i=frameLength () ; i<(rightFrame.frameLength () + inRightFrameOffset) ; i++) {
        cout << " & & & &"
            << " & \\tt " << i
            << " & \\tt " << rightFrame.nameAtIndex (i)
            << " & \\tt " << rightFrame.bitAtIndex (i)
            << " & \\tt " << bitDurationString (rightFrame.bitDurationAtIndex (i))
            << " & \\tt " << rightFrame.fieldTypeAtIndex (i)
            << " \\\\" << endl ;
      }
    }
  #endif

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //--- Private properties

  private: FrameBit mBitArray [CANFD_FRAME_MAX_LENGTH] ;
  private: uint8_t mData [64] ;
  private: const uint32_t mIdentifier ;
  private: uint32_t mFrameCRC ;
  private: uint32_t mFrameLength ;
  private: uint32_t mCRCAccumulator17 ;
  private: uint32_t mCRCAccumulator21 ;
  private: uint8_t mStuffBitCount ;
  private: const uint8_t mDataLengthCode ;
  private: const FrameFormat mFrameFormat ;
  private: const FrameProtocol mFrameProtocol ;
  private: const ESI mESI ;
  private: const AckSlot mAckSlot ;
  private: bool mLastBitValue ;
  private: uint8_t mConsecutiveBitCount ;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

} ;

//--------------------------------------------------------------------------------------------------
