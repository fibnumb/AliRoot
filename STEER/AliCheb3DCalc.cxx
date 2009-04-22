/**************************************************************************
 * Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
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


// Author: ruben.shahoyan@cern.ch   09/09/2006
//
#include <cstdlib>
#include "AliCheb3DCalc.h"

ClassImp(AliCheb3DCalc)

//__________________________________________________________________________________________
AliCheb3DCalc::AliCheb3DCalc() :
  fNCoefs(0), 
  fNRows(0), 
  fNCols(0), 
  fNElemBound2D(0), 
  fNColsAtRow(0), 
  fColAtRowBg(0),
  fCoefBound2D0(0), 
  fCoefBound2D1(0), 
  fCoefs(0), 
  fTmpCf1(0), 
  fTmpCf0(0)
{}

//__________________________________________________________________________________________
AliCheb3DCalc::AliCheb3DCalc(const AliCheb3DCalc& src) :
  TNamed(src), 
  fNCoefs(src.fNCoefs), 
  fNRows(src.fNRows), 
  fNCols(src.fNCols),
  fNElemBound2D(src.fNElemBound2D), 
  fNColsAtRow(0), 
  fColAtRowBg(0), 
  fCoefBound2D0(0), 
  fCoefBound2D1(0), 
  fCoefs(0), 
  fTmpCf1(0), 
  fTmpCf0(0)
{
  if (src.fNColsAtRow) {
    fNColsAtRow = new Int_t[fNRows]; 
    for (int i=fNRows;i--;) fNColsAtRow[i] = src.fNColsAtRow[i];
  }
  if (src.fColAtRowBg) {
    fColAtRowBg = new Int_t[fNRows]; 
    for (int i=fNRows;i--;) fColAtRowBg[i] = src.fColAtRowBg[i];
  }
  if (src.fCoefBound2D0) {
    fCoefBound2D0 = new Int_t[fNElemBound2D];
    for (int i=fNElemBound2D;i--;) fCoefBound2D0[i] = src.fCoefBound2D0[i];
  }
  if (src.fCoefBound2D1) {
    fCoefBound2D1 = new Int_t[fNElemBound2D];
    for (int i=fNElemBound2D;i--;) fCoefBound2D1[i] = src.fCoefBound2D1[i];
  }
  if (src.fCoefs) {
    fCoefs = new Float_t[fNCoefs];
    for (int i=fNCoefs;i--;) fCoefs[i] = src.fCoefs[i];
  }
  if (src.fTmpCf1) fTmpCf1 = new Float_t[fNCols];
  if (src.fTmpCf0) fTmpCf0 = new Float_t[fNRows];
}

//__________________________________________________________________________________________
AliCheb3DCalc::AliCheb3DCalc(FILE* stream) :
  fNCoefs(0), 
  fNRows(0), 
  fNCols(0), 
  fNElemBound2D(0), 
  fNColsAtRow(0), 
  fColAtRowBg(0), 
  fCoefBound2D0(0), 
  fCoefBound2D1(0), 
  fCoefs(0), 
  fTmpCf1(0), 
  fTmpCf0(0)
{
  LoadData(stream);
}

//__________________________________________________________________________________________
AliCheb3DCalc& AliCheb3DCalc::operator=(const AliCheb3DCalc& rhs)
{
  if (this != &rhs) {
    Clear();
    SetName(rhs.GetName());
    SetTitle(rhs.GetTitle());
    fNCoefs = rhs.fNCoefs;
    fNRows  = rhs.fNRows;
    fNCols  = rhs.fNCols;    
    if (rhs.fNColsAtRow) {
      fNColsAtRow = new Int_t[fNRows]; 
      for (int i=fNRows;i--;) fNColsAtRow[i] = rhs.fNColsAtRow[i];
    }
    if (rhs.fColAtRowBg) {
      fColAtRowBg = new Int_t[fNRows]; 
      for (int i=fNRows;i--;) fColAtRowBg[i] = rhs.fColAtRowBg[i];
    }
    if (rhs.fCoefBound2D0) {
      fCoefBound2D0 = new Int_t[fNElemBound2D];
      for (int i=fNElemBound2D;i--;) fCoefBound2D0[i] = rhs.fCoefBound2D0[i];
    }
    if (rhs.fCoefBound2D1) {
      fCoefBound2D1 = new Int_t[fNElemBound2D];
      for (int i=fNElemBound2D;i--;) fCoefBound2D1[i] = rhs.fCoefBound2D1[i];
    }
    if (rhs.fCoefs) {
      fCoefs = new Float_t[fNCoefs];
      for (int i=fNCoefs;i--;) fCoefs[i] = rhs.fCoefs[i];
    }
    if (rhs.fTmpCf1) fTmpCf1 = new Float_t[fNCols];
    if (rhs.fTmpCf0) fTmpCf0 = new Float_t[fNRows];    
  }
  return *this;
}

//__________________________________________________________________________________________
void AliCheb3DCalc::Clear(Option_t*)
{
  // delete all dynamycally allocated structures
  if (fTmpCf1)       { delete[] fTmpCf1;  fTmpCf1 = 0;}
  if (fTmpCf0)       { delete[] fTmpCf0;  fTmpCf0 = 0;}
  if (fCoefs)        { delete[] fCoefs;   fCoefs  = 0;}
  if (fCoefBound2D0) { delete[] fCoefBound2D0; fCoefBound2D0 = 0; }
  if (fCoefBound2D1) { delete[] fCoefBound2D1; fCoefBound2D1 = 0; }
  if (fNColsAtRow)   { delete[] fNColsAtRow;   fNColsAtRow = 0; }
  if (fColAtRowBg)   { delete[] fColAtRowBg;   fColAtRowBg = 0; }
  //
}

//__________________________________________________________________________________________
void AliCheb3DCalc::Print(Option_t* ) const
{
  printf("Chebyshev parameterization data %s for 3D->1 function.\n",GetName());
  int nmax3d = 0; 
  for (int i=fNElemBound2D;i--;) if (fCoefBound2D0[i]>nmax3d) nmax3d = fCoefBound2D0[i];
  printf("%d coefficients in %dx%dx%d matrix\n",fNCoefs,fNRows,fNCols,nmax3d);
  //
}

//__________________________________________________________________________________________
Float_t  AliCheb3DCalc::Eval(Float_t  *par) const
{
  // evaluate Chebyshev parameterization for 3D function.
  // VERY IMPORTANT: par must contain the function arguments ALREADY MAPPED to [-1:1] interval
  Float_t  &z = par[2];
  Float_t  &y = par[1];
  Float_t  &x = par[0];
  //
  int ncfRC;
  if (!fNRows) return 0.;
  for (int id0=fNRows;id0--;) {
    int nCLoc = fNColsAtRow[id0];                   // number of significant coefs on this row
    int Col0  = fColAtRowBg[id0];                   // beginning of local column in the 2D boundary matrix
    for (int id1=nCLoc;id1--;) {
      int id = id1+Col0;
      fTmpCf1[id1] = (ncfRC=fCoefBound2D0[id]) ? ChebEval1D(z,fCoefs + fCoefBound2D1[id], ncfRC) : 0.0;
    }
    fTmpCf0[id0] = nCLoc>0 ? ChebEval1D(y,fTmpCf1,nCLoc):0.0;
  }
  return ChebEval1D(x,fTmpCf0,fNRows);
  //
}

//__________________________________________________________________________________________
Float_t  AliCheb3DCalc::EvalDeriv(int dim, Float_t  *par) const
{
  // evaluate Chebyshev parameterization derivative in given dimension  for 3D function.
  // VERY IMPORTANT: par must contain the function arguments ALREADY MAPPED to [-1:1] interval
  Float_t  &z = par[2];
  Float_t  &y = par[1];
  Float_t  &x = par[0];
  //
  int ncfRC;
  for (int id0=fNRows;id0--;) {
    int nCLoc = fNColsAtRow[id0];                   // number of significant coefs on this row
    if (!nCLoc) {fTmpCf0[id0]=0; continue;}
    // 
    int Col0  = fColAtRowBg[id0];                   // beginning of local column in the 2D boundary matrix
    for (int id1=nCLoc;id1--;) {
      int id = id1+Col0;
      if (!(ncfRC=fCoefBound2D0[id])) { fTmpCf1[id1]=0; continue;}
      if (dim==2) fTmpCf1[id1] = ChebEval1Deriv(z,fCoefs + fCoefBound2D1[id], ncfRC);
      else        fTmpCf1[id1] = ChebEval1D(z,fCoefs + fCoefBound2D1[id], ncfRC);
    }
    if (dim==1)   fTmpCf0[id0] = ChebEval1Deriv(y,fTmpCf1,nCLoc);
    else          fTmpCf0[id0] = ChebEval1D(y,fTmpCf1,nCLoc);
  }
  return (dim==0) ? ChebEval1Deriv(x,fTmpCf0,fNRows) : ChebEval1D(x,fTmpCf0,fNRows);
  //
}

//__________________________________________________________________________________________
Float_t  AliCheb3DCalc::EvalDeriv2(int dim1,int dim2, Float_t  *par) const
{
  // evaluate Chebyshev parameterization 2n derivative in given dimensions  for 3D function.
  // VERY IMPORTANT: par must contain the function arguments ALREADY MAPPED to [-1:1] interval
  Float_t  &z = par[2];
  Float_t  &y = par[1];
  Float_t  &x = par[0];
  //
  Bool_t same = dim1==dim2;
  int ncfRC;
  for (int id0=fNRows;id0--;) {
    int nCLoc = fNColsAtRow[id0];                   // number of significant coefs on this row
    if (!nCLoc) {fTmpCf0[id0]=0; continue;}
    //
    int Col0  = fColAtRowBg[id0];                   // beginning of local column in the 2D boundary matrix
    for (int id1=nCLoc;id1--;) {
      int id = id1+Col0;
      if (!(ncfRC=fCoefBound2D0[id])) { fTmpCf1[id1]=0; continue;}
      if (dim1==2||dim2==2) fTmpCf1[id1] = same ? ChebEval1Deriv2(z,fCoefs + fCoefBound2D1[id], ncfRC) 
			      :                   ChebEval1Deriv(z,fCoefs + fCoefBound2D1[id], ncfRC);
      else        fTmpCf1[id1] = ChebEval1D(z,fCoefs + fCoefBound2D1[id], ncfRC);
    }
    if (dim1==1||dim2==1) fTmpCf0[id0] = same ? ChebEval1Deriv2(y,fTmpCf1,nCLoc):ChebEval1Deriv(y,fTmpCf1,nCLoc);
    else                  fTmpCf0[id0] = ChebEval1D(y,fTmpCf1,nCLoc);
  }
  return (dim1==0||dim2==0) ? (same ? ChebEval1Deriv2(x,fTmpCf0,fNRows):ChebEval1Deriv(x,fTmpCf0,fNRows)) : ChebEval1D(x,fTmpCf0,fNRows);
  //
}

//_______________________________________________
#ifdef _INC_CREATION_ALICHEB3D_
void AliCheb3DCalc::SaveData(const char* outfile,Bool_t append) const
{
  // writes coefficients data to output text file, optionallt appending on the end of existing file
  TString strf = outfile;
  gSystem->ExpandPathName(strf);
  FILE* stream = fopen(strf,append ? "a":"w");
  SaveData(stream);
  fclose(stream);
  //
}
#endif

//_______________________________________________
#ifdef _INC_CREATION_ALICHEB3D_
void AliCheb3DCalc::SaveData(FILE* stream) const
{
  // writes coefficients data to existing output stream
  // Note: fNCols, fNElemBound2D and fColAtRowBg is not stored, will be computed on fly during the loading of this file
  fprintf(stream,"#\nSTART %s\n",GetName());
  fprintf(stream,"# Number of rows\n%d\n",fNRows);
  //
  fprintf(stream,"# Number of columns per row\n");
  for (int i=0;i<fNRows;i++) fprintf(stream,"%d\n",fNColsAtRow[i]);
  //
  fprintf(stream,"# Number of Coefs in each significant block of third dimension\n");
  for (int i=0;i<fNElemBound2D;i++) fprintf(stream,"%d\n",fCoefBound2D0[i]);
  //
  fprintf(stream,"# Coefficients\n");
  for (int i=0;i<fNCoefs;i++) fprintf(stream,"%+.8e\n",fCoefs[i]);
  fprintf(stream,"END %s\n",GetName());
  //
}
#endif

//_______________________________________________
void AliCheb3DCalc::LoadData(FILE* stream)
{
  // Load coefs. from the stream 
  if (!stream) {Error("LoadData","No stream provided.\nStop"); exit(1);}
  TString buffs;
  Clear();
  ReadLine(buffs,stream);
  if (!buffs.BeginsWith("START")) {Error("LoadData","Expected: \"START <fit_name>\", found \"%s\"\nStop\n",buffs.Data());exit(1);}
  if (buffs.First(' ')>0) SetName(buffs.Data()+buffs.First(' ')+1);
  //
  ReadLine(buffs,stream); // NRows
  fNRows = buffs.Atoi(); 
  if (fNRows<1) {Error("LoadData","Expected: '<number_of_rows>', found \"%s\"\nStop\n",buffs.Data());exit(1);}
  //
  fNCols = 0;
  fNElemBound2D = 0;
  InitRows(fNRows);
  //
  for (int id0=0;id0<fNRows;id0++) {
    ReadLine(buffs,stream);               // n.cols at this row
    fNColsAtRow[id0] = buffs.Atoi();
    fColAtRowBg[id0] = fNElemBound2D;     // begining of this row in 2D boundary surface
    fNElemBound2D   += fNColsAtRow[id0];
    if (fNCols<fNColsAtRow[id0]) fNCols = fNColsAtRow[id0];
  }
  InitCols(fNCols);
  //  
  fNCoefs = 0; 
  InitElemBound2D(fNElemBound2D);
  //
  for (int i=0;i<fNElemBound2D;i++) {
    ReadLine(buffs,stream);               // n.coeffs at 3-d dimension for the given column/row
    fCoefBound2D0[i] = buffs.Atoi();
    fCoefBound2D1[i] = fNCoefs;
    fNCoefs += fCoefBound2D0[i];
  }
  //
  if (fNCoefs<=0) {Error("LoadData","Negtive (%d) number of Chebychef coeffs. is obtained.\nStop\n",fNCoefs);exit(1);}
  //
  InitCoefs(fNCoefs);
  for (int i=0;i<fNCoefs;i++) {
    ReadLine(buffs,stream);
    fCoefs[i] = buffs.Atof();
  }
  // check end_of_data record
  ReadLine(buffs,stream);
  if (!buffs.BeginsWith("END") || !buffs.Contains(GetName())) {
    Error("LoadData","Expected \"END %s\", found \"%s\".\nStop\n",GetName(),buffs.Data());
    exit(1);
  }
  //
}

//_______________________________________________
void AliCheb3DCalc::ReadLine(TString& str,FILE* stream) 
{
  // read single line from the stream, skipping empty and commented lines. EOF is not expected
  while (str.Gets(stream)) {
    str = str.Strip(TString::kBoth,' ');
    if (str.IsNull()||str.BeginsWith("#")) continue;
    return;
  }
  fprintf(stderr,"AliCheb3D::ReadLine: Failed to read from stream.\nStop");exit(1); // normally, should not reach here
}

//_______________________________________________
void AliCheb3DCalc::InitCols(int nc)
{
  // Set max.number of significant columns in the coefs matrix
  fNCols = nc;
  if (fTmpCf1) delete[] fTmpCf1;
  fTmpCf1 = new Float_t [fNCols];
}

//_______________________________________________
void AliCheb3DCalc::InitRows(int nr)
{
  // Set max.number of significant rows in the coefs matrix
  if (fNColsAtRow) delete[] fNColsAtRow;
  if (fColAtRowBg) delete[] fColAtRowBg;
  if (fTmpCf0)     delete[] fTmpCf0;
  fNRows = nr;
  fNColsAtRow = new Int_t[fNRows];
  fTmpCf0     = new Float_t [fNRows];
  fColAtRowBg = new Int_t[fNRows];
  for (int i=fNRows;i--;) fNColsAtRow[i] = fColAtRowBg[i] = 0;
}

//_______________________________________________
void AliCheb3DCalc::InitElemBound2D(int ne)
{
  // Set max number of significant coefs for given row/column of coefs 3D matrix
  if (fCoefBound2D0) delete[] fCoefBound2D0; 
  if (fCoefBound2D1) delete[] fCoefBound2D1; 
  fNElemBound2D = ne;
  fCoefBound2D0 = new Int_t[fNElemBound2D];
  fCoefBound2D1 = new Int_t[fNElemBound2D];
  for (int i=fNElemBound2D;i--;) fCoefBound2D0[i] = fCoefBound2D1[i] = 0;
}

//_______________________________________________
void AliCheb3DCalc::InitCoefs(int nc)
{
  // Set total number of significant coefs
  if (fCoefs) delete[] fCoefs; 
  fNCoefs = nc;
  fCoefs = new Float_t [fNCoefs];
  for (int i=fNCoefs;i--;) fCoefs[i] = 0.0;
}

//__________________________________________________________________________________________
Float_t AliCheb3DCalc::ChebEval1Deriv(Float_t  x, const Float_t * array, int ncf )
{
  // evaluate 1D Chebyshev parameterization's derivative. x is the argument mapped to [-1:1] interval
  if (--ncf<1) return 0;
  Float_t b0, b1, b2;
  Float_t x2 = x+x;
  b1 = b2 = 0;
  float dcf0=0,dcf1,dcf2=0;
  b0 = dcf1 = 2*ncf*array[ncf];
  if (!(--ncf)) return b0/2;
  //
  for (int i=ncf;i--;) {
    b2 = b1;      
    b1 = b0;
    dcf0 = dcf2 + 2*(i+1)*array[i+1];
    b0 = dcf0 + x2*b1 -b2;
    dcf2 = dcf1;  
    dcf1 = dcf0;
  }
  //
  return b0 - x*b1 - dcf0/2;
}

//__________________________________________________________________________________________
Float_t AliCheb3DCalc::ChebEval1Deriv2(Float_t  x, const Float_t * array, int ncf )
{
  // evaluate 1D Chebyshev parameterization's 2nd derivative. x is the argument mapped to [-1:1] interval
  if (--ncf<2) return 0;
  Float_t b0, b1, b2;
  Float_t x2 = x+x;
  b1 = b2 = 0;
  float dcf0=0,dcf1=0,dcf2=0;
  float ddcf0=0,ddcf1,ddcf2=0;
  //
  dcf2 = 2*ncf*array[ncf]; 
  --ncf; 

  dcf1 = 2*ncf*array[ncf]; 
  b0 = ddcf1 = 2*ncf*dcf2; 
  //
  if (!(--ncf)) return b0/2;
  //
  for (int i=ncf;i--;) {
    b2 = b1;                        
    b1 = b0;
    dcf0  = dcf2 + 2*(i+1)*array[i+1];
    ddcf0 = ddcf2 + 2*(i+1)*dcf1; 
    b0 = ddcf0 + x2*b1 -b2;
    //
    ddcf2 = ddcf1;  
    ddcf1 = ddcf0;
    //
    dcf2 = dcf1;
    dcf1 = dcf0;
    //
  }
  //
  return b0 - x*b1 - ddcf0/2;
}

