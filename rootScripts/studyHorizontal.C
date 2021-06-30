
R__LOAD_LIBRARY(../build/libdet.so)

#include "../include/genericDet.h"

void studyHorizontal(){
  TFile *fin=TFile::Open("../output/o_e18Px100_5e6.root");
  TTree *t=(TTree*)fin->Get("t");
  double uXsec,pXsec;
  std::vector<genHit> *bD22=0;
  std::vector<genHit> *bQ9=0;
  t->SetBranchAddress("bD22EF_hits",&bD22);
  t->SetBranchAddress("bQ9EF_hits",&bQ9);
  t->SetBranchAddress("uXsec",&uXsec);
  t->SetBranchAddress("pXsec",&pXsec);

  const int n2Dhist=3;
  TH2D *xyB22[n2Dhist],*xyBQ9[n2Dhist];
  TH2D *xEB22[n2Dhist],*xEBQ9[n2Dhist];
  TH2D *yEB22[n2Dhist],*yEBQ9[n2Dhist];
  TH1D *hEB22[n2Dhist],*hEBQ9[n2Dhist];
  TH1D *hxB22[n2Dhist],*hxBQ9[n2Dhist];
  string hNm[n2Dhist]={"E","U","P"};
  string hTi[n2Dhist]={"entries","unpol Xsec","pol Xsec"};
  for(int i=0;i<n2Dhist;i++){
    xyB22[i]=new TH2D(Form("xyB22_%s",hNm[i].c_str()),Form("bD22 %s;x[mm];y[mm]",hTi[i].c_str()),
		      200,192.57,192.63,
		      200,-0.01,0.01);
    xyBQ9[i]=new TH2D(Form("xyBQ9_%s",hNm[i].c_str()),Form("bQ9 %s;x[mm];y[mm]",hTi[i].c_str()),
		      200,100,190,
		      200,-0.07,0.07);

    xEB22[i]=new TH2D(Form("xEB22_%s",hNm[i].c_str()),Form("bD22 %s;x[mm];kE [MeV]",hTi[i].c_str()),
		      2000,192.57,192.63,
		      2000,10800,18000);
    xEBQ9[i]=new TH2D(Form("xEBQ9_%s",hNm[i].c_str()),Form("bQ9 %s;x[mm];kE [MeV]",hTi[i].c_str()),
		      2000,100,190,
		      2000,10800,18000);

    yEB22[i]=new TH2D(Form("yEB22_%s",hNm[i].c_str()),Form("bD22 %s;y[mm];kE [MeV]",hTi[i].c_str()),
		      200,-0.07,0.07,
		      200,10800,1800);
    yEBQ9[i]=new TH2D(Form("yEBQ9_%s",hNm[i].c_str()),Form("bQ9 %s;y[mm];kE [MeV]",hTi[i].c_str()),
		      200,-0.01,0.01,
		      200,10800,1800);

    hEB22[i]=new TH1D(Form("hEB22_%s",hNm[i].c_str()),Form("bD22 %s;kinetic energy [MeV]",hTi[i].c_str()),
		      200,10800,18000);
    hEBQ9[i]=new TH1D(Form("hEBQ9_%s",hNm[i].c_str()),Form("bQ9 %s;kinetic energy [MeV]",hTi[i].c_str()),
		      200,10800,18000);

    hxB22[i]=new TH1D(Form("hxB22_%s",hNm[i].c_str()),Form("bD22 %s;x [mm]",hTi[i].c_str()),
		      1000,192.57,192.63);
    hxBQ9[i]=new TH1D(Form("hxBQ9_%s",hNm[i].c_str()),Form("bQ9 %s;x [mm]",hTi[i].c_str()),
		      8000,100,190);
  }


  long nEntries=t->GetEntries();
  cout<<"\tTotal events: "<<nEntries<<endl;
  float currentProc=1,procStep=10;

  for(int i=0;i<nEntries;t->GetEntry(i++)){
    if( float(i+1)/nEntries*100 > currentProc){
      cout<<"at tree entry\t"<<i<<"\t"<< float(i+1)/nEntries*100<<endl;
      currentProc+=procStep;
    }
    

    int found=0;
    for(int j=0;j<bD22->size();j++){
      //if(bD22->at(j).pID == 11){
      if(bD22->at(j).pID == 11 && abs(bD22->at(j).ke-12380)<5){
	found++;
	double wgt[n2Dhist]={1,uXsec,pXsec};
	for(int ih=0;ih<n2Dhist;ih++){
	  xyB22[ih]->Fill(bD22->at(j).x,bD22->at(j).y,wgt[ih]);
	  xEB22[ih]->Fill(bD22->at(j).x,bD22->at(j).ke,wgt[ih]);
	  yEB22[ih]->Fill(bD22->at(j).y,bD22->at(j).ke,wgt[ih]);
	  hEB22[ih]->Fill(bD22->at(j).ke,wgt[ih]);
	  hxB22[ih]->Fill(bD22->at(j).x,wgt[ih]);
	}
      }
    }

    for(int j=0;j<bQ9->size();j++){
      if(bQ9->at(j).pID == 11 && found){
	double wgt[n2Dhist]={1,uXsec,pXsec};
	for(int ih=0;ih<n2Dhist;ih++){
	  xyBQ9[ih]->Fill(bQ9->at(j).x,bQ9->at(j).y,wgt[ih]);
	  xEBQ9[ih]->Fill(bQ9->at(j).x,bQ9->at(j).ke,wgt[ih]);
	  yEBQ9[ih]->Fill(bQ9->at(j).y,bQ9->at(j).ke,wgt[ih]);
	  hEBQ9[ih]->Fill(bQ9->at(j).ke,wgt[ih]);
	  hxBQ9[ih]->Fill(bQ9->at(j).x,wgt[ih]);
	}
      }
    }
    
  }

  xyB22[2]->Divide(xyB22[1]);
  xyB22[1]->Divide(xyB22[0]);

  xEB22[2]->Divide(xEB22[1]);
  xEB22[1]->Divide(xEB22[0]);

  yEB22[2]->Divide(yEB22[1]);
  yEB22[1]->Divide(yEB22[0]);

  hEB22[2]->Divide(hEB22[1]);
  hEB22[1]->Divide(hEB22[0]);

  hxB22[2]->Divide(hxB22[1]);
  hxB22[1]->Divide(hxB22[0]);

  xyBQ9[2]->Divide(xyBQ9[1]);
  xyBQ9[1]->Divide(xyBQ9[0]);

  xEBQ9[2]->Divide(xEBQ9[1]);
  xEBQ9[1]->Divide(xEBQ9[0]);

  hEBQ9[2]->Divide(hEBQ9[1]);
  hEBQ9[1]->Divide(hEBQ9[0]);

  gStyle->SetOptStat(0);
  gStyle->SetOptFit(1);
  auto *c1=new TCanvas("c1","xy");
  c1->Divide(2,2);
  c1->cd(1);
  xyB22[1]->DrawCopy("colz");
  c1->cd(2);
  xyB22[2]->DrawCopy("colz");
  c1->cd(3);
  xyBQ9[1]->DrawCopy("colz");
  c1->cd(4);
  xyBQ9[2]->DrawCopy("colz");

  auto *c2=new TCanvas("c2","hE");
  c2->Divide(2,2);
  c2->cd(1);
  hEB22[1]->DrawCopy();
  c2->cd(2);
  hEB22[2]->Fit("pol0");
  hEB22[2]->DrawCopy();
  c2->cd(3);
  hEBQ9[1]->DrawCopy();
  c2->cd(4);
  hEBQ9[2]->Fit("pol0");
  hEBQ9[2]->DrawCopy();

  auto *c3=new TCanvas("c3","xE");
  c3->Divide(2,2);
  c3->cd(1);
  xEB22[1]->GetXaxis()->SetRangeUser(192.587,192.601);
  xEB22[1]->GetYaxis()->SetRangeUser(12370,12390);
  xEB22[1]->DrawCopy("colz");
  c3->cd(2);
  xEB22[2]->GetXaxis()->SetRangeUser(192.587,192.601);
  xEB22[2]->GetYaxis()->SetRangeUser(12370,12390);
  xEB22[2]->DrawCopy("colz");
  c3->cd(3);
  xEBQ9[1]->GetXaxis()->SetRangeUser(128.6,129.1);
  xEBQ9[1]->GetYaxis()->SetRangeUser(12370,12390);
  xEBQ9[1]->DrawCopy("colz");
  c3->cd(4);
  xEBQ9[2]->GetXaxis()->SetRangeUser(128.6,129.1);
  xEBQ9[2]->GetYaxis()->SetRangeUser(12370,12390);
  xEBQ9[2]->DrawCopy("colz");

  auto *c4=new TCanvas("c4","yE");
  c4->Divide(2,2);
  c4->cd(1);
  yEB22[1]->DrawCopy("colz");
  c4->cd(2);
  yEB22[2]->DrawCopy("colz");
  c4->cd(3);
  yEBQ9[1]->DrawCopy("colz");
  c4->cd(4);
  yEBQ9[2]->DrawCopy("colz");

  auto *c5=new TCanvas("c5","hx");
  c5->Divide(2,2);
  c5->cd(1);
  hxB22[1]->DrawCopy();
  c5->cd(2);
  //hxB22[2]->Fit("pol0");
  hxB22[2]->DrawCopy();
  c5->cd(3);
  hxBQ9[1]->DrawCopy();
  c5->cd(4);
  //hxBQ9[2]->Fit("pol0");
  hxBQ9[2]->DrawCopy();

  fin->Close();
}
