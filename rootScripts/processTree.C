
R__LOAD_LIBRARY(../build/libdet.so)

#include "../include/genericDet.h"

void processTree(){
  TFile *fin=TFile::Open("../output/o_e18Px100_5e6.root");
  TTree *t=(TTree*)fin->Get("t");
  double uXsec,pXsec;
  std::vector<genHit> *bD22=0;
  t->SetBranchAddress("bD22EF_hits",&bD22);
  t->SetBranchAddress("uXsec",&uXsec);
  t->SetBranchAddress("pXsec",&pXsec);

  TH2D *xyE=new TH2D("xyE","xyE",
		     200,192.57,192.63,
		     200,-0.01,0.01);

  TH2D *xyU=new TH2D("xyU","xyU",
		     200,192.57,192.63,
		     200,-0.01,0.01);

  TH2D *xyP=new TH2D("xyP","xyP",
		     200,192.57,192.63,
		     200,-0.01,0.01);

  long nEntries=t->GetEntries();
  cout<<"\tTotal events: "<<nEntries<<endl;
  float currentProc=1,procStep=10;

  for(int i=0;i<nEntries;t->GetEntry(i++)){
    if( float(i+1)/nEntries*100 > currentProc){
      cout<<"at tree entry\t"<<i<<"\t"<< float(i+1)/nEntries*100<<endl;
      currentProc+=procStep;
    }
    
    
    for(int j=0;j<bD22->size();j++){
      if(bD22->at(j).pID == 11){
	xyE->Fill(bD22->at(j).x,bD22->at(j).y);
	xyU->Fill(bD22->at(j).x,bD22->at(j).y,uXsec);
	xyP->Fill(bD22->at(j).x,bD22->at(j).y,pXsec);
      }
    }
    
  }

  xyP->Divide(xyU);
  xyU->Divide(xyE);

  auto *c1=new TCanvas("c1","c1");
  c1->Divide(2);
  c1->cd(1);
  xyU->DrawCopy("colz");
  c1->cd(2);
  xyP->DrawCopy("colz");
  
  fin->Close();
}
