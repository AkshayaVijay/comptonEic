void drawComptonXY(){
  TFile *finB=TFile::Open("/w/halla-scshelf2102/moller12gev/ciprian/compton/comptonEic/beam18GeV_1e3.root","READ");
  TTree *tb=(TTree*)finB->Get("t");

  TFile *finC=TFile::Open("/lustre19/expphy/volatile/halla/moller12gev/ciprian/tmp/compton/o_e18Pz71Px71_VtxSmearXY0Z0_5e6.root","READ");
  TTree *tc=(TTree*)finC->Get("t");

  string detNm[6]={"q13DS","d22DS","q12DS","q11US","q11DS","q09US"};
  float exPos[6]={1407.9,1365.4,1357.3,1338.6,1332.0,1287.9};
  float gxPos[6]={1408,1345,1315,1245,1221,1059};

  gStyle->SetOptStat(0);
  auto *c1=new TCanvas("c1","elec",1800,1200);
  c1->Divide(3,2);
  auto *c2=new TCanvas("c2","gamma",1800,1200);
  c2->Divide(3,2);

  TH2F *exy[6],*gxy[6];
  for(int i=0;i<6;i++){
    exy[i]=new TH2F(Form("exy_%s",detNm[i].c_str()),
		    Form("uXsec %s;x[mm];y[mm]",detNm[i].c_str()),
		    200,exPos[i]-10,exPos[i]+100,
		    200,-1.2,1.2);
    c1->cd(i+1);
    int nE = tc->Project(Form("exy_%s",detNm[i].c_str()),
			 Form("%s_hits.y:%s_hits.x",detNm[i].c_str(),detNm[i].c_str()),
			 Form("(%s_hits.pID==11)",detNm[i].c_str()));
    //Form("uXsec*(%s_hits.pID==11)",detNm[i].c_str()));
    exy[i]->SetTitle(Form("%s :%d",exy[i]->GetTitle(),nE));
    exy[i]->DrawCopy("colz");
    tb->Draw(Form("%s_hits.y:%s_hits.x",detNm[i].c_str(),detNm[i].c_str()),
	     "","* && same");
    gPad->SetGridx(1);
    gPad->SetGridy(1);
    gPad->SetLogz(1);

    gxy[i]=new TH2F(Form("gxy_%s",detNm[i].c_str()),
		    Form("uXsec %s;x[mm];y[mm]",detNm[i].c_str()),
		    200,gxPos[i]-10,gxPos[i]+10,
		    200,-10,10);
    c2->cd(i+1);
    nE=tc->Project(Form("gxy_%s",detNm[i].c_str()),
		   Form("%s_hits.y:%s_hits.x",detNm[i].c_str(),detNm[i].c_str()),
		   Form("(%s_hits.pID==22)",detNm[i].c_str()));
    //Form("uXsec*(%s_hits.pID==22)",detNm[i].c_str()));
    gxy[i]->SetTitle(Form("%s :%d",gxy[i]->GetTitle(),nE));
    gxy[i]->DrawCopy("colz");
    gPad->SetGridx(1);
    gPad->SetGridy(1);
    gPad->SetLogz(1);
    // tb->Draw(Form("%s_hits.y:%s_hits.x",detNm[i].c_str(),detNm[i].c_str()),
    // 	     "","* && same");


    //t->Draw(Form("%s_hits.x-%f",detNm[i].c_str(),xPos[i]));
  }

  TH1F *exp=new TH1F("exp","beam E = 18GeV;x[mm]",200,1335,1365);
  TH1F *exu=new TH1F("exu","beam E = 18GeV;x[mm]",200,1335,1365);
  tc->Project("exp","q11US_hits.x","pXsec*(q11US_hits.pID==11)");
  tc->Project("exu","q11US_hits.x","uXsec*(q11US_hits.pID==11)");
  exp->Divide(exu);
  auto c3=new TCanvas("c3","eX");
  exp->DrawCopy();
  gPad->SetGridx(1);
  gPad->SetGridy(1);

  TH1F *gxp=new TH1F("gxp","beam E = 18GeV;x[mm]",200,1049,1069);
  TH1F *gxu=new TH1F("gxu","beam E = 18GeV;x[mm]",200,1049,1069);
  tc->Project("gxp","q09US_hits.x","pXsec*(q09US_hits.pID==22)");
  tc->Project("gxu","q09US_hits.x","uXsec*(q09US_hits.pID==22)");
  gxp->Divide(gxu);
  auto c4=new TCanvas("c4","gX");
  gxp->DrawCopy();
  gPad->SetGridx(1);
  gPad->SetGridy(1);



  TFile *fout=new TFile("o_e18Pz71Px71_noSmear_5e6.root","RECREATE");
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

}
