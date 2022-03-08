
R__LOAD_LIBRARY(../build/libdet.so)

#include "../include/genericDet.h"
#include "anaConst.h"

void processTree(string,float,float,float,float,int);
void drawGdet(string,int);
void draw1DallE();
void analyzePhotonDetAll();

void analyzePhotonDet(){

  eEnergy = 18e9;
  recalcMaxEnergy();  
  processTree("/volatile/halla/moller12gev/ciprian/tmp/compton/o_e18Pz71Px71_5e6.root",
	      502, 522, -10, 10,int(eEnergy/1e9));
  
  //analyzePhotonDetAll();
}

void analyzePhotonDetAll(){

  const int processTrees = 0;
  const int draw = 1; 

  eEnergy = 18e9;
  recalcMaxEnergy();  
  if(processTrees)
    processTree("/volatile/halla/moller12gev/ciprian/tmp/compton/o_e18Px100_5e6.root",
  		506, 518, -6, 6,int(eEnergy/1e9));
  if(draw)
    drawGdet("../output/o_e18Px100_5e6_gDet.root", int(eEnergy/1e9));

  if(processTrees)
    processTree("/volatile/halla/moller12gev/ciprian/tmp/compton/o_e18Pz71Px71_5e6.root",
  		506, 518, -6, 6,int(eEnergy/1e9));
  if(draw)
    drawGdet("../output/o_e18Pz71Px71_5e6_gDet.root", int(eEnergy/1e9));

  if(processTrees)
    processTree("/volatile/halla/moller12gev/ciprian/tmp/compton/o_e18Pz71Px71_VtxSmearXY01Z9mm_5e6.root",
		506, 518, -6, 6,int(eEnergy/1e9));
  if(draw)
    drawGdet("../output/o_e18Pz71Px71_VtxSmearXY01Z9mm_5e6_gDet.root", int(eEnergy/1e9));


  eEnergy = 10e9;
  recalcMaxEnergy();  
  if(processTrees)
    processTree("/volatile/halla/moller12gev/ciprian/tmp/compton/o_e10Pz91Px42_5e6.root",
		504, 520, -8, 8,int(eEnergy/1e9));
  if(draw)
    drawGdet("../output/o_e10Pz91Px42_5e6_gDet.root", int(eEnergy/1e9));

  eEnergy = 5e9;
  recalcMaxEnergy();  
  if(processTrees)
    processTree("/volatile/halla/moller12gev/ciprian/tmp/compton/o_e5Px100_5e6.root",
		502, 522, -10, 10,int(eEnergy/1e9));
  if(draw)
    drawGdet("../output/o_e5Px100_5e6_gDet.root", int(eEnergy/1e9));

  if(processTrees)
    processTree("/volatile/halla/moller12gev/ciprian/tmp/compton/o_e5Pz98Px22_5e6.root",
		502, 522, -10, 10,int(eEnergy/1e9));
  if(draw)
    drawGdet("../output/o_e5Pz98Px22_5e6_gDet.root", int(eEnergy/1e9));

  if(draw)
    drawGdet("../output/o_e5Pz98Px22_VtxSmearXY01Z7mm_5e6_gDet.root", int(eEnergy/1e9));

  if(draw)
    draw1DallE();
}

void processTree(string fnm="../output/o_e18Px100_5e6.root",
		 float xL=506,float xH=518,
		 float yL=-6,float yH=6, int bE=18){
  TFile *fin=TFile::Open(fnm.c_str());
  TTree *t=(TTree*)fin->Get("t");
  double uXsec,pXsec;
  std::vector<genHit> *gDet=0;
  t->SetBranchAddress("gDet_hits",&gDet);
  t->SetBranchAddress("uXsec",&uXsec);
  t->SetBranchAddress("pXsec",&pXsec);

  TH2D *xy[nWght];
  TH1D *x[nWght],*rho[nWght];
  const int nBins = 800;

  for(int i=0;i<nWght;i++){
    xy[i]=new TH2D(Form("xy_bE%d_wgt%d",bE,i),Form("beam E = %d %s;x[mm];y[mm]",bE, wgTit[i].c_str()),
		   nBins,xL,xH,
		   nBins,yL,yH);
    x[i]=new TH1D(Form("x_bE%d_wgt%d",bE,i),Form("beam E = %d %s;x[mm]",bE, wgTit[i].c_str()),
		  nBins,xL,xH);

    rho[i]=new TH1D(Form("rho_bE%d_wgt%d",bE,i),Form("beam E = %d %s;photon E / max photon E #rho ",bE, wgTit[i].c_str()),
		    nBins,0,1);
  }

  long nEntries=t->GetEntries();
  cout<<"\tTotal events: "<<nEntries<<endl;
  float currentProc=1,procStep=10;

  for(int i=0;i<nEntries;t->GetEntry(i++)){
    if( float(i+1)/nEntries*100 > currentProc){
      cout<<"at tree entry\t"<<i<<"\t"<< float(i+1)/nEntries*100<<endl;
      currentProc+=procStep;
    }
        
    for(int j=0;j<gDet->size();j++){
      if(gDet->at(j).pID == 22){
	double wg[nWght]={1,uXsec,pXsec,gDet->at(j).ke*uXsec,gDet->at(j).ke*pXsec,pXsec*sqrt(uXsec)};
	for(int k=0;k<nWght;k++){
	  xy[k]->Fill(gDet->at(j).x,gDet->at(j).y,wg[k]);
	  x[k]->Fill(gDet->at(j).x,wg[k]);
	  rho[k]->Fill(gDet->at(j).ke/1000/gMaxEnergy,wg[k]);
	}
      }
    }
    
  }

  string foutNm = Form("%s_gDet.root",fnm.substr(0,fnm.find_last_of(".")).c_str());
  TFile *fout=new TFile(foutNm.c_str(),"RECREATE");
  for(int i=nWght-1;i>=0;i--){
    if(scale[i]>=0)
      xy[i]->Divide(xy[scale[i]]);
    xy[i]->Write();

    if(scale[i]>=0)
      x[i]->Divide(x[scale[i]]);
    x[i]->Write();

    if(scale[i]>=0)
      rho[i]->Divide(rho[scale[i]]);
    rho[i]->Write();
  }
  fout->Close();
  fin->Close();
  cout<<"finished processing"<<endl;
}

void drawGdet(string fnm="../output/o_e18Px100_5e6_gDet.root",int bE=18){
  
  TFile *fin=TFile::Open(fnm.c_str(),"READ");

  gStyle->SetOptStat(0);

  string cTit = fnm.substr(fnm.find("_")+1,fnm.find("5e6")-fnm.find("_")-2);
  auto *c1=new TCanvas(Form("%s_xy",cTit.c_str()),Form("%s_xy",cTit.c_str()),1400,600);  
  c1->Divide(3,2);
  auto *c2=new TCanvas(Form("%s_x",cTit.c_str()),Form("%s_x",cTit.c_str()),1400,600);  
  c2->Divide(3,2);
  auto *c3=new TCanvas(Form("%s_rho",cTit.c_str()),Form("%s_rho",cTit.c_str()),1400,600);  
  c3->Divide(3,2);

  for(int i=0;i<nWght;i++){
    c1->cd(i+1);
    TH2D *h1=(TH2D*)fin->Get(Form("xy_bE%d_wgt%d",bE,i));
    h1->DrawCopy("colz");

    c2->cd(i+1);
    TH1D *h2=(TH1D*)fin->Get(Form("x_bE%d_wgt%d",bE,i));
    h2->DrawCopy("hist");

    c3->cd(i+1);
    TH1D *h3=(TH1D*)fin->Get(Form("rho_bE%d_wgt%d",bE,i));
    h3->DrawCopy("hist");

  }

  fin->Close();
}

void draw1DallE(){
  TFile *fin[3];
  fin[0] = TFile::Open("../output/o_e5Pz98Px22_5e6_gDet.root");
  fin[1] = TFile::Open("../output/o_e10Pz91Px42_5e6_gDet.root");
  fin[2] = TFile::Open("../output/o_e18Pz71Px71_5e6_gDet.root");
  int cls[3]={1,2,4};
  int e[3]={5,10,18};

  auto *c1=new TCanvas("cgD1","AN");
  c1->Divide(2);
  c1->cd(1);
  auto frm1=gPad->DrawFrame(502,-0.15,522,0.25);
  gPad->SetGridx(1);
  gPad->SetGridy(1);
  c1->cd(2);
  auto frm2=gPad->DrawFrame(0,-0.1,1,0.35);
  gPad->SetGridx(1);
  gPad->SetGridy(1);
  for(int i=0;i<3;i++){
    c1->cd(1);
    TH1D *x=(TH1D*)fin[i]->Get(Form("x_bE%d_wgt2",e[i]));
    x->SetLineColor(cls[i]);
    x->DrawCopy("same");
    c1->cd(2);
    TH1D *r=(TH1D*)fin[i]->Get(Form("rho_bE%d_wgt2",e[i]));
    r->SetLineColor(cls[i]);
    r->DrawCopy("hist same");
  }

  double totXsec[3]={0.0951659,0.0926631,0.0815514};
  double bunchRateHz[3]={99e6,99e6,24.75e6};
  auto *c2=new TCanvas("cgD2","rate");
  c2->Divide(2);
  c2->cd(1);
  auto frm3=gPad->DrawFrame(502,6e-5,522,1.7e-4);
  gPad->SetGridx(1);
  gPad->SetGridy(1);
  c2->cd(2);
  auto frm4=gPad->DrawFrame(502,2e4,522,1.7e5);
  gPad->SetGridx(1);
  gPad->SetGridy(1);
  for(int i=0;i<3;i++){
    c2->cd(1);
    TH1D *x=(TH1D*)fin[i]->Get(Form("x_bE%d_wgt1",e[i]));
    x->SetLineColor(cls[i]);
    x->DrawCopy("hist same");
    cout<<x->Integral()<<endl;
    c2->cd(2);
    x->Scale(bunchRateHz[i]/totXsec[i]);
    x->DrawCopy("same && hist");
    cout<<x->Integral()<<endl;

    // TH1D *r=(TH1D*)fin[i]->Get(Form("rho_bE%d_wgt2",e[i]));
    // r->SetLineColor(cls[i]);
    // r->DrawCopy("same");
  }

  fin[0]->Close();
  fin[1]->Close();
  fin[2]->Close();
}
