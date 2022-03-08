double calcOneDifferential(TH1D *ru, TH1D *rp);

void calcAn(){
  string fnmg[3]={"../output/o_e5Pz98Px22_VtxSmearXY01Z7mm_5e6_gDet.root",
		 "../output/o_e10Pz91Px42_VtxSmearXY01Z7mm_5e6_gDet.root",
		 "../output/o_e18Pz71Px71_VtxSmearXY01Z9mm_5e6_gDet.root"};

  string fnme[3]={"../output/o_e5Pz98Px22_VtxSmearXY01Z7mm_5e6_eDet.root",
		 "../output/o_e10Pz91Px42_VtxSmearXY01Z7mm_5e6_eDet.root",
		 "../output/o_e18Pz71Px71_VtxSmearXY01Z9mm_5e6_eDet.root"};

  TCanvas *c1=new TCanvas("c1","trans photon",1600,1600);
  c1->Divide(3,3);

  gStyle->SetOptStat(0);
  int bE[3]={5,10,18};
  int eCut[3]={175,170,160};
  for(int i=0;i<3;i++){
    c1->cd(1+i*3);
    TFile *fin=TFile::Open(fnmg[i].c_str(),"READ");
    TH1D *hlu = (TH1D*)fin->Get(Form("rho_bE%d_wgt1",bE[i]));
    TH1D *hlp = (TH1D*)fin->Get(Form("rho_bE%d_wgt2",bE[i]));
    cout<<bE[i]<<" glong "<<calcOneDifferential(hlu,hlp)<<" totXsec "<<hlu->Integral()<<endl<<endl;

    c1->cd(2+i*3);
    TH1D *htu = (TH1D*)fin->Get(Form("x_bE%d_wgt1",bE[i]));
    TH1D *htp = (TH1D*)fin->Get(Form("x_bE%d_wgt2",bE[i]));
    cout<<bE[i]<<" gtran "<<calcOneDifferential(htu,htp)<<" totXsec "<<htu->Integral()<<endl<<endl;
    fin->Close();

    c1->cd(3+i*3);
    fin=TFile::Open(fnme[i].c_str(),"READ");
    TH1D *elu = (TH1D*)fin->Get(Form("ex%d_Wght1",bE[i]));
    TH1D *elp = (TH1D*)fin->Get(Form("ex%d_Wght2",bE[i]));
    double totXsec= elu->Integral();
    double partXsec(0);
    for(int j=1;j<=elu->GetXaxis()->GetNbins();j++)
      if(elu->GetBinCenter(j)>eCut[i]){
    	partXsec+=elu->GetBinContent(j);
    	elu->SetBinContent(j,0);	
      }
    partXsec = totXsec - partXsec;
    cout<<bE[i]<<" elong "<<calcOneDifferential(elu,elp)<<" totXsec "<<totXsec<<" part "<<partXsec<<" p/t "<<partXsec/totXsec<<endl<<endl;
    fin->Close();
  }


}

double calcOneDifferential(TH1D *ru, TH1D *rp){
  double a(0),u(0);
  TH1D *rA2 = (TH1D*)ru->Clone(Form("rA2_%s",ru->GetName()));
  for(int i=1;i<=ru->GetXaxis()->GetNbins();i++){
    //cout<<i<<" "<<ru->GetBinCenter(i)<<" "<<ru->GetBinContent(i)<<endl;
    if(ru->GetBinContent(i)>0){
      double v1 = ru->GetBinContent(i) * pow(rp->GetBinContent(i),2);
      double v2 = ru->GetBinContent(i);
      rA2->SetBinContent(i,v1);
      a+=v1;
      u+=v2;
    }
    rA2->SetBinError(i,0);
  }
  cout<<a <<" "<<u<<" "<<a/u<<endl;
  string tit =   rA2->GetTitle();
  rA2->SetTitle(Form("%s <A^2>=%e",tit.substr(0,tit.find("unpol")).c_str(),a/u));
  rA2->Scale(1/u);
  rA2->DrawCopy("L");
  delete rA2;

  return a/u;
}
