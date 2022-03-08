void drawVTX(){
  //TFile *fin=TFile::Open("/volatile/halla/moller12gev/ciprian/tmp/compton/o_e5Pz98Px22_VtxSmearXY01Z7mm_5e6.root");
  TFile *fin=TFile::Open("/volatile/halla/moller12gev/ciprian/tmp/compton/o_e18Pz71Px71_VtxSmearXY01Z9mm_5e6.root");
  TTree *t=(TTree*)fin->Get("t");

  gStyle->SetOptStat("eMR");
  auto *c1=new TCanvas("c1","c1",2100,800);
  c1->Divide(3);
  c1->cd(1);
  //auto *frm1= gPad->DrawFrame(184,0,187,);
  t->Draw("vtx_x","","");
  // htemp->SetTitle("transverse vertex; x[mm]; y[mm]");
  // htemp->DrawCopy();
  c1->cd(2);
  t->Draw("vtx_y","","");
  c1->cd(3);
  t->Draw("vtx_z/1000","","");
  // htemp->SetTitle("longitudinal vertex; z[m]");
  // htemp->DrawCopy();
    
  fin->Close();

}
