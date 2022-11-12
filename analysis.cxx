#include <cmath>
#include <iostream>
#include <vector>

#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TH1F.h"
#include "TROOT.h"
#include "TStyle.h"

void setStyle() {
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(1122);
  gStyle->SetOptFit(111);
  gStyle->SetPalette(57);
  gStyle->SetOptTitle(0);
}

void analysis() {
  TFile *file = new TFile("particlegeneration.root");

  TH1F *h1 = (TH1F *)file->Get("h1");
  TH1F *h2 = (TH1F *)file->Get("h2");
  TH1F *h3 = (TH1F *)file->Get("h3");
  TH1F *h4 = (TH1F *)file->Get("h4");
  TH1F *h5 = (TH1F *)file->Get("h5");
  TH1F *h6 = (TH1F *)file->Get("h6");
  TH1F *h7 = (TH1F *)file->Get("h7");
  TH1F *h8 = (TH1F *)file->Get("h8");
  TH1F *h9 = (TH1F *)file->Get("h9");
  TH1F *h10 = (TH1F *)file->Get("h10");
  TH1F *h11 = (TH1F *)file->Get("h11");
  TH1F *h12 = (TH1F *)file->Get("h12");

  std::vector<TH1F *> histograms;
  histograms.push_back(h1);
  histograms.push_back(h2);
  histograms.push_back(h3);
  histograms.push_back(h4);
  histograms.push_back(h5);
  histograms.push_back(h6);
  histograms.push_back(h7);
  histograms.push_back(h8);
  histograms.push_back(h9);
  histograms.push_back(h10);
  histograms.push_back(h11);
  histograms.push_back(h12);

  // Fitting angles distribution
  double const cazimutal = 1 / (2 * M_PI);
  double const cpolar = 1 / (M_PI);
  TF1 *fazimutal = new TF1("fazimutal", "pol0", 0, 2 * M_PI);
  TF1 *fpolar = new TF1("fpolar", "pol0", 0, M_PI);
  fazimutal->SetParameter(0, cazimutal);
  fpolar->SetParameter(0, cpolar);
  h2->Fit(fazimutal);
  h3->Fit(fpolar);

  // Fitting magnitude of p
  TF1 *fp = new TF1("fp", "expo", 0, 10);
  fp->SetParameter(1, -1);
  h4->Fit(fp);

  // Analysis of invariant mass
  TH1F *h13 = new TH1F(*h8);
  TH1F *h14 = new TH1F(*h10);

  h13->Add(h8, h9, 1, -1);
  h14->Add(h10, h11, 1, -1);
  // al posto di fare le entries usare il metodo integral che da' le entries
  // correte
  double const kmass = 0.89166;
  double const kwidth = 0.050;

  TF1 *finvmass1 = new TF1("finvmass1", "gaus", 0, 2);
  TF1 *finvmass2 = new TF1("finvmass2", "gaus", 0, 2);
  finvmass1->SetParameter(1, kmass);
  finvmass1->SetParameter(2, kwidth);
  finvmass2->SetParameter(1, kmass);
  finvmass2->SetParameter(2, kwidth);

  h13->Fit(finvmass1);
  h14->Fit(finvmass2);

  // Fitting

  histograms.push_back(h13);
  histograms.push_back(h14);

  // Drawing statistical part on canvas

  TCanvas *c1 = new TCanvas("c1", "Histograms", 200, 10, 600, 400);
  c1->Divide(5, 3);

  for (int i = 0; i != 14; ++i) {
    c1->cd(i + 1);
    histograms[i]->DrawCopy("H");
    histograms[i]->DrawCopy("E, SAME");
  }

  // Saving canvas data

  c1->Print("analysis.pdf");
  c1->Print("analysis.C");
  c1->Print("analysis.root");

  // Printing on screen

  for (int i = 0; i != 12; ++i) {
    // Printig entries for each histogram
    std::cout << i + 1 << " Entries: " << histograms[i]->GetEntries() << '\n';
  }

  // Analysis of generated particles
  std::cout << '\n' << "Data Analysis: " << '\n' << '\n';

  std::cout << "Check proportions of types of particle generated: " << '\n';
  for (int i = 1; i != 8; ++i) {
    std::cout << "Bin " << i << " content: " << h1->GetBinContent(i) << " "
              << "+/-"
              << " " << h1->GetBinError(i) << '\n';
    std::cout << "Bin " << i << " pencentage of particles: "
              << (h1->GetBinContent(i) / h1->GetEntries()) << '\n';
  }

  std::cout << '\n' << "Result of fits: " << '\n' << '\n';

  std::cout << "h2 fit parameters, chi square and probability of fit: " << '\n'
            << fazimutal->GetParameter(0) << " "
            << "+/-"
            << " " << fazimutal->GetParError(0) << '\n'
            << fazimutal->GetChisquare() / fazimutal->GetNDF() << '\n'
            << fazimutal->GetProb() << '\n';
  std::cout << "h3 fit parameters, chi square and probability of fit: " << '\n'
            << fpolar->GetParameter(0) << " "
            << "+/-"
            << " " << fpolar->GetParError(0) << '\n'
            << fpolar->GetChisquare() / fpolar->GetNDF() << '\n'
            << fpolar->GetProb() << '\n';

  std::cout << "h4 fit parameters, chi square and probability of fit: " << '\n'
            << fp->GetParameter(0) << " "
            << "+/-"
            << " " << fp->GetParError(0) << '\n'
            << fp->GetParameter(1) << " "
            << "+/-"
            << " " << fp->GetParError(1) << '\n'
            << fp->GetChisquare() / fp->GetNDF() << '\n'
            << fp->GetProb() << '\n';

  std::cout << "h13 fit parameters, chi square and probability of fit: " << '\n'
            << finvmass1->GetParameter(0) << " "
            << "+/-"
            << " " << finvmass1->GetParError(0) << '\n'
            << finvmass1->GetParameter(1) << " "
            << "+/-"
            << " " << finvmass1->GetParError(1) << '\n'
            << finvmass1->GetParameter(2) << " "
            << "+/-"
            << " " << finvmass1->GetParError(2) << '\n'
            << finvmass1->GetChisquare() / finvmass1->GetNDF() << '\n'
            << finvmass1->GetProb() << '\n';

  std::cout << "h14 fit parameters, chi square and probability of fit: " << '\n'
            << finvmass2->GetParameter(0) << " "
            << "+/-"
            << " " << finvmass2->GetParError(0) << '\n'
            << finvmass2->GetParameter(1) << " "
            << "+/-"
            << " " << finvmass2->GetParError(1) << '\n'
            << finvmass2->GetParameter(2) << " "
            << "+/-"
            << " " << finvmass2->GetParError(2) << '\n'
            << finvmass2->GetChisquare() / finvmass2->GetNDF() << '\n'
            << finvmass2->GetProb() << '\n';

  file->Close();
}

int main() { analysis(); }