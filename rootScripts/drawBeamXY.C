void drawSmearXY(TTree*,TCanvas*);

void drawBeamXY(){
  //TFile *fin=TFile::Open("beam18GeV_ip6V2_1e3.root","READ");
  //TFile *fin=TFile::Open("beam18GeV_ip6V3_1e3.root","READ");
  TFile *fin=TFile::Open("../beam18GeV_ip6V4_noSync_1e3.root","READ");
  //TFile *fin=TFile::Open("beam5GeV_ip6V3_1e3.root","READ");
  TTree *t=(TTree*)fin->Get("t");
  gStyle->SetOptStat("em");
  auto *c1=new TCanvas("c1","c1",1800,1800);
  c1->Divide(4,3);
  const int nDet=12;
  string detNm[nDet]={"q09DS","q08US","q08DS","q07US","q07DS","q06US","q06DS","q05US","q05DS","q04US","q04DS","q03US"};
  float xPos[nDet]={2.463e+2 ,2.741e+2 ,2.939e+2, 3.504e+2 ,3.703e+2,4.268e+2,4.466e+2,4.763e+2,4.774e+2,4.803e+2,4.813e+2,4.956e+2};
  for(int i=0;i<nDet;i++){
    c1->cd(i+1);
    t->Draw(Form("%s_hits.x-%f",detNm[i].c_str(),xPos[i]));
  }

  auto *c2=new TCanvas("c2","c2",900,1800);
  drawSmearXY(t,c2);
  fin->Close();
}

void drawSmearXY(TTree *t, TCanvas *c){
  gStyle->SetOptStat("mr");
  c->Divide(2,3);
  c->cd(1);
  t->Draw("q06US_hits.y:q06US_hits.x","","colz");
  c->cd(3);
  t->Draw("q06US_hits.y","","");
  c->cd(5);
  t->Draw("q06US_hits.x","","");

  c->cd(2);
  t->Draw("q04US_hits.y:q04US_hits.x","","colz");
  c->cd(4);
  t->Draw("q04US_hits.y","","");
  c->cd(6);
  t->Draw("q04US_hits.x","","");

}
