
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// AliDisplay                                                           //
//                                                                      //
// Utility class to display ALICE outline, tracks, hits,..              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include <TROOT.h>
#include <TTree.h>
#include <TButton.h>
#include <TCanvas.h>
#include <TView.h>
#include <TText.h>
#include <TPolyMarker3D.h>
#include <TPaveLabel.h>
#include <TPaveText.h>
#include <TList.h>
#include <TDiamond.h>
#include <TNode.h>
#include <TArc.h>
#include <TTUBE.h>
#include <TSlider.h>
#include <TSliderBox.h>
#include <TGaxis.h>
#include <TGXW.h>
#include <TMath.h>
#include <X3DBuffer.h>

#include "AliRun.h"
#include "AliDetector.h"
#include "AliDisplay.h"
#include "AliPoints.h"
#include "GParticle.h"

const Float_t ptcutmax  = 2;
const Float_t etacutmax = 1.5;

ClassImp(AliDisplay)


//_____________________________________________________________________________
AliDisplay::AliDisplay()
{
   fCanvas = 0;
}

//_____________________________________________________________________________
AliDisplay::AliDisplay(Int_t size)
{
// Create an event display object.
// A canvas named "edisplay" is created with a vertical size in pixels
//
//    A QUICK Overview of the Event Display functions
//    ===============================================
//
//  The event display can ve invoked by executing the macro "display.C"
// A canvas like in the picture below will appear.
//
//  On the left side of the canvas, the following buttons appear:
//   *Next*       to move to the next event
//   *Previous*   to move to the previous event
//   *Top View*   to display a top view of the current event
//   *Side View*  to display a side view of the current event
//   *Front View* to display a front view of the current event
//   *All Views*  to display front/side/top/30-30 views of the current event
//   *OpenGL*     to use OpenGl to view the current event.
//                Note that OpenGL cannot be used across the network.
//                Before using OpenGL, load the GL libraries
//                by executing the macro GL.C (in $ROOTSYS/macros/GL.C.
//                Once in GL, click the HELP button of the GL canvas.
//   *X3D*        to use X3D to view the current event (Unix only).
//                Once in X3D, type M to see the list of all possible options.
//                for example type J to zoom, K to unzoom
//                use the mouse to rotate.
//   *Pick*       Select this option to be able to point on a track with the
//                mouse. Once on the track, use the right button to select
//                an action. For example, select SetMarkerAttributes to
//                change the marker type/color/size for the track.
//   *Zoom*       Select this option (default) if you want to zoom.
//                To zoom, simply select the selected area with the left button.
//   *UnZoom*     To revert to the previous picture size.
//
//   slider R     On the left side, the vertical slider can be used to
//                set the default picture size.
//   slider pcut  At the top of the canvas, a slider can be used to change
//                the momentum cut (or range) to display tracks.
//   slider eta   On the right side of the canvas, a vertical slider can be used
//                to specify a rapidity range for the tracks.
//
//    When you are in Zoom mode, you can click on the black part of the canvas
//  to select special options with the right mouse button.
//  This will display a pop-up menu with items like:
//     *Disable detector* 
//     *Enable detector*, etc.
//  For example select "Disable detector". You get a dialog box.
//  Diable detector TRD for example.
//
//  When you are in pick mode, you can "Inspect" the object pointed by the mouse.
//  When you are on a track, select the menu item "InspectParticle"
//  to display the current particle attributes.
//
//  You can activate the Root browser by selecting the Inspect menu
//  in the canvas tool bar menu. Then select "Start Browser"
//  This will open a new canvas with the browser. At this point, you may want
//  to display some histograms (from the Trees). Go to the "File" menu
//  of the browser and click on "New canvas".
//  In the browser, click on item "ROOT files" in the left pane.
//  Click on galice.root.
//  Click on TH
//  Click on TPC for example
//  Click on any variable (eg TPC.fX) to histogram the variable.
//
//   If you are lost, you can click on HELP in any Root canvas or browser.
//Begin_Html
/*
<img src="gif/alidisplay.gif">
*/
//End_Html
   
   fPad = 0;
   gAlice->SetDisplay(this);
   
   // Initialize display default parameters
   SetRange();
   SetPTcut();

   // Set front view by default
   fTheta = 0;
   fPhi   = -90;
   fPsi   = 0;
   fDrawAllViews  = kFALSE;
   fDrawHits      = kTRUE;
   fDrawParticles = kTRUE;
   fZoomMode      = 1;
   fZooms         = 0;
   fHitsCuts      = 0;
   
   // Create display canvas
   Int_t ysize = size;
   if (ysize < 100) ysize = 750;
   Int_t xsize = Int_t(size*830./ysize);
   fCanvas = new TCanvas("Canvas", "ALICE Event Display",14,47,xsize,ysize);
   fCanvas->SetEditable(kIsNotEditable);
   fCanvas->ToggleEventStatus();
   
   // Create main display pad
   fPad = new TPad("viewpad", "Alice display",0.15,0,0.97,0.96);
   fPad->Draw();
   fPad->Modified();
   fPad->SetFillColor(1);
   fPad->SetBorderSize(2);

   // Create user interface control pad
   DisplayButtons();
   fCanvas->cd();

   // Create Range and mode pad
   Float_t dxtr     = 0.15;
   Float_t dytr     = 0.45;
   fTrigPad = new TPad("trigger", "range and mode pad",0,0,dxtr,dytr);
   fTrigPad->Draw();
   fTrigPad->cd();
   fTrigPad->SetFillColor(22);
   fTrigPad->SetBorderSize(2);
   fRangeSlider = new TSlider("range","range",0.7,0.42,0.9,0.98);
   fRangeSlider->SetObject(this);
   char pickmode[] = "gAlice->Display()->SetPickMode()";
   Float_t db = 0.09;
   fPickButton = new TButton("Pick",pickmode,0.05,0.32,0.65,0.32+db);
   fPickButton->SetFillColor(38);
   fPickButton->Draw();
   char zoommode[] = "gAlice->Display()->SetZoomMode()";
   fZoomButton = new TButton("Zoom",zoommode,0.05,0.21,0.65,0.21+db);
   fZoomButton->SetFillColor(38);
   fZoomButton->Draw();
   fArcButton = new TArc(.8,fZoomButton->GetYlowNDC()+0.5*db,0.33*db);
   fArcButton->SetFillColor(kGreen);
   fArcButton->Draw();
   char butUnzoom[] = "gAlice->Display()->UnZoom()";
   TButton *button = new TButton("UnZoom",butUnzoom,0.05,0.05,0.95,0.15);
   button->SetFillColor(38);
   button->Draw();
   AppendPad(); // append display object as last object to force selection

   // Create momentum cut slider pad
   fCanvas->cd();
   fCutPad = new TPad("cutSlider", "pcut slider pad",dxtr,.96,1,1);
   fCutPad->Draw();
   fCutPad->cd();
   fCutPad->SetFillColor(22);
   fCutPad->SetBorderSize(2);
   fCutSlider   = new TSlider("pcut","Momentum cut",0,0,1,1);
   fCutSlider->SetRange(fPTcut/ptcutmax,1);
   fCutSlider->SetObject(this);
   fCutSlider->SetFillColor(45);
   TSliderBox *sbox = (TSliderBox*)fCutSlider->GetListOfPrimitives()->First();
   sbox->SetFillColor(46);
   fCutSlider->cd();
   TGaxis *cutaxis = new TGaxis(0.02,0.8,0.98,0.8,0,ptcutmax,510,"");
   cutaxis->SetLabelSize(0.5);
   cutaxis->SetTitleSize(0.6);
   cutaxis->SetTitleOffset(0.5);
   cutaxis->SetTitle("pcut .  ");
   fCutSlider->GetListOfPrimitives()->AddFirst(cutaxis);

      // Create rapidity cut slider pad
   fCanvas->cd();
   fEtaPad = new TPad("EtaSlider", "Eta slider pad",0.97,0,1,0.96);
   fEtaPad->Draw();
   fEtaPad->cd();
   fEtaPad->SetFillColor(22);
   fEtaPad->SetBorderSize(2);
   fEtaSlider   = new TSlider("etacut","Rapidity cut",0,0,1,1);
   fEtaSlider->SetObject(this);
   fEtaSlider->SetFillColor(45);
   TSliderBox *sbox2 = (TSliderBox*)fEtaSlider->GetListOfPrimitives()->First();
   sbox2->SetFillColor(46);
   fEtaSlider->cd();
   TGaxis *etaaxis = new TGaxis(0.9,0.02,0.9,0.98,-etacutmax,etacutmax,510,"");
   etaaxis->SetLabelSize(0.5);
   etaaxis->SetTitleSize(0.6);
   etaaxis->SetTitleOffset(0.2);
   cutaxis->SetTitle("Etacut .  ");
   fEtaSlider->GetListOfPrimitives()->AddFirst(etaaxis);
   fCanvas->cd();
   

   fCanvas->cd();
   fCanvas->Update();
}


//_____________________________________________________________________________
AliDisplay::~AliDisplay()
{
}

//_____________________________________________________________________________
void AliDisplay::Clear(Option_t *)
{
//    Delete graphics temporary objects
}

//----------------------------------------------------------------------------
void AliDisplay::ShowTrack(Int_t idx) {
   AliDetector *TPC=(AliDetector*)gAlice->GetDetector("TPC");
   TObjArray *points=TPC->Points();
   int ntracks=points->GetEntriesFast();
   for (int track=0;track<ntracks;track++) {
      AliPoints *pm = (AliPoints*)points->UncheckedAt(track);
      if (!pm) continue;
      if (idx == pm->GetIndex()) {
         pm->SetMarkerColor(2);
         pm->SetMarkerStyle(22);
         pm->Draw("same");
//       fPad->Update();
//       fPad->Modified();
         TClonesArray *particles=gAlice->Particles();
         GParticle *p = (GParticle*)particles->UncheckedAt(idx);
         printf("\nTrack index %d\n",idx);
         printf("Particle ID %d\n",p->GetKF());
         printf("Parent %d\n",p->GetParent());
         printf("First child %d\n",p->GetFirstChild());
         printf("Px,Py,Pz %f %f %f\n",p->GetPx(),p->GetPy(),p->GetPz());
         return;
      }
   }
}

//----------------------------------------------------------------------------
void AliDisplay::HideTrack(Int_t idx) {
   AliDetector *TPC=(AliDetector*)gAlice->GetDetector("TPC");
   TObjArray *points=TPC->Points();
   int ntracks=points->GetEntriesFast();
   for (int track=0;track<ntracks;track++) {
      AliPoints *pm = (AliPoints*)points->UncheckedAt(track);
      if (!pm) continue;
      if (idx == pm->GetIndex()) {
         pm->SetMarkerColor(5);
         pm->SetMarkerStyle(1);
         pm->Draw("same");
//       fPad->Update();
//       fPad->Modified();
         return;
      }
   }
}

//_____________________________________________________________________________
void AliDisplay::DisableDetector(const char *name)
{
//    Disable detector name from graphics views
   
   AliDetector *detector = (AliDetector*)gAlice->Detectors()->FindObject(name);
   if (!detector) return;
   detector->Disable();
   Draw();
}

//_____________________________________________________________________________
void AliDisplay::DisplayButtons()
{
//    Create the user interface buttons

   fButtons = new TPad("buttons", "newpad",0,0.45,0.15,1);
   fButtons->Draw();
   fButtons->SetFillColor(38);
   fButtons->SetBorderSize(2);
   fButtons->cd();

   Int_t butcolor = 33;
   Float_t dbutton = 0.08;
   Float_t y  = 0.96;
   Float_t dy = 0.014;
   Float_t x0 = 0.05;
   Float_t x1 = 0.95;

   TButton *button;
   char but1[] = "gAlice->Display()->ShowNextEvent(1)";
   button = new TButton("Next",but1,x0,y-dbutton,x1,y);
   button->SetFillColor(38);
   button->Draw();

   y -= dbutton +dy;
   char but2[] = "gAlice->Display()->ShowNextEvent(-1)";
   button = new TButton("Previous",but2,x0,y-dbutton,x1,y);
   button->SetFillColor(38);
   button->Draw();

   y -= dbutton +dy;
   char but3[] = "gAlice->Display()->SetView(90,-90,90)";
   button = new TButton("Top View",but3,x0,y-dbutton,x1,y);
   button->SetFillColor(butcolor);
   button->Draw();

   y -= dbutton +dy;
   char but4[] = "gAlice->Display()->SetView(90,0,-90)";
   button = new TButton("Side View",but4,x0,y-dbutton,x1,y);
   button->SetFillColor(butcolor);
   button->Draw();

   y -= dbutton +dy;
   char but5[] = "gAlice->Display()->SetView(0,-90,0)";
   button = new TButton("Front View",but5,x0,y-dbutton,x1,y);
   button->SetFillColor(butcolor);
   button->Draw();

   y -= dbutton +dy;
   char but6[] = "gAlice->Display()->DrawAllViews()";
   button = new TButton("All Views",but6,x0,y-dbutton,x1,y);
   button->SetFillColor(butcolor);
   button->Draw();

   y -= dbutton +dy;
   char but7[] = "gAlice->Display()->DrawViewGL()";
   button = new TButton("OpenGL",but7,x0,y-dbutton,x1,y);
   button->SetFillColor(38);
   button->Draw();

   y -= dbutton +dy;
   char but8[] = "gAlice->Display()->DrawViewX3D()";
   button = new TButton("X3D",but8,x0,y-dbutton,x1,y);
   button->SetFillColor(38);
   button->Draw();

   // display logo
   TDiamond *diamond = new TDiamond(0.05,0.015,0.95,0.22);
   diamond->SetFillColor(50);
   diamond->SetTextAlign(22);
   diamond->SetTextColor(5);
   diamond->SetTextSize(0.11);
   diamond->Draw();
   diamond->AddText(".. ");
   diamond->AddText("ROOT");
   diamond->AddText("ALICE");
   diamond->AddText("... ");
   diamond->AddText(" ");
}

//______________________________________________________________________________
Int_t AliDisplay::DistancetoPrimitive(Int_t px, Int_t)
{
// Compute distance from point px,py to objects in event

   gPad->SetCursor(kCross);
   
   if (gPad == fTrigPad) return 9999;
   if (gPad == fCutPad)  return 9999;
   if (gPad == fEtaPad)  return 9999;

   const Int_t big = 9999;
   Int_t dist   = big;
   Float_t xmin = gPad->GetX1();
   Float_t xmax = gPad->GetX2();
   Float_t dx   = 0.02*(xmax - xmin);
   Float_t x    = gPad->AbsPixeltoX(px);
   if (x < xmin+dx || x > xmax-dx) return dist;

   if (fZoomMode) return 0;
   else           return 7;
}

//_____________________________________________________________________________
void AliDisplay::Draw(Option_t *)
{
//    Display current event

   if (fDrawAllViews) {
      DrawAllViews();
      return;
   }

   fPad->cd();

   DrawView(fTheta, fPhi, fPsi);

   // Display the event number and title
   fPad->cd();
   DrawTitle();
}

//_____________________________________________________________________________
void AliDisplay::DrawAllViews()
{
//    Draw front,top,side and 30 deg views

   fDrawAllViews = kTRUE;
   fPad->cd();
   fPad->SetFillColor(15);
   fPad->Clear();
   fPad->Divide(2,2);

   // draw 30 deg view
   fPad->cd(1);
   DrawView(30, 30, 0);
   DrawTitle();

   // draw front view
   fPad->cd(2);
   DrawView(0, -90,0);
   DrawTitle("Front");

   // draw top view
   fPad->cd(3);
   DrawView(90, -90, 90);
   DrawTitle("Top");

   // draw side view
   fPad->cd(4);
   DrawView(90, 0, -90);
   DrawTitle("Side");

   fPad->cd(2);
}

//_____________________________________________________________________________
void AliDisplay::DrawHits()
{
//    Draw hits for all ALICE detectors

   Float_t cutmin, cutmax, etamin, etamax, pmom, smin, smax, eta, theta, r;
   Float_t *pxyz;
   Int_t ntracks,track;
   GParticle *particle;
   TObjArray *points;
   AliPoints *pm;
      
   //Get cut slider
   smax   = fCutSlider->GetMaximum();
   smin   = fCutSlider->GetMinimum();
   cutmin = ptcutmax*smin;
   if (smax < 0.98) cutmax = ptcutmax*smax;
   else             cutmax = 100000;
   
   //Get eta slider
   smax   = fEtaSlider->GetMaximum();
   smin   = fEtaSlider->GetMinimum();
   etamin = etacutmax*(2*smin-1);
   etamax = etacutmax*(2*smax-1);
   if (smin < 0.02) etamin = -1000;
   if (smax > 0.98) etamax =  1000;
      
   TIter next(gAlice->Detectors());
   AliDetector *detector;
   fHitsCuts = 0;
   while((detector = (AliDetector*)next())) {
      if (!detector->IsActive()) continue;
      points = detector->Points();
      if (!points) continue;
      ntracks = points->GetEntriesFast();
      for (track=0;track<ntracks;track++) {
         pm = (AliPoints*)points->UncheckedAt(track);
         if (!pm) continue;
         particle = pm->GetParticle();
         if (!particle) continue;
         pmom = particle->GetMomentum();
         if (pmom < cutmin) continue;
         if (pmom > cutmax) continue;
         // as a first approximation, take eta of first point
         pxyz  = pm->GetP();
         r     = TMath::Sqrt(pxyz[0]*pxyz[0] + pxyz[1]*pxyz[1]);
         theta = TMath::ATan2(r,TMath::Abs(pxyz[2]));
         if(theta) eta = -TMath::Log(TMath::Tan(0.5*theta)); else eta = 1e10;
         if (pxyz[2] < 0) eta = -eta;
         if (eta < etamin || eta > etamax) continue;
         pm->Draw();
         fHitsCuts += pm->GetN();
      }
   }
}

//_____________________________________________________________________________
void AliDisplay::DrawTitle(Option_t *option)
{
//    Draw the event title

   Float_t xmin = gPad->GetX1();
   Float_t xmax = gPad->GetX2();
   Float_t ymin = gPad->GetY1();
   Float_t ymax = gPad->GetY2();
   Float_t dx   = xmax-xmin;
   Float_t dy   = ymax-ymin;

   if (strlen(option) == 0) {
      TPaveText *title = new TPaveText(xmin +0.01*dx, ymax-0.09*dy, xmin +0.5*dx, ymax-0.01*dy);
      title->SetBit(kCanDelete);
      title->SetFillColor(42);
      title->Draw();
      char ptitle[100];
      sprintf(ptitle,"Alice event: %d, Run:%d",gAlice->GetHeader()->GetEvent(), gAlice->GetHeader()->GetRun());
      title->AddText(ptitle);
      Int_t nparticles = gAlice->Particles()->GetEntriesFast();
      sprintf(ptitle,"Nparticles = %d  Nhits = %d",nparticles, fHitsCuts);
      title->AddText(ptitle);
   } else {
      TPaveLabel *label = new TPaveLabel(xmin +0.01*dx, ymax-0.07*dy, xmin +0.2*dx, ymax-0.01*dy,option);
      label->SetBit(kCanDelete);
      label->SetFillColor(42);
      label->Draw();
   }
}

//_____________________________________________________________________________
void AliDisplay::DrawView(Float_t theta, Float_t phi, Float_t psi)
{
//    Draw a view of ALICE

   gPad->SetCursor(kWatch);
   gPad->SetFillColor(1);
   gPad->Clear();

   Int_t iret;
   TView *view = new TView(1);
   Float_t range = fRrange*fRangeSlider->GetMaximum();
   view->SetRange(-range,-range,-range,range, range, range);
   fZoomX0[0] = -1;
   fZoomY0[0] = -1;
   fZoomX1[0] =  1;
   fZoomY1[0] =  1;
   fZooms = 0;
   
   // Display Alice Geometry
   gAlice->GetGeometry()->Draw("same");
   
   //Loop on all detectors to add their products to the pad
   DrawHits();

    // add itself to the list (must be last)
   AppendPad();
   
   view->SetView(phi, theta, psi, iret);
}

//_____________________________________________________________________________
void AliDisplay::DrawViewGL()
{
//    Draw current view using OPENGL

   TPad *pad = (TPad*)gPad->GetPadSave();
   pad->cd();
   TView *view = pad->GetView();
   if (!view) return;
   pad->x3d("OPENGL");
}

//_____________________________________________________________________________
void AliDisplay::DrawViewX3D()
{
//    Draw current view using X3D

   TPad *pad = (TPad*)gPad->GetPadSave();
   pad->cd();
   TView *view = pad->GetView();
   if (!view) return;
   pad->x3d();
}

//_____________________________________________________________________________
void AliDisplay::EnableDetector(const char *name)
{
//    Enable detector name in graphics views
   
   AliDetector *detector = (AliDetector*)gAlice->Detectors()->FindObject(name);
   if (!detector) return;
   detector->Enable();
   Draw();
}

//______________________________________________________________________________
void AliDisplay::ExecuteEvent(Int_t event, Int_t px, Int_t py)
{
//  Execute action corresponding to the mouse event

   static Float_t x0, y0, x1, y1;

   static Int_t pxold, pyold;
   static Int_t px0, py0;
   static Int_t linedrawn;
   Float_t temp;

   if (px == 0 && py == 0) { //when called by sliders
      if (event == kButton1Up) {
         Draw();
      }
      return;
   }
   if (!fZoomMode && gPad->GetView()) {
      gPad->GetView()->ExecuteRotateView(event, px, py);
      return;
   }

   // something to zoom ?
//   fPad->SetCursor(kCross);
   gPad->SetCursor(kCross);
   
   switch (event) {

   case kButton1Down:
      gGXW->SetLineColor(-1);
      gPad->TAttLine::Modify();  //Change line attributes only if necessary
      x0 = gPad->AbsPixeltoX(px);
      y0 = gPad->AbsPixeltoY(py);
      px0   = px; py0   = py;
      pxold = px; pyold = py;
      linedrawn = 0;
      return;

   case kButton1Motion:
      if (linedrawn) gGXW->DrawBox(px0, py0, pxold, pyold, TGXW::kHollow);
      pxold = px;
      pyold = py;
      linedrawn = 1;
      gGXW->DrawBox(px0, py0, pxold, pyold, TGXW::kHollow);
      return;

   case kButton1Up:
      gPad->GetCanvas()->FeedbackMode(kFALSE);
      if (px == px0) return;
      if (py == py0) return;
      x1 = gPad->AbsPixeltoX(px);
      y1 = gPad->AbsPixeltoY(py);

      if (x1 < x0) {temp = x0; x0 = x1; x1 = temp;}
      if (y1 < y0) {temp = y0; y0 = y1; y1 = temp;}
      gPad->Range(x0,y0,x1,y1);
      if (fZooms < kMAXZOOMS-1) {
         fZooms++;
         fZoomX0[fZooms] = x0;
         fZoomY0[fZooms] = y0;
         fZoomX1[fZooms] = x1;
         fZoomY1[fZooms] = y1;
      }
      gPad->Modified(kTRUE);
      return;
   }

}
 
//___________________________________________
void AliDisplay::LoadPoints()
{
// Read hits info and store x,y,z info in arrays fPoints
// Loop on all detectors
 
   gAlice->ResetPoints();
   TIter next(gAlice->Detectors());
   AliDetector *detector;
   Int_t ntracks = gAlice->GetNtrack();
   for (Int_t track=0; track<ntracks;track++) {
      gAlice->ResetHits();
      gAlice->TreeH()->GetEvent(track);
      while((detector = (AliDetector*)next())) {
         detector->LoadPoints(track);
      }
      next.Reset();
   }
}

//_____________________________________________________________________________
void AliDisplay::Paint(Option_t *)
{
//    Paint miscellaneous items

}

//_____________________________________________________________________________
void AliDisplay::SetPickMode()
{
   fZoomMode = 0;

   fArcButton->SetY1(fPickButton->GetYlowNDC()+0.5*fPickButton->GetHNDC());
   fTrigPad->Modified();
}

//_____________________________________________________________________________
void AliDisplay::SetZoomMode()
{
   fZoomMode = 1;

   fArcButton->SetY1(fZoomButton->GetYlowNDC()+0.5*fZoomButton->GetHNDC());
   fTrigPad->Modified();
}

//_____________________________________________________________________________
void AliDisplay::SetPTcut(Float_t ptcut)
{
   fPTcut = ptcut;

   if (!fPad) return;
   fPad->Clear();
   Draw();
}

//_____________________________________________________________________________
void AliDisplay::SetRange(Float_t rrange, Float_t zrange)
{
// Set view range along R and Z
   fRrange = rrange;
   fZrange = zrange;

   if (!fPad) return;
   fPad->Clear();
   Draw();
}
   
//_____________________________________________________________________________
void AliDisplay::SetView(Float_t theta, Float_t phi, Float_t psi)
{
//  change viewing angles for current event

   fPad->cd();
   fDrawAllViews = kFALSE;
   fPhi   = phi;
   fTheta = theta;
   fPsi   = psi;
   Int_t iret = 0;

   TView *view = gPad->GetView();
   if (view) view->SetView(fPhi, fTheta, fPsi, iret);
   else      Draw();

   gPad->Modified();
}

//_____________________________________________________________________________
void AliDisplay::ShowNextEvent(Int_t delta)
{
//  Display (current event_number+delta)
//    delta =  1  shown next event
//    delta = -1 show previous event

  if (delta) {
     gAlice->Clear();
     Int_t current_event = gAlice->GetHeader()->GetEvent();
     Int_t new_event     = current_event + delta;
     gAlice->GetEvent(new_event);
     if (!gAlice->TreeH()) return; 
   }
  LoadPoints();
  fPad->cd(); 
  Draw();
}

//______________________________________________________________________________
void AliDisplay::UnZoom()
{
   if (fZooms <= 0) return;
   fZooms--;
   TPad *pad = (TPad*)gPad->GetPadSave();
   pad->Range(fZoomX0[fZooms],fZoomY0[fZooms], fZoomX1[fZooms],fZoomY1[fZooms]);
   pad->Modified();
}
