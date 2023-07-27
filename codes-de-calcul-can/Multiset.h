//
//  Multiset.h
//
//  Created by Pierre Molinaro on 14/09/2017.
//  Copyright © 2017 Pierre Molinaro. All rights reserved.
//
//----------------------------------------------------------------------------------------------------------------------

#pragma once

//----------------------------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdint.h>

//----------------------------------------------------------------------------------------------------------------------

class Multiset {
//--- Constructor
  public: Multiset (void)  :
  mFirstEntry (0),
  mEntryCount (0),
  mEntries (NULL) {
  }

//--- Destructor
  public: ~ Multiset (void)  {
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
  public: uint64_t count (const size_t inValue) const {
    uint64_t result = 0 ;
    if ((inValue >= mFirstEntry) && (inValue < (mFirstEntry + mEntryCount))) {
      result = mEntries [inValue - mFirstEntry] ;
    }
    return result ;
  }

//--- cbegin, cend
  public: inline uint32_t cbegin (void) const { return mFirstEntry ; }
  public: inline uint32_t cend (void) const { return mFirstEntry + mEntryCount ; }

//--- Private properties
  private: uint32_t mFirstEntry ;
  private: uint32_t mEntryCount ;
  private: uint64_t * mEntries ;

//--- No copy
  private: Multiset (const Multiset &) = delete ;
  private: Multiset & operator = (const Multiset &) = delete ;
} ;

//----------------------------------------------------------------------------------------------------------------------
