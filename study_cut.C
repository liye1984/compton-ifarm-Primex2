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
  if (gCutG_ZVsE0)  BuiltCutG();
  return gCutG_ZVsE0->IsInside(e0,z);
}
