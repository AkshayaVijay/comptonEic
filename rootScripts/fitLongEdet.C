void fitLongEdet(){
  TFile *fin=TFile::Open("../output/o_e18Pz71Px71_5e6_eDet.root");

  TH1D *ex=(TH1D*)fin->Get("ex18_Wght2");
  TH1D *gr=(TH1D*)fin->Get("grho18_Wght2");

  double xmin = ex->GetXaxis()->GetBinLowEdge(5);
  double xmax = ex->GetXaxis()->GetBinUpEdge(93);

  TH1D *er = (TH1D*)gr->Clone("er18_Wght2_proc");
  for(int i=1;i<=er->GetXaxis()->GetNbins();i++){
    double x = er->GetXaxis()->GetBinCenter(i);
    double xx  = xmax - x * (xmax-xmin);
    er->SetBinContent(i,ex->GetBinContent(ex->FindBin(xx)));
  }

  auto *c1=new TCanvas("c1","c1");
  c1->Divide(3);
  c1->cd(1);
  gr->SetLineColor(2);
  gr->DrawCopy();
  c1->cd(2);
  ex->DrawCopy();
  c1->cd(3);
  er->SetLineColor(kMagenta);
  er->DrawCopy();
  gr->DrawCopy("same");
  fin->Close();
}
