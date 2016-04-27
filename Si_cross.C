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
#include "TPaveStats.h"
using namespace std;
#include "TMultiGraph.h"
#include "TGraphErrors.h"

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
  Double_t empty_yield = par[0]*(31.65/19.91)*empty->GetBinContent(bin); 
  bin = outTcut->GetXaxis()->FindBin(xx);
  Double_t outTcut_yield = par[1]*outTcut->GetBinContent(bin);
  xx = xx-par[4]; 
  bin = simulation->GetXaxis()->FindBin(xx);
  Double_t simulation_yield = par[2]*simulation->GetBinContent(bin); 
  Double_t pair_yield = par[3]*(31.64/31.0)*pairbackground->GetBinContent(bin); 

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
  fit_func->SetParLimits(4, -0.1, -0.015);   // p4 is for mc shift//


  // h_data->Fit(Form("fit_func%d",i),"0","",-0.5, 1.5);
  h_data->Fit(Form("fit_func%d",i),"","",-0.5, 1.5);
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





void Si_cross()

{
  gStyle->SetOptStat(1);
  //  gStyle->SetOptFit(1);
  gStyle->SetStatH(0.3);



  /*

    int main(int argc, char** argv)
    {
    int run = atoi(argv[1]);
    TFile *f1 = new TFile(Form("/home/mepmsu/Desktop/premex-2/analysis/data/prim_ana%dt.root",run),"r");

  */
  
 

  float run, evn;

  float Eid;
  float tid;      // Tcounter channel ID
  float tdiff;   //time difference between Hycal and Tcounter 
  float eg;      // incoming photon energy
  float e1;      // cluster 1 energy
  float x1;      // cluster 1 x position
  float y1;      // cluster 1 y position
  float e0;     
  float e2;      // cluster 2 energy
  float x2;      // cluster 2 x position
  float y2;      // cluster 2 y position
  float z;   

  float Dyield[181],MCyield[181],Ecounter[181],Nor_MCyield[181];
  float Ec[20],flux_eid[181],flux_Bin[18],input_Bin[18];
  float Energy[18],Ecross[18],Rcross[18],ratio[18],Acc[18],Nor_Acc[18];
  float NEid[18],Binwidth[18];
  float MC_E[18],MC_cross[18];
  float cross_error[18],Acc_error[18],flux_total[18];
  float Theory[18],diff[18];


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



  const  float PI=3.1415927;	
  for ( int j=0;j<180;j++){Dyield[j]=0;MCyield[j]=0;Ecounter[j]=0;}

  TH1F *h_evn = new TH1F("h_evn","eventid",100000,0,31000000);
  TH1F *h_E = new TH1F("h_E","E0 = eg - e1 -e2",1000,-2,2);
  TH1F *h_tdiff = new TH1F("tdiff","Tdiff",1000,-10,10);  
  TH1F *h_x = new TH1F("h_x","X position",60,-30,30);
  TH1F *h_sep = new TH1F("h_sep","Cluster Separation",500,0,50);
  TH1F *h_angle = new TH1F("h_angle","Azimuthal Angle Difference",500,120,240);
  TH1F *h_Eg = new TH1F("h_Eg","Eg",500,4,6);
  TH1F *h_eid_data = new TH1F("h_eid_data","Eid_Data",200,-10,190);
  TH1F *h_tid = new TH1F("h_tid","Tid",100,-1,40);
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

  // TFile *f1 = new TFile("/home/mepmsu/Desktop/premex-2/analysis/222/Data64876_64883_e1_5_z200.root","r");
     TFile *f1 = new TFile("/w/hallb-scifs1a/primex/liye/222/Data64876_64883_e1_5_z200.root","r");

  if (!f1) cout << " erro " << endl;

  // TFile *f2 = new TFile("/home/mepmsu/Desktop/premex-2/analysis/222/MC64876_64883_e1_5_z200.root","r");
  TFile *f2 = new TFile("/w/hallb-scifs1a/primex/liye/222/MC64876_64883_e1_5_z200.root","r");
  
   if (!f2) cout << " erro " << endl;
 
  TFile *f3 = new TFile("/w/hallb-scifs1a/primex/liye/222/empty64885_e1_5_z200.root","r");
  // TFile *f3 = new TFile("/home/mepmsu/Desktop/premex-2/analysis/222/empty64885_e1_5_z200.root","r");
   if (!f3) cout << " erro " << endl;
 
  TFile *f4 = new TFile("/w/hallb-scifs1a/primex/liye/222/MCpair64877_e1_5_z200.root","r");
  //TFile *f4 = new TFile("/home/mepmsu/Desktop/premex-2/analysis/222/MCpair64877_e1_5_z200.root","r");
 if (!f4) cout << " erro " << endl;
 




  vector <int> gEEid;
  vector <float> gEinput;
  vector <float> gEEflux;


 
 
  //==========================================================================



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
      fit_his(data_Ecounter[i],empty_Ecounter[i],mc_Ecounter[i],outTcut_Ecounter[i],pair_Ecounter[i],i);
           
      TF1* func = data_Ecounter[i]->GetFunction(Form("fit_func%d",i));
      Double_t param[5];
      func->GetParameters(param);

      Dyield[i] = mc_Ecounter[i]->GetEntries() * param[2];
      //if (i==17) cout << aaaa << "---------" << endl;    
      //   cout << "Yield -------" << Dyield[i] << endl;
    
      TH1F* data_plot = (TH1F*)data_Ecounter[i]->Clone();
      data_plot->SetLineColor(kBlack);
      data_plot->SetLineWidth(2);
      data_plot->Draw();
    
      TH1F* empty_plot = (TH1F*)empty_Ecounter[i]->Clone();
      empty_plot->Scale(param[0]);
      empty_plot->SetLineColor(kGreen);
      empty_plot->SetLineWidth(2);
      empty_plot->Draw("same");
      
      TH1F* outTcut_plot = (TH1F*)outTcut_Ecounter[i]->Clone();
      outTcut_plot->Scale(param[1]);
      outTcut_plot->SetLineColor(kYellow+1.2);
      outTcut_plot->SetLineWidth(2);
      outTcut_plot->Draw("same");
      
      TH1F* mc_plot = (TH1F*)mc_Ecounter[i]->Clone();
      mc_plot->Scale(param[2]);
      TH1F* mc_shift = shifthis(mc_plot,param[4]);
      mc_shift->SetLineColor(kBlue);
      mc_shift->SetLineWidth(2);
      mc_shift->Draw("same");
    
      TH1F* pair_plot = (TH1F*)pair_Ecounter[i]->Clone();
      pair_plot->Scale(param[3]);
      pair_plot->SetLineColor(kCyan);
      pair_plot->SetLineWidth(2);
      pair_plot->Draw("same");
      //  func->Draw("same");
     
      /*
      TH1F* hsum = (TH1F*)empty_Ecounter[i]->Clone();
      hsum->Add(outTcut_Ecounter[i]);
      hsum->Add(mc_shift);
      hsum->Add(pair_Ecounter[i]);
  
      hsum->SetLineColor(kRed);
      hsum->SetLineWidth(2);
      hsum->Draw("same");
      */
      
      TLegend *leg1 = new TLegend(0.1,0.7,0.4,0.9);
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
    }

  /*
    cout << "******************************************************" << endl;
    cout << "Fitting parameters: " << endl;
    for (int i=0; i<18; i++) {
      TF1* func = data_Ecounter[i]->GetFunction(Form("fit_func%d",i));
      cout << i << ": " << func->GetParameter(0) << " " << func->GetParameter(2) << endl;
    }
    cout << "******************************************************" << endl;


  */

  



  for (int j=NEcounter_min-1 ; j<NEcounter_max ; j++)
    {
      // Dyield[j] = h_eid_data->GetBinContent(j+1); 
      //Dyield[j] = data_Ecounter[j]->GetEntries();
      //	MCyield[j] = h_eid_mc->GetBinContent(j+1); 
      MCyield[j] = mc_Ecounter[j]->GetEntries();
      Ecounter[j] = h_eid_data->GetXaxis()->GetBinCenter(j+1);
      Acc[j] = MCyield[j]/input_Bin[j]/8.0;    
      Nor_Acc[j] = -0.0152863*Energy[j]+0.11304; 
      Theory[j] = 0.77437 -0.151152*Energy[j]+0.0102022*Energy[j]*Energy[j];
     
      cout << " Dyield = " << Dyield[j] << ";"<< "MCyield = " << MCyield[j]<< " Acc=" << Acc[j] << " i=" <<j<<endl;
      
    } 

  	
  for ( int i=NEcounter_min-1 ; i< NEcounter_max ;i++)
    {
	

    //	EAcc[i] = MCEyield[i]/1000000.0;
    Ecross[i]= Dyield[i]/flux_Bin[i];
    ratio[i] = Dyield[i]/Nor_MCyield[i];
    Rcross[i]= (Dyield[i]*10e27)/(Ne*flux_Bin[i]*10e9*Acc[i]);
    cross_error[i] = sqrt(Dyield[i])*10e17/Ne/flux_Bin[i]/Acc[i]; 
    Acc_error[i] = sqrt(1.0/MCyield[i] + 1.0/input_Bin[i])*Acc[i];
    diff[i] = 100.0*(Rcross[i] - Theory[i])/Theory[i];
    //	cout << cross_error[i] <<" = "<< i << endl;


    }
	

 

  //=================================================================
  /*
	
    double mean, sigma, xmin=0, xmax=0;
    TF1 *gaus=0;
    TCanvas *c1 = new TCanvas("c1","XY position",1200,800);
    c1->Divide(3,2);
    for(int i=0;i<6;i++)
    {
    c1->cd(i+1);
    xmin=-2.0, xmax=2.0;
    m_Ecounter[i]->Draw();
    m_Ecounter[i]->Fit("gaus","QR","",xmin,xmax);
    gaus = (TF1*) m_Ecounter[i]->GetListOfFunctions()->FindObject("gaus");
    mean = gaus->GetParameter(1);
    sigma = gaus->GetParameter(2);
    xmin = mean - 2.5*sigma;
    xmax = mean + 2.5*sigma;
    m_Ecounter[i]->Fit("gaus","R","",xmin,xmax);
    gaus = (TF1*) m_Ecounter[i]->GetListOfFunctions()->FindObject("gaus");
    mean = gaus->GetParameter(1);
    sigma = gaus->GetParameter(2);
    //cout <<prun<<" "<< "Tid= "<<i<< "mean= "<<mean<<endl;		

 
    }
    c1->cd(0);
    c1->Draw();


    TCanvas *c2 = new TCanvas("c2","XY position",1200,800);
    c2->Divide(3,2);
    for(int i=0;i<6;i++)
    {
    c2->cd(i+1);
    xmin=-2.0, xmax=2.0;
    m_Ecounter[i+6]->Draw();
    m_Ecounter[i+6]->Fit("gaus","QR","",xmin,xmax);
    gaus = (TF1*) m_Ecounter[i+6]->GetListOfFunctions()->FindObject("gaus");
    mean = gaus->GetParameter(1);
    sigma = gaus->GetParameter(2);
    xmin = mean - 2.5*sigma;
    xmax = mean + 2.5*sigma;
    m_Ecounter[i+6]->Fit("gaus","R","",xmin,xmax);
    gaus = (TF1*) m_Ecounter[i+6]->GetListOfFunctions()->FindObject("gaus");
    mean = gaus->GetParameter(1);
    sigma = gaus->GetParameter(2);
    //cout <<prun<<" "<< "Tid= "<<i<< "mean= "<<mean<<endl;		

 
    }
    c2->cd(0);
    c2->Draw();


    TCanvas *c3 = new TCanvas("c3","XY position",1200,800);
    c3->Divide(3,2);
    for(int i=0;i<6;i++)
    {
    c3->cd(i+1);
    xmin=-2.0, xmax=2.0;
    m_Ecounter[i+12]->Draw();
    m_Ecounter[i+12]->Fit("gaus","QR","",xmin,xmax);
    gaus = (TF1*) m_Ecounter[i+12]->GetListOfFunctions()->FindObject("gaus");
    mean = gaus->GetParameter(1);
    sigma = gaus->GetParameter(2);
    xmin = mean - 2.5*sigma;
    xmax = mean + 2.5*sigma;
    m_Ecounter[i+12]->Fit("gaus","R","",xmin,xmax);
    gaus = (TF1*) m_Ecounter[i+12]->GetListOfFunctions()->FindObject("gaus");
    mean = gaus->GetParameter(1);
    sigma = gaus->GetParameter(2);
    //cout <<prun<<" "<< "Tid= "<<i<< "mean= "<<mean<<endl;		

 
    }
    c3->cd(0);
    c3->Draw();



  */


  //c1->SaveAs(Form("tdiff_alignment_r%d.png",prun));
   
  

  // TMultiGraph *mg = new TMultiGraph("mg"," (Datayield-MCyield)/Datayield");
  TMultiGraph *mg = new TMultiGraph("mg","cross section #64876-64883 C12 target");
  TGraphErrors *g1 = new TGraphErrors(18,Energy,Rcross,cross_error,0);
  TGraphErrors *g2 = new TGraphErrors(18,Energy,diff,0,0);	
  
  TF1 * gli = new TF1("gli", "0.77437 -0.151152*x+0.0102022*x*x", 4.2,5.4);

  TCanvas *c11 = new TCanvas("c11","counts vs T",600,300);
  //  TCanvas *c22 = new TCanvas("c22","XY position",800,800);
  gStyle->SetPalette(0,0);
  // c11->Divide(1,2);
  c11->cd();

 
  //  g1->Draw("A*");
  g1->SetName("g1");
  g1->SetTitle("");  
  g1->SetMarkerStyle(21);
  g1->SetMarkerColor(4);
  g1->SetMarkerSize(1.0);
  g1->SetDrawOption("AP");
  g1->SetLineColor(1);
  g1->SetLineWidth(0);
  g1->SetFillStyle(0);
  g1->GetYaxis()->SetTitle("");
  g1->GetXaxis()->SetTitle("");
   

  mg->Add(g1);
  //  mg->Add(g2);
  //mg->Add(gli);	

  mg->Draw("AP");
  mg->GetXaxis()->SetTitle("Energy (GeV) ");
  mg->GetYaxis()->SetTitle("   cross section (mb) ");
  gPad->Modified();
  mg->GetXaxis()->SetLimits(4.2,5.4);
  mg->SetMinimum(0.22);
  mg->SetMaximum(0.38);
  gli->Draw("same");

  TLegend *leg = new TLegend(0.1,0.7,0.48,0.9);
  leg->AddEntry("g1","#64876-64883 data (way1)");
  leg->AddEntry("gli","Theory");
  leg->Draw();
  	
  //  c11->cd(2);
 
   TCanvas *c22 = new TCanvas("c22","XY position",600,300);
  gStyle->SetPalette(0,0);
  // c22->Divide(1,2);
  c22->cd();

  g2->SetName("g2");
  g2->SetTitle("diff = (Data-Theory)/Theory ");  
  g2->SetMarkerStyle(21);
  g2->SetMarkerColor(4);
  g2->SetMarkerSize(1.0);
  g2->SetDrawOption("AP");
  g2->SetLineColor(1);
  g2->SetLineWidth(0);
  g2->SetFillStyle(0);
  g2->GetYaxis()->SetTitle("diff (%)");
  g2->GetXaxis()->SetTitle("Energy (Gev)");
  g2->SetMinimum(-10.0);
  g2->SetMaximum(10.00);	   
  g2->Fit("pol0","QR","",-0.5,1.5);
  
  g2->Draw();


 
  	
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












