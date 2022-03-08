

void drawSmearing(){
  TFile *fg[3][2],*fe[3][2];
  
  int bE[3]={5,10,18};
  string eNm[3]={"e5Pz98Px22","e10Pz91Px42","e18Pz71Px71"};
  string smrNm[3]={"VtxSmearXY01Z7mm","VtxSmearXY01Z7mm","VtxSmearXY01Z9mm"};
  for(int i=0;i<2;i++)
    for(int j=0;j<3;j++){
      if(i==0){
	fg[j][i]=TFile::Open(Form("../output/o_%s_5e6_gDet.root",eNm[j].c_str()),"READ");
	fe[j][i]=TFile::Open(Form("../output/o_%s_5e6_eDet.root",eNm[j].c_str()),"READ");
      }else{
	fg[j][i]=TFile::Open(Form("../output/o_%s_%s_5e6_gDet.root",eNm[j].c_str(),smrNm[j].c_str()),"READ");
	fe[j][i]=TFile::Open(Form("../output/o_%s_%s_5e6_eDet.root",eNm[j].c_str(),smrNm[j].c_str()),"READ");
      }
    }
  gStyle->SetOptStat(0);
  auto *c1=new TCanvas("c1","g2dp",2100,1400);
  c1->Divide(3,2);
  for(int i=0;i<3;i++){
    c1->cd(i+1);
    string hnm = Form("xy_bE%d_wgt2",bE[i]);
    cout<<hnm<<endl;
    TH2D *h = (TH2D*)fg[i][0]->Get(hnm.c_str());
    h->DrawCopy("colz");

    c1->cd(i+4);
    TH2D *hs = (TH2D*)fg[i][1]->Get(hnm.c_str());
    hs->SetTitle(Form("%s smeared",hs->GetTitle()));
    hs->DrawCopy("colz");
  }

  auto *c2=new TCanvas("c2","e2d",2100,1400);
  c2->Divide(3,2);
  for(int i=0;i<3;i++){
    c2->cd(i+1);
    string hnm = Form("exy%d_Wght2",bE[i]);
    cout<<hnm<<endl;
    TH2D *h = (TH2D*)fe[i][0]->Get(hnm.c_str());
    h->DrawCopy("colz");
    gPad->SetGridx(1);
    gPad->SetGridy(1);


    c2->cd(i+4);
    TH2D *hs = (TH2D*)fe[i][1]->Get(hnm.c_str());
    hs->SetTitle(Form("%s smeared",hs->GetTitle()));
    hs->DrawCopy("colz");
    gPad->SetGridx(1);
    gPad->SetGridy(1);

  }

  auto *c3=new TCanvas("c3","g1d",2100,2100);
  c3->Divide(3,2);
  for(int i=0;i<3;i++){
    c3->cd(i+1);
    string hnm = Form("rho_bE%d_wgt2",bE[i]);
    cout<<hnm<<endl;
    TH2D *h = (TH2D*)fg[i][0]->Get(hnm.c_str());
    TH2D *hs = (TH2D*)fg[i][1]->Get(hnm.c_str());
    hs->SetLineColor(2);
    hs->SetLineWidth(2);
    h->SetLineWidth(3);
    h->DrawCopy("hist");
    hs->DrawCopy("hist same");
    gPad->SetGridx(1);
    gPad->SetGridy(1);

    c3->cd(i+4);
    hnm = Form("x_bE%d_wgt2",bE[i]);
    cout<<hnm<<endl;
    TH2D *k = (TH2D*)fg[i][0]->Get(hnm.c_str());
    TH2D *ks = (TH2D*)fg[i][1]->Get(hnm.c_str());
    ks->SetLineColor(2);
    ks->SetLineWidth(2);
    k->SetLineWidth(3);
    k->DrawCopy("hist");
    ks->DrawCopy("hist same");
    gPad->SetGridx(1);
    gPad->SetGridy(1);
  }

  auto *c4=new TCanvas("c4","e1d",2100,2100);
  c4->Divide(3);
  for(int i=0;i<3;i++){
    c4->cd(i+1);
    string hnm = Form("ex%d_Wght2",bE[i]);
    cout<<hnm<<endl;
    TH2D *h = (TH2D*)fe[i][0]->Get(hnm.c_str());
    TH2D *hs = (TH2D*)fe[i][1]->Get(hnm.c_str());
    hs->SetLineColor(2);
    hs->SetLineWidth(2);
    h->SetLineWidth(3);
    h->DrawCopy("hist");
    hs->DrawCopy("hist same");
    gPad->SetGridx(1);
    gPad->SetGridy(1);

  }

  gStyle->SetOptStat("i");
  auto *c5=new TCanvas("c5","g2du",2100,1400);
  c5->Divide(3,2);
  for(int i=0;i<3;i++){
    c5->cd(i+1);
    string hnm = Form("xy_bE%d_wgt1",bE[i]);
    cout<<hnm<<endl;
    TH2D *h = (TH2D*)fg[i][0]->Get(hnm.c_str());
    h->DrawCopy("colz");

    c5->cd(i+4);
    TH2D *hs = (TH2D*)fg[i][1]->Get(hnm.c_str());
    hs->SetTitle(Form("%s smeared",hs->GetTitle()));
    hs->DrawCopy("colz");
  }


  for(int i=0;i<2;i++)
    for(int j=0;j<3;j++){
      fg[j][i]->Close();
      fe[j][i]->Close();
    }

}
