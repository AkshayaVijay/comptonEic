#ifndef ANACONST_H
#define ANACONST_H

#include <map>

const double hplanck = 4.135667696e-15; // eV*s
const double clight = 299792458; //m/s
const double pi = acos(-1);
const double m2tobarn = 1e-28;

const double eRadius = 2.817940285e-15;  // classical electron radius (m)
const double eMass = 0.510998902e+6;   // electron mass (eV)

double gLambda = 532e-9; //m
double eEnergy = 18e9; //eV

double gEnergy = hplanck * clight / gLambda; //eV
double Gamma = eEnergy / eMass;
double a = 1 / ( 1 + 4*Gamma*gEnergy/eMass);//
double gMaxEnergy = 4*a*gEnergy*std::pow(Gamma,2)/1e9;//GeV

const int nWght = 6;//[counts,upolXsec, asym, E, asym*E, asym*sqrt(unpolXsec)]
const string wgTit[nWght]={"counts","unpolXsec","polXsec","kE*unpolXsec","kE*polXsec","polXsec*sqrt(unpolXsec)"};
const int scale[nWght]={-1,0,1,0,3,1}; //needs to be run in reverse

void recalcMaxEnergy(){
  Gamma = eEnergy / eMass;
  a = 1 / ( 1 + 4*Gamma*gEnergy/eMass);//
  gMaxEnergy = 4*a*gEnergy*std::pow(Gamma,2)/1e9;//GeV
}

#endif
