/**************************************************************************
 * Copyright(c) 2004, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/
/* $Id$ */
/** @file    AliFMDBaseDigit.cxx
    @author  Christian Holm Christensen <cholm@nbi.dk>
    @date    Mon Mar 27 12:37:41 2006
    @brief   Digits for the FMD 
    @ingroup FMD_base
*/
//////////////////////////////////////////////////////////////////////
//
//  Digits classes for the FMD                
//
//  Digits consists of
//   - Detector #
//   - Ring ID                                             
//   - Sector #     
//   - Strip #
//   - ADC count in this channel                                  
//
//  Digits consists of
//   - Detector #
//   - Ring ID                                             
//   - Sector #     
//   - Strip #
//   - Total energy deposited in the strip
//   - ADC count in this channel                                  
//
// As the Digits and SDigits have so much in common, the classes
// AliFMDDigit and AliFMDSDigit are implemented via a base
// class AliFMDBaseDigit.
///
//              +-----------------+
//              | AliFMDBaseDigit |
//              +-----------------+
//                      ^
//                      |
//                +------------+
//                |            |
//      +-------------+ +--------------+
//      | AliFMDDigit |	| AliFMDSDigit |
//      +-------------+	+--------------+
//
// (Note, that I'd really would have liked to implement AliFMDHit as a
// derived class from some base class - say AliFMDStrip, and the Digit
// classes would (eventually) have derived from that as well.
// However, ROOT doesn't do well with multiple inheritance, so I chose
// not to anyway).
//
// Latest changes by Christian Holm Christensen
//
//////////////////////////////////////////////////////////////////////

#include "AliFMDBaseDigit.h"	// ALIFMDDIGIT_H
#include "Riostream.h"		// ROOT_Riostream
#include <TString.h>
// #include <AliLog.h>
#include "AliFMDDebug.h" // Better debug macros

//====================================================================
ClassImp(AliFMDBaseDigit)
#if 0
  ; // This is here to keep Emacs from indenting the next line
#endif

//____________________________________________________________________
AliFMDBaseDigit::AliFMDBaseDigit()
  : fDetector(0), 
    fRing('\0'), 
    fSector(0), 
    fStrip(0), 
    fName("")
{}

//____________________________________________________________________
AliFMDBaseDigit::AliFMDBaseDigit(UShort_t detector, 
				 Char_t   ring, 
				 UShort_t sector, 
				 UShort_t strip)
  : fDetector(detector), 
    fRing(ring), 
    fSector(sector), 
    fStrip(strip),
    fName("")
{
  //
  // Creates a base data digit object
  //
  // Parameters 
  //
  //    detector  Detector # (1, 2, or 3)                      
  //    ring	  Ring ID ('I' or 'O')
  //    sector	  Sector # (For inner/outer rings: 0-19/0-39)
  //    strip	  Strip # (For inner/outer rings: 0-511/0-255)
}

//____________________________________________________________________
void
AliFMDBaseDigit::Print(Option_t* /* option*/) const 
{
  // Print digit to standard out 
  cout << ClassName() << ": FMD" << GetName() << flush;
}

//____________________________________________________________________
const char*
AliFMDBaseDigit::GetName() const 
{ 
  // Get the name of a FMD digit.
  if (fName.IsNull()) 
    fName = Form("FMD%d%c[%2d,%3d]", fDetector, fRing, fSector, fStrip);
  return fName.Data();
}
#define fMaxStrips  512
#define fMaxSectors 40
#define fMaxRings   2

//____________________________________________________________________
ULong_t
AliFMDBaseDigit::Hash() const
{
  // Calculate a hash value based on the detector coordinates. 
  size_t ringi = (fRing == 'I' ||  fRing == 'i' ? 0 : 1);
  return fStrip + fMaxStrips * 
    (fSector + fMaxSectors * (ringi + fMaxRings * (fDetector - 1)));
}


//____________________________________________________________________
Int_t
AliFMDBaseDigit::Compare(const TObject* o) const
{
  // Compare to other digit.  If the passed pointer to TObject does
  // not point to an object of class AliFMDBaseDigit (or one of it's
  // derived classes), then a fatal exception is made. 
  // 
  // Returns -1, if this object's detector coordinates are smaller
  // than passed object's detector coordinates. 
  // 
  // Returns  0, if this object's detector coordinates is the same as
  // passed object's detector coordinates.  
  // 
  // Returns  1, if this object's detector coordinates are larger
  // than passed object's detector coordinates. 
  if (!o) 
    AliFatal("Can not compare to NULL!");
  if (o->IsA() != AliFMDBaseDigit::Class()) 
    AliFatal(Form("Cannot compare to a %s object", o->ClassName()));
  // AliFMDBaseDigit* of = static_cast<AliFMDBaseDigit*>(o);
  if (Hash() == o->Hash()) return 0;
  if (Hash() < o->Hash()) return -1;
  return 1;
}


//____________________________________________________________________
//
// EOF
//
