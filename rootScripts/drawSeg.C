TH1D *hRef;
double refFunc(double *x, double *par);

void drawSeg(string fnm="../output/o_e18Pz71Px71_5e6_gDet.root"){
  TFile *fin=TFile::Open(fnm.c_str(),"READ");

  hRef = (TH1D*)fin->Get("x_bE18_wgt2");
  TF1 *fct = new TF1("fRef",refFunc,-0.05,0.05,1);

  TH1D *refU = (TH1D*)fin->Get("x_bE18_wgt1");

  const int nR = 6;
  int seg[nR]={100,200,400,1000,2000,4000};//um

  gStyle->SetOptStat(0);
  auto *c1=new TCanvas();
  c1->Divide(3,2);
  for(int i=0;i<nR;i++){
    c1->cd(i+1);
    TH1D *hA=new TH1D(Form("rA%d",i),Form("segmentation = %d um",seg[i]),
		      int(20000/seg[i]),502,522);
    TH1D *hN=new TH1D(Form("rN%d",i),Form("segmentation = %d um",seg[i]),
		      int(20000/seg[i]),502,522);
    for(int j=0;j<100000;j++){
      double x = refU->GetRandom();
      double an = hRef->GetBinContent(hRef->FindBin(x));
      hA->Fill(x,an);
      hN->Fill(x);
    }
    hA->Divide(hN);
    hA->Fit("fRef","Q");
    hA->DrawCopy();
    gPad->SetGridx(1);
    gPad->SetGridy(1);
    cout<<hA->GetTitle()<<" "<<fct->GetParameter(0)*100<<" pm "<<fct->GetParError(0)*100<<endl;
  }


  fin->Close();
}

double refFunc(double *x, double *par){
  double norm = par[0];
  double xVal = x[0] + par[1];
  int bx = hRef->GetXaxis()->FindBin(xVal);

  // cout<<x[0] <<" "<<x[1]<<" | "<<par[0]<<" "<<par[1]<<" "<<par[2]<<endl;
  // cout<<xVal<<" "<<bx<<" "<<yVal<<" "<<by<<" "<<hRef->GetBinContent(bx,by)<<endl<<endl;
  // std::cin.ignore();

  return norm*hRef->GetBinContent(bx);
}

// void detRes(){
//   TFile *fin = TFile::Open("../output/o_e18Pz71Px71_5e6_gDet.root"); 
//   TH1D *refP = (TH1D*)fin->Get("x_bE18_wgt2");
//   TH1D *refU = (TH1D*)fin->Get("x_bE18_wgt1");

//   const int nR = 6;
//   int seg[nR]={5,10,50,100,200,400};//um

//   auto *c1=new TCanvas();
//   c1->Divide(3,2);
//   for(int i=0;i<nR;i++){
//     c1->cd(i+1);
//     TH1D *hA=new TH1D(Form("rA%d",i),Form("segmentation = %d um",seg[i]),
// 		      int(20000/seg[i]),502,522);
//     TH1D *hN=new TH1D(Form("rN%d",i),Form("segmentation = %d um",seg[i]),
// 		      int(20000/seg[i]),502,522);
//     for(int j=0;j<1000000;j++){
//       double x = regU->GetRandom();
//       double an = refP->GetBinContent(refP->FindBin(x));
//       hA->Fill(x,an);
//       hN(x);
//     }
//     hA->Divide(hN);
//     hA->DrawCopy();
//   }
// }
