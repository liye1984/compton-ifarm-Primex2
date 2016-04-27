#include <iostream>
#include <fstream>
#include <iomanip>

//using namespace std;

#include "TCanvas.h"
#include "TFile.h"
#include "TNtuple.h"
#include "TProfile.h"
#include "TBenchmark.h"
#include "TStyle.h"
#include "TPaveText.h"
#include "TFrame.h"
#include "TF1.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TInterpreter.h"
#include "TStyle.h"
#include "TH1.h"
#include "TH2.h"
#include "TMath.h"
#include "TTree.h"
#include "TH1F.h"


using namespace std;


void plot1()

{


  TFile *f2 = new TFile("./result02.root","r");
  if (!f2) cout << "can not open file" <<endl;
 
  const Int_t NRGBs = 5;
  const Int_t NCont = 255;
  Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
  Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
  Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };

  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);

  gStyle->SetNumberContours(NCont);






const int kMax = 1000;

  int run, eventid;

  int  Eid;
  int  Tid;      // Tcounter channel ID
  float Tdiff;   //time difference between Hycal and Tcounter 
  float Eg;      // incoming photon energy
  float e1;      // cluster 1 energy
  float x1;      // cluster 1 x position
  float y1;      // cluster 1 y position
  float e0;     
  float e2;      // cluster 2 energy
  float x2;      // cluster 2 x position
  float y2;      // cluster 2 y position

  
  float PI=3.1415927;	
 

 

  TH1F *h_evn = new TH1F("h_evn","eventid",100000,0,31000000);
  TH1F *h_E = new TH1F("h_E","E0 = eg - e1 -e2",1000,-2,2);
  TH1F *h_tdiff = new TH1F("tdiff","Tdiff",1000,-10,10);  
  TH1F *h_x = new TH1F("h_x","X position",60,-30,30);
  TH1F *h_sep = new TH1F("h_sep","Cluster Separation",500,0,50);
  TH1F *h_angle = new TH1F("h_angle","Azimuthal Angle Difference",500,120,240);
  TH1F *h_z = new TH1F("h_z","Z position",600,-30,1200);

	
  TH2F *h_x_y_1 = new TH2F("h_x_y_1"," XY position for cluster1", 200,-20,20,500,-20,20);
  TH2F *h_x_y_2 = new TH2F("h_x_y_2"," XY position for cluster2", 200,-20,20,500,-20,20);
  TH2F *h_sep_2d = new TH2F("h_sep_2d"," Cluster Separation", 200,-40,40,200,-40,40);
  TH1F *h_cosphi = new TH1F("h_cosphi","cosphi",600,0,4);



//THStack *helass = new THStack("helass","");
  TH1F *h0 = new TH1F("h0","Elasticity:  E_{0} - (E_{1} + E_{2})",300,-1.5,1.5);
  TH1F *h1 = new TH1F("h1","Elasticity:  E_{0} - (E_{1} + E_{2})",300,-1.5,1.5);
  TH1F *h2 = new TH1F("h2","Elasticity:  E_{0} - (E_{1} + E_{2})",300,-1.5,1.5);
  TH1F *h3 = new TH1F("h3","Elasticity:  E_{0} - (E_{1} + E_{2})",300,-1.5,1.5);


  TH1F *z0 = new TH1F("h_z","Z position",600,-30,1200);
  TH1F *z1 = new TH1F("h_z","Z position",600,-30,1200);
  TH1F *z2 = new TH1F("h_z","Z position",600,-30,1200);
  TH1F *z3 = new TH1F("h_z","Z position",600,-30,1200);






  h0 = (TH1F*)f2->Get("helas0");
  h1 = (TH1F*)f2->Get("helas1");
  h2 = (TH1F*)f2->Get("helas2");
  h3 = (TH1F*)f2->Get("helas3");

  z0 = (TH1F*)f2->Get("h_z0");
  z1 = (TH1F*)f2->Get("h_z1");
  z2 = (TH1F*)f2->Get("h_z2");
  z3 = (TH1F*)f2->Get("h_z3");






/*
  TH1F *helas4 = new TH1F("helas4","Elasticity:  E_{0} - (E_{1} + E_{2})",300,-0.5,1.5);
  helass->Add(helas4);
  TH1F *helas5 = new TH1F("helas5","Elasticity:  E_{0} - (E_{1} + E_{2})",300,-0.5,1.5);
  helass->Add(helas5);
  TH1F *helas6 = new TH1F("helas6","Elasticity:  E_{0} - (E_{1} + E_{2})",300,-0.5,1.5);
  helass->Add(helas6);
  TH1F *helas7 = new TH1F("helas7","Elasticity:  E_{0} - (E_{1} + E_{2})",300,-0.5,1.5);

  helass->Add(helas7);

*/

  TLatex latex;
  latex.SetNDC();
  latex.SetTextFont(42); 
  latex.SetTextSize(0.040); 
  latex.SetTextColor(1);        



 TCanvas *c1 = new TCanvas("c1","XY position",800,800);
  gStyle->SetPalette(0,0);
  c1->cd();


	 

  /*
 
  h0->SetFillColor(kGreen+4);
  h1->SetFillColor(kGreen+3);
  h2->SetFillColor(kGreen+2);
  h3->SetFillColor(kGreen+1);
  
  h3->SetTitleSize(0.035,"X");
  h3->GetXaxis()->SetTitle("Elasticity: E_{0} - (E_{1} + E_{2}) (GeV)");


  latex.DrawLatex(0.465,0.035,"Elasticity: E_{0} - (E_{1} + E_{2})  (GeV)");
  */    
  TLegend *legend;
  legend = new TLegend(0.750,0.750,0.920,0.915);
 
  legend->SetTextFont(52);
  legend->AddEntry(h0,"All Events","f");
  legend->AddEntry(h1,"R > 16 cm","f");
  legend->AddEntry(h2,"R > 17 cm","f");
  legend->AddEntry(h3,"R > 18 cm","f");
 
  
  h0->SetFillColor(kGreen+10);
  h1->SetFillColor(kGreen+6);
  h2->SetFillColor(kGreen+1);
  h3->SetFillColor(kYellow);


  z0->SetFillColor(kGreen+10);
  z1->SetFillColor(kGreen+6);
  z2->SetFillColor(kGreen+1);
  z3->SetFillColor(kYellow);

  h0->SetTitleSize(0.035,"X");
  h0->GetXaxis()->SetTitle("Elasticity: E_{0} - (E_{1} + E_{2}) (GeV)");
  z0->SetTitleSize(0.035,"X");
  z0->GetXaxis()->SetTitle("Z reconstruction (cm)");
 
  h0->Draw();
  h1->Draw("same");
  h2->Draw("same");
  h3->Draw("same");
 
  // z0->Draw();
  // z1->Draw("same");
  // z2->Draw("same");
  // z3->Draw("same");






legend->Draw("same");


  //	h_E->GetXaxis()->SetTitle("eg-(e1+e2) (GeV)");	
  //	h_E->Draw();
  //	h_tdiff->Draw();
   // h_x_y_1->Draw("zcol");
  //	h_angle->GetXaxis()->SetTitle(" Azimuthal Angle difference (deg)");
  //	h_angle->Draw();
  //	h_sep->Draw();	
  //	h_sep_2d->Draw("zcol");
  //	h_z->Draw();

 
  //c1->SaveAs("/w/hallb-scifs1a/primex/liye/compton_work/plot/z_recon_65078.png");
  
  
}












