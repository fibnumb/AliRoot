#ifndef FMDV0_H
#define FMDV0_H
////////////////////////////////////////////////
//  Manager and hits classes for set:FMD     //
////////////////////////////////////////////////
 
#include "AliFMD.h"
 
class AliFMDv0 : public AliFMD {
  
public:
  AliFMDv0();
  AliFMDv0(const char *name, const char *title);
  virtual       ~AliFMDv0() {}
  virtual void   CreateGeometry();
  virtual void   CreateMaterials();
  virtual void   DrawDetector();
  virtual Int_t  IsVersion() const {return 0;}
 
  ClassDef(AliFMDv0,1)  //Class for FMD version 0
};

#endif
