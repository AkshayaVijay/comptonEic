TGraphErrors *procE(TH1F*,int);
TH1F *procG(TH1F*,int);

void drawV3(){
  // TFile* f0 = TFile::Open("/volatile/halla/moller12gev/ciprian/tmp/compton/o_e5Pz99Pxm9_ip6V3_VtxSmearXY0Z0_1e6_1Dana.root","READ");
  // TFile* f1 = TFile::Open("/volatile/halla/moller12gev/ciprian/tmp/compton/o_e5Pz97Pxm26_ip6V3p1_VtxSmearXY0Z0_1e6_1Dana.root","READ");
  TFile* f0 = TFile::Open("/volatile/halla/moller12gev/ciprian/tmp/compton/o_e18Pz95Pxm31_ip6V3_VtxSmearXY0Z0_1e6_1Dana.root","READ");
  TFile* f1 = TFile::Open("/volatile/halla/moller12gev/ciprian/tmp/compton/o_e18Pz59Pxm81_ip6V3p1_VtxSmearXY0Z0_1e6_1Dana.root","READ");

  TH1F *e0=(TH1F*)f0->Get("exp_q07US");
  if(!e0) return;
  TGraphErrors *ge0=procE(e0,0);
  TH1F *e1=(TH1F*)f1->Get("exp_q04US");
  if(!e1) return;
  TGraphErrors *ge1=procE(e1,1);
  auto c1=new TCanvas("c1","c1",1400,600);
  c1->Divide(2);
  c1->cd(1);
  e0->DrawCopy("hist");
  c1->cd(2);
  e1->SetLineColor(2);
  e1->DrawCopy("hist");
  gPad->SetGridx(1);
  gPad->SetGridy(1);

  auto c2=new TCanvas("c2","c2");
  ge0->SetLineColor(4);
  ge1->SetLineColor(2);
  ge1->Draw("AL");
  ge0->Draw("L");
  gPad->SetGridx(1);
  gPad->SetGridy(1);

  TH1F *g0=(TH1F*)f0->Get("gxp_q03US");
  TH1F *g1=(TH1F*)f1->Get("gxp_q03US");
  TH1F *gg0=procG(g0,0);
  TH1F *gg1=procG(g1,1);

  auto c3=new TCanvas("c3","c3",1400,600);
  c3->Divide(2);
  c3->cd(1);
  g0->DrawCopy("hist");
  c3->cd(2);
  g1->SetLineColor(2);
  g1->DrawCopy("hist");
  gPad->SetGridx(1);
  gPad->SetGridy(1);

  gStyle->SetOptStat(0);
  auto c4=new TCanvas("c4","c4");
  gg0->SetLineColor(4);
  gg1->SetLineColor(2);
  gg0->DrawCopy("hist");
  gg1->DrawCopy("hist&&same");
  gPad->SetGridx(1);
  gPad->SetGridy(1);

  f1->Close();
  f0->Close();
}

TGraphErrors *procE(TH1F *ex, int flip){
  double x[600],y[600],dx[600],dy[600];

  int nB = ex->GetXaxis()->GetNbins();
  cout<<ex->GetName()<<" "<<nB<<endl;
  int nd = 0;
  float xV = 0;
  for(int i=1;i<=nB;i++){
    int ii = (flip == 0) ? i : nB-i+1;
 
    if(ex->GetBinError(ii)!=0){
      // cout<<i<<" "<<ii<<" "<<ex->GetBinCenter(ii)<<" "<<ex->GetBinContent(ii)<<" "<<ex->GetBinWidth(ii)<<endl;
      // std::cin.ignore();

      if(nd==0){
	xV = ex->GetBinCenter(ii);
      }
      x[nd] = (flip==0) ? ex->GetBinCenter(ii) - xV : -ex->GetBinCenter(ii) + xV ;
      dx[nd] = ex->GetBinWidth(ii);
      y[nd] = ex->GetBinContent(ii);
      dy[nd] = ex->GetBinError(ii);
      nd++;
    }
  }

  TGraphErrors *gr = new TGraphErrors(nd,x,y,dx,dy);
  gr->SetName(ex->GetName());
  gr->SetTitle(ex->GetTitle());

  return gr;
}

TH1F *procG(TH1F *gx,int v){
  TH1F *gr=new TH1F(Form("proc_%s_%d",gx->GetName(),v),gx->GetTitle(),
		    600,-10,10);
  int nB=gx->GetXaxis()->GetNbins();
  cout<<gx->GetName()<<" "<<nB<<endl;
  for(int i=1;i<=nB;i++){
    gr->SetBinContent(i,gx->GetBinContent(i));
    gr->SetBinError(i,gx->GetBinError(i));
  }
  return gr;
}
