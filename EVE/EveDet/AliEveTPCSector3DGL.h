// $Id$
// Main authors: Matevz Tadel & Alja Mrak-Tadel: 2006, 2007

/**************************************************************************
 * Copyright(c) 1998-2008, ALICE Experiment at CERN, all rights reserved. *
 * See http://aliceinfo.cern.ch/Offline/AliRoot/License.html for          *
 * full copyright notice.                                                 *
 **************************************************************************/

#ifndef AliEveTPCSector3DGL_H
#define AliEveTPCSector3DGL_H

#include <TGLObject.h>

#include <EveDet/AliEveTPCSectorData.h>

class TEveBoxSetGL;

class AliEveTPCSector3D;

//------------------------------------------------------------------------------
// AliEveTPCSector3DGL
//
// GL renderer for AliEveTPCSector3D.
//

class AliEveTPCSector3DGL : public TGLObject
{
  AliEveTPCSector3DGL(const AliEveTPCSector3DGL&);            // Not implemented
  AliEveTPCSector3DGL& operator=(const AliEveTPCSector3DGL&); // Not implemented

protected:
  AliEveTPCSector3D  *fSector; // Model object.
  TEveBoxSetGL       *fBoxRnr; // Box-set GL renderer.

  mutable UInt_t      fRTS;    // Rendering time-stamp.

  virtual void DirectDraw(TGLRnrCtx & rnrCtx) const;

public:
  AliEveTPCSector3DGL();
  virtual ~AliEveTPCSector3DGL();

  virtual Bool_t   ShouldDLCache(const TGLRnrCtx&) const { return kFALSE; }
  virtual ELODAxes SupportedLODAxes()              const { return kLODAxesAll; }
  virtual Short_t  QuantizeShapeLOD(Short_t shapeLOD, Short_t combiLOD) const;

  virtual Bool_t SetModel(TObject* obj, const Option_t* opt=0);
  virtual void   SetBBox();

  void DrawSegmentFrame(const AliEveTPCSectorData::SegmentInfo& s,
                        Int_t botExtraPads=0, Int_t topExtraPads=0) const;

  virtual Bool_t SupportsSecondarySelect() const { return kTRUE; }
  virtual void   ProcessSelection(TGLRnrCtx & rnrCtx, TGLSelectRecord & rec);

  ClassDef(AliEveTPCSector3DGL, 0); // GL renderer for AliEveTPCSector3D.
};

#endif
