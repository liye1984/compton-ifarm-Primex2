
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
  TFile *f1 = new TFile(Form("/w/hallb-scifs1a/primex/liye/data/li_prim_ana64885_empty.root",run1),"r");
  if (!f1) cout << " error open f1 files " << endl;
 
  
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




 
 
 
  TTree *event = (TTree*)f1->Get("event");

  event->SetBranchAddress("run", &run);
  event->SetBranchAddress("eventid", &eventid);
  event->SetBranchAddress("Tdiff", &Tdiff);
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
  // event->SetBranchAddress("z",&z);


  TH1F *h_evn = new TH1F("h_evn","eventid",310,0,31000000);
  TH1F *h_E_elas = new TH1F("h_E_elas","E0 = eg - e1 -e2",500,-1,1);
  TH1F *h_E_cal = new TH1F("h_E_cal","Ecal = eg - cal_e1 -cal_e2",500,-2,2);
  TH1F *h_tdiff = new TH1F("tdiff","Tdiff",240,-6,6);  
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

  TH2F *h_cross = new TH2F("h_cross"," Cross section", 100,4.2,5.5,100,0.7,1.3);
  TH2F *h_nor_yield = new TH2F("h_nor_yield"," Cross section", 100,4.2,5.5,100,15000,35000);

  TH1F *h_z_rotate = new TH1F("h_z_rotate","rotate Z",300,-300,300);
  TH1F *h_e_rotate = new TH1F("h_e_rotate","rotate E",400,-2,2);

  TH1F *empty_Ecounter[18];
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
      sprintf(hName,"empty_Ecounter%02d",i+1);
      sprintf(hTitle,"Ecounter%02d",i+1);
      empty_Ecounter[i] = new TH1F(hName,hTitle,300,-1.5,1.5);  
    }


 


  int nevents = event->GetEntries();
  //      nevents = 30;
  cout<<"number of events: "<<nevents<<endl;
  for(int i=1;i<=nevents;i++){
    // if(i%100000==0)cout<<i<<endl;
    //Progress bar
    Double_t  entries=nevents;
    Double_t ratio = i/(Double_t)entries;
    cout<<setw(8)<<(int)(ratio*100)<<"%\r"<<flush;
    event->GetEntry(i);
        
      

    // ========  cut ==================
    
     

    float angle_diff=0,  angle1_a=0 , angle2_a=0, sep=0, sep_x=0, sep_y=0 ;
	
  
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
  
    if (abs(x1)< 4.154 && abs(y1)< 4.154) continue;
    if (abs(x1)> 32.2 && abs(y1)>32.2) continue;
    if (abs(x2)< 4.154 && abs(y2)< 4.154) continue;	
    if (abs(x2)> 32.2 && abs(y2)> 32.2) continue;
 	
    if (e1 < 0.5 || e2 <0.5 ) continue;
    if (e1+e2 <=3.5 ) continue;
    if (e1+e2 >=8.0 ) continue;
    if (abs(Tdiff)>5.0 ) continue;
    if (abs(angle_diff-180.0) > 40.0 ) continue;  
        
    if (sep < 19.0 ) continue;
  
    h_z_e0->Fill(e0,z);
   

      if (e0 > 1.5 || e0 < -0.5 ) continue;
    if (abs(z) > 200.0) continue;
    //======================new cut =================================
    //if ( z <= (-100.0-360.0*e0) ) continue;
    //if ( z >= (100.0-360.0*e0) ) continue;
    //if ( z <= (-100.0+130.0*e0) ) continue;
    //if ( z >= (100.0+130.0*e0) ) continue;
     
    //    if ( ellipse > 1 ) continue;
    //   if (!IsGoodZNE0(e0,z)) continue;

    h_z_e0_cut->Fill(e0,z);
   

    h_x_y_1->Fill(x1,y1);
    h_x_y_2->Fill(x2,y2);		
    h_E_elas->Fill(e0);
    h_tdiff->Fill(Tdiff);
    h_eid->Fill(Eid);
    h_tid->Fill(Tid);
    h_angle->Fill(angle_diff);   
    h_sep->Fill(sep);
    h_sep_2d->Fill(sep_x,sep_y);
    h_z0->Fill(z);
    h_evn->Fill(eventid);
    h_Eg->Fill(Eg);
    h_E_cal->Fill(E_cal);
   

    int idy=int((Eid-1)/10);
    if (idy >= 18) cout << idy << " idy erro "<<endl;
    if(idy <18) empty_Ecounter[idy]->Fill(e0);

    Energy[Eid] = Eg;

 }
  
  /*
  for (int j=0;j<181;j++)
    {
      yield[j] = h_eid->GetBinContent(j); 
      Ecounter[j] = h_eid->GetXaxis()->GetBinCenter(j);
      cout << Energy[j]<< " -- " << Ecounter[j] << " -- " << yield[j] << " -- " << gEEflux[j-1] << endl;
    }
  
  
  for (int j=1;j<181;j++)
    {
      
      //   Ecounter[j] = h_eid_data->GetXaxis()->GetBinCenter(j+1);
      Nor_Acc[j] = -0.0152863*Energy[j]+0.11304;  
      Nor_yield[j]= yield[j]/gEEflux[j-1];
      h_nor_yield->Fill(Energy[j],Nor_yield[j]);
      cross[j] = yield[j]*10e+17/Ne/gEEflux[j]/Nor_Acc[j];
      h_cross->Fill(Energy[j],cross[j]);
 
    } 
  */

 
  //==========================================================================
  /*
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

  

  //for (int j=1;j<181;j++) cout << Energy[j]<<"  " <<  j <<endl;


  
  for(int k = 0; k < 18; ++k) {
    Bin_width[k] = 0;
    for( int i = k*10+1; i <= k*10+10; ++i) {
      if(Energy[i]&&Energy[i+1]) {
	Bin_width[k] += Energy[i+1] - Energy[i];
      }
    }
  }
  Bin_width[17] += Energy[180] - Energy[179];
  
  for(int k=0;k<18;k++)
    { 
      cout << Bin_width[k]<< " -Bin width- " << k << " -- " << endl;
    }

  */
 
 TFile *f = new TFile("/w/hallb-scifs1a/primex/liye/222/empty64885_e1_5_z200.root","RECREATE");
  h_z_e0->Write();
  h_z_e0_cut->Write();
 
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
  h_E_cal->Write();
  h_E_elas->Write();

  for ( int k=0;k<18;k++)
    {
      empty_Ecounter[k]->Write();
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








