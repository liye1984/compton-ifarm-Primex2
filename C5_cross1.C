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
#include "TChain.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TPaveStats.h"
#include "TText.h"


using namespace std;
#include "TMultiGraph.h"
#include "TGraphErrors.h"

#include "TObjArray.h"
#include "TFractionFitter.h"


const int NEcounter_min = 1 ;
const int NEcounter_max = 18 ;

TH1F *empty ;
TH1F *outTcut ;
TH1F *simulation ;
TH1F *pairbackground ;

Double_t fit_compton(Double_t *x,Double_t *par)
{
  Double_t xx = x[0];
  Int_t bin = empty->GetXaxis()->FindBin(xx);
  Double_t empty_yield = par[0]*empty->GetBinContent(bin); 
  bin = outTcut->GetXaxis()->FindBin(xx);
  Double_t outTcut_yield = par[1]*outTcut->GetBinContent(bin);
  xx = xx-par[4]; 
  bin = simulation->GetXaxis()->FindBin(xx);
  Double_t simulation_yield = par[2]*simulation->GetBinContent(bin); 
  Double_t pair_yield = par[3]*pairbackground->GetBinContent(bin); 
 
  return empty_yield + outTcut_yield + simulation_yield + pair_yield; 



}


void fit_his(TH1F *h_data,TH1F *h_empty, TH1F *h_simulation, TH1F *h_outTcut, TH1F *h_pair, int i )
{
  empty = h_empty;
  outTcut = h_outTcut;
  simulation = h_simulation;
  pairbackground = h_pair;

  TF1 *fit_func = new TF1 (Form("fit_func%d",i), fit_compton, -1.5, 1.5, 5); 
 
  Double_t max1 = h_data->GetMaximum();
  Double_t max2 = h_simulation->GetMaximum();

  fit_func->SetParameters(1.0, 1.0 ,0.7 , 1.0,-0.02);
  fit_func->SetParLimits(0, 0.9, 1.2);    // po is for empty//
  fit_func->SetParLimits(1, 0.2, 2.5);    // p1 is for outTcut//
  fit_func->SetParLimits(2, 0.65, 0.72);    // p2 is for simulation// 
  fit_func->SetParLimits(3, 0.9, 1.1);    // p3 is for pair back ground //
  fit_func->SetParLimits(4, -0.1, 0.1);   // p4 is for mc shift//


  // h_data->Fit(Form("fit_func%d",i),"0","",-0.5, 1.5);
  h_data->Fit(Form("fit_func%d",i),"","",-0.5, 1.4);
  cout << h_data->GetName() << "---------------max1/max2 = " << max1/max2 << endl;

}

TH1F* shifthis(TH1F* horig, Double_t x) {
  Double_t nbins = horig->GetSize()-2;
  Double_t width = horig->GetBinWidth(1);
  Double_t xlow = horig->GetBinCenter(1)-width/2.;
  Double_t xhigh = horig->GetBinCenter(nbins)+width/2.;

  TString name = horig->GetName();
  name += "_shift";
  TH1F* hshift = new TH1F(name.Data(),"Shifted histogram",nbins,xlow+x,xhigh+x);
  for (Int_t i=1; i<=nbins; i++) hshift->SetBinContent(i,horig->GetBinContent(i));

  return hshift;

}





void C5_cross1()

{
  gStyle->SetOptStat(1);
  gStyle->SetOptFit(1);
  gStyle->SetStatH(0.3);


  float Dyield_1[181],Dyield_2[181],MCyield[181],Ecounter[181],Nor_MCyield[181];
  float flux_Bin[18],input_Bin[18];
  float Energy[18],Ecross[18],Rcross_1[18],Rcross_2[18],ratio[18],Acc[18],Nor_Acc[18];
  float NEid[18],Binwidth[18];
  float MC_E[18],MC_cross[18];
  float Theory[18],diff_1[18],diff_2[18];
  float sys_error_yield[18],sys_error_const[18],sys_error_acc[18],sys_error_flux[18]; 
  float sys_error_fit[18],sys_error_cut[18];
  float sys_error[18],stat_error[18]; 
  float cross_error[18],cross_error_plot[18],diff_error_plot[18];


  Double_t p0,p1,p2;
  Double_t error0,error1,error2;

  const float Na =  6.02214e+23;
  const float Ne = (2.1236)*Na/2.0 ; 

  //===========  E-counter energy ======================//
  
  Energy[0]= 5.281;	Energy[1]= 5.241;	Energy[2]= 5.195;
  Energy[3]= 5.145;	Energy[4]= 5.093;	Energy[5]= 5.041;
  Energy[6]= 4.988;	Energy[7]= 4.937;	Energy[8]= 4.883;
  Energy[9]= 4.827;	Energy[10]= 4.774;	Energy[11]= 4.726;
  Energy[12]= 4.673;	Energy[13]= 4.609;	Energy[14]= 4.554;
  Energy[15]= 4.503;	Energy[16]= 4.459;	Energy[17]= 4.400;

  



  Binwidth[0] = 0.03904;	Binwidth[1] = 0.04032;	Binwidth[2] = 0.04889;
  Binwidth[3] = 0.05197;	Binwidth[4] = 0.05209;	Binwidth[5] = 0.05153;
  Binwidth[6] = 0.05387;	Binwidth[7] = 0.05119;	Binwidth[8] = 0.05647;
  Binwidth[9] = 0.05566;	Binwidth[10]= 0.05242;	Binwidth[11]= 0.04564;
  Binwidth[12]= 0.02215;	Binwidth[13]= 0.05520;	Binwidth[14]= 0.05663;
  Binwidth[15]= 0.05357;	Binwidth[16]= 0.04049;	Binwidth[17]= 0.05416;


  MC_E[0]= 5.2864;	MC_E[1]= 5.2399;	MC_E[2]= 5.1977;
  MC_E[3]= 5.1484;	MC_E[4]= 5.0959;	MC_E[5]= 5.0451;
  MC_E[6]= 4.9920;	MC_E[7]= 4.9377;	MC_E[8]= 4.8826;
  MC_E[9]= 4.8308;	MC_E[10]= 4.7775;	MC_E[11]= 4.7214;
  MC_E[12]= 4.6676;	MC_E[13]= 4.6115;	MC_E[14]= 4.5565;
  MC_E[15]= 4.5003;	MC_E[16]= 4.4473;	MC_E[17]= 4.3925;
  
  MC_cross[0]= 0.26040;		MC_cross[1]= 0.26245;	MC_cross[2]= 0.26434;
  MC_cross[3]= 0.26659;		MC_cross[4]= 0.26905;	MC_cross[5]= 0.27158;
  MC_cross[6]= 0.27412;		MC_cross[7]= 0.27677;	MC_cross[8]= 0.27957;
  MC_cross[9]= 0.28228;		MC_cross[10]= 0.28503;	MC_cross[11]= 0.28807;
  MC_cross[12]= 0.29104;	MC_cross[13]= 0.29428;	MC_cross[14]= 0.29763;
  MC_cross[15]= 0.30072;	MC_cross[16]= 0.30394;	MC_cross[17]= 0.30728;

  flux_Bin[0]= 1.41593;		flux_Bin[1]= 1.66562;	flux_Bin[2]= 1.75406;
  flux_Bin[3]= 1.67320;		flux_Bin[4]= 1.77147;	flux_Bin[5]= 1.91901;
  flux_Bin[6]= 2.06904;		flux_Bin[7]= 1.90396;	flux_Bin[8]= 1.91878;
  flux_Bin[9]= 1.79185;		flux_Bin[10]= 1.98481;	flux_Bin[11]= 1.59074;
  flux_Bin[12]= 0.82188;	flux_Bin[13]= 1.90192;	flux_Bin[14]= 2.02469;
  flux_Bin[15]= 1.82050;	flux_Bin[16]= 2.06731;	flux_Bin[17]= 1.55134;






  for (int i=0;i<18;i++) {NEid[i]= i+1.0;}



 
  for ( int j=0;j<180;j++){Dyield_1[j]=0;Dyield_2[j]=0;MCyield[j]=0;Ecounter[j]=0;}

  TH1F *h_eid_data = new TH1F("h_eid_data","Eid_Data",200,-10,190);
  TH1F *h_eid_mc = new TH1F("h_eid_mc","Eid_MC",200,-10,190);


  TH1F *data_Ecounter[18]; 
  TH1F *outTcut_Ecounter[18];  
  TH1F *empty_Ecounter[18];   
  TH1F *mc_Ecounter[18];   
  TH1F *pair_Ecounter[18];    


  char hName[255],hTitle[255];
  for(int ii=0;ii<18;ii++) 
    {	
      sprintf(hName,"data_Ecounter%02d",ii);
      sprintf(hTitle,"data_Ecounter%02d",ii);
      data_Ecounter[ii] = new TH1F(hName,hTitle,200,-2,2);  
    }
  for(int ii=0;ii<18;ii++) 
    {	
      sprintf(hName,"mc_Ecounter%02d",ii);
      sprintf(hTitle,"mc_Ecounter%02d",ii);
      mc_Ecounter[ii] = new TH1F(hName,hTitle,200,-2,2);  
    }

  for(int ii=0;ii<18;ii++) 
    {	
      sprintf(hName,"empty_Ecounter%02d",ii);
      sprintf(hTitle,"empty_Ecounter%02d",ii);
      empty_Ecounter[ii] = new TH1F(hName,hTitle,200,-2,2);  
    }

  for(int ii=0;ii<18;ii++) 
    {	
      sprintf(hName,"outTcut_Ecounter%02d",ii);
      sprintf(hTitle,"outTcut_Ecounter%02d",ii);
      outTcut_Ecounter[ii] = new TH1F(hName,hTitle,200,-2,2);  
    }
  for(int ii=0;ii<18;ii++) 
    {	
      sprintf(hName,"pair_Ecounter%02d",ii);
      sprintf(hTitle,"pair_Ecounter%02d",ii);
      pair_Ecounter[ii] = new TH1F(hName,hTitle,200,-2,2);  
    }




  /*
    TFile *f1 = new TFile(Form("/w/hallb-scifs1a/primex/liye/data/ellcut_cook_Data%d.root",prun),"r");
    if (!f1) cout << " erro " << endl;
    TFile *f2 = new TFile(Form("/w/hallb-scifs1a/primex/liye/data/ellcut_cook_MC%d.root",prun),"r");
    if (!f1) cout << " erro " << endl;
  */

   TFile *f1 = new TFile("/home/mepmsu/Desktop/premex-2/analysis/222/Data64876_64883_e1_5_z200.root","r");
  //   TFile *f1 = new TFile("/w/hallb-scifs1a/primex/liye/222/Data64876_64883_e1_5_z200.root","r");

  if (!f1) cout << " erro " << endl;

   TFile *f2 = new TFile("/home/mepmsu/Desktop/premex-2/analysis/222/cor_MC64876_64883_e1_5_z200.root","r");
  //TFile *f2 = new TFile("/w/hallb-scifs1a/primex/liye/222/cor_MC64876_64883_e1_5_z200.root","r");
  
   if (!f2) cout << " erro " << endl;
 
  //TFile *f3 = new TFile("/w/hallb-scifs1a/primex/liye/222/empty64885_e1_5_z200.root","r");
   TFile *f3 = new TFile("/home/mepmsu/Desktop/premex-2/analysis/222/empty64885_e1_5_z200.root","r");
   if (!f3) cout << " erro " << endl;
 
  //TFile *f4 = new TFile("/w/hallb-scifs1a/primex/liye/222/MCpair64877_e1_5_z200.root","r");
  TFile *f4 = new TFile("/home/mepmsu/Desktop/premex-2/analysis/222/MCpair64877_e1_5_z200.root","r");
 if (!f4) cout << " erro " << endl;
 




  vector <int> gEEid;
  vector <float> gEinput;
  vector <float> gEEflux;


 
 
  //==========================================================================

/*

  ifstream INPUT_fileb;
  // INPUT_fileb.open("/home/mepmsu/Desktop/premex-2/analysis/data/ge10113.hbk.estat");
  INPUT_fileb.open("/w/hallb-scifs1a/primex/liye/data/ge10113.hbk.estat");

  if( !INPUT_fileb)cout<<"ERROR!!!! CAN'T OPEN input  ge10113.hbk.estat file for run "<<endl;

  char strline1[2561];
  int m=0;
  float inputN;

  while(!INPUT_fileb.eof())
    {
      INPUT_fileb>>inputN;
      INPUT_fileb.getline(strline1,2560);
     
      gEinput.push_back(inputN);
	

      //	cout<<"eflux="<<eflux<<"; "<<endl;
      m++;
    }
   
  //   for(int i=0;i<180;i++) cout<<gEinput[i]<<" =="<< i <<endl;
 
  for(int k=0;k<18;k++)
    { 
      input_Bin[k]=0;
      for (int kk=0;kk<10;kk++)
	{ 
	  int kkk = kk + 10*k;
	  input_Bin[k] = input_Bin[k]+ gEinput[kkk];

	  //	  cout << gEEflux[kkk] <<"  " << kkk << endl;
	}
  
      //    cout << input_Bin[k] <<"  " << k << endl;

    }
  
*/

  //===========================================================================
 

  
  h_eid_data = (TH1F*)f1->Get("h_eid");
  h_eid_mc = (TH1F*)f2->Get("h_eid");
 
  data_Ecounter[0]  = (TH1F*)f1->Get("inTcut_Ecounter01");
  data_Ecounter[1]  = (TH1F*)f1->Get("inTcut_Ecounter02");	
  data_Ecounter[2]  = (TH1F*)f1->Get("inTcut_Ecounter03");
  data_Ecounter[3]  = (TH1F*)f1->Get("inTcut_Ecounter04");	
  data_Ecounter[4]  = (TH1F*)f1->Get("inTcut_Ecounter05");	
  data_Ecounter[5]  = (TH1F*)f1->Get("inTcut_Ecounter06");	
  data_Ecounter[6]  = (TH1F*)f1->Get("inTcut_Ecounter07");	
  data_Ecounter[7]  = (TH1F*)f1->Get("inTcut_Ecounter08");	
  data_Ecounter[8]  = (TH1F*)f1->Get("inTcut_Ecounter09");	
  data_Ecounter[9]  = (TH1F*)f1->Get("inTcut_Ecounter10");	
  data_Ecounter[10] = (TH1F*)f1->Get("inTcut_Ecounter11");	
  data_Ecounter[11] = (TH1F*)f1->Get("inTcut_Ecounter12");	
  data_Ecounter[12] = (TH1F*)f1->Get("inTcut_Ecounter13");	
  data_Ecounter[13] = (TH1F*)f1->Get("inTcut_Ecounter14");	
  data_Ecounter[14] = (TH1F*)f1->Get("inTcut_Ecounter15");	
  data_Ecounter[15] = (TH1F*)f1->Get("inTcut_Ecounter16");	
  data_Ecounter[16] = (TH1F*)f1->Get("inTcut_Ecounter17");	
  data_Ecounter[17] = (TH1F*)f1->Get("inTcut_Ecounter18");	
    
  outTcut_Ecounter[0]  = (TH1F*)f1->Get("outTcut_Ecounter01");
  outTcut_Ecounter[1]  = (TH1F*)f1->Get("outTcut_Ecounter02");	
  outTcut_Ecounter[2]  = (TH1F*)f1->Get("outTcut_Ecounter03");
  outTcut_Ecounter[3]  = (TH1F*)f1->Get("outTcut_Ecounter04");	
  outTcut_Ecounter[4]  = (TH1F*)f1->Get("outTcut_Ecounter05");	
  outTcut_Ecounter[5]  = (TH1F*)f1->Get("outTcut_Ecounter06");	
  outTcut_Ecounter[6]  = (TH1F*)f1->Get("outTcut_Ecounter07");	
  outTcut_Ecounter[7]  = (TH1F*)f1->Get("outTcut_Ecounter08");	
  outTcut_Ecounter[8]  = (TH1F*)f1->Get("outTcut_Ecounter09");	
  outTcut_Ecounter[9]  = (TH1F*)f1->Get("outTcut_Ecounter10");	
  outTcut_Ecounter[10] = (TH1F*)f1->Get("outTcut_Ecounter11");	
  outTcut_Ecounter[11] = (TH1F*)f1->Get("outTcut_Ecounter12");	
  outTcut_Ecounter[12] = (TH1F*)f1->Get("outTcut_Ecounter13");	
  outTcut_Ecounter[13] = (TH1F*)f1->Get("outTcut_Ecounter14");	
  outTcut_Ecounter[14] = (TH1F*)f1->Get("outTcut_Ecounter15");	
  outTcut_Ecounter[15] = (TH1F*)f1->Get("outTcut_Ecounter16");	
  outTcut_Ecounter[16] = (TH1F*)f1->Get("outTcut_Ecounter17");	
  outTcut_Ecounter[17] = (TH1F*)f1->Get("outTcut_Ecounter18");

  mc_Ecounter[0]  = (TH1F*)f2->Get("h_Ecounter01");
  mc_Ecounter[1]  = (TH1F*)f2->Get("h_Ecounter02");	
  mc_Ecounter[2]  = (TH1F*)f2->Get("h_Ecounter03");
  mc_Ecounter[3]  = (TH1F*)f2->Get("h_Ecounter04");	
  mc_Ecounter[4]  = (TH1F*)f2->Get("h_Ecounter05");	
  mc_Ecounter[5]  = (TH1F*)f2->Get("h_Ecounter06");	
  mc_Ecounter[6]  = (TH1F*)f2->Get("h_Ecounter07");	
  mc_Ecounter[7]  = (TH1F*)f2->Get("h_Ecounter08");	
  mc_Ecounter[8]  = (TH1F*)f2->Get("h_Ecounter09");	
  mc_Ecounter[9]  = (TH1F*)f2->Get("h_Ecounter10");	
  mc_Ecounter[10] = (TH1F*)f2->Get("h_Ecounter11");	
  mc_Ecounter[11] = (TH1F*)f2->Get("h_Ecounter12");	
  mc_Ecounter[12] = (TH1F*)f2->Get("h_Ecounter13");	
  mc_Ecounter[13] = (TH1F*)f2->Get("h_Ecounter14");	
  mc_Ecounter[14] = (TH1F*)f2->Get("h_Ecounter15");	
  mc_Ecounter[15] = (TH1F*)f2->Get("h_Ecounter16");	
  mc_Ecounter[16] = (TH1F*)f2->Get("h_Ecounter17");	
  mc_Ecounter[17] = (TH1F*)f2->Get("h_Ecounter18");	
  	

  empty_Ecounter[0]  = (TH1F*)f3->Get("empty_Ecounter01");
  empty_Ecounter[1]  = (TH1F*)f3->Get("empty_Ecounter02");	
  empty_Ecounter[2]  = (TH1F*)f3->Get("empty_Ecounter03");
  empty_Ecounter[3]  = (TH1F*)f3->Get("empty_Ecounter04");	
  empty_Ecounter[4]  = (TH1F*)f3->Get("empty_Ecounter05");	
  empty_Ecounter[5]  = (TH1F*)f3->Get("empty_Ecounter06");	
  empty_Ecounter[6]  = (TH1F*)f3->Get("empty_Ecounter07");	
  empty_Ecounter[7]  = (TH1F*)f3->Get("empty_Ecounter08");	
  empty_Ecounter[8]  = (TH1F*)f3->Get("empty_Ecounter09");	
  empty_Ecounter[9]  = (TH1F*)f3->Get("empty_Ecounter10");	
  empty_Ecounter[10] = (TH1F*)f3->Get("empty_Ecounter11");	
  empty_Ecounter[11] = (TH1F*)f3->Get("empty_Ecounter12");	
  empty_Ecounter[12] = (TH1F*)f3->Get("empty_Ecounter13");	
  empty_Ecounter[13] = (TH1F*)f3->Get("empty_Ecounter14");	
  empty_Ecounter[14] = (TH1F*)f3->Get("empty_Ecounter15");	
  empty_Ecounter[15] = (TH1F*)f3->Get("empty_Ecounter16");	
  empty_Ecounter[16] = (TH1F*)f3->Get("empty_Ecounter17");	
  empty_Ecounter[17] = (TH1F*)f3->Get("empty_Ecounter18");	
  	
  pair_Ecounter[0]  = (TH1F*)f4->Get("pair_Ecounter01");
  pair_Ecounter[1]  = (TH1F*)f4->Get("pair_Ecounter02");	
  pair_Ecounter[2]  = (TH1F*)f4->Get("pair_Ecounter03");
  pair_Ecounter[3]  = (TH1F*)f4->Get("pair_Ecounter04");	
  pair_Ecounter[4]  = (TH1F*)f4->Get("pair_Ecounter05");	
  pair_Ecounter[5]  = (TH1F*)f4->Get("pair_Ecounter06");	
  pair_Ecounter[6]  = (TH1F*)f4->Get("pair_Ecounter07");	
  pair_Ecounter[7]  = (TH1F*)f4->Get("pair_Ecounter08");	
  pair_Ecounter[8]  = (TH1F*)f4->Get("pair_Ecounter09");	
  pair_Ecounter[9]  = (TH1F*)f4->Get("pair_Ecounter10");	
  pair_Ecounter[10] = (TH1F*)f4->Get("pair_Ecounter11");	
  pair_Ecounter[11] = (TH1F*)f4->Get("pair_Ecounter12");	
  pair_Ecounter[12] = (TH1F*)f4->Get("pair_Ecounter13");	
  pair_Ecounter[13] = (TH1F*)f4->Get("pair_Ecounter14");	
  pair_Ecounter[14] = (TH1F*)f4->Get("pair_Ecounter15");	
  pair_Ecounter[15] = (TH1F*)f4->Get("pair_Ecounter16");	
  pair_Ecounter[16] = (TH1F*)f4->Get("pair_Ecounter17");	
  pair_Ecounter[17] = (TH1F*)f4->Get("pair_Ecounter18");
	

  for (int i=NEcounter_min-1; i<NEcounter_max; i++)
    {
      TCanvas* canvas = new TCanvas;
      canvas->cd();
      TH1F* data_plot = (TH1F*)data_Ecounter[i]->Clone();
      data_plot->SetLineColor(kBlack);
      data_plot->SetLineWidth(2);
      data_plot->GetXaxis()->SetRangeUser(-0.5, 1.0);
      data_plot->Draw("Ep");

      TH1F* empty_fit = (TH1F*)empty_Ecounter[i]->Clone();
      empty_fit->Scale(31.65/19.91);
      TH1F* mc_fit = (TH1F*)mc_Ecounter[i]->Clone();
      TH1F* outTcut_fit = (TH1F*)outTcut_Ecounter[i]->Clone();
      TH1F* pair_fit = (TH1F*)pair_Ecounter[i]->Clone();
      pair_fit->Scale(31.65/31.0);


      fit_his(data_plot,empty_fit,mc_fit,outTcut_fit,pair_fit,i);
           
      TF1* func = data_plot->GetFunction(Form("fit_func%d",i));
      Double_t param[5];
      func->GetParameters(param);

       Dyield_1[i] = mc_Ecounter[i]->GetEntries() * param[2];
      //if (i==17) cout << aaaa << "---------" << endl;    
      //   cout << "Yield -------" << Dyield[i] << endl;
    
    
      TH1F* empty_plot = (TH1F*)empty_fit->Clone();
      empty_plot->Scale(param[0]);
      empty_plot->SetLineColor(kGreen);
      empty_plot->SetLineWidth(2);
      empty_plot->GetXaxis()->SetRangeUser(-0.5, 1.0);
      empty_plot->Draw("same");
      
      TH1F* outTcut_plot = (TH1F*)outTcut_fit->Clone();
      outTcut_plot->Scale(param[1]);
      outTcut_plot->SetLineColor(kYellow+1.2);
      outTcut_plot->SetLineWidth(2);
      outTcut_plot->GetXaxis()->SetRangeUser(-0.5, 1.0);
      outTcut_plot->Draw("same");
     
      TH1F* mc_plot = (TH1F*)mc_fit->Clone();
     
     

      TH1F* mc_shift = shifthis(mc_plot,param[4]);
   
      mc_shift->Scale(param[2]);
      mc_shift->SetLineColor(kBlue);
      mc_shift->SetLineWidth(2);
      mc_shift->GetXaxis()->SetRangeUser(-0.5, 1.0);
      mc_shift->Draw("same");
    
      TH1F* pair_plot = (TH1F*)pair_fit->Clone();
      pair_plot->Scale(param[3]);
      pair_plot->SetLineColor(kCyan);
      pair_plot->SetLineWidth(2);
      pair_plot->GetXaxis()->SetRangeUser(-0.5, 1.0);
      pair_plot->Draw("same");
      //  func->Draw("same");
     
   
      //  TLegend *leg1 = new TLegend(0.1,0.7,0.4,0.9);
      TLegend *leg1 = new TLegend(0.6,0.3,0.9,0.5);
      
      leg1->AddEntry(empty_plot,  "empty run back ground -[p0]","l");
      leg1->AddEntry(outTcut_plot,"accidental back ground -[p1]","l");
      leg1->AddEntry(mc_shift,    "compton simulation      -[p2]","l");
      leg1->AddEntry(pair_plot,   "e+e- back ground         -[p3]","l");
      leg1->AddEntry(data_plot,   "#64876-64883 data ","l");
      leg1->AddEntry(func,   "fit ","l");
      
      

      leg1->Draw();
      canvas->Update();

     
      TPaveStats *st = (TPaveStats*)data_plot->GetListOfFunctions()->FindObject("stats");
      //st->SetTextSize(0.02);
      st->SetX1NDC(0.75);
      st->SetY1NDC(0.55);
      st->SetX2NDC(0.9);
      st->SetY2NDC(0.9);
   
     
      canvas->Modified(); 
      
	//    canvas->Print("test.pdf");
     
      TCanvas* canvas2 = new TCanvas;
    
      canvas2->cd();
      gStyle->SetOptStat(0);

      Double_t norm_data   =    data_Ecounter[i]->Integral();
      Double_t norm_mc     =      mc_Ecounter[i]->Integral();
      Double_t norm_outT   = outTcut_Ecounter[i]->Integral();
      Double_t norm_empty  =   empty_fit->Integral();
      Double_t norm_pair   =    pair_fit->Integral();
      
   cout << " norm_data: "<<  norm_data << " --norm_mc:" << norm_mc <<"-- norm_outT:"<< norm_outT<< " -- norm_empty:"<<norm_empty<<"--norm_pair:"<<norm_pair<<endl;


      TH1F* data_0 = (TH1F*)data_Ecounter[i]->Clone();
      TH1F* data_2 = (TH1F*)data_Ecounter[i]->Clone();
      data_2->SetName(Form("%s_mcNacci",data_0->GetName()));
      TH1F* outTcut_2 = (TH1F*)outTcut_Ecounter[i]->Clone();
      //  outTcut_2->Scale(norm_data/norm_outT);
      TH1F* empty_2 = (TH1F*)empty_fit->Clone();
      //  empty_2->Scale(norm_data/norm_empty);
      TH1F* mc_2 = (TH1F*)mc_Ecounter[i]->Clone();
      //  TH1F* mc_shift_2 = shifthis(mc_2,param[4]-0.05);
      TH1F* mc_shift_2 = shifthis(mc_2,-0.5);
      //  mc_shift_2->Scale(norm_data/norm_mc);
      TH1F* pair_2 = (TH1F*)pair_fit->Clone();
      TH1F* pair_shift_2 = shifthis(pair_2,-0.02812);
      //   pair_shift_2->Scale(norm_data/norm_pair);
    

        data_2->Add(empty_2,-32./31.64);
      //   data_2->Add(pair_2,-32./19.91);
      data_2->SetLineColor(1);
      data_2->SetLineWidth(4);
      mc_shift_2->SetLineColor(8);
      //just for view
      // data_2->Draw();
      //data_0->Draw("same");
      //  mc_shift_2->Draw("same");
      // 
      Double_t norm_data2 = data_2->Integral();
      Double_t norm_mc2 = mc_shift_2->Integral();
      Double_t norm_outT2 = outTcut_2->Integral();
      Double_t norm_empty2 = empty_2->Integral();
      Double_t norm_pair2 = pair_shift_2->Integral();
     


         cout << " norm_data2: "<<  norm_data2 << " --norm_mc2:" << norm_mc2 <<"-- norm_outT2:"<< norm_outT2<< " -- norm_empty2:"<<norm_empty2<<"--norm_pair2:"<<norm_pair2<<endl;

      TObjArray *mc = new TObjArray(3);        // MC histograms are put in this array
      mc->Add(mc_2);
      mc->Add(outTcut_2);
      mc->Add(pair_shift_2);

	TFractionFitter* fit = new TFractionFitter(data_2, mc); // initialise
	fit->Constrain(0,0.7,1.1);               // constrain fraction 1 to be between 0 and 1
	//	fit->Constrain(1,0.01,0.14);   
	//	fit->Constrain(2,0.07,1.1);     
	
		fit->Constrain(1,0.00,0.08);   
		fit->Constrain(2,0.07,1.0);  



	//	fit->SetRangeX(100,200);                    // use only the first 15 bins in the fit
	Int_t status = fit->Fit();               // perform the fit


	fit->GetResult(0, p0, error0);
	fit->GetResult(1, p1, error1);
	fit->GetResult(2, p2, error2);
	//	fit->GetResult(3, p3, error3);
       

	std::cout << "fit status: " << status << std::endl;
	if (status == 0) {                       // check on fit status
	  TH1F* result = (TH1F*) fit->GetPlot();

	  Double_t chi2 = fit->GetChisquare();
	  Int_t ndf = fit->GetNDF();
	  Double_t chi2_ndf = chi2/ndf;

	  data_2->GetXaxis()->SetRangeUser(-0.5, 1.0);
	  result->GetXaxis()->SetRangeUser(-0.5, 1.0);
	  mc_2->GetXaxis()->SetRangeUser(-0.5, 1.0);
	  outTcut_2->GetXaxis()->SetRangeUser(-0.5, 1.0);
	  pair_shift_2->GetXaxis()->SetRangeUser(-0.5, 1.0);

	  data_2->SetLineColor(1);
	  data_2->SetLineWidth(2);
	  data_2->Draw("Ep");
	 
	  result->SetFillColor(0);
	  result->SetLineColor(2);
	  result->SetLineWidth(2);
	  result->Draw("same");
	 
	  TH1F* mc_3 = (TH1F*)mc_2->Clone();
	  mc_3->Scale(p0*norm_data/norm_mc);
	  mc_3->SetFillColor(0);
	  mc_3->SetLineColor(4);
	  mc_3->SetLineWidth(1.0);
	  mc_3->Draw("same");

	  TH1F* out_3 = (TH1F*)outTcut_2->Clone();
	  out_3->Scale(p1*norm_data/norm_outT);
	  out_3->SetFillColor(3);
	  out_3->SetLineColor(3);
	  out_3->SetLineWidth(1.0);
	  out_3->Draw("same");

	  TH1F* pair_3 = (TH1F*)pair_shift_2->Clone();
	  pair_3->Scale(p2*norm_data/norm_pair);
	  pair_3->SetFillColor(0);
	  pair_3->SetLineColor(7);
	  pair_3->SetLineWidth(1.0);
	  pair_3->Draw("same");

	  TLegend *leg2 = new TLegend(0.6,0.3,0.9,0.5);
      
	  leg2->AddEntry(data_2,  "data point","l");
	  leg2->AddEntry(mc_3,    "compton simulation    -[p0]","l");
	  leg2->AddEntry(out_3,   "accidental             -[p1]","l");
	  leg2->AddEntry(pair_3,  "e+e- simulation      -[p2]","l");
	  
	  leg2->AddEntry(result,   "fit result ","l");
      
      

	  leg2->Draw();


	  TLatex latex;
	  latex.SetNDC();
	  latex.SetTextFont(42); 
	  latex.SetTextSize(0.040); 
	  latex.SetTextColor(1);        
	 
	  latex.DrawLatex(0.55,0.80,Form("simu p0 = %.3f +- %.5f",p0*norm_data/norm_mc,error0*norm_data/norm_mc)); 
	  latex.DrawLatex(0.55,0.75,Form("acci  p1 = %.3f +- %.5f",p1*norm_data/norm_outT,error1*norm_data/norm_outT));
	  latex.DrawLatex(0.55,0.70,Form("pair  p2 = %.3f +- %.5f",p2*norm_data/norm_pair,error2*norm_data/norm_pair));
	  latex.DrawLatex(0.55,0.60,Form("Chi2/DNF = %.3f",chi2_ndf));  

	
      
	  canvas2->Update();
	  canvas2->Modified(); 





///////////// error calculation //////////////////////////////////////


  Dyield_2[i] = norm_data*p0;
        
sys_error_fit[i] = error0/p0;


///////////////////////////////////////////////////////////////////////
//		check out put

/*
cout<<"###############################"<< endl;
cout<<"Ecounter : " << i+1 << endl;
//cout<<" -- p0 : "<<p0<<" -- error0:"<<error0<<" -- real p0 mc: ---"<<p0*norm_data/norm_mc<< endl;
//cout<<" -- p1 : "<<p1<<" -- error1:"<<error1<<" -- real p1 outT: ---"<<p1*norm_data/norm_outT<< endl;
//cout<<" -- p2 : "<<p2<<" -- error3:"<<error2<<" -- real p2 pair: ---"<<p2*norm_data/norm_pair<< endl;
//cout<<"chi2/ndf:"<<chi2_ndf<<endl;

cout<<"###############################"<<endl;

*/
	}

      

   }

 
   
  const double acc_p0 = -0.04842 ;  	
  const double acc_p1 =  0.1158  ;  	
  const double acc_p2 = -0.01504 ;  	



  for (int j=NEcounter_min-1 ; j<NEcounter_max ; j++)
    {
      //Dyield[j] = h_eid_data->GetBinContent(j+1); 
      //Dyield[j] = data_Ecounter[j]->GetEntries();
      //MCyield[j] = h_eid_mc->GetBinContent(j+1); 
      MCyield[j] = mc_Ecounter[j]->GetEntries();
      Ecounter[j] = h_eid_data->GetXaxis()->GetBinCenter(j+1);
      Acc[j]= (acc_p0 + acc_p1*Energy[j]+acc_p2*Energy[j]*Energy[j])*(1.0-0.008);
   
      Theory[j] = 0.77437 -0.151152*Energy[j]+0.0102022*Energy[j]*Energy[j];
     
    } 

for ( int i=NEcounter_min-1 ; i< NEcounter_max ;i++)
    {
	

    //	EAcc[i] = MCEyield[i]/1000000.0;
    Ecross[i]= Dyield_1[i]/flux_Bin[i];
    Rcross_1[i]= (Dyield_1[i]*10e27)/(Ne*flux_Bin[i]*10e9*Acc[i]);
    Rcross_2[i]= (Dyield_2[i]*10e27)/(Ne*flux_Bin[i]*10e9*Acc[i]);
      
    diff_1[i] = 100.0*(Rcross_1[i] - Theory[i])/Theory[i];
    diff_2[i] = 100.0*(Rcross_2[i] - Theory[i])/Theory[i];
    

    }
	

for ( int i=NEcounter_min-1 ; i< NEcounter_max ;i++)
    {
	

sys_error_cut[i]   = 1.0/100.0;
sys_error_yield[i] = sqrt(sys_error_fit[i]*sys_error_fit[i]+sys_error_cut[i]*sys_error_cut[i]) ;

sys_error_const[i] = 0.02/100.0;      // C12 target error 0.02%
sys_error_acc[i]   = 0.4/100.0 ;
sys_error_flux[i]  = 1.0/100.0;
sys_error[i]       = sqrt(sys_error_yield[i]*sys_error_yield[i]+sys_error_const[i]*sys_error_const[i]+sys_error_acc[i]*sys_error_acc[i]+sys_error_flux[i]*sys_error_flux[i]);

stat_error[i] = 1.0 / sqrt(Dyield_2[i]);

cross_error[i] = (sys_error[i] + stat_error[i]) ;
  
cross_error_plot[i] = cross_error[i] * Rcross_2[i];
diff_error_plot[i]  = cross_error[i] * 100.0;

    }
	




///////////////// print check information  //////////////////////
  
  for ( int i=NEcounter_min-1 ; i< NEcounter_max ;i++)
    {
	
	cout<<"-------------------------------"<<endl;
	
cout<<"sys_error_yield"<<i<<" ~~ "<<sys_error_yield[i]*100.0<<endl;
cout<<"sys_error"<<i<<" ~~ "<<sys_error[i]*100.0<<endl;
cout<<"stat_error"<<i<<" ~~ "<<stat_error[i]*100.0<<endl;
cout<<"cross_error"<<i<<" ~~ "<<cross_error[i]*100.0<<endl;


	
	cout<<"--------------------------------"<<endl;
    }
	
/////////////////////////////////////////////////////////////////




  // TMultiGraph *mg = new TMultiGraph("mg"," (Datayield-MCyield)/Datayield");
  TMultiGraph *mg1 = new TMultiGraph("mg1","cross section #64876-64883 C12 target");
  TMultiGraph *mg2 = new TMultiGraph("mg1","cross section diff #64876-64883 C12 target");
 
  TGraphErrors *g1 = new TGraphErrors(18,Energy,Rcross_1,0,0);
  TGraphErrors *g2 = new TGraphErrors(18,Energy,Rcross_2,0,cross_error_plot);
  TGraphErrors *g3 = new TGraphErrors(18,Energy,diff_1,0,0);	
  TGraphErrors *g4 = new TGraphErrors(18,Energy,diff_2,0,diff_error_plot);
  
  TF1 * gli = new TF1("gli", "0.77437 -0.151152*x+0.0102022*x*x", 4.2,5.4);
  TF1 * bound1 = new TF1("bound1", " 2.0", 4.2,5.4);
  TF1 * bound2 = new TF1("bound2", "-2.0", 4.2,5.4);



  TCanvas *c11 = new TCanvas("c11","counts vs T",600,300);
  //  TCanvas *c22 = new TCanvas("c22","XY position",800,800);
  gStyle->SetPalette(0,0);
  // c11->Divide(1,2);
  c11->cd();

 
  //  g1->Draw("A*");
  g1->SetName("g1");
  g1->SetTitle("");  
  g1->SetMarkerStyle(20);
  g1->SetMarkerColor(4);
  g1->SetMarkerSize(1.0);
  g1->SetDrawOption("AP");
  g1->SetLineColor(1);
  g1->SetLineWidth(0);
  g1->SetFillStyle(0);
  g1->GetYaxis()->SetTitle("");
  g1->GetXaxis()->SetTitle("");
   
  g2->SetName("g2");
  g2->SetTitle("");  
  g2->SetMarkerStyle(22);
  g2->SetMarkerColor(4);
  g2->SetMarkerSize(1.0);
  g2->SetDrawOption("AP");
  g2->SetLineColor(1);
  g2->SetLineWidth(0);
  g2->SetFillStyle(0);
  g2->GetYaxis()->SetTitle("");
  g2->GetXaxis()->SetTitle("");
   




 // mg1->Add(g1);
  mg1->Add(g2);
  //mg->Add(gli);	

  mg1->Draw("AP");
  mg1->GetXaxis()->SetTitle("Energy (GeV) ");
  mg1->GetYaxis()->SetTitle("   cross section (mb) ");
  gPad->Modified();
  mg1->GetXaxis()->SetLimits(4.3,5.4);
  mg1->SetMinimum(0.22);
  mg1->SetMaximum(0.38);
  gli->Draw("same");

  TLegend *leg1 = new TLegend(0.5,0.7,0.9,0.9);
  //leg1->AddEntry("g1","#64876-64883 data (func fit)");
  leg1->AddEntry("g2","#64876-64883 data (TfractionFitter)");
  leg1->AddEntry("gli","Theory");
  leg1->Draw();
  	
  //  c11->cd(2);
 
   TCanvas *c22 = new TCanvas("c22","XY position",600,300);
  gStyle->SetPalette(0,0);
  // c22->Divide(1,2);
  c22->cd();

  
  g3->SetName("g3");
  g3->SetTitle("");  
  g3->SetMarkerStyle(20);
  g3->SetMarkerColor(4);
  g3->SetMarkerSize(1.0);
  g3->SetDrawOption("AP");
  g3->SetLineColor(1);
  g3->SetLineWidth(0);
  g3->SetFillStyle(0);
  g3->GetYaxis()->SetTitle("");
  g3->GetXaxis()->SetTitle("");
   
  g4->SetName("g4");
  g4->SetTitle("");  
  g4->SetMarkerStyle(22);
  g4->SetMarkerColor(4);
  g4->SetMarkerSize(1.0);
  g4->SetDrawOption("AP");
  g4->SetLineColor(1);
  g4->SetLineWidth(0);
  g4->SetFillStyle(0);
  g4->GetYaxis()->SetTitle("");
  g4->GetXaxis()->SetTitle("");
   


 
 // mg2->Add(g3);
  mg2->Add(g4);

  mg2->Draw("AP");
  mg2->GetXaxis()->SetTitle("Energy (GeV) ");
  mg2->GetYaxis()->SetTitle("  diff (%) ");
  gPad->Modified();
  mg2->GetXaxis()->SetLimits(4.3,5.4);
  mg2->SetMinimum(-10.0);
  mg2->SetMaximum(10.0);
  bound1->Draw("same");
  bound2->Draw("same");

  TLegend *leg2 = new TLegend(0.5,0.7,0.9,0.9);
  leg2->AddEntry("bound1","+-2% bound");
  leg2->AddEntry("g4","#64876-64883 data (TfractionFitter)");
 
  leg2->Draw();
 
  	
 /*  
  TCanvas *c22 = new TCanvas("c22","XY position",800,800);
  c22->cd();

  mc_Ecounter[0]->Draw();
  empty_Ecounter[0]->SetLineColor(kGreen);
  empty_Ecounter[0]->Draw("same");
  data_Ecounter[0]->SetLineColor(kRed);
  data_Ecounter[0]->Draw("same");
  outTcut_Ecounter[0]->SetLineColor(kYellow);
  outTcut_Ecounter[0]->Draw("same");
 */




}












