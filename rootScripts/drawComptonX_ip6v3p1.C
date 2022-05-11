void drawComptonX_ip6v3p1(){
  //TFile *finB=TFile::Open("/work/halla/moller12gev/ciprian/compton/comptonEic/beam18GeV_ip6V4_smearReal_1e3.root","READ");
  //TFile *finB=TFile::Open("/work/halla/moller12gev/ciprian/compton/comptonEic/beam5GeV_ip6V3p1_1e3.root","READ");
  //TTree *tb=(TTree*)finB->Get("t");

  //TFile *finC=TFile::Open("/volatile/halla/moller12gev/ciprian/tmp/compton/o_e18Pz59Pxm81_ip6V4_noSync_VtxSmearX0Y0Z0_1e6.root","READ");
  //TFile *finC=TFile::Open("/volatile/halla/moller12gev/ciprian/tmp/compton/o_e18Pz59Pxm81_ip6V4_VtxSmearX0Y0Z0_1e6.root","READ");
  //TFile *finC=TFile::Open("/volatile/halla/moller12gev/ciprian/tmp/compton/o_e18Pz59Pxm81_ip6V4_VtxSmearX492Pxm16Y0Z0_1e6.root","READ");
  TFile *finC=TFile::Open("/volatile/halla/moller12gev/ciprian/tmp/compton/o_e18Pz59Pxm81_ip6V4_Sync_VtxSmearX492Pxm16Y99Pym06Z0_1e6.root","READ");
  //TFile *finC=TFile::Open("/volatile/halla/moller12gev/ciprian/tmp/compton/o_e5Pz97Pxm26_ip6V3p1_VtxSmearXY0Z0_1e6.root","READ");
  TTree *tc=(TTree*)finC->Get("t");

  const int nDet=12;
  string detNm[nDet]={"q09DS","q08US","q08DS","q07US","q07DS","q06US","q06DS","q05US","q05DS","q04US","q04DS","q03US"};
  float xPos[nDet]={2.463e+2 ,2.741e+2 ,2.939e+2, 3.504e+2 ,3.703e+2,4.268e+2,4.466e+2,4.763e+2,4.774e+2,4.803e+2,4.813e+2,4.956e+2};
  float gOffset[nDet]={0,0,0,0,0,0,-2,-29,-49,-107,-127,-383};
  const int nEvProc=1000000;

  gStyle->SetOptStat(0);
  auto *c1=new TCanvas("c1","elec",1800,1600);
  c1->Divide(4,3);
  auto *c2=new TCanvas("c2","gammaB",1800,1600);
  c2->Divide(4,3);

  TH1F *ex[nDet],*gx[nDet];
  TH1F *exp[nDet],*gxp[nDet];
  for(int i=0;i<nDet;i++){
    cout<<i<<endl;
    ex[i]=new TH1F(Form("ex_%s",detNm[i].c_str()),
		    Form("uXsec %s;x[mm];Asymmetr",detNm[i].c_str()),
		   600,xPos[i]-120,xPos[i]+10);
    exp[i]=new TH1F(Form("exp_%s",detNm[i].c_str()),
		    Form("pXsec %s;x[mm];Asymmetry",detNm[i].c_str()),
		    600,xPos[i]-120,xPos[i]+10);
    c1->cd(i+1);
    int nE = tc->Project(Form("ex_%s",detNm[i].c_str()),
			 Form("%s_hits.x",detNm[i].c_str()),
			 Form("uXsec*(%s_hits.pID==11)",detNm[i].c_str()),"",nEvProc);
    ex[i]->SetTitle(Form("%s :%d",ex[i]->GetTitle(),nE));
    nE = tc->Project(Form("exp_%s",detNm[i].c_str()),
			 Form("%s_hits.x",detNm[i].c_str()),
			 Form("pXsec*(%s_hits.pID==11)",detNm[i].c_str()),"",nEvProc);
    exp[i]->SetTitle(Form("%s :%d",exp[i]->GetTitle(),nE));
    exp[i]->Divide(ex[i]);
    exp[i]->DrawCopy("hist");
    gPad->SetGridx(1);
    gPad->SetGridy(1);
    gPad->SetLogz(1);

    cout<<i<<endl;
    gx[i]=new TH1F(Form("gx_%s",detNm[i].c_str()),
		   Form("uXsec %s;x[mm];Asymmetry",detNm[i].c_str()),
		   600,-30+xPos[i]-gOffset[i],30+xPos[i]-gOffset[i]);
    gxp[i]=new TH1F(Form("gxp_%s",detNm[i].c_str()),
		    Form("pXsec %s;x[mm];Asymmetry",detNm[i].c_str()),
		    600,-30+xPos[i]-gOffset[i],30+xPos[i]-gOffset[i]);
    c2->cd(i+1);
    nE=tc->Project(Form("gx_%s",detNm[i].c_str()),
		   Form("%s_hits.x",detNm[i].c_str()),
		   Form("uXsec*(%s_hits.pID==22 && %s_hits.trackID==1)",detNm[i].c_str(),detNm[i].c_str()),"",nEvProc);
    gx[i]->SetTitle(Form("%s :%d",gx[i]->GetTitle(),nE));
    nE=tc->Project(Form("gxp_%s",detNm[i].c_str()),
		   Form("%s_hits.x",detNm[i].c_str()),
		   Form("pXsec*(%s_hits.pID==22 && %s_hits.trackID==1)",detNm[i].c_str(),detNm[i].c_str()),"",nEvProc);
    gxp[i]->SetTitle(Form("%s :%d",gxp[i]->GetTitle(),nE));

    gxp[i]->Divide(gx[i]);
    gxp[i]->DrawCopy("hist");
    gPad->SetGridx(1);
    gPad->SetGridy(1);
    gPad->SetLogz(1);
  }

  string finNm = finC->GetName();
  string foutNm = finNm.substr(0,finNm.find_last_of("."))+"_1Dana.root";

  TFile *fout = new TFile(foutNm.c_str(),"RECREATE");
  fout->cd();
  for(int i=0;i<nDet;i++){
    exp[i]->SetDirectory(0);
    exp[i]->Write();
    gxp[i]->SetDirectory(0);
    gxp[i]->Write();
    gx[i]->SetDirectory(0);
    gx[i]->Write();
  }
  fout->Close();

  return;
}
