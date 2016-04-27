
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

const float Na =  6.02214e+23;
const float Ne = (2.1236)*Na/2.0 ; 
const float data_flux = 31.64610863 ;          // flux for 64876-64883 8 runs
const float empty_flux = 19.905342 ;           // flux for empty run# 64885 
const float pair_flux = 31.0 ;                 // flux for pair run#
float scale_empty = data_flux/empty_flux;
float scale_pair  = data_flux/pair_flux;

void C5_cross2()

{
  gStyle->SetOptStat(1);
  gStyle->SetOptFit(1);
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
  float Acc_error[18],flux_total[18];
  float Theory[18],diff[18];
  float Ydata[18],Yempty[18],Ypair[18];

  float sys_error_yield[18],sys_error_const[18],sys_error_acc[18],sys_error_flux[18]; 	
  float sys_error[18],stat_error[18]; 
  float sys_error_fit[18],sys_error_cut[18];  
  float cross_error[18],cross_error_plot[18],diff_error_plot[18];

  float flux_Bin1[18],flux_Bin2[18],flux_Bin3[18],flux_Bin4[18],flux_Bin5[18],flux_Bin6[18],flux_Bin7[18],flux_Bin8[18];
  float flux_total[18];

  

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
  TH1F *data_z = new TH1F("data_z0","Z position",600,-300,300);
  TH1F *pair_z = new TH1F("pair_z0","Z position",600,-300,300);
  TH1F *mc_z = new TH1F("mc_z0","Z position",600,-300,300);

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

 

  TFile *f1 = new TFile("/home/mepmsu/Desktop/premex-2/analysis/222/Data64876_64883_e1_5_z200.root","r");
  //   TFile *f1 = new TFile("/w/hallb-scifs1a/primex/liye/222/Data64876_64883_e1_5_z200.root","r");

  if (!f1) cout << " erro " << endl;

   TFile *f2 = new TFile("/home/mepmsu/Desktop/premex-2/analysis/222/MC64876_64883_e1_5_z200.root","r");
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
 
  vector <float> gEEflux1;
  vector <float> gEEflux2;
  vector <float> gEEflux3;
  vector <float> gEEflux4;
  vector <float> gEEflux5;
  vector <float> gEEflux6;
  vector <float> gEEflux7;
  vector <float> gEEflux8;
  vector <float> gEEflux9;

  ifstream INPUT_file1;
  ifstream INPUT_file2;
  ifstream INPUT_file3;
  ifstream INPUT_file4;
  ifstream INPUT_file5;
  ifstream INPUT_file6;
  ifstream INPUT_file7;
  ifstream INPUT_file8;
  ifstream INPUT_file9;

  INPUT_file1.open("/home/mepmsu/Desktop/premex-2/analysis/flux/eflux_64876.dat");
  INPUT_file2.open("/home/mepmsu/Desktop/premex-2/analysis/flux/eflux_64877.dat");
  INPUT_file3.open("/home/mepmsu/Desktop/premex-2/analysis/flux/eflux_64878.dat");
  INPUT_file4.open("/home/mepmsu/Desktop/premex-2/analysis/flux/eflux_64879.dat");
  INPUT_file5.open("/home/mepmsu/Desktop/premex-2/analysis/flux/eflux_64880.dat");
  INPUT_file6.open("/home/mepmsu/Desktop/premex-2/analysis/flux/eflux_64881.dat");
  INPUT_file7.open("/home/mepmsu/Desktop/premex-2/analysis/flux/eflux_64882.dat");
  INPUT_file8.open("/home/mepmsu/Desktop/premex-2/analysis/flux/eflux_64883.dat");
  


  char strline[2561];
  int n=0, runnumber,eid;
  float eflux;

  while(!INPUT_file1.eof())
    {
      INPUT_file1>>runnumber>>eid>>eflux;
      INPUT_file1.getline(strline,2560);
     
      gEEid.push_back(eid);
      gEEflux1.push_back(eflux);

      n++;
    }
   
  
  for(int k=0;k<18;k++)
    { 
      flux_Bin1[k]=0;
      for (int kk=0;kk<10;kk++)
	{ 
	  int kkk = kk + 10*k;
	  flux_Bin1[k] = flux_Bin1[k]+ gEEflux1[kkk];

	  //	  cout << gEEflux[kkk] <<"  " << kkk << endl;
	}
  
      //   cout << flux_Bin[k] <<"  " << k << endl;

    }
  

  while(!INPUT_file2.eof())
    {
      INPUT_file2>>runnumber>>eid>>eflux;
      INPUT_file2.getline(strline,2560);
     
      gEEflux2.push_back(eflux);

    }
   
  
  for(int k=0;k<18;k++)
    { 
      flux_Bin2[k]=0;
      for (int kk=0;kk<10;kk++)
	{ 
	  int kkk = kk + 10*k;
	  flux_Bin2[k] = flux_Bin2[k]+ gEEflux2[kkk];
	
	}
 
    }
  

  while(!INPUT_file3.eof())
    {
      INPUT_file3>>runnumber>>eid>>eflux;
      INPUT_file3.getline(strline,2560);
     
      gEEflux3.push_back(eflux);

    }
   
  
  for(int k=0;k<18;k++)
    { 
      flux_Bin3[k]=0;
      for (int kk=0;kk<10;kk++)
	{ 
	  int kkk = kk + 10*k;
	  flux_Bin3[k] = flux_Bin3[k]+ gEEflux3[kkk];
	
	}
 
    }
  

  while(!INPUT_file4.eof())
    {
      INPUT_file4>>runnumber>>eid>>eflux;
      INPUT_file4.getline(strline,2560);
     
      gEEflux4.push_back(eflux);

    }
   
  
  for(int k=0;k<18;k++)
    { 
      flux_Bin4[k]=0;
      for (int kk=0;kk<10;kk++)
	{ 
	  int kkk = kk + 10*k;
	  flux_Bin4[k] = flux_Bin4[k]+ gEEflux4[kkk];
	
	}
 
    }
  

  while(!INPUT_file5.eof())
    {
      INPUT_file5>>runnumber>>eid>>eflux;
      INPUT_file5.getline(strline,2560);
     
      gEEflux5.push_back(eflux);

    }
   
  
  for(int k=0;k<18;k++)
    { 
      flux_Bin5[k]=0;
      for (int kk=0;kk<10;kk++)
	{ 
	  int kkk = kk + 10*k;
	  flux_Bin5[k] = flux_Bin5[k]+ gEEflux5[kkk];
	
	}
 
    }
  

  while(!INPUT_file6.eof())
    {
      INPUT_file6>>runnumber>>eid>>eflux;
      INPUT_file6.getline(strline,2560);
     
      gEEflux6.push_back(eflux);

    }
   
  
  for(int k=0;k<18;k++)
    { 
      flux_Bin6[k]=0;
      for (int kk=0;kk<10;kk++)
	{ 
	  int kkk = kk + 10*k;
	  flux_Bin6[k] = flux_Bin6[k]+ gEEflux6[kkk];
	
	}
 
    }
  
  while(!INPUT_file7.eof())
    {
      INPUT_file7>>runnumber>>eid>>eflux;
      INPUT_file7.getline(strline,2560);
     
      gEEflux7.push_back(eflux);

    }
   
  
  for(int k=0;k<18;k++)
    { 
      flux_Bin7[k]=0;
      for (int kk=0;kk<10;kk++)
	{ 
	  int kkk = kk + 10*k;
	  flux_Bin7[k] = flux_Bin7[k]+ gEEflux7[kkk];
	
	}
 
    }
  

  while(!INPUT_file8.eof())
    {
      INPUT_file8>>runnumber>>eid>>eflux;
      INPUT_file8.getline(strline,2560);
     
      gEEflux8.push_back(eflux);

    }
   
  
  for(int k=0;k<18;k++)
    { 
      flux_Bin8[k]=0;
      for (int kk=0;kk<10;kk++)
	{ 
	  int kkk = kk + 10*k;
	  flux_Bin8[k] = flux_Bin8[k]+ gEEflux8[kkk];
	
	}
 
    }
  
  
  
for (int i =0;i<18;i++)
    { 
      flux_total[i] = flux_Bin1[i]+flux_Bin2[i]+flux_Bin3[i]+flux_Bin4[i]+flux_Bin5[i]+flux_Bin6[i]+flux_Bin7[i]+flux_Bin8[i]; 
     cout<< flux_total[i]<<endl;
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


  for (int i=0; i<18; i++)
    {
      TCanvas* canvas = new TCanvas;
      
      Ydata[i] = data_Ecounter[i]->GetEntries();
      Yempty[i] = empty_Ecounter[i]->GetEntries();
      Ypair[i] = pair_Ecounter[i]->GetEntries();


      //if (i==17) cout << aaaa << "---------" << endl;    
      //   cout << "Yield -------" << Dyield[i] << endl;
    
      TH1F* data_plot = (TH1F*)data_Ecounter[i]->Clone();
      data_plot->SetLineColor(kBlack);
      data_plot->SetLineWidth(2);
      data_plot->Draw();
    
      TH1F* empty_plot = (TH1F*)empty_Ecounter[i]->Clone();
      empty_plot->Scale(scale_empty);
      empty_plot->SetLineColor(kGreen);
      empty_plot->SetLineWidth(2);
      empty_plot->Draw("same");
      


    /*   
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
      */ 
      TH1F* pair_plot = (TH1F*)pair_Ecounter[i]->Clone();
      pair_plot->Scale(scale_pair);
      pair_plot->SetLineColor(kCyan);
      pair_plot->SetLineWidth(2);
      pair_plot->Draw("same");
      //  func->Draw("same");

      TH1F* aftercut = (TH1F*)data_Ecounter[i]->Clone();
      TH1F* pair_fit = (TH1F*)pair_Ecounter[i]->Clone();
      TH1F* empty_fit = (TH1F*)empty_Ecounter[i]->Clone();
      pair_fit->Scale(scale_pair);
      empty_fit->Scale(scale_empty);



      aftercut->Add(pair_fit,-1);
      aftercut->Add(empty_fit,-1);
      aftercut->SetLineColor(kRed);
      aftercut->SetLineWidth(2);
      aftercut->Draw("same");
   
      TLegend *leg1 = new TLegend(0.1,0.7,0.4,0.9);
      leg1->AddEntry(empty_plot,  "empty run back ground -[p0]","l");
      leg1->AddEntry(pair_plot,   "e+e- back ground         -[p3]","l");
      leg1->AddEntry(data_plot,   "#64876-64883 data ","l");
       

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


  const double acc_p0 = -0.1573 ;  	
  const double acc_p1 =  0.1599  ;  	
  const double acc_p2 = -0.01947 ;  	


  for (int j=0 ; j<18 ; j++)
    {
      //Dyield[j] = h_eid_data->GetBinContent(j+1); 
      //Dyield[j] = data_Ecounter[j]->GetEntries();
      //MCyield[j] = h_eid_mc->GetBinContent(j+1); 
      Dyield[j] = (1.0-0.02638)*(Ydata[j]-scale_empty*Yempty[j]-scale_pair*Ypair[j]);
      MCyield[j] = mc_Ecounter[j]->GetEntries();
      Ecounter[j] = h_eid_data->GetXaxis()->GetBinCenter(j+1);
      Acc[j]= (acc_p0 + acc_p1*Energy[j]+acc_p2*Energy[j]*Energy[j])*(1.0-0.008);   // 0.008 correction 
      Theory[j] = 0.77437 -0.151152*Energy[j]+0.0102022*Energy[j]*Energy[j];
     
     
    } 

  	
  for ( int i=0 ; i<18 ;i++)
    {
    Ecross[i]= Dyield[i]/flux_total[i];
    ratio[i] = Dyield[i]/Nor_MCyield[i];
    Rcross[i]= (Dyield[i]*10e27)/(Ne*flux_total[i]*10e9*Acc[i]);
    diff[i] = 100.0*(Rcross[i] - Theory[i])/Theory[i];
  
    }
	
for ( int i=0 ; i< 18 ;i++)
    {
	
sys_error_fit[i]   = 1.0/100.0;
sys_error_cut[i]   = 1.0/100.0;
sys_error_yield[i] = sqrt(sys_error_fit[i]*sys_error_fit[i]+sys_error_cut[i]*sys_error_cut[i]) ;

sys_error_const[i] = 0.02/100.0;      // C12 target error 0.02%
sys_error_acc[i]   = 0.4/100;
sys_error_flux[i]  = 1.0/100.0;
sys_error[i]       = sqrt(sys_error_yield[i]*sys_error_yield[i]+sys_error_const[i]*sys_error_const[i]+sys_error_acc[i]*sys_error_acc[i]+sys_error_flux[i]*sys_error_flux[i]);

stat_error[i] = 1.0 / sqrt(Dyield[i]);

cross_error[i] = (sys_error[i] + stat_error[i]) ;
  
cross_error_plot[i] = cross_error[i] * Rcross[i];
diff_error_plot[i]  = cross_error[i] * 100.0;


    }
	

///////////////// print check information  //////////////////////
  
  for ( int i=0 ; i< 18 ;i++)
    {
	
	cout<<"-------------------------------"<<endl;
cout<<"cross"<<i<<" ~~ "<<Rcross[i]<<endl;
cout<<"sys_error_yield"<<i<<" ~~ "<<sys_error_yield[i]*100.0<<endl;
cout<<"sys_error"<<i<<" ~~ "<<sys_error[i]*100.0<<endl;
cout<<"stat_error"<<i<<" ~~ "<<stat_error[i]*100.0<<endl;
cout<<"cross_error"<<i<<" ~~ "<<cross_error[i]*100.0<<endl;
cout<<"diff_error"<<i<<" ~~ "<<diff_error_plot[i]<<endl;

	
	cout<<"--------------------------------"<<endl;
    }
	
/////////////////////////////////////////////////////////////////











  // TMultiGraph *mg = new TMultiGraph("mg"," (Datayield-MCyield)/Datayield");
  TMultiGraph *mg1 = new TMultiGraph("mg","cross section #64876-64883 C12  ");
  TMultiGraph *mg2 = new TMultiGraph("mg1","cross section diff #64876-64883 C12 target");
  TGraphErrors *g1 = new TGraphErrors(18,Energy,Rcross,0,cross_error_plot);
  TGraphErrors *g2 = new TGraphErrors(18,Energy,diff,0,diff_error_plot);	
  
  TF1 * gli = new TF1("gli", "0.77437 -0.151152*x+0.0102022*x*x", 4.2,5.4);
  TF1 * bound1 = new TF1("bound1", " 2.0", 4.2,5.4);
  TF1 * bound2 = new TF1("bound2", "-2.0", 4.2,5.4);

  TCanvas *c11 = new TCanvas("c11","counts vs T",600,300);
   gStyle->SetPalette(0,0);
  // c11->Divide(1,2);
  c11->cd();

 
  //  g1->Draw("A*");
  g1->SetName("g1");
  g1->SetTitle("");  
  g1->SetMarkerStyle(22);
  g1->SetMarkerColor(4);
  g1->SetMarkerSize(1.0);
  g1->SetDrawOption("AP");
  g1->SetLineColor(1);
  g1->SetLineWidth(0);
  g1->SetFillStyle(0);
  g1->GetYaxis()->SetTitle("");
  g1->GetXaxis()->SetTitle("");
   

  mg1->Add(g1);
 

  mg1->Draw("AP");
  mg1->GetXaxis()->SetTitle("Energy (GeV) ");
  mg1->GetYaxis()->SetTitle("   cross section (mb) ");
  gPad->Modified();
  mg1->GetXaxis()->SetLimits(4.2,5.4);
  mg1->SetMinimum(0.22);
  mg1->SetMaximum(0.38);
  gli->Draw("same");

  TLegend *leg = new TLegend(0.5,0.7,0.9,0.9);
  leg->AddEntry("g1","#64876-64883 data (Subtraction)");
  leg->AddEntry("gli","Theory");
  leg->Draw();
  	
  // c11->cd(2);
 
  //***************************************************
  //***************
  //*******

  TCanvas *c22 = new TCanvas("c22","XY position",600,300);
  gStyle->SetPalette(0,0);
  // c22->Divide(1,2);
  c22->cd();

  g2->SetName("g2");
  g2->SetTitle("diff = (Data-Theory)/Theory ");  
  g2->SetMarkerStyle(22);
  g2->SetMarkerColor(4);
  g2->SetMarkerSize(1.0);
  g2->SetDrawOption("AP");
  g2->SetLineColor(1);
  g2->SetLineWidth(0);
  g2->SetFillStyle(0);
  g2->GetYaxis()->SetTitle("");
  g2->GetXaxis()->SetTitle("");
 
  mg2->Add(g2);

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
  leg2->AddEntry("bound1","+-2% bound ");	
  leg2->AddEntry("g2","#64876-64883 data (Subtraction)");
 
  leg2->Draw();
 
 


}












