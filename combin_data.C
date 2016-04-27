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
#include "TChain.h"

using namespace std;


vector <int> gEStart;
vector <int> gEEnd;
vector <int> gEType;
vector <int> gEEid;
vector <float> gEEflux;
vector <float> gEinput;


float Tid_65078[18],Tid_64878[18];
float Bin_Energy[18],Bin_yield[18],Nor_yield[18],Bin_flux[18];
float Nor_Acc[18],cross[18],Bin_width[18];
const float Ne = (2.1236)*6.02214e+23/2.0 ; 
float Energy[200],yield[200],Ecounter[200],input_Bin[18];




int ReadBadEventId(int run1)
{
  gEStart.clear();
  gEEnd.clear();
  gEType.clear();
 
  ifstream INPUT_filea;
  INPUT_filea.open(Form("/w/hallb-scifs1a/primex/liye/flux/flux_%06d.dat",run1));
  if( !INPUT_filea)cout<<"ERROR!!!! CAN'T OPEN flux file for run "<<run1<<endl;

  char strline[2561];
  int n=0, runnumber,start,end,e3,type;

  while(!INPUT_filea.eof())
    {
      INPUT_filea>>runnumber>>start>>end>>e3>>type;
      INPUT_filea.getline(strline,2560);
      gEStart.push_back(start);
      gEEnd.push_back(end);
      gEType.push_back(type);
      if ( type <1 || type >2 )
	{ 
	  cout<<setw(12)<<start<<" "<<setw(12)<<end<<" "<<setw(6)<<type<<endl;
	}
      else
	n++;
    }
  return n;

}


bool IsGoodEvent(int eventid)
{
  vector<int>::iterator it;
  int size = (int)gEType.size();
  int i=0;
  for (i=0; i<size; i++)
    {
      if (eventid >= gEStart[i] && eventid <= gEEnd[i]) break;
    }
  if(gEType[i]!=1 && gEType[i]!=2)  
    {
      //cout<<"event "<<eventid<<" is a bad event\n";
      return false;
    }
  else return true;
}






//usage: exe <run#>
int main(int argc, char** argv)
{
 
  int ngoodevent=0;
 
  int eventid;
  int run;
  int  Eid;
  int  Tid;      // Tcounter channel ID
  float Tdiff;   //time difference between Hycal and Tcounter 
  float Eg;      // incoming photon energy
  int id1;
  int id2;
  int type1;
  int type2;
  float z;
  float sep;
  float e1;      // cluster 1 energy
  float x1;      // cluster 1 x position
  float y1;      // cluster 1 y position
  float e0;     
  float e2;      // cluster 2 energy
  float x2;      // cluster 2 x position
  float y2;      // cluster 2 y position
  float Tdiff_a;
  char strline[2561];
  int n=0, flux_runnumber,flux_eid;
  float eflux;
 
  TTree* event = 0;

  TFile *f = new TFile("/w/hallb-scifs1a/primex/liye/222/Data64876_64883.root","RECREATE");
  

  TTree *haha = new TTree("haha","haha");
 
  haha->Branch("run", &run);
  haha->Branch("eventid", &eventid);
  haha->Branch("Tdiff_a", &Tdiff_a);
  haha->Branch("Tid", &Tid);
  haha->Branch("Eid", &Eid);
  haha->Branch("Eg",&Eg);
  haha->Branch("x1",&x1);
  haha->Branch("y1",&y1);
  haha->Branch("x2",&x2);
  haha->Branch("y2",&y2);
  haha->Branch("e1",&e1);
  haha->Branch("e2",&e2);
  haha->Branch("e0",&e0);
  haha->Branch("z",&z);
  haha->Branch("sep",&sep);
 


 

  TH1F *h_evn = new TH1F("h_evn","eventid",310,0,31000000);
  TH1F *h_elas = new TH1F("h_elas","E0 = eg - e1 -e2",500,-3,3);
  TH1F *h_Zcut_elas = new TH1F("h_Zcut_elas","E0 = eg - e1 -e2",500,-3,3);
  TH1F *inTcut_elas = new TH1F("inTcut_elas","E0 = eg - e1 -e2",500,-3,3);
  TH1F *outTcut_elas = new TH1F("outTcut_elas","E0 = eg - e1 -e2",500,-3,3);
  TH1F *outTcut_Zcut_elas = new TH1F("outTcut_Zcut_elas","E0 = eg - e1 -e2",500,-3,3);
  TH1F *h_e1e2 = new TH1F("h_e1e2"," e1 + e2 ",500,0,10);

  TH1F *h_tdiff = new TH1F("tdiff","Tdiff",240,-15,15);  
  TH1F *h_x = new TH1F("h_x","X position",60,-30,30);
  TH1F *h_sep = new TH1F("h_sep","Cluster Separation",500,0,50);
  TH1F *h_angle = new TH1F("h_angle","Azimuthal Angle Difference",480,120,240);
  TH1F *h_Eg = new TH1F("h_Eg","Eg",500,4.34,5.31);
  //TH1F *h_eid = new TH1F("h_eid","Eid",180,0.5,180.5);
  TH1F *h_eid = new TH1F("h_eid","Eid",18,1,181);
 
  TH1F *h_tid = new TH1F("h_tid","Tid",100,-1,40);
  TH1F *h_z0 = new TH1F("h_z0","Z",600,-300,300);

  TH2F *h_x_y_1 = new TH2F("h_x_y_1"," XY position for cluster1", 200,-20,20,500,-20,20);
  TH2F *h_x_y_2 = new TH2F("h_x_y_2"," XY position for cluster2", 200,-20,20,500,-20,20);
  TH2F *h_sep_2d = new TH2F("h_sep_2d"," Cluster Separation", 200,-40,40,200,-40,40);
  TH1F *h_cosphi = new TH1F("h_cosphi","cosphi",600,0,4);
  TH2F *h_z_e0 = new TH2F("h_z_e0"," Z position VS e0", 200,-1.5,1.5,200,-300,300);
  TH2F *h_z_e0_cut = new TH2F("h_z_e0_cut","Z position VS e0", 200,-1.5,1.5,200,-300,300);
  TH2F *h_z_e_rotate = new TH2F("h_z_e_rotate"," Cluster Separation", 200,-1.5,1.5,200,-300,300);
  TH2F *h_z_e_rotate_cut = new TH2F("h_z_e_rotate_cut"," Cluster Separation", 200,-1.5,1.5,200,-300,300);

  TH2F *h_cross = new TH2F("h_cross"," Cross section", 100,4.2,5.5,100,0.1,1.3);
  TH2F *h_nor_yield = new TH2F("h_nor_yield"," Cross section", 100,4.2,5.5,100,15000,35000);

  TH1F *h_z_rotate = new TH1F("h_z_rotate","rotate Z",300,-300,300);
  TH1F *h_e_rotate = new TH1F("h_e_rotate","rotate E",400,-2,2);

  TH1F *h_eid_data = new TH1F("h_eid_data","Eid_Data",18,1,181);
  TH1F *h_eid_mc = new TH1F("h_eid_mc","Eid_MC",18,1,181);


  TH1F *h_Ecounter[18] ;
  TH1F *inTcut_Ecounter[18];
  TH1F *outTcut_Ecounter[18];
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
      h_Ecounter[i] = new TH1F(hName,hTitle,200,-1.5,1.5);  
    }

  for(int i=0;i<18;i++) 
    {
      sprintf(hName,"inTcut_Ecounter%02d",i+1);
      sprintf(hTitle,"in_Ecounter%02d",i+1);
      inTcut_Ecounter[i] = new TH1F(hName,hTitle,200,-1.5,1.5);  
    }

  for(int i=0;i<18;i++) 
    {
      sprintf(hName,"outTcut_Ecounter%02d",i+1);
      sprintf(hTitle,"out_Ecounter%02d",i+1);
      outTcut_Ecounter[i] = new TH1F(hName,hTitle,200,-1.5,1.5);  
    }




  for (int j=atoi(argv[1]); j<=atoi(argv[2]); j++)
    {
      int  run1 = j;
      TFile *f1 = new TFile(Form("/w/hallb-scifs1a/primex/liye/data/li_prim_ana%d.root",run1),"r");
  
 
 



 
      //========== tdiff alignment for run 65077-65080 ======================
      Tid_65078[0] = 2.598;	Tid_65078[1] = 2.583;	Tid_65078[2] = 2.486;
      Tid_65078[3] = 2.434;	Tid_65078[4] = 2.415;	Tid_65078[5] = 2.334;
      Tid_65078[6] = 2.336;	Tid_65078[7] = 2.418;	Tid_65078[8] = 2.197;
      Tid_65078[9] = 2.129;	Tid_65078[10]= 2.035;	Tid_65078[11]= 2.138;
      Tid_65078[12]= 1.957;	Tid_65078[13]= 1.671;	Tid_65078[14]= 1.717;
      Tid_65078[15]= 1.434;	Tid_65078[16]= 1.431;	Tid_65078[17]= 1.202;
      //========== tdiff alignment for run 64876-64883 ======================

      Tid_64878[0] = 2.614;	Tid_64878[1] = 2.568;	Tid_64878[2] = 2.528;
      Tid_64878[3] = 2.515;	Tid_64878[4] = 2.575;	Tid_64878[5] = 2.477;
      Tid_64878[6] = 2.470;	Tid_64878[7] = 2.281;	Tid_64878[8] = 2.309;
      Tid_64878[9] = 2.258;	Tid_64878[10]= 2.201;	Tid_64878[11]= 2.289;
      Tid_64878[12]= 2.184;	Tid_64878[13]= 1.997;	Tid_64878[14]= 1.925;
      Tid_64878[15]= 1.654;	Tid_64878[16]= 1.637;	Tid_64878[17]= 1.419;

      Bin_Energy[0] = 5.281;    Bin_Energy[1] = 5.241;	Bin_Energy[2] = 5.195;
      Bin_Energy[3] = 5.145;    Bin_Energy[4] = 5.093;	Bin_Energy[5] = 5.041;
      Bin_Energy[6] = 4.988;    Bin_Energy[7] = 4.937;	Bin_Energy[8] = 4.883;
      Bin_Energy[9] = 4.827;    Bin_Energy[10]= 4.774;	Bin_Energy[11]= 4.727;
      Bin_Energy[12]= 4.663;    Bin_Energy[13]= 4.609;	Bin_Energy[14]= 4.554;
      Bin_Energy[15]= 4.503;    Bin_Energy[16]= 4.447;	Bin_Energy[17]= 4.400;



  
  
      float PI=3.1415927;	
      int goodnumber=ReadBadEventId(run1);
      if(goodnumber<1) return -1;


      //TChain* event = new TChain("event");
      //event->Add("/w/hallb-scifs1a/primex/liye/data/li_prim_ana64876.root"); 
      //event->Add("/w/hallb-scifs1a/primex/liye/data/li_prim_ana64877.root");
      //event->Add("/w/hallb-scifs1a/primex/liye/data/li_prim_ana64878.root");
      //event->Add("/w/hallb-scifs1a/primex/liye/data/li_prim_ana64879.root");
      //event->Add("/w/hallb-scifs1a/primex/liye/data/li_prim_ana64880.root");
      //event->Add("/w/hallb-scifs1a/primex/liye/data/li_prim_ana64881.root");
      //event->Add("/w/hallb-scifs1a/primex/liye/data/li_prim_ana64882.root");
      //event->Add("/w/hallb-scifs1a/primex/liye/data/li_prim_ana64883.root");
   




      event = (TTree*)f1->Get("event");


      event->SetBranchAddress("run", &run);
      event->SetBranchAddress("eventid", &eventid);
      event->SetBranchAddress("Tdiff", &Tdiff);
      event->SetBranchAddress("Tid", &Tid);
      event->SetBranchAddress("Eid", &Eid);
      event->SetBranchAddress("Eg",&Eg);
      // event->SetBranchAddress("id1",&id1);
      // event->SetBranchAddress("id2",&id2);
      //  event->SetBranchAddress("type1",type1);
      //  event->SetBranchAddress("type2",type2);
      event->SetBranchAddress("x1",&x1);
      event->SetBranchAddress("y1",&y1);
      event->SetBranchAddress("x2",&x2);
      event->SetBranchAddress("y2",&y2);
      event->SetBranchAddress("e1",&e1);
      event->SetBranchAddress("e2",&e2);
      event->SetBranchAddress("e0",&e0);

 

      int nevents = event->GetEntries();
      //      nevents = 30;
      cout<<"number of events: "<<nevents<<endl;
      for(int i=0;i<nevents;i++)
	{
	  // if(i%100000==0)cout<<i<<endl;
	  //Progress bar
	  Double_t  entries=nevents;
	  Double_t ratio = i/(Double_t)entries;
	  cout<<setw(8)<<(int)(ratio*100)<<"%\r"<<flush;
	  event->GetEntry(i);
        
      

	  // ========  cut ==================
	  if(!IsGoodEvent(eventid)) continue;
     

	  float angle_diff=0,  angle1_a=0 , angle2_a=0,  sep_x=0, sep_y=0 ;
	
  
	
	  //float e0 =0;
	
  
	  double a = 1.0 - (0.5109989e-3*Eg)/e1/e2;
	  double b = (x1*x1*x2*x2 + x1*x1*y2*y2 + y1*y1*x2*x2 + y1*y1*y2*y2);
	  double c = (x1*x1 + y1*y1 + x2*x2 + y2*y2);
	  double d = (x1*x1*x2*x2 + y1*y1*y2*y2 + 2*x1*x2*y1*y2);
	  double e = x1*x2 + y1*y2;
	  double A = a*a - 1;
	  double B = a*a*c - 2*e;
	  double C = a*a*b - d;
	  double t = (- B - sqrt(B*B - 4*A*C))/2./A;
	  if (t >= 0) {
	    z = (sqrt(t)-706.0);
	  } else {
	    z = -100.;}

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


	  if (Tid > 36 ) continue;
  
	  int idx = int((Tid-1)/2);
	  if (idx >= 18) cout << idx << " idx erro "<<endl;
	  Tdiff_a = Tdiff + Tid_64878[idx] ;

	  haha->Fill();
	  ngoodevent++;
	  int idy=int((Eid-1)/10);

	  //======================cut ============================//
	  
	  //	  if (abs(x1)< 4.154 && abs(y1)< 4.154) continue;
	  //	  if (abs(x1)> 32.2 && abs(y1)>32.2) continue;
	  //	  if (abs(x2)< 4.154 && abs(y2)< 4.154) continue;	
	  //	  if (abs(x2)> 32.2 && abs(y2)> 32.2) continue;
 	
	  //	  if (e1 < 0.5 || e2 <0.5 ) continue;
	  //	  if (e1+e2 <=2.5 ) continue;
	  //	  if (abs(angle_diff-180.0) > 40.0 ) continue;  
	  //	  if (sep < 19.0 ) continue;
	  //	  if (abs(z)>100.0)continue;
	  //  if (abs(e0)>0.5) continue;

	  h_z_e0->Fill(e0,z);
	  h_elas->Fill(e0);
    
	  h_tdiff->Fill(Tdiff_a);

	  //  if ( ellipse > 1 ) continue;
	  //	  if (Tdiff_a > 5.0 || Tdiff_a < -5.0) 
	  //	    { 
	  //	      outTcut_elas->Fill(e0);
	  // if (abs(z)<150.0){ outTcut_Zcut_elas->Fill(e0);}
	  //	      if (idy >= 18) cout << idy << " idy erro "<<endl;
	  //	      if(idy <18) outTcut_Ecounter[idy]->Fill(e0);
	  //	    }
	  //	  else 
	  //	    {
	  h_z_e0_cut->Fill(e0,z);
   

	  h_x_y_1->Fill(x1,y1);
	  h_x_y_2->Fill(x2,y2);		
	  //	      inTcut_elas->Fill(e0);
	  // h_tdiff->Fill(Tdiff_a);
	  h_eid->Fill(Eid);
	  h_tid->Fill(Tid);
	  h_angle->Fill(angle_diff);   
	  h_sep->Fill(sep);
	  h_sep_2d->Fill(sep_x,sep_y);
	  h_z0->Fill(z);
	  h_evn->Fill(eventid);
	  h_Eg->Fill(Eg);
	  h_e1e2->Fill(e1+e2);
   
	  if (idy >= 18) cout << idy << " idy erro "<<endl;
	  if(idy <18) h_Ecounter[idy]->Fill(e0);
   
    

	  Energy[Eid] = Eg;

	}
	  
    }

  // clog << "Run " << run1 << ": " << ngoodevent << endl;
  // ngoodevent = 0;
     
  

  //===============calculate cross section ================//
  /*

    ifstream INPUT_fileb;
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
 
    TFile *f2 = new TFile("/w/hallb-scifs1a/primex/liye/data/ellcut_cook_MC_64876.root","r");
    if (!f2) cout << " erro open MC file " << endl;
 
    h_eid_data = (TH1F*)f1->Get("h_eid");
    h_eid_mc = (TH1F*)f2->Get("h_eid");

 
    for (int j=0;j<18;j++)
    {
    Dyield[j] = h_eid_data->GetBinContent(j+1); 
    MCyield[j] = h_eid_mc->GetBinContent(j+1); 
      
    Ecounter[j] = h_eid_data->GetXaxis()->GetBinCenter(j+1);
    Acc[j] = MCyield[j]/input_Bin[j];    
	

    //   cout << " yield = " << yield[i] << ";"<< "E-counter=" << Ecounter[i] << " i=" <<i<<endl;
    //  cout << " Acc = " << Acc[j] << ";"<< "E-counter=" << Ecounter[j] << " i=" <<i<<endl;
    } 



  
   
    for (int j=0;j<19;j++)
    {
    yield[j] = h_eid->GetBinContent(j); 
    Ecounter[j] = h_eid->GetXaxis()->GetBinCenter(j);
    cout << Bin_Energy[j]<< " -- " << Ecounter[j] << " -- " << yield[j] << " -- " << Bin_flux[j-1] << endl;
    }

  
    for (int j=1;j<19;j++)
    {
      
    //   Ecounter[j] = h_eid_data->GetXaxis()->GetBinCenter(j+1);
    Nor_Acc[j] = -0.0152863*Bin_Energy[j]+0.11304;  
    Nor_yield[j]= yield[j]/Bin_flux[j-1];
    h_nor_yield->Fill(Bin_Energy[j],Nor_yield[j]);
    cross[j] = yield[j]*10e+17/Ne/Bin_flux[j-1]/Nor_Acc[j];
    h_cross->Fill(Bin_Energy[j],cross[j]);
 
    } 

  */




  f->cd();

  haha->Write();
  
  h_z_e0->Write();
  h_z_e0_cut->Write();
  h_e1e2->Write();
  h_cross->Write();
  h_nor_yield->Write();
  h_z0->Write();
  h_x_y_1->Write();
  h_x_y_2->Write();
  h_tdiff->Write();
  h_angle->Write();
  h_sep_2d->Write();
  h_sep->Write();
  h_evn->Write();
 
  h_eid->Write();
  h_tid->Write();
  h_Eg->Write();
  h_elas->Write();
  h_Zcut_elas->Write();
  //   inTcut_elas->Write();
  //   outTcut_elas->Write();
  //   outTcut_Zcut_elas->Write();
 
  for ( int k=0;k<18;k++)
    {
      h_Ecounter[k]->Write();
    
    }

 
  //delete haha;
 
  f->Close();
  return 0;
}








