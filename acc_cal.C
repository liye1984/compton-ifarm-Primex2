
Double_t func(Double_t *x, Double_t *par)
{
   Float_t xx=x[0];
   	Double_t E = 4.4 ;
	Double_t a = xx*3.1415927/180.0;
	Double_t p = 1/(1+(1000.0*E/0.511)*(1-cos(a)));
	Double_t f = 3.97*1.0e-30*p*p*(p+1.0/p-1.0+cos(a)*cos(a))  ;
//	Double_t f =pp*p;	

	Double_t b = cos(a);

   return f  ;

}



void acc_cal()
{
   TF1 *f1 = new TF1("f1",func,0,500,1);
 //  f1->SetParameters(4.4,0);
 //  f1->Draw();
   
   TH1F *h1 = new TH1F("h1","h1",3000,0,30);
   h1->FillRandom("f1",1000000) ;
   h1->Draw();
   double acc = h1->Integral(0,254);
   cout << acc/1000000.0 << endl;
	

}
