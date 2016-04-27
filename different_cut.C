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
   TFile *f1 = new TFile(Form("li_prim_ana%d.root",run),"r");
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
 
  TH2F *h_x_y_1 = new TH2F("h_x_y_1"," XY position for cluster1", 200,-20,20,500,-20,20);
  TH2F *h_x_y_2 = new TH2F("h_x_y_2"," XY position for cluster2", 200,-20,20,500,-20,20);
  TH2F *h_sep_2d = new TH2F("h_sep_2d"," Cluster Separation", 200,-40,40,200,-40,40);
  
 
  TH1F *h_tdiff_tid[18];
  TH1F *m_sep[9] ;
  TH1F *m_elas[9];
  TH1F *m_z[9];
  TH1F *m_tdiff[9];
  TH1F *m_angle[9];




  char hName[255],hTitle[255];
  char m_elas_Name[255],m_elas_Title[255], m_z_Name[255],m_z_Title[255];
  char m_sep_Name[255],m_sep_Title[255], m_tdiff_Name[255],m_tdiff_Title[255];
  char m_angle_Name[255],m_angle_Title[255];
 
  for(int i=0;i<18;i++) 
    {
      sprintf(hName,"h_tdiff_tid%02d",i+1);
      sprintf(hTitle,"h_tdiff_tid%02d",i+1);
      h_tdiff_tid[i] = new TH1F(hName,hTitle,120,-6,6);  
    }

  for(int k=0;k<6;k++)
    {
      sprintf(m_sep_Name,"m_sep%02d",k);
      sprintf(m_sep_Title,"m_sep%02d",k);
      m_sep[k] = new TH1F(m_sep_Name,m_sep_Title,500,0,50);  
  
      sprintf(m_tdiff_Name,"m_tdiff%02d",k);
      sprintf(m_tdiff_Title,"m_tdiff%02d",k);
      m_tdiff[k] = new TH1F(m_tdiff_Name,m_tdiff_Title,240,-6,6);  

      sprintf(m_z_Name,"m_z%02d",k);
      sprintf(m_z_Title,"m_z%02d",k);
      m_z[k] = new TH1F(m_z_Name,m_z_Title,600,0,1200);  

      sprintf(m_elas_Name,"m_elas%02d",k);
      sprintf(m_elas_Title,"m_elas%02d",k);
      m_elas[k] = new TH1F(m_elas_Name,m_elas_Title,400,-2,2);  
  
      sprintf(m_angle_Name,"m_angle%02d",k);
      sprintf(m_angle_Title,"m_angle%02d",k);
      m_angle[k] = new TH1F(m_angle_Name,m_angle_Title,480,120,240);  

    }

	
 
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


    h_tdiff0->Fill(Tdiff);
    h_tdiff1->Fill(Tdiff_a);

    m_tdiff[0]->Fill(Tdiff_a);
    m_angle[0]->Fill(angle_diff);
    m_sep[0]->Fill(sep);
    m_elas[0]->Fill(e0);
    m_z[0]->Fill(z);
   
    if (Tdiff_a < -4.0 || Tdiff_a > 4.0) continue;
   
    m_tdiff[1]->Fill(Tdiff_a);
    m_angle[1]->Fill(angle_diff);
    m_sep[1]->Fill(sep);
    m_elas[1]->Fill(e0);
    m_z[1]->Fill(z);
     
    if (angle_diff < 140.0 || angle_diff > 220.0 ) continue;  
  
    m_tdiff[2]->Fill(Tdiff_a);
    m_angle[2]->Fill(angle_diff);
    m_sep[2]->Fill(sep);
    m_elas[2]->Fill(e0);
    m_z[2]->Fill(z);
      
    if (sep < 18.0 ) continue;
  
    m_tdiff[3]->Fill(Tdiff_a);
    m_angle[3]->Fill(angle_diff);
    m_sep[3]->Fill(sep);
    m_elas[3]->Fill(e0);
    m_z[3]->Fill(z);


    if (e0 > 0.5 || e0 < -0.5 ) continue;
  
    m_tdiff[4]->Fill(Tdiff_a);
    m_angle[4]->Fill(angle_diff);
    m_sep[4]->Fill(sep);
    m_elas[4]->Fill(e0);
    m_z[4]->Fill(z);

    if (z > 800.0 || z < 600.0 ) continue;

    m_tdiff[5]->Fill(Tdiff_a);
    m_angle[5]->Fill(angle_diff);
    m_sep[5]->Fill(sep);
    m_elas[5]->Fill(e0);
    m_z[5]->Fill(z);

	
   
    h_sep_2d->Fill(sep_x,sep_y);
    h_evn->Fill(eventid);
    h_Eid->Fill(Eid);
    h_Tid->Fill(Tid);

    //  if ( sep > 16.0 ) {helas1->Fill(e0); h_z1->Fill(z);}
    //   if ( sep > 17.0 ) {helas2->Fill(e0); h_z2->Fill(z);}
    //   if ( sep > 18.0 ) {helas3->Fill(e0); h_z3->Fill(z);}
    // cout<<event->GetEntries()<<endl;

    //return 0;
  

  }

  //  TFile *f = new TFile(Form("/w/hallb-scifs1a/primex/liye/222/result_%d.root",run),"RECREATE");
 
 TFile *f = new TFile(Form("/w/hallb-scifs1a/primex/liye/222/MC_different_cut_%d.root",run),"RECREATE");
 

  h_x_y_1->Write();
  h_x_y_2->Write();
  h_sep_2d->Write();
  h_evn->Write();
  h_Eid->Write();
  h_Tid->Write();
  h_tdiff0->Write();
  h_tdiff1->Write();


  for (int j=0;j<6;j++)
    {
      m_tdiff[j]->Write();
      m_angle[j]->Write();
      m_sep[j]->Write();
      m_elas[j]->Write();
      m_z[j]->Write();
    }
 
  /*
  for (int j=0;j<5;j++)
    {
      h_tdiff_tid[j]->Write();
    }
  */

 
  TLegend *legend1 = new TLegend(0.65,0.5,0.98,0.75);
 
  legend1->SetTextFont(52);
  legend1->AddEntry(m_tdiff[0],"All Events","f");
  legend1->AddEntry(m_tdiff[1],"+ tdiff cut(-4~4)","f");
  legend1->AddEntry(m_tdiff[2],"+ angle cut(140~220)","f");
  legend1->AddEntry(m_tdiff[3],"+ R min cut (R>18)","f");
  legend1->AddEntry(m_tdiff[4],"+ elas cut (-0.5~0.5)","f");
  legend1->AddEntry(m_tdiff[5],"+ Z cut (600~800)","f");
  
  
  TCanvas *c1 = new TCanvas("c1","tdiff",800,800);
  gStyle->SetPalette(0,0);
  c1->cd();
 
  m_tdiff[0]->SetFillColor(kGreen+10);
  m_tdiff[1]->SetFillColor(kGreen+8);
  m_tdiff[2]->SetFillColor(kGreen+6);
  m_tdiff[3]->SetFillColor(kGreen+4);
  m_tdiff[4]->SetFillColor(kGreen+1);
  m_tdiff[5]->SetFillColor(kYellow);
 
  m_tdiff[0]->SetTitleSize(0.035,"X");
  m_tdiff[0]->GetXaxis()->SetTitle("tdiff (ns)");
 
  m_tdiff[0]->Draw();
  m_tdiff[1]->Draw("same");
  m_tdiff[2]->Draw("same");
  m_tdiff[3]->Draw("same");
  m_tdiff[4]->Draw("same");
  m_tdiff[5]->Draw("same");

  legend1->Draw();

  c1->SaveAs("/w/hallb-scifs1a/primex/liye/plot/MC_diff_cut_tdiff_65080.png");
  
  // ==================================================================

  TLegend *legend2 = new TLegend(0.65,0.5,0.98,0.75);
 
  legend2->SetTextFont(52);
  legend2->AddEntry(m_angle[0],"All Events","f");
  legend2->AddEntry(m_angle[1],"+ tdiff cut(-4~4)","f");
  legend2->AddEntry(m_angle[2],"+ angle cut(140~220)","f");
  legend2->AddEntry(m_angle[3],"+ R min cut (R>18)","f");
  legend2->AddEntry(m_angle[4],"+ elas cut (-0.5~0.5)","f");
  legend2->AddEntry(m_angle[5],"+ Z cut (600~800)","f");
  
  
  TCanvas *c2 = new TCanvas("c1","angle",800,800);
  gStyle->SetPalette(0,0);
  c2->cd();
 
  m_angle[0]->SetFillColor(kGreen+10);
  m_angle[1]->SetFillColor(kGreen+8);
  m_angle[2]->SetFillColor(kGreen+6);
  m_angle[3]->SetFillColor(kGreen+4);
  m_angle[4]->SetFillColor(kGreen+1);
  m_angle[5]->SetFillColor(kYellow);
 
  m_angle[0]->SetTitleSize(0.035,"X");
  m_angle[0]->GetXaxis()->SetTitle("angle (deg)");
 
  m_angle[0]->Draw();
  m_angle[1]->Draw("same");
  m_angle[2]->Draw("same");
  m_angle[3]->Draw("same");
  m_angle[4]->Draw("same");
  m_angle[5]->Draw("same");

  legend2->Draw();

  c2->SaveAs("/w/hallb-scifs1a/primex/liye/plot/MC_diff_cut_angle_65080.png");
  //========================================================================
 TLegend *legend3 = new TLegend(0.65,0.5,0.98,0.75);
 
  legend3->SetTextFont(52);
  legend3->AddEntry(m_sep[0],"All Events","f");
  legend3->AddEntry(m_sep[1],"+ tdiff cut(-4~4)","f");
  legend3->AddEntry(m_sep[2],"+ angle cut(140~220)","f");
  legend3->AddEntry(m_sep[3],"+ R min cut (R>18)","f");
  legend3->AddEntry(m_sep[4],"+ elas cut (-0.5~0.5)","f");
  legend3->AddEntry(m_sep[5],"+ Z cut (600~800)","f");
  
  

 TCanvas *c3 = new TCanvas("c1","sep",800,800);
  gStyle->SetPalette(0,0);
  c3->cd();
 
  m_sep[0]->SetFillColor(kGreen+10);
  m_sep[1]->SetFillColor(kGreen+8);
  m_sep[2]->SetFillColor(kGreen+6);
  m_sep[3]->SetFillColor(kGreen+4);
  m_sep[4]->SetFillColor(kGreen+1);
  m_sep[5]->SetFillColor(kYellow);
 
  m_sep[0]->SetTitleSize(0.035,"X");
  m_sep[0]->GetXaxis()->SetTitle("sep (cm)");
 
  m_sep[0]->Draw();
  m_sep[1]->Draw("same");
  m_sep[2]->Draw("same");
  m_sep[3]->Draw("same");
  m_sep[4]->Draw("same");
  m_sep[5]->Draw("same");

  legend3->Draw();

  c3->SaveAs("/w/hallb-scifs1a/primex/liye/plot/MC_diff_cut_sep_65080.png");
  //========================================================================


  TLegend *legend4 = new TLegend(0.65,0.5,0.98,0.75);
 
  legend4->SetTextFont(52);
  legend4->AddEntry(m_elas[0],"All Events","f");
  legend4->AddEntry(m_elas[1],"+ tdiff cut(-4~4)","f");
  legend4->AddEntry(m_elas[2],"+ angle cut(140~220)","f");
  legend4->AddEntry(m_elas[3],"+ R min cut (R>18)","f");
  legend4->AddEntry(m_elas[4],"+ elas cut (-0.5~0.5)","f");
  legend4->AddEntry(m_elas[5],"+ Z cut (600~800)","f");
  
  

 TCanvas *c4 = new TCanvas("c1","elas",800,800);
  gStyle->SetPalette(0,0);
  c4->cd();
 
  m_elas[0]->SetFillColor(kGreen+10);
  m_elas[1]->SetFillColor(kGreen+8);
  m_elas[2]->SetFillColor(kGreen+6);
  m_elas[3]->SetFillColor(kGreen+4);
  m_elas[4]->SetFillColor(kGreen+1);
  m_elas[5]->SetFillColor(kYellow);
 
  m_elas[0]->SetTitleSize(0.035,"X");
  m_elas[0]->GetXaxis()->SetTitle(" e0 = eg-e1-e2");
 
  m_elas[0]->Draw();
  m_elas[1]->Draw("same");
  m_elas[2]->Draw("same");
  m_elas[3]->Draw("same");
  m_elas[4]->Draw("same");
  m_elas[5]->Draw("same");

  legend4->Draw();

  c4->SaveAs("/w/hallb-scifs1a/primex/liye/plot/MC_diff_cut_elas_65080.png");
  //=======================================================================
TLegend *legend5 = new TLegend(0.65,0.5,0.98,0.75);
 
  legend5->SetTextFont(52);
  legend5->AddEntry(m_z[0],"All Events","f");
  legend5->AddEntry(m_z[1],"+ tdiff cut(-4~4)","f");
  legend5->AddEntry(m_z[2],"+ angle cut(140~220)","f");
  legend5->AddEntry(m_z[3],"+ R min cut (R>18)","f");
  legend5->AddEntry(m_z[4],"+ elas cut (-0.5~0.5)","f");
  legend5->AddEntry(m_z[5],"+ Z cut (600~800)","f");
  
  

 TCanvas *c5 = new TCanvas("c1","z",800,800);
  gStyle->SetPalette(0,0);
  c5->cd();
 
  m_z[0]->SetFillColor(kGreen+10);
  m_z[1]->SetFillColor(kGreen+8);
  m_z[2]->SetFillColor(kGreen+6);
  m_z[3]->SetFillColor(kGreen+4);
  m_z[4]->SetFillColor(kGreen+1);
  m_z[5]->SetFillColor(kYellow);
 
  m_z[0]->SetTitleSize(0.035,"X");
  m_z[0]->GetXaxis()->SetTitle("z (cm)");
 
  m_z[0]->Draw();
  m_z[1]->Draw("same");
  m_z[2]->Draw("same");
  m_z[3]->Draw("same");
  m_z[4]->Draw("same");
  m_z[5]->Draw("same");

  legend5->Draw();

  c5->SaveAs("/w/hallb-scifs1a/primex/liye/plot/MC_diff_cut_z_65080.png");
  





  return 0;
}











