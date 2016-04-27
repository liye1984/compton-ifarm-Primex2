
void draw_cross(){


  Double_t Ecounter[20],yeild[20],flux[20],cross[20];
   Double_t  yerr1[20],yerr2[20],yerr3[20]; 
  // static Int_t n=3;
/*
  Double_t fit(Double_t *x,Double_t *par){
    return par[0]+par[1]*x[0];
  }

  TF1 *fitf = new TF1("fitf",fit,-10,25,2);
*/
   
 	y1[0]= 16.86/2.359;
	y1[1]= 22.16/4.063;
	y1[2]= 27.37/6.23;
	y1[3]= 32.83/8.854;
	y1[4]= 3.797/1.194;
	y1[5]= 5.886/2.89;
	y1[6]= 7.973/5.32;


E[0]= 0.8; 
	y2[0]= 0.7432/2.359*2;
E[1]= 1.05; 
	y2[1]= 0.9432/4.063*2;
E[2]= 1.25;
	y2[2]= 1.17/6.23*2;
E[3]= 1.55;	
	y2[3]= 1.39/8.854*2.0;
E[4]= 1.8;
	y2[4]= 0.16/1.194*2;
E[5]= 2.8;
	y2[5]= 0.2506/2.89*2;
E[6]= 3.8;
	y2[6]= 0.339/5.32*2;



/*
x[0]= 35;
	y1[0]= 0.3178;
	y1[1]= (207.0*100.0/26083.0)*0.8726;
	y1[2]= (445.0*100.0/26083.0)*0.8726;
	y1[3]= (638.0*100.0/26083.0)*0.8726;
	y1[4]= (1444.0*100.0/26083.0)*0.8726;
	y1[5]= (3139.0*100.0/26083.0)*0.8726;
x[1]= 45;
	y2[0]= 0.1356;
	y2[1]= (36.0*100.0/14580.0)*0.8726;
	y2[2]= (81.0*100.0/14580.0)*0.8726;
	y2[3]= (107.0*100.0/14580.0)*0.8726;
	y2[4]= (269.0*100.0/14580.0)*0.8726;
	y2[5]= (666.0*100.0/14580.0)*0.8726;
x[2]= 55;
	y3[0]= 0.0595;
	y3[1]= (13.0*100.0/9960.0)*0.8726;
	y3[2]= (31.0*100.0/9960.0)*0.8726;
	y3[3]= (37.0*100.0/9960.0)*0.8726;
	y3[4]= (74.0*100.0/9960.0)*0.8726;
	y3[5]= (190.0*100.0/9960.0)*0.8726;
x[3]= 60;
x[4]= 75;
x[5]= 95;



*/

/*
     yerr1[0] = 0.0085;
     yerr1[1] = 0.0092;
     yerr1[2] = 0.0099;
     yerr1[3] = 0.0080;
     yerr1[4] = 0.0124;
     yerr1[5] = 0.0127;
   
yerr2[0] = 0.097;
yerr2[1] = 0.071;
yerr2[2] = 0.0617;
yerr2[3] = 0.0313;
yerr2[4] = 0.0292;
yerr2[5] = 0.0264;
	yerr3[0] = 0.0174;
	yerr3[1] = 0.0362;
	yerr3[2] = 0.0559;
	yerr3[3] = 0.0611;
	yerr3[4] = 0.0864;
	yerr3[5] = 0.1384;

*/


   TMultiGraph *mg = new TMultiGraph("mg","Q2 resolution");
   TGraphErrors *g1 = new TGraphErrors(7,x,y1,0,0);
   TGraphErrors *g2 = new TGraphErrors(7,x,y2,0,0);	
//   TGraphErrors *g3 = new TGraphErrors(6,x,y3,0,yerr3);

  TCanvas *c11 = new TCanvas("c11","counts vs T",600,600);

//  g1->Draw("A*");
g1->SetName("g1");
g1->SetTitle("");  
g1->SetMarkerStyle(21);
g1->SetMarkerColor(1);
g1->SetMarkerSize(1.0);
g1->SetDrawOption("AP");
g1->SetLineColor(1);
g1->SetLineWidth(0);
g1->SetFillStyle(0);
g1->GetYaxis()->SetTitle("Q2 resolution (%)");
g1->GetXaxis()->SetTitle("Angle bin (deg)");


g2->SetName("g2");
g2->SetTitle("");
g2->SetMarkerStyle(22); 
g2->SetMarkerSize(1.5);
g2->SetMarkerColor(4);
g2->SetDrawOption("P");
g2->SetLineColor(2);
g2->SetLineWidth(0);
g2->SetFillStyle(0);



/*
g3->SetName("g3");
g3->SetMarkerStyle(22);
g3->SetMarkerSize(0.8);
g3->SetMarkerColor(4);
g3->SetDrawOption("P");
g3->SetLineColor(7);
g3->SetLineWidth(2);
g3->SetFillStyle(0);

*/

//mg->Add(g3);
mg->Add(g1);
mg->Add(g2);

//g1->Draw("ALP");
mg->Draw("AP");
mg->GetXaxis()->SetTitle(" Angle bin (deg)");
mg->GetYaxis()->SetTitle(" Q2 resolution (%)");
gPad->Modified();


//mg->GetYaxis()->SetLimits(0,1);




leg = new TLegend(0.1,0.7,0.48,0.9);
leg->AddEntry("g1","Hycal Q2 resolution ");
leg->AddEntry("g2","Gem Q2 resolution");
//leg->AddEntry("g3","1.30-1.55deg angle bin");
leg->Draw();






  //g1->GetXaxis()->SetRange(0,1400);
/*
  g1->Fit("fitf","","",0,0);
  Double_t parameter[2];

  fitf->GetParameters(parameter);
  cout<<"y[500]="<<parameter[0] + parameter[1] * 500<<endl;

*/



 }






