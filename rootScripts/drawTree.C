#include "anaConst.h"

// const int nXsec=2;
// const string xSecNm[nXsec]={"uX","pX"};
// const string xSecTi[nXsec]={"#sigma_{unpol}","#sigma_{pol}"};
// TH1D *hXsec[nXsec], *iXsec[nXsec];

// const int nDet = 8;
// const double zPos[nDet]={0.05,5,6.5,16,16.7,25,27,27.7};//m


// double partXBinLimit[nPart][2]={{-48.5,-47.5},{-10,12}};
// double partYBinLimit[nPart][2]={{-1,1},{-0.07,0.07}};
// TH2D *xy[nPart][nDet][nWght];
// const int nSeg=9;
// const double seg[nSeg]={500,400,300,200,100,50,10,5,1};
// const double segL[nSeg]={0.10,0.08,0.09,0.08,0.08,0.08,0.08,0.08,0.08};
// TH1D *yElec[nWght][nSeg];


const int nDet = 6;
const double zPos[nDet]={0.7,3.9,4.9,9.1,10.1,31.6};
const string detNm[nDet]={  "bD22EF","aD22EF",  "aQ10EF", "bQ9EF",  "aQ9EF",  "gDet"};
const float xLim[nDet][2]={{185,200},{180,250},{170,260},{100,310},{110,300},{460,540}};

const string wght[nWght]={"1","uXsec","pXsec","ke*uXsec","ke*pXsec","pXsec*sqrt(uXsec)"};

const int nPart=2;
std::map<int,int> pid;
const string partT[nPart]={"gamma","electron"};
const string partN[nPart]={"g","e"};
 
const int n2Dbins=800;
TFile *finC, *finB;

void drawAllDets(int ene);
void drawEdet(int ene);
void drawGdet(int ene);

void drawTree(){

  //finC=TFile::Open("../output/o_e18Px100_5e6.root");
  //finC=TFile::Open("../output/o_e5Px100_5e6.root");

  //finC=TFile::Open("/volatile/halla/moller12gev/ciprian/tmp/compton/o_e5Pz98Px22_5e6.root");
  //finC=TFile::Open("/volatile/halla/moller12gev/ciprian/tmp/compton/o_e10Pz91Px42_5e6.root");
  finC=TFile::Open("/volatile/halla/moller12gev/ciprian/tmp/compton/o_e18Pz71Px71_5e6.root");
  //finC=TFile::Open("/volatile/halla/moller12gev/ciprian/tmp/compton/o_e18Pz71Px71_VtxSmearXY01Z9mm_5e6.root");
  //finC=TFile::Open("/volatile/halla/moller12gev/ciprian/tmp/compton/o_e5Pz98Px22_VtxSmearXY01Z7mm_5e6.root");
  //finC=TFile::Open("/volatile/halla/moller12gev/ciprian/tmp/compton/o_e10Pz91Px42_VtxSmearXY01Z7mm_5e6.root");

  finB=TFile::Open("../output/beam18GeV_1e3.root");
  
  drawEdet(18);
  //drawGdet(5);
}

void drawAllDets(int ene){
  TH2D *xyAll[nDet];
  TTree *tC = (TTree*)finC->Get("t");
  TTree *tB = (TTree*)finB->Get("t");
  gStyle->SetOptStat(0);

  TCanvas *cA[nDet];
  for(int i=0;i<nDet;i++){
    xyAll[i]=new TH2D(Form("xyAll%d_%d",ene,i),Form("%dGeV %s at z=%4.1f;x[mm];y[mm]",ene,detNm[i].c_str(),zPos[i]),
		      400,xLim[i][0],xLim[i][1],
		      400,-10,10);
    cA[i]=new TCanvas(Form("cA%d_%d",ene,i),Form("cA%d_%d",ene,i),1800,800);
    string plt = Form("%s_hits.y:%s_hits.x",detNm[i].c_str(),detNm[i].c_str());
    string cuts = Form("uXsec*(%s_hits.trackID<3 && %s_hits.parentID==0)",detNm[i].c_str(),detNm[i].c_str());
    tC->Project(xyAll[i]->GetName(),plt.c_str(),cuts.c_str());
    xyAll[i]->DrawCopy("colz");
    tB->Draw(plt.c_str(),"","* SAME");
  }
}

void drawEdet(int ene){
  TH2D *exy[nWght];
  TH1D *ex[nWght],*grho[nWght];

  TTree *tC = (TTree*)finC->Get("t");
  TTree *tB = (TTree*)finB->Get("t");
  gStyle->SetOptStat(0);

  eEnergy = ene*1e9;
  recalcMaxEnergy();

  string plt;
  const int postProc[nWght]={0,0,1,0,3,1};
  for(int i=0;i<nWght;i++){

    exy[i]=new TH2D(Form("exy%d_Wght%d",ene,i),Form("%dGeV eDet(bQ9) %s ;x[mm];y[mm]",ene,wgTit[i].c_str()),
		    400,100,185,
		    400,-1.0,1.0);

    ex[i]=new TH1D(Form("ex%d_Wght%d",ene,i),Form("%dGeV eDet(bQ9) %s ;x[mm]",ene,wgTit[i].c_str()),
		    400,100,185);

    grho[i]=new TH1D(Form("grho%d_Wght%d",ene,i),
		     Form("%dGeV eDet(bQ9) %s ; #gamma energy/max #gamma energy #rho",ene,wgTit[i].c_str()),
		     100,0,1);
    string wg = wght[i];
    if(i==3 || i==4) wg="bQ9EF_hits."+wg;

    plt = "bQ9EF_hits.y:bQ9EF_hits.x";
    string cuts = Form("%s*(bQ9EF_hits.pID==11 && bQ9EF_hits.parentID==0)",wg.c_str());
    tC->Project(exy[i]->GetName(),plt.c_str(),cuts.c_str());

    plt = "bQ9EF_hits.x";
    tC->Project(ex[i]->GetName(),plt.c_str(),cuts.c_str());

    plt = Form("bQ9EF_hits.ke/%f/1000",gMaxEnergy);
    cuts = Form("%s*(bQ9EF_hits.pID==22 && bQ9EF_hits.parentID==0)",wg.c_str());
    tC->Project(grho[i]->GetName(),plt.c_str(),cuts.c_str());

    if(postProc[i]!=0){
      exy[i]->Divide(exy[postProc[i]]);
      ex[i]->Divide(ex[postProc[i]]);
      grho[i]->Divide(grho[postProc[i]]);
    }

  }

  exy[1]->Divide(exy[0]);
  exy[3]->Divide(exy[0]);
  ex[1]->Divide(ex[0]);
  ex[3]->Divide(ex[0]);
  grho[1]->Divide(grho[0]);
  grho[3]->Divide(grho[0]);

  TCanvas *ce2d=new TCanvas(Form("ce%d2d",ene),Form("ce%d2d",ene),2000,800);
  TCanvas *ce1d=new TCanvas(Form("ce%d1d",ene),Form("ce%d1d",ene),2000,800);
  TCanvas *cerh=new TCanvas(Form("ce%drh",ene),Form("ce%drh",ene),2000,800);
  ce2d->Divide(3,2);
  ce1d->Divide(3,2);
  cerh->Divide(3,2);
  for(int i=0;i<nWght;i++){
    ce2d->cd(i+1);
    exy[i]->DrawCopy("colz");
    plt = "bQ9EF_hits.y:bQ9EF_hits.x";
    tB->Draw(plt.c_str(),"","* SAME");
    ce1d->cd(i+1);
    ex[i]->DrawCopy();
    cerh->cd(i+1);
    grho[i]->DrawCopy();
  }
  
  //TFile *fout=new TFile("../output/o_e5Pz98Px22_VtxSmearXY01Z7mm_5e6_eDet.root","RECREATE");
  //TFile *fout=new TFile("../output/o_e10Pz91Px42_VtxSmearXY01Z7mm_5e6_eDet.root","RECREATE");
  //TFile *fout=new TFile("../output/o_e5Pz98Px22_5e6_eDet.root","RECREATE");
  //TFile *fout=new TFile("../output/o_e10Pz91Px42_5e6_eDet.root","RECREATE");
  TFile *fout=new TFile("../output/o_e18Pz71Px71_5e6_eDet.root","RECREATE");
  for(int i=0;i<nWght;i++){
    exy[i]->Write();
    ex[i]->Write();
    grho[i]->Write();
  }
  fout->Close();

}


void drawGdet(int ene){
  TH2D *gxy[nWght];
  TTree *tC = (TTree*)finC->Get("t");
  TTree *tB = (TTree*)finB->Get("t");
  gStyle->SetOptStat(0);

  string plt = "gDet_hits.y:gDet_hits.x";
  const int postProc[nWght]={0,0,1,0,3,1};
  for(int i=0;i<nWght;i++){
    gxy[i]=new TH2D(Form("gxy%d_Wght%d",ene,i),Form("%dGeV gDet(31.6m) %s ;x[mm];y[mm]",ene,wgTit[i].c_str()),
		    400,502.5,522.5,
		    400,-10,10);
    string cuts = Form("%s*(gDet_hits.pID==22 && gDet_hits.parentID==0)",wght[i].c_str());
    tC->Project(gxy[i]->GetName(),plt.c_str(),cuts.c_str());
    if(postProc[i]!=0)
      gxy[i]->Divide(gxy[postProc[i]]);
  }

  gxy[1]->Divide(gxy[0]);
  gxy[3]->Divide(gxy[0]);

  TCanvas *cg=new TCanvas(Form("cg%d",ene),Form("cg%d",ene),1600,800);
  cg->Divide(3,2);
  for(int i=0;i<nWght;i++){
    cg->cd(i+1);
    gxy[i]->DrawCopy("colz");
  }
  
}
