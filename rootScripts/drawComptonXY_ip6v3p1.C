void drawComptonXY_ip6v3p1(){
  TFile *finB=TFile::Open("/work/halla/moller12gev/ciprian/compton/comptonEic/beam18GeV_ip6V3p1_1e3.root","READ");
  //TFile *finB=TFile::Open("/work/halla/moller12gev/ciprian/compton/comptonEic/beam5GeV_ip6V3p1_1e3.root","READ");
  TTree *tb=(TTree*)finB->Get("t");

  TFile *finC=TFile::Open("/volatile/halla/moller12gev/ciprian/tmp/compton/o_e18Pz59Pxm81_ip6V3p1_VtxSmearXY0Z0_1e6.root","READ");
  //TFile *finC=TFile::Open("/volatile/halla/moller12gev/ciprian/tmp/compton/o_e5Pz97Pxm26_ip6V3p1_VtxSmearXY0Z0_1e6.root","READ");
  TTree *tc=(TTree*)finC->Get("t");

  const int nDet=12;
  string detNm[nDet]={"q09DS","q08US","q08DS","q07US","q07DS","q06US","q06DS","q05US","q05DS","q04US","q04DS","q03US"};
  float xPos[nDet]={2.463e+2 ,2.741e+2 ,2.939e+2, 3.504e+2 ,3.703e+2,4.268e+2,4.466e+2,4.763e+2,4.774e+2,4.803e+2,4.813e+2,4.956e+2};
  float gOffset[nDet]={0,0,0,0,0,0,-2,-29,-49,-107,-127,-390};
  const int nEvProc=1000000;

  gStyle->SetOptStat(0);
  auto *c1=new TCanvas("c1","elec",1800,1600);
  c1->Divide(4,3);
  auto *c2=new TCanvas("c2","gammaB",1800,1600);
  c2->Divide(4,3);
  auto *c3=new TCanvas("c3","gamma",1800,1600);
  c3->Divide(4,3);

  TH2F *exy[nDet],*gxyB[nDet],*gxy[nDet];
  for(int i=0;i<nDet;i++){
    cout<<i<<endl;
    exy[i]=new TH2F(Form("exy_%s",detNm[i].c_str()),
		    Form("uXsec %s;x[mm];y[mm]",detNm[i].c_str()),
		    200,xPos[i]-120,xPos[i]+50,
		    200,-1.2,1.2);
    c1->cd(i+1);
    int nE = tc->Project(Form("exy_%s",detNm[i].c_str()),
			 Form("%s_hits.y:%s_hits.x",detNm[i].c_str(),detNm[i].c_str()),
			 Form("uXsec*(%s_hits.pID==11)",detNm[i].c_str()),"",nEvProc);
    //Form("uXsec*(%s_hits.pID==11)",detNm[i].c_str()));
    exy[i]->SetTitle(Form("%s :%d",exy[i]->GetTitle(),nE));
    exy[i]->DrawCopy("colz");
    tb->Draw(Form("%s_hits.y:%s_hits.x",detNm[i].c_str(),detNm[i].c_str()),
	     "","* && same");
    gPad->SetGridx(1);
    gPad->SetGridy(1);
    gPad->SetLogz(1);

    cout<<i<<endl;
    gxyB[i]=new TH2F(Form("gxyB_%s",detNm[i].c_str()),
		    Form("uXsec %s;(x - xB)[mm];y[mm]",detNm[i].c_str()),
		    200,-20,400,
		    200,-8,8);
    c2->cd(i+1);
    nE=tc->Project(Form("gxyB_%s",detNm[i].c_str()),
		   Form("%s_hits.y:(%s_hits.x-%f)",detNm[i].c_str(),detNm[i].c_str(),xPos[i]),
		   Form("uXsec*(%s_hits.pID==22)",detNm[i].c_str()),"",nEvProc);
    gxyB[i]->SetTitle(Form("%s :%d",gxyB[i]->GetTitle(),nE));
    gxyB[i]->DrawCopy("colz");
    tb->Draw(Form("%s_hits.y:(%s_hits.x-%f)",detNm[i].c_str(),detNm[i].c_str(),xPos[i]),
	     "","* && same");
    gPad->SetGridx(1);
    gPad->SetGridy(1);
    gPad->SetLogz(1);

    cout<<i<<endl;
    gxy[i]=new TH2F(Form("gxy_%s",detNm[i].c_str()),
		    Form("uXsec %s;x[mm];y[mm]",detNm[i].c_str()),
		    200,-10+xPos[i]-gOffset[i],10+xPos[i]-gOffset[i],
		    200,-10,10);
    c3->cd(i+1);
    nE=tc->Project(Form("gxy_%s",detNm[i].c_str()),
		   Form("%s_hits.y:%s_hits.x",detNm[i].c_str(),detNm[i].c_str()),
		   Form("uXsec*(%s_hits.pID==22)",detNm[i].c_str()),"",nEvProc);
    gxy[i]->SetTitle(Form("%s :%d",gxy[i]->GetTitle(),nE));
    gxy[i]->DrawCopy("colz");
    gPad->SetGridx(1);
    gPad->SetGridy(1);
    gPad->SetLogz(1);

  }

  return;
  /*
  TH1F *exp=new TH1F("exp","beam E = 18GeV;x[mm]",200,1335,1365);
  TH1F *exu=new TH1F("exu","beam E = 18GeV;x[mm]",200,1335,1365);
  tc->Project("exp","q07US_hits.x","pXsec*(q11US_hits.pID==11)");
  tc->Project("exu","q07US_hits.x","uXsec*(q11US_hits.pID==11)");
  exp->Divide(exu);
  auto c3=new TCanvas("c3","eX");
  exp->DrawCopy();
  gPad->SetGridx(1);
  gPad->SetGridy(1);

  TH1F *gxp=new TH1F("gxp","beam E = 18GeV;x[mm]",200,1049,1069);
  TH1F *gxu=new TH1F("gxu","beam E = 18GeV;x[mm]",200,1049,1069);
  tc->Project("gxp","q03US_hits.x","pXsec*(q09US_hits.pID==22)");
  tc->Project("gxu","q03US_hits.x","uXsec*(q09US_hits.pID==22)");
  gxp->Divide(gxu);
  auto c4=new TCanvas("c4","gX");
  gxp->DrawCopy();
  gPad->SetGridx(1);
  gPad->SetGridy(1);



  TFile *fout=new TFile("o_e18Pz71Px71_ip6V3_noSmear_5e6.root","RECREATE");
  for(int i=0;i<6;i++){
    fout->cd();
    exy[i]->Write();
    gxy[i]->Write();
  }
  exp->Write();
  gxp->Write();
  fout->Close();

  finB->Close();
  finC->Close();
  */
}
