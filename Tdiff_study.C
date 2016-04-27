
#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdlib.h>

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
#include "TCutG.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TInterpreter.h"
#include "TStyle.h"
#include "TH1.h"
#include "TH2.h"
#include "TMath.h"
#include "TTree.h"
#include "TH1F.h"
#include "TMultiGraph.h"
#include "TGraphErrors.h"


using namespace std;


vector <int> gEStart;
vector <int> gEEnd;
vector <int> gEType;
vector <int> gEEid;
vector <float> gEEflux;



float Tid_65078[18],Tid_64878[18];
float Bin_Energy[18],Bin_yield[18],Nor_yield[18],Bin_flux[18];
float Nor_Acc[18],cross[18],Bin_width[18];
const float Ne = (2.1236)*6.02214e+23/2.0 ; 
float Energy[200],yield[200],Ecounter[200];

/*

  TCutG *gCutG_ZVsE0=0;

  void BuiltCutG()
  {
  //========= Macro generated from object: CUTG/Graph
  //========= by ROOT version5.34/09
  gCutG_ZVsE0 = new TCutG("CUTG_ZvsE0",11);
  gCutG_ZVsE0->SetVarX("e0");
  gCutG_ZVsE0->SetVarY("z");
  gCutG_ZVsE0->SetTitle("Graphic Cut for Z vs E0");
  gCutG_ZVsE0->SetPoint(0,-0.0680873,54.5026);
  gCutG_ZVsE0->SetPoint(1,-0.0275602,56.8686);
  gCutG_ZVsE0->SetPoint(2,0.137666,12.7036);
  gCutG_ZVsE0->SetPoint(3,0.242101,-32.25);
  gCutG_ZVsE0->SetPoint(4,0.179752,-54.3325);
  gCutG_ZVsE0->SetPoint(5,-0.0182078,-53.5438);
  gCutG_ZVsE0->SetPoint(6,-0.217726,11.9149);
  gCutG_ZVsE0->SetPoint(7,-0.192786,38.7294);
  gCutG_ZVsE0->SetPoint(8,-0.139789,46.616);
  gCutG_ZVsE0->SetPoint(9,-0.099262,50.5593);
  gCutG_ZVsE0->SetPoint(10,-0.0680873,54.5026);

  }
  int IsGoodZNE0(double e0, double z)
  {
  if (!gCutG_ZVsE0)  BuiltCutG();
  return gCutG_ZVsE0->IsInside(e0,z);
  }
*/


//usage: exe <run#>
int main(int argc, char** argv)
{
  int run1 = atoi(argv[1]);
  TFile *f1 = new TFile(Form("/w/hallb-scifs1a/primex/liye/222/Data64876_%d.root",run1),"r");
  
 
  
  //  ifstream INPUT_file;
  //INPUT_file.open(Form("/w/hallb-scifs1a/primex/liye/flux/eflux_%05d.dat",run));
  //  if( !INPUT_file)cout<<"ERROR!!!! CAN'T OPEN input file for run "<<run<<endl;
 
  int run;
  int eventid;
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
  float Tdiff_a;
  float z;
  float sep;
  const float PI= 3.1415927;


  char strline[2561];
  int n=0, flux_runnumber,flux_eid;
  float eflux;
 
  /*

    for (int n=0;n<200;n++)
    { Energy[n]=0; yield[n]=0; Ecounter[n]=0;}
  
    while(!INPUT_file.eof())
    {
    INPUT_file>>flux_runnumber>>flux_eid>>eflux;
    INPUT_file.getline(strline,2560);
      
    gEEid.push_back(flux_eid);
    gEEflux.push_back(eflux);

    //	cout<<"eflux="<<eflux<<"; "<<endl;
    n++;
    }
   
    for(int i=0;i<190;i++) cout<<gEEflux[i]<<" =="<<i<<endl;
 
    for(int k=0;k<18;k++)
    { 
    Bin_flux[k]=0;
    for (int kk=0;kk<10;kk++)
    { 
    int kkk = kk + 10*k;
    Bin_flux[k] = Bin_flux[k]+ gEEflux[kkk];

	  
    }
  
     
    }
  

  */

  Bin_Energy[0] = 5.281;    Bin_Energy[1] = 5.241;	Bin_Energy[2] = 5.195;
  Bin_Energy[3] = 5.145;    Bin_Energy[4] = 5.093;	Bin_Energy[5] = 5.041;
  Bin_Energy[6] = 4.988;    Bin_Energy[7] = 4.937;	Bin_Energy[8] = 4.883;
  Bin_Energy[9] = 4.827;    Bin_Energy[10]= 4.774;	Bin_Energy[11]= 4.727;
  Bin_Energy[12]= 4.663;    Bin_Energy[13]= 4.609;	Bin_Energy[14]= 4.554;
  Bin_Energy[15]= 4.503;    Bin_Energy[16]= 4.447;	Bin_Energy[17]= 4.400;




 
 
 
  TTree *event = (TTree*)f1->Get("haha");

  event->SetBranchAddress("run", &run);
  event->SetBranchAddress("eventid", &eventid);
  event->SetBranchAddress("Tdiff_a", &Tdiff_a);
  event->SetBranchAddress("Tid", &Tid);
  event->SetBranchAddress("Eid", &Eid);
  event->SetBranchAddress("Eg",&Eg);
  // event->SetBranchAddress("id1",&id1);
  // event->SetBranchAddress("id2",&id2);
  // event->SetBranchAddress("type1",type1);
  // event->SetBranchAddress("type2",type2);
  event->SetBranchAddress("x1",&x1);
  event->SetBranchAddress("y1",&y1);
  event->SetBranchAddress("x2",&x2);
  event->SetBranchAddress("y2",&y2);
  event->SetBranchAddress("e1",&e1);
  event->SetBranchAddress("e2",&e2);
  event->SetBranchAddress("e0",&e0);
  event->SetBranchAddress("z",&z);
  event->SetBranchAddress("sep",&sep);

  TH1F *h_evn = new TH1F("h_evn","eventid",310,0,31000000);
  TH1F *h_elas = new TH1F("h_E_elas","E0 = eg - e1 -e2",500,-3,3);
  TH1F *h_Zcut_elas = new TH1F("h_Zcut_elas","E0 = eg - e1 -e2",500,-3,3);
  TH1F *inTcut_elas = new TH1F("inTcut_elas","E0 = eg - e1 -e2",500,-3,3);
  TH1F *outTcut_elas = new TH1F("outTcut_elas","E0 = eg - e1 -e2",500,-3,3);
  TH1F *outTcut_Zcut_elas = new TH1F("outTcut_Zcut_elas","E0 = eg - e1 -e2",500,-3,3);

 
  TH1F *outTcut00 = new TH1F("outTcut00"," T = -5ns ~ -4ns ",200,-0.5,1.5);
  TH1F *outTcut01 = new TH1F("outTcut01"," T = -4ns ~ -3ns ",200,-0.5,1.5);
  TH1F *outTcut02 = new TH1F("outTcut02"," T = -3ns ~ -2ns ",200,-0.5,1.5);
  TH1F *outTcut03 = new TH1F("outTcut03"," T = -2ns ~ -1ns ",200,-0.5,1.5);
  TH1F *outTcut04 = new TH1F("outTcut04"," T = -1ns ~ 0ns ",200,-0.5,1.5);
  TH1F *outTcut05 = new TH1F("outTcut05"," T = 0ns ~ 1ns ",200,-0.5,1.5);
  TH1F *outTcut06 = new TH1F("outTcut06"," T = 1ns ~ 2ns ",200,-0.5,1.5);
  TH1F *outTcut07 = new TH1F("outTcut07"," T = 2ns ~ 3ns ",200,-0.5,1.5);
  TH1F *outTcut08 = new TH1F("outTcut08"," T = 3ns ~ 4ns ",200,-0.5,1.5);
  TH1F *outTcut09 = new TH1F("outTcut09"," T = 4ns ~ 5ns ",200,-0.5,1.5);

 
  TH1F * outTcut_nocuts00 = new TH1F("outTcut_nocuts00"," T = -13ns ~ -11ns ",200,-1.0,1.5);
  TH1F * outTcut_nocuts01 = new TH1F("outTcut_nocuts01"," T = -11ns ~ -09ns ",200,-1.0,1.5);
  TH1F * outTcut_nocuts02 = new TH1F("outTcut_nocuts02"," T = -09ns ~ -07ns ",200,-1.0,1.5);
  TH1F * outTcut_nocuts03 = new TH1F("outTcut_nocuts03"," T = -07ns ~ -05ns ",200,-1.0,1.5);
  TH1F * outTcut_nocuts04 = new TH1F("outTcut_nocuts04"," T =  05ns  ~ 07ns ",200,-1.0,1.5);
  TH1F * outTcut_nocuts05 = new TH1F("outTcut_nocuts05"," T =  07ns ~  09ns ",200,-1.0,1.5);
  TH1F * outTcut_nocuts06 = new TH1F("outTcut_nocuts06"," T =  09ns ~  11ns ",200,-1.0,1.5);
  TH1F * outTcut_nocuts07 = new TH1F("outTcut_nocuts07"," T =  11ns ~  13ns ",200,-1.0,1.5);
  TH1F * outTcut_nocuts08 = new TH1F("outTcut_nocuts08"," T =  13ns ~  15ns ",200,-1.0,1.5);
  TH1F * outTcut_nocuts09 = new TH1F("outTcut_nocuts09"," T =  15ns ~  17ns ",200,-1.0,1.5);
  TH1F * outTcut_nocuts10 = new TH1F("outTcut_nocuts10"," T =  17ns ~  19ns ",200,-1.0,1.5);
  TH1F * outTcut_nocuts11 = new TH1F("outTcut_nocuts11"," T =  19ns ~  21ns ",200,-1.0,1.5);
  TH1F * outTcut_nocuts12 = new TH1F("outTcut_nocuts12"," T =  21ns ~  23ns ",200,-1.0,1.5);
  TH1F * outTcut_nocuts13 = new TH1F("outTcut_nocuts13"," T =  23ns ~  25ns ",200,-1.0,1.5);
  TH1F * outTcut_nocuts14 = new TH1F("outTcut_nocuts14"," T = -13ns ~ -05ns ",200,-1.0,1.5);
  TH1F * outTcut_nocuts15 = new TH1F("outTcut_nocuts15"," T =  05ns ~  25ns ",200,-1.0,1.5);
  
  TH1F * outTcut_withcuts00 = new TH1F("outTcut_withcuts00"," T = -13ns ~ -11ns ",200,-1.0,1.5);
  TH1F * outTcut_withcuts01 = new TH1F("outTcut_withcuts01"," T = -11ns ~ -09ns ",200,-1.0,1.5);
  TH1F * outTcut_withcuts02 = new TH1F("outTcut_withcuts02"," T = -09ns ~ -07ns ",200,-1.0,1.5);
  TH1F * outTcut_withcuts03 = new TH1F("outTcut_withcuts03"," T = -07ns ~ -05ns ",200,-1.0,1.5);
  TH1F * outTcut_withcuts04 = new TH1F("outTcut_withcuts04"," T =  05ns ~  07ns ",200,-1.0,1.5);
  TH1F * outTcut_withcuts05 = new TH1F("outTcut_withcuts05"," T =  07ns ~  09ns ",200,-1.0,1.5);
  TH1F * outTcut_withcuts06 = new TH1F("outTcut_withcuts06"," T =  09ns ~  11ns ",200,-1.0,1.5);
  TH1F * outTcut_withcuts07 = new TH1F("outTcut_withcuts07"," T =  11ns ~  13ns ",200,-1.0,1.5);
  TH1F * outTcut_withcuts08 = new TH1F("outTcut_withcuts08"," T =  13ns ~  15ns ",200,-1.0,1.5);
  TH1F * outTcut_withcuts09 = new TH1F("outTcut_withcuts09"," T =  15ns ~  17ns ",200,-1.0,1.5);
  TH1F * outTcut_withcuts10 = new TH1F("outTcut_withcuts10"," T =  17ns ~  19ns ",200,-1.0,1.5);
  TH1F * outTcut_withcuts11 = new TH1F("outTcut_withcuts11"," T =  19ns ~  21ns ",200,-1.0,1.5);
  TH1F * outTcut_withcuts12 = new TH1F("outTcut_withcuts12"," T =  21ns ~  23ns ",200,-1.0,1.5);
  TH1F * outTcut_withcuts13 = new TH1F("outTcut_withcuts13"," T =  23ns ~  25ns ",200,-1.0,1.5);
  TH1F * outTcut_withcuts14 = new TH1F("outTcut_withcuts14"," T = -13ns ~ -05ns ",200,-1.0,1.5);
  TH1F * outTcut_withcuts15 = new TH1F("outTcut_withcuts15"," T =  05ns ~  25ns ",200,-1.0,1.5);





  //==================================================
  TH1F *h_tdiff_total = new TH1F("h_tdiff_total","Tdiff_total",500,-25,25);  
  TH1F *h_tdiff_cut = new TH1F("h_tdiff_cut","Tdiff_cut",500,-25,25);
 
  TH1F *h_x = new TH1F("h_x","X position",60,-30,30);
  TH1F *h_sep = new TH1F("h_sep","Cluster Separation",500,0,50);
  TH1F *h_angle = new TH1F("h_angle","Azimuthal Angle Difference",480,120,240);
  TH1F *h_Eg = new TH1F("h_Eg","Eg",500,4.34,5.31);
  //TH1F *h_eid = new TH1F("h_eid","Eid",180,0.5,180.5);
  TH1F *h_eid = new TH1F("h_eid","Eid",18,1,181);
 
  TH1F *h_tid = new TH1F("h_tid","Tid",100,-1,40);
  TH1F *h_z0 = new TH1F("h_z0","Z",1000,-500,500);
  TH1F *h_z_inT = new TH1F("h_z_inT","Z",1000,-500,500);
  TH1F *h_z_incut = new TH1F("h_z_incut","Z",600,-300,300);

  TH2F *h_x_y_1 = new TH2F("h_x_y_1"," XY position for cluster1", 200,-20,20,500,-20,20);
  TH2F *h_x_y_2 = new TH2F("h_x_y_2"," XY position for cluster2", 200,-20,20,500,-20,20);
  TH2F *h_sep_2d = new TH2F("h_sep_2d"," Cluster Separation", 200,-40,40,200,-40,40);
  TH1F *h_cosphi = new TH1F("h_cosphi","cosphi",600,0,4);

  //========================================================
  TH2F *h_z_e0 = new TH2F("h_z_e0"," Z position VS e0", 200,-1.5,1.5,200,-300,300);
  TH2F *h_z_e0_cut = new TH2F("h_z_e0_cut","Z position VS e0", 200,-1.5,1.5,200,-300,300);
  TH2F *h_z_e_rotate = new TH2F("h_z_e_rotate"," Cluster Separation", 200,-1.5,1.5,200,-300,300);
  TH2F *h_z_e_rotate_cut = new TH2F("h_z_e_rotate_cut"," Cluster Separation", 200,-1.5,1.5,200,-300,300);

  TH2F *h_cross = new TH2F("h_cross"," Cross section", 100,4.2,5.5,100,0.7,1.3);
  TH2F *h_nor_yield = new TH2F("h_nor_yield"," Cross section", 100,4.2,5.5,100,15000,35000);

  TH1F *h_z_rotate = new TH1F("h_z_rotate","rotate Z",300,-300,300);
  TH1F *h_e_rotate = new TH1F("h_e_rotate","rotate E",400,-2,2);

  TH1F *h_eid_data = new TH1F("h_eid_data","Eid_Data",18,1,181);
  TH1F *h_eid_mc = new TH1F("h_eid_mc","Eid_MC",18,1,181);


  TH2F *left_z_e0_cut = new TH2F("left_z_e0_cut","Z position VS e0 (-13ns~-5ns)", 200,-1.5,1.5,200,-300,300);
  TH2F *right_z_e0_cut = new TH2F("right_z_e0_cut","Z position VS e0 (5ns~25ns)", 200,-1.5,1.5,200,-300,300);
  TH1F *left_Eg = new TH1F("left_Eg","Eg (-13ns~-5ns)",500,4.34,5.31);
  TH1F *right_Eg = new TH1F("right_Eg","Eg (5ns~25ns)",500,4.34,5.31);
  TH1F *left_e0 = new TH1F("left_e0","E0 = eg - e1 -e2 (-13ns~-5ns)",500,-3,3);
  TH1F *right_e0 = new TH1F("right_e0","E0 = eg - e1 -e2 (5ns~25ns)",600,-3,3);
  TH1F *left_e1 = new TH1F("left_e1","e1 (-13ns~-5ns)",600,0,6);
  TH1F *right_e1 = new TH1F("right_e1","e1 (5ns~25ns)",600,0,6);
  TH1F *left_e2 = new TH1F("left_e2","e2 (-13ns~-5ns)",600,0,6);
  TH1F *right_e2 = new TH1F("right_e2","e2 (5ns~25ns)",600,0,6);
  TH2F *left_x_y_1 = new TH2F("left_x_y_1"," XY position for cluster1 (-13ns~-5ns)", 200,-20,20,500,-20,20);
  TH2F *left_x_y_2 = new TH2F("left_x_y_2"," XY position for cluster2 (-13ns~-5ns)", 200,-20,20,500,-20,20);
  TH2F *right_x_y_1 = new TH2F("right_x_y_1"," XY position for cluster1 (5ns~25ns)", 200,-20,20,500,-20,20);
  TH2F *right_x_y_2 = new TH2F("right_x_y_2"," XY position for cluster2 (5ns~25ns)", 200,-20,20,500,-20,20);
  TH1F *left_tdiff_cut = new TH1F("left_tdiff_cut","Tdiff_cut (-13ns~-5ns)",500,-25,25);
  TH1F *right_tdiff_cut = new TH1F("right_tdiff_cut","Tdiff_cut (5ns~25ns)",500,-25,25);
  TH1F *left_angle = new TH1F("left_angle","Azimuthal Angle Difference (-13ns~-5ns)",480,120,240);
  TH1F *right_angle = new TH1F("right_angle","Azimuthal Angle Difference (5ns~25ns)",480,120,240);
  TH1F *left_sep = new TH1F("left_sep","Cluster Separation (-13ns~-5ns)",500,0,50);
  TH1F *right_sep = new TH1F("right_sep","Cluster Separation (5ns~25ns)",500,0,50);
  TH2F *left_sep_2d = new TH2F("left_sep_2d"," Cluster Separation (-13ns~-5ns)", 200,-40,40,200,-40,40);
  TH2F *right_sep_2d = new TH2F("right_sep_2d"," Cluster Separation (5ns~25ns)", 200,-40,40,200,-40,40);
  TH1F *left_z = new TH1F("left_z","Z position (-13ns~-5ns)",1000,-500,500);
  TH1F *right_z = new TH1F("right_z","Z position (5ns~25ns)",1000,-500,500);




















  TH1F *h_Ecounter[18] ;
  TH1F *left_Ecounter[18]; 
  TH1F *right_Ecounter[18];

  char hName[255],hTitle[255];
 
  /*
    for(int i=0;i<18;i++) 
    {
    sprintf(hName,"h_Ecounter%02d",i+1);
    sprintf(hTitle,"Ecounter%02d",i+1);
    h_Ecounter[i] = new TH2F(hName,hTitle,200,-1.5,1.5,200,-300,300);  
    }
  */
  for(int i=0;i<18;i++) 
    {
      sprintf(hName,"h_Ecounter%02d",i+1);
      sprintf(hTitle,"Ecounter%02d",i+1);
      h_Ecounter[i] = new TH1F(hName,hTitle,300,-1.5,1.5);  
    }


  for(int i=0;i<18;i++) 
    {
      sprintf(hName,"left_Ecounter%02d",i+1);
      sprintf(hTitle,"left_Ecounter%02d",i+1);
      left_Ecounter[i] = new TH1F(hName,hTitle,300,-1.5,1.5);  
    }

  for(int i=0;i<18;i++) 
    {
      sprintf(hName,"right_Ecounter%02d",i+1);
      sprintf(hTitle,"right_Ecounter%02d",i+1);
      right_Ecounter[i] = new TH1F(hName,hTitle,300,-1.5,1.5);  
    }




  int nevents = event->GetEntries();
  //      nevents = 30;
  cout<<"number of events: "<<nevents<<endl;
  for(int i=1;i<=nevents;i++)
    {
      // if(i%100000==0)cout<<i<<endl;
      //Progress bar
      Double_t  entries=nevents;
      Double_t ratio = i/(Double_t)entries;
      cout<<setw(8)<<(int)(ratio*100)<<"%\r"<<flush;
      event->GetEntry(i);
        
      

      // ========  cut ==================
    
     

      float angle_diff=0,  angle1_a=0 , angle2_a=0, sep=0, sep_x=0, sep_y=0 ;
	
  
	
   

      if ( x1 >0 ) {  angle1_a = atan(y1/x1)*180.0/PI;  }
      if ( x1 <0 ) {  angle1_a = atan(y1/x1)*180.0/PI + 180.0 ; }
      if ( x2 >0 ) {  angle2_a = atan(y2/x2)*180.0/PI; } 
      if ( x2 <0 ) {  angle2_a = atan(y2/x2)*180.0/PI + 180.0 ; }

      angle_diff = abs(angle1_a - angle2_a);
   
      sep_x = x1-x2 ;
      sep_y = y1-y2 ;
      sep = sqrt(sep_x*sep_x + sep_y*sep_y);

      double const me = 0.5109989e-3;
      double r1 = sqrt(x1*x1+y1*y1);
      double r2 = sqrt(x2*x2+y2*y2);
      double theta1 = atan(r1/706.0);   
      double theta2 = atan(r2/706.0);  
      double E_1 = Eg/(1.0+2.0*sin(theta1/2.0)*sin(theta1/2.0)*Eg/me);
      double E_2 = Eg/(1.0+2.0*sin(theta2/2.0)*sin(theta2/2.0)*Eg/me);
      double E_cal = Eg - E_1 - E_2;
      double rotate_angle = atan(15.0/10000.0);
      double rotate_E = cos(rotate_angle)*e0 + sin(rotate_angle)*z;
      double rotate_Z = sin(rotate_angle)*e0 - cos(rotate_angle)*z;
      double rotate_a = 0.4*0.4;
      double rotate_b = 100.0*100.0;
      double ellipse = rotate_E*rotate_E/rotate_a + rotate_Z*rotate_Z/rotate_b;
  
      int idy=int((Eid-1)/10);

      if (Tid > 36 ) continue;
  
      if ( Tdiff_a < -11.0  && Tdiff_a >= -13.0  )   outTcut_nocuts00->Fill(e0);
      if ( Tdiff_a < -9.0   && Tdiff_a >= -11.0  )   outTcut_nocuts01->Fill(e0);
      if ( Tdiff_a < -7.0   && Tdiff_a >=  -9.0  )   outTcut_nocuts02->Fill(e0);
      if ( Tdiff_a < -5.0   && Tdiff_a >=  -7.0  )   outTcut_nocuts03->Fill(e0);
      if ( Tdiff_a < 7.0    && Tdiff_a >=   5.0  )   outTcut_nocuts04->Fill(e0);
      if ( Tdiff_a < 9.0    && Tdiff_a >=   7.0  )   outTcut_nocuts05->Fill(e0);
      if ( Tdiff_a < 11.0   && Tdiff_a >=   9.0  )   outTcut_nocuts06->Fill(e0);
      if ( Tdiff_a < 13.0   && Tdiff_a >=  11.0  )   outTcut_nocuts07->Fill(e0);
      if ( Tdiff_a < 15.0   && Tdiff_a >=  13.0  )   outTcut_nocuts08->Fill(e0);
      if ( Tdiff_a < 17.0   && Tdiff_a >=  15.0  )   outTcut_nocuts09->Fill(e0);
      if ( Tdiff_a < 19.0   && Tdiff_a >=  17.0  )   outTcut_nocuts10->Fill(e0);
      if ( Tdiff_a < 21.0   && Tdiff_a >=  19.0  )   outTcut_nocuts11->Fill(e0);
      if ( Tdiff_a < 23.0   && Tdiff_a >=  21.0  )   outTcut_nocuts12->Fill(e0);
      if ( Tdiff_a < 25.0   && Tdiff_a >=  23.0  )   outTcut_nocuts13->Fill(e0);
     
      if ( Tdiff_a < -5.0   && Tdiff_a >= -13.0  )   outTcut_nocuts14->Fill(e0);
      if ( Tdiff_a < 25.0   && Tdiff_a >=   5.0  )   outTcut_nocuts15->Fill(e0);

      //    if (abs(x1)< 4.154 && abs(y1)< 4.154) continue;
      //    if (abs(x1)> 32.2 && abs(y1)>32.2) continue;
      //    if (abs(x2)< 4.154 && abs(y2)< 4.154) continue;	
      //     if (abs(x2)> 32.2 && abs(y2)> 32.2) continue;
 	
      //     if (e1 < 0.5 || e2 <0.5 ) continue;
      //     if (e1+e2 <=3.5 ) continue;
      //     if (e1+e2 >=8.0 ) continue;
      //    if (abs(angle_diff-180.0) > 40.0 ) continue;  
      //     if (sep < 19.0 ) continue;
  
      h_z_e0->Fill(e0,z);
      h_z0->Fill(z);
  

	  /*
	  if ( Tdiff_a < -25.0  && Tdiff_a >= -20.0  )  inTcut00->Fill(e0);
	  if ( Tdiff_a < -20.0  && Tdiff_a >= -15.0  )  inTcut01->Fill(e0);
	  if ( Tdiff_a < -15.0  && Tdiff_a >= -10.0  )  inTcut02->Fill(e0);
	  if ( Tdiff_a < -10.0  && Tdiff_a >= -5.0  )  inTcut03->Fill(e0);
	  if ( Tdiff_a <  5.0   && Tdiff_a >= 10.0  )  inTcut04->Fill(e0);
	  if ( Tdiff_a < 10.0   && Tdiff_a >= 15.0  )  inTcut05->Fill(e0);
	  if ( Tdiff_a < 15.0   && Tdiff_a >= 20.0  )  inTcut06->Fill(e0);
	  if ( Tdiff_a < 20.0   && Tdiff_a >= 25.0  )  inTcut07->Fill(e0);
	  //  if ( Tdiff_a <  4.0   && Tdiff_a >=  3.0  )  inTcut08->Fill(e0);
	  //  if ( Tdiff_a <  5.0   && Tdiff_a >=  4.0  )  inTcut09->Fill(e0);
	  

	  */


      //   if (e0 > 1.5 || e0 < -0.5 ) continue;
      //    if (abs(z) > 300.0) continue;
      //======================new cut =================================
      //if ( z <= (-100.0-360.0*e0) ) continue;
      //if ( z >= (100.0-360.0*e0) ) continue;
      //if ( z <= (-100.0+130.0*e0) ) continue;
      //if ( z >= (100.0+130.0*e0) ) continue;
     
      //    if ( ellipse > 1 ) continue;
      //   if (!IsGoodZNE0(e0,z)) continue;
   

      if ( Tdiff_a < -11.0  && Tdiff_a >= -13.0  )   outTcut_withcuts00->Fill(e0);
      if ( Tdiff_a < -9.0   && Tdiff_a >= -11.0  )   outTcut_withcuts01->Fill(e0);
      if ( Tdiff_a < -7.0   && Tdiff_a >=  -9.0  )   outTcut_withcuts02->Fill(e0);
      if ( Tdiff_a < -5.0   && Tdiff_a >=  -7.0  )   outTcut_withcuts03->Fill(e0);
      if ( Tdiff_a < 7.0    && Tdiff_a >=   5.0  )   outTcut_withcuts04->Fill(e0);
      if ( Tdiff_a < 9.0    && Tdiff_a >=   7.0  )   outTcut_withcuts05->Fill(e0);
      if ( Tdiff_a < 11.0   && Tdiff_a >=   9.0  )   outTcut_withcuts06->Fill(e0);
      if ( Tdiff_a < 13.0   && Tdiff_a >=  11.0  )   outTcut_withcuts07->Fill(e0);
      if ( Tdiff_a < 15.0   && Tdiff_a >=  13.0  )   outTcut_withcuts08->Fill(e0);
      if ( Tdiff_a < 17.0   && Tdiff_a >=  15.0  )   outTcut_withcuts09->Fill(e0);
      if ( Tdiff_a < 19.0   && Tdiff_a >=  17.0  )   outTcut_withcuts10->Fill(e0);
      if ( Tdiff_a < 21.0   && Tdiff_a >=  19.0  )   outTcut_withcuts11->Fill(e0);
      if ( Tdiff_a < 23.0   && Tdiff_a >=  21.0  )   outTcut_withcuts12->Fill(e0);
      if ( Tdiff_a < 25.0   && Tdiff_a >=  23.0  )   outTcut_withcuts13->Fill(e0);
   
      if ( Tdiff_a < -5.0   && Tdiff_a >= -13.0  )   outTcut_withcuts14->Fill(e0);
      if ( Tdiff_a < 25.0   && Tdiff_a >=   5.0  )   outTcut_withcuts15->Fill(e0);


      if (idy >= 18) cout << idy << " idy erro "<<endl;
      if(idy <18) h_Ecounter[idy]->Fill(e0);

      h_elas->Fill(e0);
      h_tdiff_total->Fill(Tdiff_a);

      if ( Tdiff_a < -5.0 && Tdiff_a > -13.0  )
	{
	  left_z_e0_cut->Fill(e0,z);
	  left_Eg->Fill(Eg);
	  left_e0->Fill(e0);
	  left_e1->Fill(e1);
	  left_e2->Fill(e2);
	  left_x_y_1->Fill(x1,y1);
	  left_x_y_2->Fill(x2,y2);		
	  left_tdiff_cut->Fill(Tdiff_a);
	 
	  left_angle->Fill(angle_diff);   
	  left_sep->Fill(sep);
	  left_sep_2d->Fill(sep_x,sep_y);
	  left_z->Fill(z);
	 


	  if (idy >= 18) cout << idy << " idy erro "<<endl;
	  if(idy <18) left_Ecounter[idy]->Fill(e0);




	}
      
    
      if ( Tdiff_a < 25.0 && Tdiff_a > 5.0  )
	{
	  right_z_e0_cut->Fill(e0,z);
	  right_Eg->Fill(Eg);
	  right_e0->Fill(e0);
	  right_e1->Fill(e1);
	  right_e2->Fill(e2);
	  right_x_y_1->Fill(x1,y1);
	  right_x_y_2->Fill(x2,y2);		
	  right_tdiff_cut->Fill(Tdiff_a);
	 
	  right_angle->Fill(angle_diff);   
	  right_sep->Fill(sep);
	  right_sep_2d->Fill(sep_x,sep_y);
	  right_z->Fill(z);
	 


	  if (idy >= 18) cout << idy << " idy erro "<<endl;
	  if(idy <18) right_Ecounter[idy]->Fill(e0);




	}
      

      



    }
 
  TFile *f = new TFile("/w/hallb-scifs1a/primex/liye/222/Tdiff_study_nocut.root","RECREATE");
 

  left_z_e0_cut->Write();
  left_Eg->Write();
  left_e0->Write();
  left_e1->Write();
  left_e2->Write();
  left_x_y_1->Write();
  left_x_y_2->Write();		
  left_tdiff_cut->Write();
  left_angle->Write();  
  left_sep->Write();
  left_sep_2d->Write();
  left_z->Write();


  right_z_e0_cut->Write();
  right_Eg->Write();
  right_e0->Write();
  right_e1->Write();
  right_e2->Write();
  right_x_y_1->Write();
  right_x_y_2->Write();		
  right_tdiff_cut->Write();
  right_angle->Write();  
  right_sep->Write();
  right_sep_2d->Write();
  right_z->Write();





  outTcut_withcuts00->Write();
  outTcut_withcuts01->Write();
  outTcut_withcuts02->Write();
  outTcut_withcuts03->Write();
  outTcut_withcuts04->Write();
  outTcut_withcuts05->Write();
  outTcut_withcuts06->Write();
  outTcut_withcuts07->Write();
  outTcut_withcuts08->Write();
  outTcut_withcuts09->Write();
  outTcut_withcuts10->Write();
  outTcut_withcuts11->Write();
  outTcut_withcuts12->Write();
  outTcut_withcuts13->Write();
  outTcut_withcuts14->Write();
  outTcut_withcuts15->Write();

  outTcut_nocuts00->Write();
  outTcut_nocuts01->Write(); 
  outTcut_nocuts02->Write(); 
  outTcut_nocuts03->Write();
  outTcut_nocuts04->Write();
  outTcut_nocuts05->Write();
  outTcut_nocuts06->Write();
  outTcut_nocuts07->Write();
  outTcut_nocuts08->Write();
  outTcut_nocuts09->Write(); 
  outTcut_nocuts10->Write(); 
  outTcut_nocuts11->Write();
  outTcut_nocuts12->Write();
  outTcut_nocuts13->Write();
  outTcut_nocuts14->Write();
  outTcut_nocuts15->Write();

  
  for ( int k=0;k<18;k++)
    {
      h_Ecounter[k]->Write();
      left_Ecounter[k]->Write();
      right_Ecounter[k]->Write();
    }




  /*
    latex.DrawLatex(0.465,0.035,"Elasticity: E_{0} - (E_{1} + E_{2})  (GeV)");
    cout << "\n===>>> IN TIME: " << intime << "\n" << endl;
    TLegend *legend;
    if (intime) legend = new TLegend(0.650,0.650,0.920,0.915);
    if (!intime) legend = new TLegend(0.110,0.650,0.390,0.915);
    legend->SetTextFont(52);
    legend->AddEntry(helas0,"All Events","f");
    legend->AddEntry(helas1,"Coincidence","f");
    legend->AddEntry(helas2,"+ Coplanarity","f");
    legend->AddEntry(helas3,"+ Fiducial","f");
    legend->AddEntry(helas4,"+ R minimum","f");
    //~ legend->AddEntry(helas5,"+ no Pair Bgrd","f");
    //  legend->AddEntry(helas6,"+ z Target","f");
    //    legend->AddEntry(helas7,"+ Energy Diff.","f");
    //      legend->Draw();
    */



  /*
    TCanvas *c1 = new TCanvas("c1","XY position",800,800);
    gStyle->SetPalette(0,0);
    c1->cd();
    //	h_E->GetXaxis()->SetTitle("eg-(e1+e2) (GeV)");	
    //	h_E->Draw();
    //	h_tdiff->Draw();
    h_x_y_1->Draw("zcol");
    //	h_angle->GetXaxis()->SetTitle(" Azimuthal Angle difference (deg)");
    //	h_angle->Draw();
    //	h_sep->Draw();	
    //	h_sep_2d->Draw("zcol");
    //	h_z->Draw();

    TCanvas *c2 = new TCanvas("c2","e0 = Eg-e1-e2",800,800);
    c2->cd();
    h_z->Draw(); 
    //c1->SaveAs("/w/hallb-scifs1a/primex/liye/compton_work/plot/z_recon_65078.png");
    */
  return 0;
}








