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
#include "TROOT.h"
#include "TSystem.h"
#include "TInterpreter.h"
#include "TStyle.h"
#include "TH1.h"
#include "TH2.h"
#include "TMath.h"
#include "TTree.h"
#include "TH1F.h"
#include "TLegend.h"

using namespace std;
vector <int> gEStart;
vector <int> gEEnd;
vector <int> gEType;

int ReadBadEventId(int run)
{
  ifstream INPUT_filea;
  INPUT_filea.open(Form("/w/hallb-scifs1a/primex/liye/compton_work/ana_data/flux/flux_%06d.dat",run));
  if( !INPUT_filea)cout<<"ERROR!!!! CAN'T OPEN flux file for run "<<run<<endl;

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
  int run = atoi(argv[1]);
   TFile *f1 = new TFile(Form("/w/hallb-scifs1a/primex/liye/data/li_prim_ana%d.root",run),"r");
  //  TFile *f1 = new TFile(Form("li_MC%d.root",run),"r");

 

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
  float Tid_65078[18],Tid_64878[18];
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

  /*
  float  id1, id2, type1, type2;
  float invm[kMax], angle[kMax];
  */
  float PI=3.1415927;	
  // int goodnumber=ReadBadEventId(run);
  // if(goodnumber<1) return -1;

  
  TTree *event = (TTree*)f1->Get("event");

  //TDirectory *dd = (TDirectory*)f1->Get("tagger");
  // TNtuple *event = (TNtuple*) dd->Get("cmpt_ntuple");

  //event->SetBranchAddress("run", &run);
  event->SetBranchAddress("eventid", &eventid);
  event->SetBranchAddress("Tdiff", &Tdiff);
  event->SetBranchAddress("Tid", &Tid);
  event->SetBranchAddress("Eid", &Eid);
  event->SetBranchAddress("Eg",&Eg);
  //event->SetBranchAddress("id1",&id1);
  //event->SetBranchAddress("id2",&id2);
  //  event->SetBranchAddress("type1",type1);
  //  event->SetBranchAddress("type2",type2);
  event->SetBranchAddress("x1",&x1);
  event->SetBranchAddress("y1",&y1);
  event->SetBranchAddress("x2",&x2);
  event->SetBranchAddress("y2",&y2);
  event->SetBranchAddress("e1",&e1);
  event->SetBranchAddress("e2",&e2);
  event->SetBranchAddress("e0",&e0);

  TH1F *h_evn = new TH1F("h_evn","eventid",310,0,31000000);
  TH1F *h_E = new TH1F("h_E","E0 = eg - e1 -e2",1000,-2,2);
  TH1F *h_tdiff0 = new TH1F("h_tdiff0","Tdiff",120,-6,6);  
  TH1F *h_tdiff1 = new TH1F("h_tdiff1","Tdiff",120,-6,6);  
  TH1F *h_x = new TH1F("h_x","X position",60,-30,30);
  TH1F *h_sep = new TH1F("h_sep","Cluster Separation",500,0,50);
  TH1F *h_angle = new TH1F("h_angle","Azimuthal Angle Difference",500,120,240);
  TH1F *h_Eid = new TH1F("h_Eid","Eid",180,0,180);
  TH1F *h_Tid = new TH1F("h_Tid","Tid",40,0,40);
  TH1F *h_elas = new TH1F("h_elas","elas",400,-5,5);


  TH2F *h_x_y_1 = new TH2F("h_x_y_1"," XY position for cluster1", 200,-20,20,500,-20,20);
  TH2F *h_x_y_2 = new TH2F("h_x_y_2"," XY position for cluster2", 200,-20,20,500,-20,20);
  TH2F *h_sep_2d = new TH2F("h_sep_2d"," Cluster Separation", 200,-40,40,200,-40,40);
  
  TH1F *tdiff_Tcut_3ns = new TH1F("tdiff_Tcut_3ns","Tdiff",120,-20,20); 
  TH1F *tdiff_Tcut_4ns = new TH1F("tdiff_Tcut_4ns","Tdiff",120,-6,6); 
  TH1F *tdiff_Tcut_5ns = new TH1F("tdiff_Tcut_5ns","Tdiff",120,-6,6); 
  TH1F *tdiff_Tcut_6ns = new TH1F("tdiff_Tcut_6ns","Tdiff",120,-6,6); 
  TH1F *check_tdiff_Tcut_3ns = new TH1F("check_tdiff_Tcut_3ns","Tdiff",120,-6,6); 
  TH1F *check_tdiff_Tcut_4ns = new TH1F("check_tdiff_Tcut_4ns","Tdiff",120,-6,6); 
  TH1F *check_tdiff_Tcut_5ns = new TH1F("check_tdiff_Tcut_5ns","Tdiff",120,-6,6); 
  TH1F *check_tdiff_Tcut_6ns = new TH1F("check_tdiff_Tcut_6ns","Tdiff",120,-6,6); 
 
  TH1F *elas_Tcut_3ns = new TH1F("elas_Tcut_3ns","elas",400,-2,2);
  TH1F *elas_Tcut_4ns = new TH1F("elas_Tcut_4ns","elas",400,-2,2);
  TH1F *elas_Tcut_5ns = new TH1F("elas_Tcut_5ns","elas",400,-2,2);
  TH1F *elas_Tcut_6ns = new TH1F("elas_Tcut_6ns","elas",400,-2,2);

  TH2F *sep2d_Tcut_3ns = new TH2F("sep2d_Tcut_3ns"," Cluster Separation Tcut check", 200,-40,40,200,-40,40);
  TH2F *sep2d_Tcut_4ns = new TH2F("sep2d_Tcut_4ns"," Cluster Separation Tcut check", 200,-40,40,200,-40,40);
  TH2F *sep2d_Tcut_5ns = new TH2F("sep2d_Tcut_5ns"," Cluster Separation Tcut check", 200,-40,40,200,-40,40);
  TH2F *sep2d_Tcut_6ns = new TH2F("sep2d_Tcut_6ns"," Cluster Separation Tcut check", 200,-40,40,200,-40,40);

  TH1F *elas_Acut_30deg = new TH1F("elas_Acut_30deg","elas",400,-2,2);
  TH1F *elas_Acut_35deg = new TH1F("elas_Acut_35deg","elas",400,-2,2);
  TH1F *elas_Acut_40deg = new TH1F("elas_Acut_40deg","elas",400,-2,2);
  TH1F *elas_Acut_50deg = new TH1F("elas_Acut_50deg","elas",400,-2,2);

  TH2F *sep2d_Acut_30deg = new TH2F("sep2d_Acut_30deg"," Cluster Separation Acut check", 200,-40,40,200,-40,40);
  TH2F *sep2d_Acut_35deg = new TH2F("sep2d_Acut_35deg"," Cluster Separation Acut check", 200,-40,40,200,-40,40);
  TH2F *sep2d_Acut_40deg = new TH2F("sep2d_Acut_40deg"," Cluster Separation Acut check", 200,-40,40,200,-40,40);
  TH2F *sep2d_Acut_50deg = new TH2F("sep2d_Acut_50deg"," Cluster Separation Acut check", 200,-40,40,200,-40,40);


  TH1F *elas_Rcut_16cm = new TH1F("elas_Rcut_16cm","elas",400,-2,2);
  TH1F *elas_Rcut_17cm = new TH1F("elas_Rcut_17cm","elas",400,-2,2);
  TH1F *elas_Rcut_18cm = new TH1F("elas_Rcut_18cm","elas",400,-2,2);
  TH1F *elas_Rcut_19cm = new TH1F("elas_Rcut_19cm","elas",400,-2,2);
  TH1F *elas_Rcut_20cm = new TH1F("elas_Rcut_20cm","elas",400,-2,2);


  TH2F *sep2d_Rcut_16cm = new TH2F("sep2d_Rcut_16cm"," Cluster Separation Rcut check", 200,-40,40,200,-40,40);
  TH2F *sep2d_Rcut_17cm = new TH2F("sep2d_Rcut_17cm"," Cluster Separation Rcut check", 200,-40,40,200,-40,40);
  TH2F *sep2d_Rcut_18cm = new TH2F("sep2d_Rcut_18cm"," Cluster Separation Rcut check", 200,-40,40,200,-40,40);
  TH2F *sep2d_Rcut_19cm = new TH2F("sep2d_Rcut_19cm"," Cluster Separation Rcut check", 200,-40,40,200,-40,40);
  TH2F *sep2d_Rcut_20cm = new TH2F("sep2d_Rcut_20cm"," Cluster Separation Rcut check", 200,-40,40,200,-40,40);

  TH1F *elas_Zcut_50cm = new TH1F("elas_Zcut_50cm","elas",400,-2,2);
  TH1F *elas_Zcut_100cm = new TH1F("elas_Zcut_100cm","elas",400,-2,2);
  TH1F *elas_Zcut_150cm = new TH1F("elas_Zcut_150cm","elas",400,-2,2);
  TH1F *elas_Zcut_200cm = new TH1F("elas_Zcut_200cm","elas",400,-2,2);

  TH2F *sep2d_Zcut_50cm = new TH2F("sep2d_Zcut_50cm"," Cluster Separation Zcut check", 200,-40,40,200,-40,40);
  TH2F *sep2d_Zcut_100cm = new TH2F("sep2d_Zcut_100cm"," Cluster Separation Zcut check", 200,-40,40,200,-40,40);
  TH2F *sep2d_Zcut_150cm = new TH2F("sep2d_Zcut_150cm"," Cluster Separation Zcut check", 200,-40,40,200,-40,40);
  TH2F *sep2d_Zcut_200cm = new TH2F("sep2d_Zcut_200cm"," Cluster Separation Zcut check", 200,-40,40,200,-40,40);
















  // =================================================================//
  // ==================== begin fill events ==========================// 

  int nevents = event->GetEntries();
        nevents = 174340;
  cout<<"number of events: "<<nevents<<endl;
  for(int i=1;i<=nevents;i++){
    // if(i%100000==0)cout<<i<<endl;
    //Progress bar
    Double_t  entries=nevents;
    Double_t ratio = i/(Double_t)entries;
    cout<<setw(8)<<(int)(ratio*100)<<"%\r"<<flush;
    event->GetEntry(i);
        
      

    // ========  cut ==================
    //   if(!IsGoodEvent(eventid)) continue;
     

    float angle_diff=0,  angle1_a=0 , angle2_a=0, sep=0, sep_x=0, sep_y=0 ;
	
    float r1=0,r2=0;
	
    //float e0 =0;
    //========== Z position reconstruction ==========//	
    double z;
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
      z = sqrt(t);
    } else {
      z = -100.;}

    if ( x1 >0 ) {  angle1_a = atan(y1/x1)*180.0/PI;  }
    if ( x1 <0 ) {  angle1_a = atan(y1/x1)*180.0/PI + 180.0 ; }
    if ( x2 >0 ) {  angle2_a = atan(y2/x2)*180.0/PI; } 
    if ( x2 <0 ) {  angle2_a = atan(y2/x2)*180.0/PI + 180.0 ; }

    angle_diff =abs( angle1_a - angle2_a);
   

    sep_x = x1-x2 ;
    sep_y = y1-y2 ;
    sep = sqrt(sep_x*sep_x + sep_y*sep_y);

    //============= Tdiff alienment =============//
   
    if (Tid > 36 ) continue;
    if (abs(x1)< 4.154 && abs(y1)< 4.154) continue;
    if (abs(x1)> 32.2 && abs(y1)>32.2) continue;
    if (abs(x2)< 4.154 && abs(y2)< 4.154) continue;	
    if (abs(x2)> 32.2 && abs(y2)> 32.2) continue;

    h_x_y_1->Fill(x1,y1);
    h_x_y_2->Fill(x2,y2);		
 
    if (e1 < 0.5 || e2 <0.5 ) continue;
    if (e1+e2 <=2.5 ) continue;
  


    int idx = int((Tid-1)/2);
    Tdiff_a = Tdiff + Tid_64878[idx] ;
    //  h_tdiff_tid[idx]->Fill(Tdiff);

    //========================== Tdiff cut    =============================//
    if (abs(Tdiff_a)>3.0 ) 
      {
	sep2d_Tcut_3ns->Fill(sep_x,sep_y);
	elas_Tcut_3ns->Fill(e0);
      }
   
   if (abs(Tdiff_a)>4.0 ) 
      {
	sep2d_Tcut_4ns->Fill(sep_x,sep_y);
	elas_Tcut_4ns->Fill(e0);
      }
   
   if (abs(Tdiff_a)>5.0 ) 
      {
	sep2d_Tcut_5ns->Fill(sep_x,sep_y);
	elas_Tcut_5ns->Fill(e0);
      }
   
   if (abs(Tdiff_a)>6.0 ) 
      {
	sep2d_Tcut_6ns->Fill(sep_x,sep_y);
	elas_Tcut_6ns->Fill(e0);
      }
   
 
     
 //========================== Angle cut    =============================//
     
   if (abs(angle_diff-180.0)> 30.0 )
     {
       sep2d_Acut_30deg->Fill(sep_x,sep_y);
       elas_Acut_30deg->Fill(e0);
     }  
   
   if (abs(angle_diff-180.0)> 35.0 )
     {
       sep2d_Acut_35deg->Fill(sep_x,sep_y);
       elas_Acut_35deg->Fill(e0);
     }  
   if (abs(angle_diff-180.0)> 40.0 )
     {
       sep2d_Acut_40deg->Fill(sep_x,sep_y);
       elas_Acut_40deg->Fill(e0);
     }  
   if (abs(angle_diff-180.0)> 50.0 )
     {
       sep2d_Acut_50deg->Fill(sep_x,sep_y);
       elas_Acut_50deg->Fill(e0);
     }  

   //========================== R cut    =============================//
      
    if (sep < 20.0 ) 
      {
	sep2d_Rcut_20cm->Fill(sep_x,sep_y);
	elas_Rcut_20cm->Fill(e0);
      }
  
    if (sep < 19.0 ) 
      {
	sep2d_Rcut_19cm->Fill(sep_x,sep_y);
	elas_Rcut_19cm->Fill(e0);
      }
  
    if (sep < 18.0 ) 
      {
	sep2d_Rcut_18cm->Fill(sep_x,sep_y);
	elas_Rcut_18cm->Fill(e0);
      }
  
    if (sep < 17.0 ) 
      {
	sep2d_Rcut_17cm->Fill(sep_x,sep_y);
	elas_Rcut_17cm->Fill(e0);
      }
  
    if (sep < 16.0 ) 
      {
	sep2d_Rcut_16cm->Fill(sep_x,sep_y);
	elas_Rcut_16cm->Fill(e0);
      }
  
   //========================== R cut    =============================//

    if ( abs(z-700.0)>50.0  )
      {
	sep2d_Zcut_50cm->Fill(sep_x,sep_y);
	elas_Zcut_50cm->Fill(e0);
      }

    if ( abs(z-700.0)>100.0  )
      {
	sep2d_Zcut_100cm->Fill(sep_x,sep_y);
	elas_Zcut_100cm->Fill(e0);
      }
    if ( abs(z-700.0)> 150.0  )
      {
	sep2d_Zcut_150cm->Fill(sep_x,sep_y);
	elas_Zcut_150cm->Fill(e0);
      }
    if ( abs(z-700.0)> 200.0  )
      {
	sep2d_Zcut_200cm->Fill(sep_x,sep_y);
	elas_Zcut_200cm->Fill(e0);
      }
      




    if (e0 > 0.5 || e0 < -0.5 ) continue;
  
   
    h_sep_2d->Fill(sep_x,sep_y);
    h_evn->Fill(eventid);
    h_Eid->Fill(Eid);
    h_Tid->Fill(Tid);
    h_elas->Fill(e0);
    //  if ( sep > 16.0 ) {helas1->Fill(e0); h_z1->Fill(z);}
    //   if ( sep > 17.0 ) {helas2->Fill(e0); h_z2->Fill(z);}
    //   if ( sep > 18.0 ) {helas3->Fill(e0); h_z3->Fill(z);}
    // cout<<event->GetEntries()<<endl;

    //return 0;
  

  }

  //  TFile *f = new TFile(Form("/w/hallb-scifs1a/primex/liye/222/result_%d.root",run),"RECREATE");
 
 TFile *f = new TFile(Form("/w/hallb-scifs1a/primex/liye/222/check_different_cut_%d.root",run),"RECREATE");
 

  h_x_y_1->Write();
  h_x_y_2->Write();
  h_sep_2d->Write();
  h_evn->Write();
  h_Eid->Write();
  h_Tid->Write();
  h_elas->Write();

 
  elas_Tcut_3ns->Write();
  elas_Tcut_4ns->Write();
  elas_Tcut_5ns->Write();
  elas_Tcut_6ns->Write();
  
  sep2d_Tcut_3ns->Write();
  sep2d_Tcut_4ns->Write();
  sep2d_Tcut_5ns->Write();
  sep2d_Tcut_6ns->Write();
 

  elas_Acut_30deg->Write();
  elas_Acut_35deg->Write();
  elas_Acut_40deg->Write();
  elas_Acut_50deg->Write();

  sep2d_Acut_30deg->Write();
  sep2d_Acut_35deg->Write();
  sep2d_Acut_40deg->Write();
  sep2d_Acut_50deg->Write();

  elas_Rcut_16cm->Write();
  elas_Rcut_17cm->Write();
  elas_Rcut_18cm->Write();
  elas_Rcut_19cm->Write();
  elas_Rcut_20cm->Write();

  sep2d_Rcut_16cm->Write();
  sep2d_Rcut_17cm->Write();
  sep2d_Rcut_18cm->Write();
  sep2d_Rcut_19cm->Write();
  sep2d_Rcut_20cm->Write();
 
  elas_Zcut_50cm->Write();
  elas_Zcut_100cm->Write();
  elas_Zcut_150cm->Write();
  elas_Zcut_200cm->Write();

  sep2d_Zcut_50cm->Write();
  sep2d_Zcut_100cm->Write();
  sep2d_Zcut_150cm->Write();
  sep2d_Zcut_200cm->Write();






 // TLegend *legend1 = new TLegend(0.65,0.5,0.98,0.75);
  TLegend *legend1 = new TLegend(0.1,0.7,0.5,0.9);
  legend1->SetTextFont(52);
  legend1->AddEntry(elas_Tcut_3ns,"Tcut_3ns","f");
  legend1->AddEntry(elas_Tcut_4ns,"Tcut_4ns","f");
  legend1->AddEntry(elas_Tcut_5ns,"Tcut_5ns","f");
  legend1->AddEntry(elas_Tcut_6ns,"Tcut_6ns","f");
 
  
  
  TCanvas *c1 = new TCanvas("c1","tdiff",800,800);
  gStyle->SetPalette(0,0);
  c1->cd();
 
  elas_Tcut_3ns->SetFillColor(kYellow);
  elas_Tcut_4ns->SetFillColor(kGreen);
  elas_Tcut_5ns->SetFillColor(kGreen+4);
  elas_Tcut_6ns->SetFillColor(kGreen+8);

  elas_Tcut_3ns->Draw();
  elas_Tcut_4ns->Draw("same");
  elas_Tcut_5ns->Draw("same");
  elas_Tcut_6ns->Draw("same");
  elas_Tcut_3ns->SetTitleSize(0.035,"X");
  elas_Tcut_3ns->SetTitle("elas_Tcut_run64876");
  elas_Tcut_3ns->GetXaxis()->SetTitle("e0 (GeV)");
  legend1->Draw();

  c1->SaveAs(Form("/w/hallb-scifs1a/primex/liye/plot/check_Tcut_%d.png",run),"RECREATE");
  
  // ==================================================================
  
  //TLegend *legend2 = new TLegend(0.65,0.5,0.98,0.75);
  TLegend *legend2 = new TLegend(0.1,0.7,0.5,0.9);
  legend2->SetTextFont(52);
  legend2->AddEntry(elas_Acut_30deg,"Acut_30deg","f");
  legend2->AddEntry(elas_Acut_35deg,"Acut_35deg","f");
  legend2->AddEntry(elas_Acut_40deg,"Acut_40deg","f");
  legend2->AddEntry(elas_Acut_50deg,"Acut_50deg","f");
    
  
  TCanvas *c2 = new TCanvas("c1","angle",800,800);
  gStyle->SetPalette(0,0);
  c2->cd();
 
  elas_Acut_30deg->SetFillColor(kYellow);
  elas_Acut_35deg->SetFillColor(kGreen);
  elas_Acut_40deg->SetFillColor(kGreen+4);
  elas_Acut_50deg->SetFillColor(kGreen+8);

  elas_Acut_30deg->Draw();
  elas_Acut_35deg->Draw("same");
  elas_Acut_40deg->Draw("same");
  elas_Acut_50deg->Draw("same");
  elas_Acut_30deg->SetTitleSize(0.035,"X");
  elas_Acut_30deg->SetTitle("elas_Acut_run64876");
  elas_Acut_30deg->GetXaxis()->SetTitle("e0 (GeV)");


  legend2->Draw();
  c2->SaveAs(Form("/w/hallb-scifs1a/primex/liye/plot/check_Acut_%d.png",run),"RECREATE");
 
  //========================================================================
  // TLegend *legend3 = new TLegend(0.65,0.5,0.98,0.75);
  TLegend *legend3 = new TLegend(0.1,0.7,0.5,0.9);
  legend3->SetTextFont(52);
  legend3->AddEntry( elas_Rcut_16cm,"elas_Rcut_16cm","f");
  legend3->AddEntry( elas_Rcut_17cm,"elas_Rcut_17cm","f");
  legend3->AddEntry( elas_Rcut_18cm,"elas_Rcut_18cm","f");
  legend3->AddEntry( elas_Rcut_19cm,"elas_Rcut_19cm","f");
  legend3->AddEntry( elas_Rcut_20cm,"elas_Rcut_20cm","f");
 
  
  

 TCanvas *c3 = new TCanvas("c1","sep",800,800);
  gStyle->SetPalette(0,0);
  c3->cd();
 
  elas_Rcut_16cm ->SetFillColor(kGreen+10);
  elas_Rcut_17cm ->SetFillColor(kGreen+8);
  elas_Rcut_18cm ->SetFillColor(kGreen+6);
  elas_Rcut_19cm->SetFillColor(kGreen+4);
  elas_Rcut_20cm->SetFillColor(kYellow);
 
  elas_Rcut_20cm->Draw();
  elas_Rcut_19cm->Draw("same");
  elas_Rcut_18cm->Draw("same");
  elas_Rcut_17cm->Draw("same");
  elas_Rcut_16cm->Draw("same");
  elas_Rcut_20cm->SetTitleSize(0.035,"X");
  elas_Rcut_20cm->SetTitle("elas_Rcut_run64876");
  elas_Rcut_20cm->GetXaxis()->SetTitle("e0 (GeV)");


  legend3->Draw();
  c3->SaveAs(Form("/w/hallb-scifs1a/primex/liye/plot/check_Rcut_%d.png",run),"RECREATE");
  
  //========================================================================
  // TLegend *legend4 = new TLegend(0.65,0.5,0.98,0.75);
  TLegend *legend4 = new TLegend(0.1,0.7,0.5,0.9);
  legend4->SetTextFont(52);
  legend4->AddEntry(elas_Zcut_50cm,"elas_Zcut_50cm","f");
  legend4->AddEntry(elas_Zcut_100cm,"elas_Zcut_100cm","f");
  legend4->AddEntry(elas_Zcut_150cm,"elas_Zcut_150cm","f");
  legend4->AddEntry(elas_Zcut_200cm,"elas_Zcut_200cm","f");
  
  

 TCanvas *c4 = new TCanvas("c1","elas",800,800);
  gStyle->SetPalette(0,0);
  c4->cd();
 
  elas_Zcut_50cm->SetFillColor(kYellow);
  elas_Zcut_100cm->SetFillColor(kGreen);
  elas_Zcut_150cm->SetFillColor(kGreen+4);
  elas_Zcut_200cm->SetFillColor(kGreen+8);
 
  elas_Zcut_50cm->Draw();
  elas_Zcut_100cm->Draw("same");
  elas_Zcut_150cm->Draw("same");
  elas_Zcut_200cm->Draw("same");
  elas_Zcut_50cm->SetTitleSize(0.035,"X");
  elas_Zcut_50cm->SetTitle("elas_Zcut_run64876");
  elas_Zcut_50cm->GetXaxis()->SetTitle("e0 (GeV)");


  legend4->Draw();
  c4->SaveAs(Form("/w/hallb-scifs1a/primex/liye/plot/check_Zcut_%d.png",run),"RECREATE");
 
 



  return 0;
}











