#include <iostream>
#include <fstream>
using namespace std;

#include <TH1.h>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include <fstream>
using namespace std;

#include <TH1.h>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TMath.h>
void selection_flux(){

        ifstream INPUT_filea;
        ifstream INPUT_fileb;
        ifstream INPUT_filec;
        ifstream INPUT_filed;
	ifstream INPUT_filee;

        INPUT_filea.open("flux_065077.dat");
        INPUT_fileb.open("flux_065078.dat");
        INPUT_filec.open("flux_065079.dat");
        INPUT_filed.open("flux_065080.dat");
	INPUT_filee.open("flux_065081.dat");


	if( !INPUT_filea)cout<<"ERROR!!!! CAN'T OPEN output.dat!!!!"<<endl;

	double x,y,x1,y1,E1,d,Angle1,Angle1,E2,Angle2,E3,Angle3,E4,Angle4;

	TH1F *h_E = new TH1F("h_E","Energy Distribution",1300,100,1300);
	TH1F *h_x = new TH1F("h_x","X position",60,-30,30);
	TH1F *h_y = new TH1F("h_y","Y position",60,-30,30);
	TH1F *h_Angle = new TH1F("Angle Distribution","Angle Distribution",100,-0.1,3.9);
	TH2F *h_x_y = new TH2F("h_x_y","XY plot",60,-30,30,60,-30,30);
	TH2F *E_Angle1 = new TH2F("E_angle1","background from flange on GEM",500,0.0,4.0,1200,0,1200);
	TH2F *E_Angle2 = new TH2F("E_angle2","background from tube on GEM",500,0.0,4.0,1200,0,1200);
	TH2F *E_Angle3 = new TH2F("E_angle3","background from flange on Hycal",500,0.0,4.0,1200,0,1200);
	TH2F *E_Angle4 = new TH2F("E_angle4","background from tube on Hycal",500,0.0,4.0,1200,0,1200);


	
	int mp = 0 , runnumber = 0 , e_start=0,e_end=0,e3=0;
	int np = 0 , ID= 0 ;
	char strline[2560];	 

	while(!INPUT_filea.eof())
	  {
	    INPUT_filea>>runnumber>>e_start>>e_end>>e3>>ID;
	    INPUT_filea.getline(strline,2550);
	    if(ID>2){
	       
		//	  h_E->Fill(E);
		//	  h_Angle->Fill(Angle);
                //  E_Angle1->Fill(Angle1,E1);		 
			   	 
			    //		  	}
	
	      cout<<"d="<<runnumber << "  " << e_start<< "  " << e_end<< "  " << e3 <<" " << ID << endl;}
  //	cout<<"angle="<<Angle<<endl;
	      
	      
	}
	/*       
	while(!INPUT_fileb.eof())
	  {
	    INPUT_fileb>>Angle2>>E2;
	    //  cout<<"E="<<E<<endl;
	    if(E2>0.00001 && Angle2>0.438 && Angle2<4.05)
	      { 
		 
                  E_Angle2->Fill(Angle2,E2);		 
		  mp++;	   	 	 
			    //		  	}
	
   //	cout<<"d="<<d<<endl;
  //	cout<<"angle="<<Angle<<endl;
	      
	      }
	}
	cout <<"np="<< np << "mp="<< mp << endl;

	while(!INPUT_filec.eof())
	  {
	    INPUT_filec>>Angle3>>E3;
	   
	    if(E3>0.00001 && Angle3>0.427 && Angle3<4.05)
	      { 
		 
                  E_Angle3->Fill(Angle3,E3);		 

	      }
	}

	while(!INPUT_filed.eof())
	  {
	    INPUT_filed>>Angle4>>E4;
	   
	    if(E4>0.00001 && Angle4>0.427 && Angle4<4.05)
	      { 
		 
                  E_Angle4->Fill(Angle4,E4);		 

	      }
	}


	TCanvas *c1 = new TCanvas("c1","DC simulation",800,800);
	gStyle->SetPalette(0,0);
	c1->Divide(2,2);

	c1->cd(1);


//	E_Angle->GetXaxis()->SetTitle("Angle(deg)");

//	E_Angle->GetYaxis()->SetTitle("   E");
	//E_Angle->GetXaxis()->SetNdivisions(5);
	

		E_Angle1->SetFillColor(kRed);
		E_Angle1->Draw("box");
	       
	
	

		c1->cd(2);

	
	
		E_Angle2->SetFillColor(kBlue);
		E_Angle2->Draw("box");
	


	//	c2->cd(2);

	//	h_Angle->GetXaxis()->SetTitle("Angle");
	//	h_Angle->GetYaxis()->SetTitle("Counts");
	//	h_Angle->Draw();
	
	//			h_E->GetXaxis()->SetTitle("Energy");
	//			h_E->GetYaxis()->SetTitle("Count");
	//			h_E->Draw();

	//	c1->cd(2);
	//	h_x->GetXaxis()->SetTitle("X-axis Position(mm)");
	//	h_x->GetYaxis()->SetTitle("Count");
	//	h_x->Draw();
	//	h_y->Draw("same");
	//	h_y->SetLineColor(3);


		c1->cd(3);
	//	h_y->GetXaxis()->SetTitle("Y-axis Position(mm)");
	//	h_y->GetYaxis()->SetTitle("Count");
	//	h_y->Draw();
                E_Angle3->SetFillColor(kRed);
		E_Angle3->Draw("box");
	
		c1->cd(4);
	//	h_x_y->GetXaxis()->SetTitle("X-axis(mm)");
	//	h_x_y->GetYaxis()->SetTitle("Y-axis(mm)");
	//  	h_x_y->Draw("COLZ");
E_Angle4->SetFillColor(kBlue);
E_Angle4->Draw("box");

		c1->Update();

	*/
	//	c1->SaveAs("a.png");



}
	
