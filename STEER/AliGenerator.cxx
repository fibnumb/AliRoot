///////////////////////////////////////////////////////////////////
//                                                               //
//    Generate the final state of the interaction as the input   //
//    to the MonteCarlo                                          //
//
//Begin_Html
/*
<img src="gif/AliGeneratorClass.gif">
</pre>
<br clear=left>
<font size=+2 color=red>
<p>The responsible person for this module is
<a href="mailto:andreas.morsch@cern.ch">Andreas Morsch</a>.
</font>
<pre>
*/
//End_Html
//                                                               //
///////////////////////////////////////////////////////////////////

#include "AliGenerator.h"
#include "AliRun.h"

ClassImp(AliGenerator)

TGenerator* AliGenerator::fgMCEvGen=0;

//____________________________________________________________
AliGenerator::AliGenerator()
{
    gAlice->SetGenerator(this);
    SetThetaRange();
    SetPhiRange();
    SetMomentumRange();
    SetPtRange();
    SetYRange();
    SetNumberParticles();
  //
  //  fName="Default";
  //  fTitle="Base Generator Class";
  //
    fOrigin.Set(3);
    fOsigma.Set(3);
    fOrigin[0]=fOrigin[1]=fOrigin[2]=0;
    fOsigma[0]=fOsigma[1]=fOsigma[2]=0;
}

//____________________________________________________________
AliGenerator::AliGenerator(Int_t npart)
    : TNamed(" "," ")
{
    gAlice->SetGenerator(this);
    SetThetaRange();
    SetPhiRange();
    SetMomentumRange();
    SetPtRange();
    SetYRange();
    SetNumberParticles(npart);
  //
  //  fName="Default";
  //  fTitle="Base Generator Class";
  //
    fVertexSmear=none;
    fOrigin.Set(3);
    fOsigma.Set(3);
    fOrigin[0]=fOrigin[1]=fOrigin[2]=0;
    fOsigma[0]=fOsigma[1]=fOsigma[2]=0;
    fVMin.Set(3);
    fVMin[0]=fVMin[1]=fVMin[2]=0;
    fVMax.Set(3);
    fVMax[0]=fVMax[1]=fVMax[2]=10000;
}

//____________________________________________________________
AliGenerator::~AliGenerator()
{
  fOrigin.Set(0);
  fOsigma.Set(0);
  delete fgMCEvGen;
}

void AliGenerator::Init()
{   
}


