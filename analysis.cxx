#include <cmath>
#include <iostream>
#include <vector>

#include "TAxis.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TH1F.h"
#include "TLegend.h"
#include "TROOT.h"
#include "TStyle.h"

void setStyle() {
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(1120);
  gStyle->SetOptFit(1111);
  gStyle->SetPalette(57);
  gStyle->SetOptTitle(1);
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

  std::vector<TH1F *> histograms{h1, h2, h3, h4,  h5,  h6,
                                 h7, h8, h9, h10, h11, h12};

  // Fitting angles distribution

  TF1 *fazimutal = new TF1("fazimutal", "pol0", 0, 2 * M_PI);
  TF1 *fpolar = new TF1("fpolar", "pol0", 0, M_PI);

  fazimutal->SetLineColor(kBlue);
  fpolar->SetLineColor(kBlue);

  h2->Fit(fazimutal);
  h3->Fit(fpolar);

  // Fitting magnitude of p
  TF1 *fp = new TF1("fp", "expo", 0, 10);
  fp->SetParameter(1, -1);
  fp->SetLineColor(kBlue);
  fp->SetLineStyle(2);

  h4->Fit(fp);

  // Analysis of invariant mass
  TH1F *h13 = new TH1F(*h8);
  TH1F *h14 = new TH1F(*h10);

  h13->Add(h8, h9, 1, -1);
  h14->Add(h10, h11, 1, -1);

  double const kmass = 0.89166;
  double const kwidth = 0.050;

  TF1 *finvmass1 = new TF1("finvmass1", "gaus", 0, 2);
  TF1 *finvmass2 = new TF1("finvmass2", "gaus", 0, 2);
  finvmass1->SetParameter(1, kmass);
  finvmass1->SetParameter(2, kwidth);
  finvmass2->SetParameter(1, kmass);
  finvmass2->SetParameter(2, kwidth);

  finvmass1->SetLineColor(kBlue);
  finvmass2->SetLineColor(kBlue);

  h13->Fit(finvmass1);
  h14->Fit(finvmass2);

  // Fitting

  histograms.push_back(h13);
  histograms.push_back(h14);

  // Cosmetics

  h1->SetTitle("Types of particle");
  h2->SetTitle("Distribution of azimutal angles");
  h3->SetTitle("Distribution of polar angles");
  h4->SetTitle("Distribution of p");
  h5->SetTitle("Distribution of transverse p");
  h6->SetTitle("Particles energy");
  h12->SetTitle("Invariant mass between decay products (benchmark)");
  h13->SetTitle("K* imvariant mass histogram (all particles)");
  h14->SetTitle("K* invariant mass histogram (pion/kaon)");

  for (long unsigned int i = 0; i != histograms.size(); ++i) {
    histograms[i]->SetFillColor(29);
  }

  for (long unsigned int i = 0; i != histograms.size(); ++i) {
    histograms[i]->GetYaxis()->SetTitle("Entries");
  }
  for (long unsigned int i = 1; i != 3; ++i) {
    histograms[i]->GetXaxis()->SetTitle("Angles (rad)");
  }
  for (long unsigned int i = 3; i != 5; ++i) {
    histograms[i]->GetXaxis()->SetTitle("p (GeV/c)");
  }
  h6->GetXaxis()->SetTitle("Energy (GeV)");
  for (long unsigned int i = 11; i != histograms.size(); ++i) {
    histograms[i]->GetXaxis()->SetTitle("Invariant mass (GeV/c^2)");
  }

  // Drawing statistical part on canvas

  TCanvas *c1 = new TCanvas("c1", "Histograms", 200, 10, 750, 400);
  h1->GetXaxis()->SetTitleOffset(1);
  h1->GetXaxis()->SetBinLabel(1, "Pion+");
  h1->GetXaxis()->SetBinLabel(2, "Pion-");
  h1->GetXaxis()->SetBinLabel(3, "Kaon+");
  h1->GetXaxis()->SetBinLabel(4, "Kaon-");
  h1->GetXaxis()->SetBinLabel(5, "Proton+");
  h1->GetXaxis()->SetBinLabel(6, "Proton-");
  h1->GetXaxis()->SetBinLabel(7, "K*");
  histograms[0]->DrawCopy("H");
  histograms[0]->DrawCopy("E, P, SAME");

  TCanvas *c2 = new TCanvas("c2", "Angles distribution", 200, 10, 1500, 400);
  c2->Divide(2, 1);
  TLegend *legaangles =
      new TLegend(.1, .7, .3, .9, "Azimutal angles distribution");
  legaangles->AddEntry(h1, "histogram");
  legaangles->AddEntry(fazimutal, "fit");
  TLegend *legpangles =
      new TLegend(.1, .7, .3, .9, "Polar angles distribution");
  legpangles->AddEntry(h2, "histogram");
  legpangles->AddEntry(fpolar, "fit");
  c2->cd(1);
  h2->GetXaxis()->SetTitleOffset(1);
  h2->GetYaxis()->SetTitleOffset(1.3);

  h2->SetMinimum(8500);
  h2->SetMaximum(11500);
  histograms[1]->DrawCopy("H");
  histograms[1]->DrawCopy("E, P, SAME");
  legaangles->Draw("SAME");
  c2->cd(2);
  h3->SetMinimum(8500);
  h3->SetMaximum(11500);
  h3->GetXaxis()->SetTitleOffset(1);
  h3->GetYaxis()->SetTitleOffset(1.3);

  histograms[2]->DrawCopy("H");
  histograms[2]->DrawCopy("E, P, SAME");
  legpangles->Draw("SAME");

  TCanvas *c3 = new TCanvas("c3", "p distribution", 200, 10, 1500, 400);
  c3->Divide(2, 1);
  TLegend *legp = new TLegend(.1, .7, .3, .9, "p distribution");
  legp->AddEntry(h3, "histogram");
  legp->AddEntry(fp, "fit");
  c3->cd(1);
  h5->GetXaxis()->SetTitleOffset(1);
  h5->GetYaxis()->SetTitleOffset(1.3);

  histograms[4]->DrawCopy("H");
  histograms[4]->DrawCopy("E, P, SAME");
  c3->cd(2);
  h4->GetXaxis()->SetTitleOffset(1);
  h4->GetYaxis()->SetTitleOffset(1.3);

  histograms[3]->DrawCopy("H");
  histograms[3]->DrawCopy("E, P, SAME");
  legp->Draw("SAME");

  TCanvas *c4 = new TCanvas("c4", "K* distribution", 200, 10, 1500, 400);
  c4->Divide(2, 1);
  TLegend *leginvmass1 = new TLegend(.1, .7, .3, .9, "K* distribution");
  leginvmass1->AddEntry(h12, "histogram");
  leginvmass1->AddEntry(finvmass1, "fit");
  c4->cd(1);
  h12->GetXaxis()->SetTitleOffset(1);
  h12->GetYaxis()->SetTitleOffset(1.3);

  histograms[11]->DrawCopy("H");
  histograms[11]->DrawCopy("E, P, SAME");

  c4->cd(2);
  h13->GetXaxis()->SetTitleOffset(1);
  h13->GetYaxis()->SetTitleOffset(1.3);

  histograms[12]->DrawCopy("H");
  histograms[12]->DrawCopy("E, P, SAME");
  leginvmass1->Draw("SAME");

  TCanvas *c5 = new TCanvas("c5", "K* distribution", 200, 10, 1500, 400);
  c5->Divide(2, 1);
  TLegend *leginvmass2 = new TLegend(.1, .7, .3, .9, "K* distribution");
  leginvmass2->AddEntry(h13, "histogram");
  leginvmass2->AddEntry(finvmass2, "fit");
  c5->cd(1);
  h12->GetXaxis()->SetTitleOffset(1);
  h12->GetYaxis()->SetTitleOffset(1.3);

  histograms[11]->DrawCopy("H");
  histograms[11]->DrawCopy("E, P, SAME");

  c5->cd(2);
  h14->GetXaxis()->SetTitleOffset(1);
  h14->GetYaxis()->SetTitleOffset(1.3);

  histograms[13]->DrawCopy("H");
  histograms[13]->DrawCopy("E, P, SAME");
  leginvmass2->Draw("SAME");

  TCanvas *c6 = new TCanvas("c6", "Energy", 200, 10, 750, 400);
  histograms[5]->DrawCopy("H");
  histograms[5]->DrawCopy("E, P, SAME");

  // Saving canvas data

  c1->Print("particleanalysis.pdf");
  c1->Print("particleanalysis.C");
  c1->Print("particleanalysis.root");
  c1->Print("particleanalysis.png");

  c2->Print("anglesanalysis.pdf");
  c2->Print("anglesanalysis.C");
  c2->Print("anglesanalysis.root");
  c2->Print("anglesanalysis.png");

  c3->Print("panalysis.pdf");
  c3->Print("panalysis.C");
  c3->Print("panalysis.root");
  c3->Print("panalysis.png");

  c4->Print("kdistrib1analysis.pdf");
  c4->Print("kdistrib1analysis.C");
  c4->Print("kdistrib1analysis.root");
  c4->Print("kdistrib1analysis.root");

  c5->Print("kdistrib2analysis.pdf");
  c5->Print("kdistrib2analysis.C");
  c5->Print("kdistrib2analysis.root");
  c5->Print("kdistrib2analysis.root");

  c6->Print("energyanalysis.pdf");
  c6->Print("energyanalysis.C");
  c6->Print("energyanalysis.root");
  c6->Print("energyanalysis.root");

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
            << "p0: " << fazimutal->GetParameter(0) << " "
            << "+/-"
            << " " << fazimutal->GetParError(0) << '\n'
            << "Chi square: " << fazimutal->GetChisquare() / fazimutal->GetNDF()
            << '\n'
            << "Fit probability: " << fazimutal->GetProb() << '\n';
  std::cout << "h3 fit parameters, chi square and probability of fit: " << '\n'
            << "p0: " << fpolar->GetParameter(0) << " "
            << "+/-"
            << " " << fpolar->GetParError(0) << '\n'
            << "Chi square: " << fpolar->GetChisquare() / fpolar->GetNDF()
            << '\n'
            << "Fit probability: " << fpolar->GetProb() << '\n';

  std::cout << "h4 fit parameters, chi square and probability of fit: " << '\n'
            << "p0: " << fp->GetParameter(0) << " "
            << "+/-"
            << " " << fp->GetParError(0) << '\n'
            << "p1: " << fp->GetParameter(1) << " "
            << "+/-"
            << " " << fp->GetParError(1) << '\n'
            << "Chi square: " << fp->GetChisquare() / fp->GetNDF() << '\n'
            << "Fit probability: " << fp->GetProb() << '\n';

  std::cout
      << "h13 entries, fit parameters, chi square and probability of fit: "
      << '\n'
      << "Entries: " << histograms[12]->Integral() << '\n'
      << "p0: " << finvmass1->GetParameter(0) << " "
      << "+/-"
      << " " << finvmass1->GetParError(0) << '\n'
      << "p1: " << finvmass1->GetParameter(1) << " "
      << "+/-"
      << " " << finvmass1->GetParError(1) << '\n'
      << "p2: " << finvmass1->GetParameter(2) << " "
      << "+/-"
      << " " << finvmass1->GetParError(2) << '\n'
      << "Chi square: " << finvmass1->GetChisquare() / finvmass1->GetNDF()
      << '\n'
      << "Fit probability: " << finvmass1->GetProb() << '\n';

  std::cout
      << "h14 entries, fit parameters, chi square and probability of fit: "
      << '\n'
      << "Entries: " << histograms[13]->Integral() << '\n'
      << "p0: " << finvmass2->GetParameter(0) << " "
      << "+/-"
      << " " << finvmass2->GetParError(0) << '\n'
      << "p1: " << finvmass2->GetParameter(1) << " "
      << "+/-"
      << " " << finvmass2->GetParError(1) << '\n'
      << "p2: " << finvmass2->GetParameter(2) << " "
      << "+/-"
      << " " << finvmass2->GetParError(2) << '\n'
      << "Chi square: " << finvmass2->GetChisquare() / finvmass2->GetNDF()
      << '\n'
      << "Fit probability: " << finvmass2->GetProb() << '\n';

  file->Close();
}

int main() { analysis(); }