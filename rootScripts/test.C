void test(){
  TFile *fin=TFile::Open("../output/o_e18Pz71Px71_5e6_gDet.root");
  TH1D *h=(TH1D*)fin->Get("x_bE18_wgt0");

  int nBins = h->GetXaxis()->GetNbins();
  for(int i=1;i<=nBins;i++)
    cout<<"Bin "<<i<<" x = "<<h->GetBinCenter(i)<<" val = "<<h->GetBinContent(i)<<endl;
  fin->Close();
}
